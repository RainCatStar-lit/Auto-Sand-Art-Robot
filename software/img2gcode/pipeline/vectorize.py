#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""向量化：骨架 → 线段 → Douglas-Peucker 简化。"""

import cv2
import numpy as np


def skeleton_to_segments(skeleton, min_line_length=5):
    """
    从骨架图提取线段。
    方法：概率性霍夫变换检测线段，适合骨架后的细线。
    输出：[((x1, y1), (x2, y2)), ...]
    """
    lines = cv2.HoughLinesP(
        skeleton,
        rho=1,
        theta=np.pi / 180,
        threshold=15,
        minLineLength=min_line_length,
        maxLineGap=3,
    )

    segments = []
    if lines is not None:
        for line in lines[:, 0, :]:
            x1, y1, x2, y2 = line
            segments.append(((float(x1), float(y1)), (float(x2), float(y2))))

    return segments


def simplify_segments(segments, tolerance=0.3):
    """
    对每条线段做基本过滤和简化（去重、去过短线段）。
    更复杂的多点曲线简化可在后续接入 douglas_peucker。
    """
    simplified = []
    seen = set()
    for (x1, y1), (x2, y2) in segments:
        # 跳过过短的线段
        if abs(x1 - x2) < 1 and abs(y1 - y2) < 1:
            continue
        # 简单去重（首尾互换视为同一条）
        key = tuple(sorted([(round(x1, 1), round(y1, 1)), (round(x2, 1), round(y2, 1))]))
        if key in seen:
            continue
        seen.add(key)
        simplified.append(((x1, y1), (x2, y2)))
    return simplified


def douglas_peucker(points, epsilon):
    """
    经典 Douglas-Peucker 曲线简化算法。
    points: [(x, y), ...]
    epsilon: 简化容差
    """
    if len(points) <= 2:
        return points

    start = np.array(points[0])
    end = np.array(points[-1])
    line_vec = end - start
    line_len = np.linalg.norm(line_vec)
    if line_len == 0:
        return [points[0]]

    max_dist = 0
    max_idx = 0
    for i in range(1, len(points) - 1):
        p = np.array(points[i])
        dist = abs(np.cross(line_vec, p - start)) / line_len
        if dist > max_dist:
            max_dist = dist
            max_idx = i

    if max_dist > epsilon:
        left = douglas_peucker(points[: max_idx + 1], epsilon)
        right = douglas_peucker(points[max_idx:], epsilon)
        return left[:-1] + right
    else:
        return [points[0], points[-1]]