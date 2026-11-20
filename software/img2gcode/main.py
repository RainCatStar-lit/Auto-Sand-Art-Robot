#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
img2gcode MVP - 图片 → 骨架 → 向量化 → G-code（沙画机器人专用）

用法：
    python main.py input.jpg output.gcode --preview

说明：
- 输入：任意 JPG/PNG 图片
- 输出：标准 G-code (.gcode)，可直接发给 ESP32 WebServer 执行
- 沙画特有指令：M106 S<pwm> 开沙，M107 停沙
- --preview：弹窗预览轨迹，方便在电脑上先看到效果
"""

import argparse
import sys

import cv2
import numpy as np

from pipeline.preprocess import preprocess
from pipeline.skeletonize import extract_skeleton
from pipeline.vectorize import skeleton_to_segments, simplify_segments
from pipeline.gcode_gen import segments_to_gcode, write_gcode


def sort_segments_nearest(segments):
    """贪心最近邻排序，减少空跑和笔升次数。"""
    if not segments:
        return segments

    segments = list(segments)
    sorted_segs = [segments.pop(0)]
    current_end = sorted_segs[0][1]

    while segments:
        best_idx = 0
        best_dist = float("inf")
        flipped = False

        for i, (p1, p2) in enumerate(segments):
            d1 = np.hypot(p1[0] - current_end[0], p1[1] - current_end[1])
            d2 = np.hypot(p2[0] - current_end[0], p2[1] - current_end[1])
            if d1 < best_dist:
                best_dist = d1
                best_idx = i
                flipped = False
            if d2 < best_dist:
                best_dist = d2
                best_idx = i
                flipped = True

        p1, p2 = segments.pop(best_idx)
        if flipped:
            p1, p2 = p2, p1
        sorted_segs.append((p1, p2))
        current_end = p2

    return sorted_segs


def preview_trajectory(segments, scale=1.0):
    """matplotlib 预览轨迹。"""
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print("[警告] 未安装 matplotlib，跳过预览")
        return

    fig, ax = plt.subplots(figsize=(8, 8))
    for (x1, y1), (x2, y2) in segments:
        ax.plot([x1 * scale, x2 * scale], [y1 * scale, y2 * scale], "k-", linewidth=0.5)
    ax.set_aspect("equal")
    ax.invert_yaxis()  # 图片坐标系 y 向下
    ax.set_title("轨迹预览（沙画 G-code）")
    ax.set_xlabel("X (mm)")
    ax.set_ylabel("Y (mm)")
    plt.grid(True, alpha=0.3)
    plt.show()


def main():
    parser = argparse.ArgumentParser(description="图片转 G-code（沙画机器人 MVP）")
    parser.add_argument("input", help="输入图片路径 (JPG/PNG)")
    parser.add_argument("output", help="输出 G-code 路径 (.gcode)")
    parser.add_argument("--preview", action="store_true", help="弹窗预览轨迹")
    parser.add_argument("--scale", type=float, default=1.0, help="缩放比例（毫米/像素）")
    parser.add_argument(
        "--feed", type=float, default=1500.0, help="出沙进给速度 (mm/min)"
    )
    parser.add_argument(
        "--flow-pwm", type=int, default=128, help="出沙 PWM 占空比 (0-255)"
    )
    args = parser.parse_args()

    # 读取图片
    img = cv2.imread(args.input, cv2.IMREAD_GRAYSCALE)
    if img is None:
        print(f"[错误] 无法读取图片: {args.input}")
        sys.exit(1)

    print(f"[信息] 读取图片: {args.input}, 尺寸 {img.shape[1]}x{img.shape[0]}")

    # 1. 预处理：灰度已读入，做高斯模糊 + 自适应阈值
    binary = preprocess(img)

    # 2. 骨架提取：单像素宽骨架
    skeleton = extract_skeleton(binary)

    # 3. 向量化：骨架 → 线段 → 简化
    segments = skeleton_to_segments(skeleton)
    segments = simplify_segments(segments, tolerance=0.3 * args.scale)

    # 4. 路径排序：最近邻，减少空跑
    segments = sort_segments_nearest(segments)

    # 5. 生成 G-code
    gcode = segments_to_gcode(
        segments,
        scale=args.scale,
        feed_mm_min=args.feed,
        flow_pwm=args.flow_pwm,
    )

    # 6. 写入文件
    write_gcode(gcode, args.output)
    print(f"[信息] G-code 已写入: {args.output}, 共 {len(gcode)} 行，线段 {len(segments)} 条")

    # 7. 预览
    if args.preview:
        preview_trajectory(segments, scale=args.scale)


if __name__ == "__main__":
    main()