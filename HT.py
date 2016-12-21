import cv2, math
import numpy as np


def HoughTransform(input):
    global h, w
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

    phaseTwoFilteredLines = averagingStages(phaseOneFilteredLines, 90, 10)
    phaseThreeFilteredLines = findLines(phaseTwoFilteredLines)
    phaseFourFilteredLines = checkForAllLanes(phaseThreeFilteredLines)

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

def findLines(lines):
    returnLines = list()
    for line in lines:
        [x1, y1, x2, y2] = line
        if (x1-x2) == 0:
            x = np.float32(x1)
            returnLines.append([x, 0, x, h])
        else:
            slope = (y2 - y1) / (x2 - x1)
            theta = math.atan(slope)
            if theta > (95 * np.pi / 180) or theta < (85 * np.pi / 180):
                continue
            new_x1 = np.float32((0 - y1) / slope + x1)
            new_x2 = np.float32((h - y1) / slope + x1)
            returnLines.append([new_x1, 0, new_x2, h])
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

def averagingStages(lines, max, step):
    filtered = lines
    for i in range(0, max, step):
        filtered = averaging(0, w, 10+i, filtered, 0)
    return filtered

def checkForAllLanes(lines):
    if(len(lines) == 3):
        if (abs(lines[0][0] - lines[1][0]) < w/2) and (abs(lines[1][0] - lines[2][0]) < w/2):
            return lines
        else:
            raise ValueError('[ERROR] unknown detected lanes!')
    if(len(lines) == 1):
        raise IndexError('[ERROR] just one line is detected!')
    line1_x1, line2_x1, line1_x2, line2_x2 = lines[0][0], lines[1][0], lines[0][2], lines[1][2]
    if abs(line1_x1 - line2_x1) > w/2:
        x1 = (line1_x1 + line2_x1)/2
        x2 = (line1_x2 + line2_x2)/2
        lines.insert(1, [x1, 0, x2, h])
        return lines
    if abs(line1_x1 - line2_x1) < w/2 and line2_x1 > w/2:
        x1 = 2 * line1_x1 - line2_x1
        x2 = 2 * line1_x2 - line2_x2
        lines.insert(0, [x1, 0, x2, h])
        return lines
    if abs(line1_x1 - line2_x1) < w/2 and line1_x1 < w/2:
        x1 = 2 * line2_x1 - line1_x1
        x2 = 2 * line2_x2 - line1_x2
        lines.insert(2, [x1, 0, x2, h])
        return lines
    return lines