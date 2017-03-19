import cv2
import numpy as np
import numpy.polynomial.polynomial as poly
import math


def determinePtsAndDst(width, height, videoFile):
    dst = np.array([[0, 0], [height, 0], [height, width-200], [0, width-200]], dtype="float32")
    if videoFile == "sample1":
        pts = np.array([[357, 280], [528, 282], [778, 478], [146, 478]], dtype="float32")
    elif videoFile == "sample2":
        # pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
        pts = np.array([[250, 225], [360, 225], [425, 300], [200, 300]], dtype="float32")
    elif videoFile == "sample3":
        # pts = np.array([[220, 200], [322, 200], [361, 230], [85, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif videoFile == "sample4":
        # pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
        # pts = np.array([[(350, 260), (460, 260), (560, 345), (280, 345)]], dtype="float32")
    elif videoFile == "sample5":
        # pts = np.array([[241, 208], [345, 208], [450, 297], [23, 297]], dtype="float32")
        pts = np.array([[260, 196], [354, 196], [442, 280], [204, 280]], dtype="float32")
    else:
        raise ValueError("unknown video file!")
    return pts, dst


def fourPointTransform(normalFrame, pts, dst):
    height, width = normalFrame.shape[:2]
    HomographyToInv = cv2.getPerspectiveTransform(pts, dst)
    HomographyToOriginal = cv2.getPerspectiveTransform(dst, pts)
    ipmFrame = cv2.warpPerspective(normalFrame, HomographyToInv, (height, width))
    return ipmFrame, HomographyToOriginal


def lineSegmentDetector(inputFrame):
    out = cv2.cvtColor(inputFrame, cv2.COLOR_BGR2GRAY)
    out = cv2.GaussianBlur(out, (21, 21), 0)
    LSD = cv2.createLineSegmentDetector(cv2.LSD_REFINE_STD)
    lines, w, prec, nfa = LSD.detect(out)
    # inputFrame = LSD.drawSegments(inputFrame, lines)
    # out = cv2.Canny(out, 10, 200, apertureSize=3, L2gradient=True)
    # minLineLength = 20
    # maxLineGap = 1
    # maxVotes = 10
    # lines = cv2.HoughLinesP(out, 1, np.pi/180, maxVotes, minLineLength, maxLineGap)
    if lines is None:
        return []
    return lines


def doInverse(points, HomographyToOriginal):
    outputPoints = list()
    size = len(points)
    if size == 0:
        return np.array(outputPoints)
    for pt in points:
        Z = 1 / (HomographyToOriginal[2][0] * pt[0] + HomographyToOriginal[2][1] * pt[1] + HomographyToOriginal[2][2])
        ptx = np.float32(((HomographyToOriginal[0][0] * pt[0] + HomographyToOriginal[0][1] * pt[1] +
                           HomographyToOriginal[0][2]) * Z))
        pty = np.float32(((HomographyToOriginal[1][0] * pt[0] + HomographyToOriginal[1][1] * pt[1] +
                           HomographyToOriginal[1][2]) * Z))
        outputPoints.append([ptx, pty])
    assert size == len(outputPoints)
    return np.array(outputPoints)


def eliminateFalseDetection(lines):
    threshold_length = 30
    threshold_angle = range(70, 110)
    filtered_lines = []
    for line in lines:
        for x1, y1, x2, y2 in line:
            if y1 < y2:
                x1, x2 = x2, x1
                y1, y2 = y2, y1
            theta = cv2.fastAtan2((y2 - y1), (x2 - x1))
            if theta > 180:
                theta -= 180
            length = math.sqrt(math.pow((x2 - x1), 2) + math.pow((y2 - y1), 2))
            if int(theta) in threshold_angle and length >= threshold_length:
                filtered_lines.append([int(x1), int(y1), int(x2), int(y2), int(theta), int(length), 0])
    return filtered_lines


def linesToPoints(left_lines, right_lines):
    left_points = []
    right_points = []
    for line in left_lines:
        left_points.append((line[0], line[1]))
        left_points.append((line[2], line[3]))
    for line in right_lines:
        right_points.append((line[0], line[1]))
        right_points.append((line[2], line[3]))

    return np.array(left_points), np.array(right_points)


