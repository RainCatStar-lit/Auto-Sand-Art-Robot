#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""图像预处理：灰度、高斯模糊、自适应阈值、降噪。"""

import cv2
import numpy as np


def preprocess(img_gray, blur_ksize=5, block_size=15, C=10):
    """
    输入：单通道灰度图
    输出：二值化图像 (0/255)，白色 (255) 为前景线条
    """
    # 高斯模糊去噪
    blurred = cv2.GaussianBlur(img_gray, (blur_ksize, blur_ksize), 0)

    # 自适应阈值二值化（适应光照不均）
    binary = cv2.adaptiveThreshold(
        blurred,
        255,
        cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
        cv2.THRESH_BINARY_INV,  # 前景为白 (255)
        block_size,
        C,
    )

    # 形态学开运算，去掉微小噪点
    kernel = np.ones((2, 2), np.uint8)
    binary = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations=1)

    return binary