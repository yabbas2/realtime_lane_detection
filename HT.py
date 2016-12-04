import cv2
import numpy as np


def HoughTransform(input, houghOutput):
    h, w = input.shape[:2]
    # using PHT

    minLineLength = 60  # 50
    maxLineGap = 1  # 5
    maxVotes = 90  # 50
    lines = cv2.HoughLinesP(input, 1, np.pi / 180, maxVotes, minLineLength, maxLineGap)
    array_of_lines = [[0 for i in range(w)] for j in range(h)]
    percentage = list()
    x = 0
    for line in lines:
        for x1, y1, x2, y2 in line:
            if not (abs(x1 - x2) == 0 and x1 > 15 and x1 < w-30):  # 0 and abs(x1 - x2) < ):  #catch vertical lines
                continue
            # print x1, x2, y1, y2  # debugging
            array_of_lines[x][x1] = x1
            x = (x + 1) % w
    percentage = calculateMax(array_of_lines, h, w)
    for i in range(0, len(percentage), 1):
        sumP = percentage[i]  # sum(percentage[i:i+17])
        avg_i = i  # ((i * 16) + 120) / 16
        if sumP > 0.4:
            cv2.line(houghOutput, (avg_i, 0), (avg_i, h), (0, 0, 255), 4)
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
