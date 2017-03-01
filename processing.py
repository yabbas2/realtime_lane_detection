import cv2
import numpy as np


def determinePtsAndDst(args, ext):
    [width, height] = [int(args['res'].split('x')[0]), int(args['res'].split('x')[1])]
    dst = np.array([[0, 0], [height, 0], [height, width], [0, width]], dtype="float32")
    if args['video'].endswith("sample1"+ext):
        pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
    elif args['video'].endswith("sample2"+ext):
        pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
    elif args['video'].endswith("sample3"+ext):
        pts = np.array([[220, 200], [322, 200], [361, 230], [85, 230]], dtype="float32")
    elif args['video'].endswith("sample4"+ext):
        pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
    elif args['video'].endswith("sample5"+ext):
        pts = np.array([[241, 208], [345, 208], [450, 297], [23, 297]], dtype="float32")
    else:
        pts = np.zeros((4, 2), dtype="float32")
    return pts, dst


def fourPointTransform(normalFrame, pts, dst):
    height, width = normalFrame.shape[:2]
    HomographyToInv = cv2.getPerspectiveTransform(pts, dst)
    HomographyToOriginal = cv2.getPerspectiveTransform(dst, pts)
    ipmFrame = cv2.warpPerspective(normalFrame, HomographyToInv, (height, width))
    return ipmFrame, HomographyToOriginal


def lineSegmentDetector(inputFrame):
    out = cv2.cvtColor(inputFrame, cv2.COLOR_BGR2GRAY)
    out = cv2.GaussianBlur(out, (15, 15), 0)
    LSD = cv2.createLineSegmentDetector(cv2.LSD_REFINE_STD)
    lines, w, prec, nfa = LSD.detect(out)
    # lines = np.array(lines)
    # inputFrame = LSD.drawSegments(inputFrame, lines)
    if lines is None:
        return []
    return lines


def calcMargin(lines, windowSize, width):
    margin = 0
    lanesNum = "none"
    leftMargin, leftLaneNum = checkLanesFromLeft(lines, windowSize, width)
    rightMargin, rightLaneNum = checkLanesFromRight(lines, windowSize, width)
    if leftMargin == rightMargin and leftMargin > 0:
        margin = leftMargin
        lanesNum = leftLaneNum
    else:
        if rightMargin == 0 and leftMargin > 0:
            margin = leftMargin
            lanesNum = leftLaneNum
        elif leftMargin == 0 and rightMargin > 0:
            margin = rightMargin
            lanesNum = rightLaneNum
    return margin, lanesNum


def filterAndAverage(lines, start, end, windowSize, margin, inputShape):
    height, width = inputShape
    detectedLines = list()
    for i in range(start, end, windowSize+margin):
        sum_x1, sum_x2, size = 0.0, 0.0, 0
        for line in lines:
            for x1, y1, x2, y2 in line:
                if int(x1) in range(i, i+windowSize) and int(x2) in range(i, i+windowSize):
                    sum_x1 += x1
                    sum_x2 += x2
                    size += 1
        if size != 0:
            avg_x1 = sum_x1 / size
            avg_x2 = sum_x2 / size
            detectedLines.append([avg_x1, 0, avg_x2, height])
    return detectedLines


def doInverse(lines, HomographyToOriginal, args):
    height = int(args['res'].split('x')[1])
    size = len(lines)
    outputLines = list()
    X1Points = list()
    X2Points = list()
    arrow = list()
    for row in lines:
        Z = 1 / (HomographyToOriginal[2][0] * row[0] + HomographyToOriginal[2][1] * row[1] + HomographyToOriginal[2][2])
        px1 = np.float32(((HomographyToOriginal[0][0] * row[0] + HomographyToOriginal[0][1] * row[1] +
                           HomographyToOriginal[0][2]) * Z))
        py1 = np.float32(((HomographyToOriginal[1][0] * row[0] + HomographyToOriginal[1][1] * row[1] +
                           HomographyToOriginal[1][2]) * Z))
        Z = 1 / (HomographyToOriginal[2][0] * row[2] + HomographyToOriginal[2][1] * row[3] + HomographyToOriginal[2][2])
        px2 = np.float32(((HomographyToOriginal[0][0] * row[2] + HomographyToOriginal[0][1] * row[3] +
                           HomographyToOriginal[0][2]) * Z))
        py2 = np.float32(((HomographyToOriginal[1][0] * row[2] + HomographyToOriginal[1][1] * row[3] +
                           HomographyToOriginal[1][2]) * Z))
        slope = np.subtract(py2, py1) / np.subtract(px2, px1)
        px2 = (np.subtract(height, py1) / slope) + px1
        py2 = height
        X2Points.append(float(px2))
        X1Points.append(float(px1))
        outputLines.append([px1, py1, px2, py2])
    assert (len(outputLines) == size)
    if size > 5 or size == 0:
        return [], []
    max = 0
    index = 0
    for i in range(0, size-1, 1):
        if X2Points[i] < 0 or X2Points[i+1] < 0:
            continue
        if (X2Points[i] - X2Points[i+1]) < max:
            max = X2Points[i] - X2Points[i+1]
            index = i
    if max == 0:
        return [], []
    arrow.append(int((X1Points[index]+X1Points[index+1])/2))
    arrow.append(height-50)
    arrow.append(int((X1Points[index]+X1Points[index+1])/2))
    arrow.append(height-100)
    return outputLines, arrow


def checkLanesFromLeft(lines, windowSize, width):
    margin = 0
    i = 0
    lines = sorted(lines, key=lambda l: l[0][0], reverse=False)
    while int(lines[i][0][0]) in range(0, 51):
        i += 1
    if int(lines[i][0][0]) in range(310, 361):  # one lane
        margin = (width - (2 * windowSize)) / 1
        return int(margin), "one"
    if int(lines[i][0][0]) in range(150, 211):  # two lanes
        margin = (width - (3 * windowSize)) / 2
        return int(margin), "two"
    if int(lines[i][0][0]) in range(96, 157):
        while int(lines[i][0][0]) in range(96, 157):
            i += 1
        if int(lines[i][0][0]) in range(202, 263):
            margin = (width - (4 * windowSize)) / 3  # three lanes
            return int(margin), "three"

    if int(lines[i][0][0]) in range(70, 131):
        while int(lines[i][0][0]) in range(70, 131):
            i += 1
        if int(lines[i][0][0]) in range(150, 211):
            margin = (width - (5 * windowSize)) / 4  # four lanes
            return int(margin), "four"
    return int(margin), "none"


def checkLanesFromRight(lines, windowSize, width):
    margin = 0
    i = 0
    lines = sorted(lines, key=lambda l: l[0][0], reverse=True)
    while int(lines[i][0][0]) in range(310, 361):
        i += 1
    if int(lines[i][0][0]) in range(0, 51):  # one lane
        margin = (width - (2 * windowSize)) / 1
        return int(margin), "one"
    if int(lines[i][0][0]) in range(150, 211):  # two lanes
        margin = (width - (3 * windowSize)) / 2
        return int(margin), "two"
    if int(lines[i][0][0]) in range(204, 265):
        while int(lines[i][0][0]) in range(204, 265):
            i += 1
        if int(lines[i][0][0]) in range(98, 159):
            margin = (width - (4 * windowSize)) / 3  # three lanes
            return int(margin), "three"
    if int(lines[i][0][0]) in range(230, 291):
        while int(lines[i][0][0]) in range(230, 291):
            i += 1
        if int(lines[i][0][0]) in range(150, 211):
            margin = (width - (5 * windowSize)) / 4  # four lanes
            return int(margin), "four"
    return int(margin), "none"
