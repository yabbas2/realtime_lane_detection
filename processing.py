import cv2
import numpy as np
import numpy.polynomial.polynomial as poly
import math

left_kalmans = []
right_kalmans = []
for i in range(0, 20):
    left_kalmans.append(cv2.KalmanFilter(4, 2))
for i in range(0, 20):
    right_kalmans.append(cv2.KalmanFilter(4, 2))
for kalman in left_kalmans:
    kalman.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
    kalman.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32)
    kalman.processNoiseCov = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32) * 0.0003
for kalman in right_kalmans:
    kalman.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
    kalman.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32)
    kalman.processNoiseCov = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32) * 0.0003


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
    elif videoFile == "sample6":
        pts = np.array([[350, 270], [560, 270], [700, 385], [260, 385]], dtype="float32")
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
    for lineI in left_temp:
        if FOUND:
            break
        for lineJ in left_temp:
            if lineI == lineJ:
                continue
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
    for lineI in right_temp:
        if FOUND:
            break
        for lineJ in right_temp:
            if lineI == lineJ:
                continue
            if abs(lineI[0] - lineJ[0]) <= 20 and abs(lineI[1] - lineJ[1]) <= 5:
                right_seed_line = lineI
                FOUND = 1
                break
    if left_seed_line == 0:
        for line in lines:
            if line[0] < width / 2:
                left_seed_line = line
                break
    if right_seed_line == 0:
        for line in lines:
            if line[0] > width / 2:
                right_seed_line = line
                break

    return left_seed_line, right_seed_line


def leftRegionGrowing(lines, seed_line):
    if seed_line == 0:
        return []
    left_region = []
    threshold_angle = 10
    threshold_x = 40
    USED = 1
    left_lines = sorted(lines, key=lambda l: l[1], reverse=True)
    seed_line[6] = USED
    left_region.append(seed_line)
    sum_angle = seed_line[4]
    region_x = seed_line[2]
    region_angle = sum_angle / len(left_region)
    for line in left_lines:
        if line[6] == USED:
            continue
        x1, y1, x2, y2 = line[0], line[1], line[2], line[3]
        theta = line[4]
        length = line[5]
        avg_x = x1
        if abs(region_angle - theta) <= threshold_angle:
            if abs(region_x - avg_x) <= threshold_x:
                line[6] = USED
                left_region.append(line)
                sum_angle += theta
                region_x = x2
                region_angle = sum_angle / len(left_region)
            elif abs(region_x - x2) <= 20:  # note: we can check for delta y too.
                line[6] = USED
                left_region.append(line)
                sum_angle += theta
                region_x = x2
                region_angle = sum_angle / len(left_region)

    return left_region


def rightRegionGrowing(lines, seed_line):
    if seed_line == 0:
        return []
    right_region = []
    threshold_angle = 10
    threshold_x = 40
    USED = 1
    right_lines = sorted(lines, key=lambda l: l[1], reverse=True)
    seed_line[6] = USED
    right_region.append(seed_line)
    sum_angle = seed_line[4]
    region_x = seed_line[2]
    region_angle = sum_angle / len(right_region)
    for line in right_lines:
        if line[6] == USED:
            continue
        x1, y1, x2, y2 = line[0], line[1], line[2], line[3]
        theta = line[4]
        length = line[5]
        avg_x = x1
        if abs(region_angle - theta) <= threshold_angle:
            if abs(region_x - avg_x) <= threshold_x:
                line[6] = USED
                right_region.append(line)
                sum_angle += theta
                region_x = x2
                region_angle = sum_angle / len(right_region)
            elif abs(region_x - x2) <= 20:  # note: we can check for delta y too.
                line[6] = USED
                right_region.append(line)
                sum_angle += theta
                region_x = x2
                region_angle = sum_angle / len(right_region)

    return right_region


def enhanceCurveFitting(left_points, right_points, height):
    if left_points.size > 8:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 2, height, 20)
    elif left_points.size in range(2, 9):
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 1, height, 20)
    else:
        left_points = []
    if right_points.size > 8 and right_points.size != 0:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, height, 20)
    elif right_points.size in range(2, 9):
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 1, height, 20)
    else:
        right_points = []
    return left_points, right_points


def curveFit(points_x, points_y, degree, height, pointsNum):
    points = []
    coeffs = poly.polyfit(points_y, points_x, degree)
    ffit = poly.Polynomial(coeffs)
    points_y = np.linspace(100, height-100, pointsNum)
    points_x = ffit(points_y)
    for i in range(0, len(points_y), 1):
        points.append([points_x[i], points_y[i]])
    return points