def leftRegionGrowing(lines, image):
    left_region = []
    threshold_angle = 5
    threshold_x = 100
    USED = 1
    seed_line = 0
    height = image.shape[0]
    width = image.shape[1]
    seedThreshold = width/2
    left_lines = sorted(lines, key=lambda l: l[1], reverse=True)
    for line in left_lines:
        if line[0] < width/2 and width/2 - line[0] < seedThreshold:
            seedThreshold = width/2 - line[0]
            seed_line = line
        if line[1] < height/2:
            break
    if seed_line == 0:
        return []
    seed_line[6] = USED
    left_region.append(seed_line)
    theta = seed_line[4]
    Sx = np.cos(theta)
    Sy = np.sin(theta)
    # sum_x = (seed_line[0] + seed_line[2]) / 2
    # region_x = sum_x / len(left_region)
    region_x = seed_line[2]
    x1 = seed_line[0]
    y1 = seed_line[1]
    x2 = seed_line[2]
    y2 = seed_line[3]
    region_angle = np.arctan(Sy/Sx)
    if x2 == x1:
        m = 99
    else:
        m = (y2 - y1) / (x2 - x1)
    c = y1 - m * x2
    for line in left_lines:
        if line[6] == USED:
            continue
        x1, y1, x2, y2 = line[0], line[1], line[2], line[3]
        theta = line[4]
        Sxt = np.cos(theta)
        Syt = np.sin(theta)
        angle = np.arctan(Syt/Sxt)
        length = line[5]
        avg_x = (x1 + x2) / 2
        if abs(region_angle - angle) <= threshold_angle and abs(((y1-c)/m)-x1) < 20:
            line[6] = USED
            left_region.append(line)
            # sum_x += avg_x
            Sx = Sx + Sxt
            Sy = Sy + Syt
            region_angle = np.arctan(Sy / Sx)
            # sum_angle += theta
            # region_x = sum_x / len(left_region)
            region_x = x2
            if x2 == x1:
                m = 99
            else:
                m = (y2-y1)/(x2-x1)
            c = y1 - m*x2

            # region_angle = sum_angle / len(left_region)

    return left_region


def rightRegionGrowing(lines, image):
    right_region = []
    threshold_angle = 10
    threshold_x = 80
    USED = 1
    seed_line = 0
    right_lines = sorted(lines, key=lambda l: l[1], reverse=True)
    for line in right_lines:
        if line[0] > (image.shape[1] / 2):
            seed_line = line
            break
    if seed_line == 0:
        return []
    seed_line[6] = USED
    right_region.append(seed_line)
    sum_angle = seed_line[4]
    sum_x = image.shape[1] - (seed_line[0] + seed_line[2]) / 2
    region_x = sum_x / len(right_region)
    region_angle = sum_angle / len(right_region)
    for line in right_lines:
        if line[6] == USED:
            continue
        x1, y1, x2, y2 = line[0], line[1], line[2], line[3]
        theta = line[4]
        length = line[5]
        avg_x = image.shape[1] - (x1 + x2) / 2
        if abs(region_angle - theta) <= threshold_angle and abs(region_x - avg_x) <= threshold_x:
            line[6] = USED
            right_region.append(line)
            sum_x += avg_x
            sum_angle += theta
            region_x = sum_x / len(right_region)
            region_angle = sum_angle / len(right_region)

    return right_region


def curveFit(points_x, points_y, degree, height, pointsNum):
    points = []
    coeffs = poly.polyfit(points_y, points_x, degree)
    ffit = poly.Polynomial(coeffs)
    points_y = np.linspace(0, height, pointsNum)
    points_x = ffit(points_y)
    for i in range(0, len(points_y), 1):
        points.append([points_x[i], points_y[i]])
    return points


def debug_draw(points, image):
    cv2.polylines(image, np.int32([points]), False, (0, 0, 255), 2, cv2.LINE_AA)


def enhanceCurveFitting(left_points, right_points, height):
    if left_points.size > 8:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 2, height, 50)
    elif left_points.size in range(2, 9):
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 1, height, 50)
    else:
        left_points = []
    if right_points.size > 8 and right_points.size != 0:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, height, 50)
    elif right_points.size in range(2, 9):
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 1, height, 50)
    else:
        right_points = []
    return left_points, right_points


def getAvgLineOfTwoLines(lines):
    distThreshold = 40
    filteredLines = []
    for line in lines:
        inFlag = 0
        x1 = line[0]
        y1 = line[1]
        x2 = line[2]
        y2 = line[3]
        for lineC in lines:
            x1c = lineC[0]
            y1c = lineC[1]
            x2c = lineC[2]
            y2c = lineC[3]
            if x1 == x1c and x2c == x2c:
                continue
            if abs(x1 - x1c) <= distThreshold and abs(x2 - x2c) <= distThreshold and abs(y1 - y1c) <= distThreshold and abs(y2 - y2c) <= distThreshold:
                x1avg = int((x1+x1c)/2)
                y1avg = int((y1+y1c)/2)
                x2avg = int((x2+x2c)/2)
                y2avg = int((y2+y2c)/2)
                thetaAvg = int((line[4]+lineC[4])/2)
                lengthAvg = int((line[5]+lineC[5])/2)
                if not [x1avg, y1avg, x2avg, y2avg, thetaAvg, lengthAvg, 0] in filteredLines:
                    filteredLines.append([x1avg, y1avg, x2avg, y2avg, thetaAvg, lengthAvg, 0])
                inFlag = 1
                break
        if not inFlag:
            filteredLines.append(line)
    return filteredLines

