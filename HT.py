import cv2
import numpy as np


def HoughTransform(input, houghOutput):
    h, w = input.shape[:2]
    minLineLength = 70  # 50
    maxLineGap = 1  # 5
    maxVotes = 90  # 50
    filtered_lines = list()
    lines = cv2.HoughLinesP(input, 1, np.pi / 180, maxVotes, minLineLength, maxLineGap)
    for line in lines:
        for x1, y1, x2, y2 in line:
            if not (x1 > 15 and x1 < w - 30):
                continue
            filtered_lines.append([x1, y1, x2, y2])
    for i in range(10, w-10, 40):
        counter = int()
        sum_x1 = float()
        sum_x2 = float()
        for x in range(i, i + 40, 1):
            flag, x_1, x_2 = searchForLine(x, filtered_lines)
            if flag:
                counter += 1
                sum_x1 += x_1
                sum_x2 += x_2
        if counter > 0:
            avg_x1 = sum_x1 / counter
            avg_x2 = sum_x2 / counter
            cv2.line(houghOutput, (int(avg_x1), 0), (int(avg_x2), h), (0, 255, 0), 6)
    return houghOutput


def searchForLine(x, filtered_lines):
    for line in filtered_lines:
        if int(line[0]) == x:
            return True, line[0], line[2]
    return False, 0, 0

'''def calculateMax(matrix, row, column):
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
'''