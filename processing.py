import cv2
import numpy as np
import numpy.polynomial.polynomial as poly
import math


def determinePtsAndDst(width, height, videoFile):
    dst = np.array([[0, 0], [height, 0], [height, width], [0, width]], dtype="float32")
    if videoFile == "sample1":
        pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
    elif videoFile == "sample2":
        # pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif videoFile == "sample3":
        # pts = np.array([[220, 200], [322, 200], [361, 230], [85, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif videoFile == "sample4":
        # pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
        # pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
        pts = np.array([[(350, 260), (460, 260), (560, 345), (280, 345)]], dtype="float32")
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
    out = cv2.GaussianBlur(out, (27, 27), 0)
    LSD = cv2.createLineSegmentDetector(cv2.LSD_REFINE_STD)
    lines, w, prec, nfa = LSD.detect(out)
    # inputFrame = LSD.drawSegments(inputFrame, lines)
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
    threshold_length = 60
    threshold_angle = range(70, 110)
    filtered_lines = []
    for line in lines:
        for x1, y1, x2, y2 in line:
            theta = cv2.fastAtan2((y2 - y1), (x2 - x1))
            if theta > 180:
                theta -= 180
            length = math.sqrt(math.pow((x2 - x1), 2) + math.pow((y2 - y1), 2))
            if int(theta) in threshold_angle and length >= threshold_length:
                if y1 > y2:
                    filtered_lines.append([x1, y1, x2, y2, theta, 0])
                else:
                    filtered_lines.append([x2, y2, x1, y1, theta, 0])
    return filtered_lines


def combineLineSegments(lines, image):
    left_points = []
    right_points = []
    threshold_position = int(image.shape[1] / 2)
    USED = 1
    threshold_angle = 2
    threshold_distance = 30
    for lineI in lines:
        if lineI[5] == USED:
            continue
        Ix1, Iy1, Ix2, Iy2 = lineI[0], lineI[1], lineI[2], lineI[3]
        Itheta = lineI[4]
        for lineJ in lines:
            if lineI == lineJ or lineJ[5] == USED:
                continue
            Jx1, Jy1, Jx2, Jy2 = lineJ[0], lineJ[1], lineJ[2], lineJ[3]
            Jtheta = lineJ[4]
            if abs(Jx1 - Ix1) <= threshold_distance and abs(Jx2 - Ix2) <= threshold_distance and \
                    abs(Itheta - Jtheta) <= threshold_angle:
                avg_x1 = (Jx1 + Ix1) / 2
                avg_x2 = (Jx2 + Ix2) / 2
                avg_y1 = Jy1
                avg_y2 = Jy2
                # print(image[image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)])
                if image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)][0] in range(110, 255) and \
                        image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)][1] in range(110, 255) and \
                        image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)][2] in range(110, 255):
                    if avg_x1 < threshold_position and avg_x2 < threshold_position:
                        left_points.append((avg_x1, avg_y1))
                        left_points.append((avg_x2, avg_y2))
                    else:
                        right_points.append((avg_x1, avg_y1))
                        right_points.append((avg_x2, avg_y2))
                    lineI[5] = USED
                    lineJ[5] = USED

    return np.array(left_points), np.array(right_points)


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


def enhanceCurveFitting(left_points, right_points, ipmFrame):
    if left_points.size > 4:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 2, ipmFrame.shape[0], 50)
    elif left_points.size == 4:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 1, ipmFrame.shape[0], 50)
    else:
        left_points = []
    if right_points.size > 4:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, ipmFrame.shape[0], 50)
    elif right_points.size == 4:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 1, ipmFrame.shape[0], 50)
    else:
        right_points = []
    return left_points, right_points
