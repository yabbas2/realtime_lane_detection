import cv2
import numpy as np


def determinePtsAndDst(args):
    [height, width] = [int(args['res'].split('x')[0]), int(args['res'].split('x')[1])]
    dst = np.array([[0, 0], [height, 0], [height, width], [0, width]], dtype="float32")
    if args['video'].endswith("sample1.mp4"):
        pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
    elif args['video'].endswith("sample2.mp4"):
        pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
    elif args['video'].endswith("sample3.mp4"):
        pts = np.array([[220, 200], [322, 200], [361, 230], [85, 230]], dtype="float32")
    elif args['video'].endswith("sample4.mp4"):
        pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
    elif args['video'].endswith("sample5.mp4"):
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
    return lines


def filterAndAverage(lines, start, end, windowSize, margin, inputShape):
    height, width = inputShape
    detectedLines = list()
    for i in range(start, end, windowSize+margin):
        sum_x1, sum_x2, size = 0.0, 0.0, 0
        for line in lines:
            for x1, y1, x2, y2 in line:
                if x1 in range(i, i+windowSize) and x2 in range(i, i+windowSize):
                    sum_x1 += x1
                    sum_x2 += x2
                    size += 1
        if size != 0:
            avg_x1 = sum_x1 / size
            avg_x2 = sum_x2 / size
            detectedLines.append([avg_x1, 0, avg_x2, height])
    return detectedLines