def removeShadowPoints(hsvFrame, ipmFrame, points):
    filteredPoints = []
    boundaries = [([0, 100, 0], [154, 255, 154])]
    for (lower, upper) in boundaries:
        # create NumPy arrays from the boundaries
        lower = np.array(lower, dtype="uint8")
        upper = np.array(upper, dtype="uint8")
    mask = cv2.inRange(hsvFrame, lower, upper)
    ipmFrame = cv2.bitwise_and(ipmFrame, ipmFrame, mask=mask)
    im2, contours, hierarchy = cv2.findContours(mask, 1, 2)
    cnts = sorted(contours, key=cv2.contourArea, reverse=True)
    cv2.drawContours(hsvFrame, cnts, 0, (0, 255, 255), 2)
    for x, y in points:
        flag = 0
        if(cnts == []):
            return points
        for cnt in cnts:
            dist = cv2.pointPolygonTest(cnt, (x, y), True)
            if( -17 <= dist <= 2):
                flag = 1
        if flag == 1:
            continue
        else:
            filteredPoints.append([x, y])
    return np.array(filteredPoints)


def removeShadow(hsvFrame, ipmFrame, lines):
    filteredLines = []
    boundaries = [([0, 100, 0], [154, 255, 154])]
    for (lower, upper) in boundaries:
        # create NumPy arrays from the boundaries
        lower = np.array(lower, dtype="uint8")
        upper = np.array(upper, dtype="uint8")
    mask = cv2.inRange(hsvFrame, lower, upper)
    ipmFrame = cv2.bitwise_and(ipmFrame, ipmFrame, mask=mask)
    im2, contours, hierarchy = cv2.findContours(mask, 1, 2)
    cnts = sorted(contours, key=cv2.contourArea, reverse=True)
    cv2.drawContours(hsvFrame, cnts, 0, (0, 255, 255), 2)
    for line in lines:
        flag = 0
        if (cnts == []):
            return lines
        for cnt in cnts:
            dist = cv2.pointPolygonTest(cnt, (line[0], line[1]), True)
            dist2 = cv2.pointPolygonTest(cnt, (line[2], line[3]), True)
            if (-17 <= dist or -17 <= dist2):
                flag = 1
        if flag == 1:
            continue
        else:
            filteredLines.append([int(line[0]), int(line[1]), int(line[2]), int(line[3]), line[4], line[5], line[6]])
    return filteredLines


def debug_draw(points, image):
    if len(points) == 0:
        return
    # if status == "s":
    #     cv2.polylines(image, np.int32([points]), False, (0, 0, 255), 2, cv2.LINE_AA)
    # elif status == "d":
    cv2.polylines(image, np.int32([points]), False, (255, 127, 127), 2, cv2.LINE_AA)


def isDashed(lines, seedLine):
    if len(lines) == 0 or seedLine == 0:
        return False
    yThreshold = 150
    dashed = 0
    solid = 0
    lines = sorted(lines, key=lambda l: l[1], reverse=True)
    y1 = seedLine[1]
    y2 = seedLine[3]
    for line in lines:
        y1c = line[1]
        y2c = line[3]
        if abs(y1 - y1c) < yThreshold:
            continue
        elif abs(y2 - y1c) < yThreshold:
            solid += 1
        else:
            dashed += 1
        y1 = y1c
        y2 = y2c
    if solid > dashed or dashed == 0:
        return False
    else:
        return True


def kalman(points, s):
    global left_kalmans
    global right_kalmans
    if s == "l":
        kalmans = left_kalmans
    else:
        kalmans = right_kalmans
    points = points.tolist()
    predicted_points = []
    mp = np.array((2, 1), np.float32)  # measurement

    if kalmans[0].statePre[0][0] == 0:
        for i in range(0, 20):
            for ii in range(0, 50):
                mp = np.array([[np.float32(points[i][0])], [np.float32(points[i][1])]])
                kalmans[i].correct(mp)
                tp = kalmans[i].predict()

    for i in range(0, 20):
        x, y = points[i][0], points[i][1]
        mp = np.array([[np.float32(x)], [np.float32(y)]])
        kalmans[i].correct(mp)
        tp = kalmans[i].predict()
        predicted_points.append((tp[0], tp[1]))
    return np.array(predicted_points)


def eliminateFalseDetection2(lines, mask, height, width):
    filtered_lines = []
    flag = 0
    for line in lines:
        x1 = line[0]
        y1 = line[1]
        for i in range(0, 50):
            if (x1+i > width-1) or (x1-i < 0) or (y1+i > height-1) or (y1-i < 0):
                break
            if mask[int(y1)][int(x1)-i] == 0 or mask[int(y1)][int(x1)+i] == 0 or mask[int(y1)-i][int(x1)] == 0 or mask[int(y1)+i][int(x1)] == 0:
                flag = 1
                break
        if not flag:
            filtered_lines.append(line)
        flag = 0
    return filtered_lines

