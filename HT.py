import cv2
import numpy as np


def HoughTransform(input, houghOutput):
    h, w = input.shape[:2]
    minLineLength = 60  # 50
    maxLineGap = 1  # 5
    maxVotes = 90  # 50
    lines = cv2.HoughLinesP(input, 1, np.pi / 180, maxVotes, minLineLength, maxLineGap)
    for line in lines:
        for x1, y1, x2, y2 in line:
            if not (x1 > 15 and x1 < w - 30):
                continue
            cv2.line(houghOutput, (x1, y1), (x2, y2), (0, 255, 0), 1)
    return houghOutput


def calculateMax(matrix, row, column):
    percentage = list()
    for i in range(column):
        zero_counter = 0
        value_counter = 0
        for j in range(row):
            if matrix[j][i] == 0:
                zero_counter += 1
            else:
                value_counter += 1
        percentage.append((float(value_counter) / (zero_counter + value_counter)) * 100)
    # print percentage
    return percentage
