#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""骨架提取：将二值图降为单像素宽的骨架。"""

import cv2
import numpy as np


def extract_skeleton(binary):
    """
    输入：二值图像 (0/255)，白色为前景
    输出：骨架图像 (0/255)
    """
    thin = cv2.ximgproc.thinning(
        binary,
        thinningType=cv2.ximgproc.THINNING_ZHANGSUEN,
    )
    return thin