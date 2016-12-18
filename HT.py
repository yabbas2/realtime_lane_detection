import cv2
import numpy as np


def HoughTransform(input, houghOutput):
    h, w = input.shape[:2]
    minLineLength = 70  # 50
    maxLineGap = 1  # 5
    maxVotes = 90  # 50
    phaseOneFilteredLines = list()
    phaseTwoFilteredLines = list()
    phaseThreeFilteredLines = list()
    lines = cv2.HoughLinesP(input, 1, np.pi / 180, maxVotes, minLineLength, maxLineGap)
    for line in lines:
        for x1, y1, x2, y2 in line:
            if not (x1 > 15 and x1 < w - 30):
                continue
            phaseOneFilteredLines.append([x1, y1, x2, y2])
    for i in range(15, w-30, 40):
        counter = int()
        sum_x1 = float()
        sum_x2 = float()
        sum_y1 = float()
        sum_y2 = float()
        for x in range(i, i + 40, 1):
            flag, x_1, y_1, x_2, y_2 = searchForLine(x, phaseOneFilteredLines)
            if flag:
                counter += 1
                sum_x1 += x_1
                sum_x2 += x_2
                sum_y1 += y_1
                sum_y2 += y_2
        if counter > 0:
            avg_x1 = sum_x1 / counter
            avg_x2 = sum_x2 / counter
            avg_y1 = sum_y1 / counter
            avg_y2 = sum_y2 / counter
            phaseTwoFilteredLines.append([avg_x1, avg_y1, avg_x2, avg_y2])
    '''for i in range(0, len(phaseTwoFilteredLines), 1):
        try:
            if abs(phaseTwoFilteredLines[i][0] - phaseTwoFilteredLines[i+1][0]) > 10 and abs(phaseTwoFilteredLines[i][2] - phaseTwoFilteredLines[i+1][2]) > 10:
                phaseThreeFilteredLines.append(phaseTwoFilteredLines[i+1])
        except:
            break'''
    houghOutput = drawLines(phaseTwoFilteredLines, houghOutput)
    return houghOutput


def drawLines(lines, image):
    h, w = image.shape[:2]
    for line in lines:
        perfectVertical = False
        [x1, y1, x2, y2] = line
        try:
            slope = (y2-y1)/(x2-x1)
        except ZeroDivisionError:
            perfectVertical = True
        finally:
            if(perfectVertical):
                x = np.float32(x1)
                cv2.line(image, (x, 0), (x, h), (0, 255, 0), 4)
            else:
                new_x1 = np.float32((0-y1)/slope + x1)
                new_x2 = np.float32((h-y1)/slope + x1)
                cv2.line(image, (new_x1, 0), (new_x2, h), (0, 255, 0), 4)
    return image


def searchForLine(x, filtered_lines):
    for line in filtered_lines:
        if int(line[0]) == x:
            return True, line[0], line[1], line[2], line[3]
    return False, 0, 0, 0, 0