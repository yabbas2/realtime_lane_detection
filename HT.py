import cv2
import numpy as np


def HoughTransform(input, height):
    h, w = input.shape[:2]
    minLineLength = 60  # 70
    maxLineGap = 1  # 5
    maxVotes = 90  # 90
    phaseOneFilteredLines = list()
    lines = cv2.HoughLinesP(input, 1, np.pi / 180, maxVotes, minLineLength, maxLineGap)
    for line in lines:
        for x1, y1, x2, y2 in line:
            if not (x1 > 15 and x1 < w - 30):
                continue
            phaseOneFilteredLines.append([x1, y1, x2, y2])
    #Stage one
    phaseTwoFilteredLines = averaging(15, w - 30, 10, phaseOneFilteredLines, 0)
    #Stage two
    phaseThreeFilteredLines = averaging(15, w - 30, 20, phaseTwoFilteredLines, 0)
    #Stage three
    phaseThreeFilteredLines = averaging(15, w - 30, 40, phaseThreeFilteredLines, 0)
    #Stage Four
    phaseThreeFilteredLines = averaging(15, w - 30, 80, phaseThreeFilteredLines, 0)

    phaseFourFilteredLines = findLines(phaseThreeFilteredLines, height)
    return phaseFourFilteredLines


def averaging(start, end, windowSize, _2D_array, threshold):
    filtered = list()
    for i in range(start, end, windowSize):
        counter = int()
        sum_x1 = float()
        sum_x2 = float()
        sum_y1 = float()
        sum_y2 = float()
        for x in range(i, i + windowSize, 1):
            flag, x_1, y_1, x_2, y_2 = searchForLine(x, _2D_array)
            if flag:
                counter += 1
                sum_x1 += x_1
                sum_x2 += x_2
                sum_y1 += y_1
                sum_y2 += y_2
        if counter > threshold:
            avg_x1 = sum_x1 / counter
            avg_x2 = sum_x2 / counter
            avg_y1 = sum_y1 / counter
            avg_y2 = sum_y2 / counter
            filtered.append([avg_x1, avg_y1, avg_x2, avg_y2])
    return filtered

def findLines(lines, height):
    returnLines = list()
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
                returnLines.append([x, 0, x, height])
                #cv2.line(image, (x, 0), (x, h), (0, 255, 0), 4)
            else:
                new_x1 = np.float32((0-y1)/slope + x1)
                new_x2 = np.float32((height-y1)/slope + x1)
                returnLines.append([new_x1, 0, new_x2, height])
                #cv2.line(image, (new_x1, 0), (new_x2, h), (0, 255, 0), 4)
    return returnLines


def searchForLine(x, filtered_lines):
    for line in filtered_lines:
        [x1, y1, x2, y2] = line
        if int(x1) == x:
            return True, x1, y1, x2, y2
    return False, 0, 0, 0, 0