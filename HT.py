import cv2, math
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
            if not (x1 > 0 and x1 < w):
                continue
            phaseOneFilteredLines.append([x1, y1, x2, y2])
    #Stage one
    phaseTwoFilteredLines = averaging(0, w, 10, phaseOneFilteredLines, 0)
    #Stage two
    phaseThreeFilteredLines = averaging(0, w, 20, phaseTwoFilteredLines, 0)
    #Stage three
    phaseThreeFilteredLines = averaging(0, w, 40, phaseThreeFilteredLines, 0)
    #Stage four
    phaseThreeFilteredLines = averaging(0, w, 80, phaseThreeFilteredLines, 0)
    #Stage five
    phaseThreeFilteredLines = averaging(0, w, 100, phaseThreeFilteredLines, 0)

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
            flag, detectedLines = searchForLine(x, _2D_array)
            if flag:
                for line in detectedLines:
                    [x_1, y_1, x_2, y_2] = line
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
        [x1, y1, x2, y2] = line
        if (x1-x2) == 0:
            x = np.float32(x1)
            returnLines.append([x, 0, x, height])
        else:
            slope = (y2 - y1) / (x2 - x1)
            theta = math.atan(slope)
            if theta > (95 * np.pi / 180) or theta < (85 * np.pi / 180):
                continue
            new_x1 = np.float32((0 - y1) / slope + x1)
            new_x2 = np.float32((height - y1) / slope + x1)
            returnLines.append([new_x1, 0, new_x2, height])
    return returnLines


def searchForLine(x, filtered_lines):
    detectedLines = list()
    for line in filtered_lines:
        [x1, y1, x2, y2] = line
        if int(x1) == x:
            detectedLines.append([x1, y1, x2, y2])
    if(len(detectedLines) > 0):
        return True, detectedLines
    return False, [0, 0, 0, 0]