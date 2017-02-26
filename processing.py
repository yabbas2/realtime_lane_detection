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


def lineSegmentDetector(inputFrame, margin, windowSize):
    out = cv2.cvtColor(inputFrame, cv2.COLOR_BGR2GRAY)
    LSD = cv2.createLineSegmentDetector(cv2.LSD_REFINE_STD)
    lines, w, prec, nfa = LSD.detect(out)
    lines = filterAndAverage(lines, 0, inputFrame.shape[1], windowSize, margin, inputFrame.shape[:2])
    # lines = np.array(lines)
    # inputFrame =  LSD.drawSegments(inputFrame, lines)
    return lines


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
        return None, None
    max = 0
    index = 0
    for i in range(0, size-1, 1):
        if X2Points[i] < 0 or X2Points[i+1] < 0:
            continue
        if (X2Points[i] - X2Points[i+1]) < max:
            max = X2Points[i] - X2Points[i+1]
            index = i
    if max == 0:
        return None, None
    arrow.append(int((X1Points[index]+X1Points[index+1])/2))
    arrow.append(height-50)
    arrow.append(int((X1Points[index]+X1Points[index+1])/2))
    arrow.append(height-100)
    return outputLines, arrow
