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
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
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


def findSeedLines(lines, width):
    left_seed_line = 0
    right_seed_line = 0
    lines = sorted(lines, key=lambda l: l[1], reverse=True)
    left_lines = lines.copy()
    left_temp = []
    counter = 0
    FOUND = 0
    for line in left_lines:
        if line[0] < width / 2:
            left_temp.append(line)
            counter += 1
        if counter == 5:
            break
    for lineI in left_lines:
        if FOUND:
            break
        for lineJ in left_lines:
            if abs(lineI[0]-lineJ[0]) <= 20 and abs(lineI[1]-lineJ[1]) <= 5:
                left_seed_line = lineI
                FOUND = 1
                break
    right_lines = lines.copy()
    right_temp = []
    counter = 0
    FOUND = 0
    for line in right_lines:
        if line[0] > width / 2:
            right_temp.append(line)
            counter += 1
        if counter == 5:
            break
    for lineI in right_lines:
        if FOUND:
            break
        for lineJ in right_lines:
            if abs(lineI[0] - lineJ[0]) <= 20 and abs(lineI[1] - lineJ[1]) <= 5:
                right_seed_line = lineI
                FOUND = 1
                break

    return left_seed_line, right_seed_line


def leftRegionGrowing(lines, seed_line):
    left_region = []
    threshold_angle = 10
    threshold_x = 80
    USED = 1
    left_lines = sorted(lines, key=lambda l: l[1], reverse=True)
    seed_line[6] = USED
    left_region.append(seed_line)
    sum_angle = seed_line[4]
    sum_x = (seed_line[0] + seed_line[2]) / 2
    region_x = sum_x / len(left_region)
    region_angle = sum_angle / len(left_region)
    for line in left_lines:
        if line[6] == USED:
            continue
        x1, y1, x2, y2 = line[0], line[1], line[2], line[3]
        theta = line[4]
        length = line[5]
        avg_x = (x1 + x2) / 2
        if abs(region_angle - theta) <= threshold_angle and abs(region_x - avg_x) <= threshold_x:
            line[6] = USED
            left_region.append(line)
            sum_x += avg_x
            sum_angle += theta
            region_x = sum_x / len(left_region)
            region_angle = sum_angle / len(left_region)

    return left_region


def rightRegionGrowing(lines, seed_line, width):
    right_region = []
    threshold_angle = 10
    threshold_x = 80
    USED = 1
    right_lines = sorted(lines, key=lambda l: l[1], reverse=True)
    seed_line[6] = USED
    right_region.append(seed_line)
    sum_angle = seed_line[4]
    sum_x = width - (seed_line[0] + seed_line[2]) / 2
    region_x = sum_x / len(right_region)
    region_angle = sum_angle / len(right_region)
    for line in right_lines:
        if line[6] == USED:
            continue
        x1, y1, x2, y2 = line[0], line[1], line[2], line[3]
        theta = line[4]
        length = line[5]
        avg_x = width - (x1 + x2) / 2
        if abs(region_angle - theta) <= threshold_angle and abs(region_x - avg_x) <= threshold_x:
            line[6] = USED
            right_region.append(line)
            sum_x += avg_x
            sum_angle += theta
            region_x = sum_x / len(right_region)
            region_angle = sum_angle / len(right_region)

    return right_region


def enhanceCurveFitting(left_points, right_points, height):
    if left_points.size > 8:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 2, height, 50)
    elif left_points.size in range(2, 9):
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 1, height, 50)
    else:
        left_points = []
    if right_points.size > 8:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, height, 50)
    elif right_points.size in range(2, 9):
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 1, height, 50)
    else:
        right_points = []
    return left_points, right_points


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
