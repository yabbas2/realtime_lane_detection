import cv2
import numpy as np
import numpy.polynomial.polynomial as poly
import math


def determinePtsAndDst(args, ext):
    [width, height] = [int(args['res'].split('x')[0]), int(args['res'].split('x')[1])]
    dst = np.array([[0, 0], [height, 0], [height, width], [0, width]], dtype="float32")
    if args['video'].endswith("sample1" + ext):
        pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
    elif args['video'].endswith("sample2" + ext):
        # pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif args['video'].endswith("sample3" + ext):
        # pts = np.array([[220, 200], [322, 200], [361, 230], [85, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif args['video'].endswith("sample4" + ext):
        # pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif args['video'].endswith("sample5" + ext):
        # pts = np.array([[241, 208], [345, 208], [450, 297], [23, 297]], dtype="float32")
        pts = np.array([[260, 196], [354, 196], [442, 280], [204, 280]], dtype="float32")

    else:
        pts = np.zeros((4, 2), dtype="float32")
    return pts, dst


def fourPointTransform(normalFrame, pts, dst):
    height, width = normalFrame.shape[:2]
    HomographyToInv = cv2.getPerspectiveTransform(pts, dst)
    HomographyToOriginal = cv2.getPerspectiveTransform(dst, pts)
    ipmFrame = cv2.warpPerspective(normalFrame, HomographyToInv, (height, width+100))
    return ipmFrame, HomographyToOriginal


def lineSegmentDetector(inputFrame):
    out = cv2.cvtColor(inputFrame, cv2.COLOR_BGR2GRAY)
    out = cv2.GaussianBlur(out, (27, 27), 0)
    LSD = cv2.createLineSegmentDetector(cv2.LSD_REFINE_STD)
    lines, w, prec, nfa = LSD.detect(out)
    # lines = np.array(lines)
    # inputFrame = LSD.drawSegments(inputFrame, lines)
    if lines is None:
        return []
    return lines


def doInverse(points, HomographyToOriginal):
    size = len(points)
    outputPoints = list()
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
    threshold_angle = range(80, 100)
    filtered_lines = []
    for line in lines:
        for x1, y1, x2, y2 in line:
            theta = cv2.fastAtan2((y2 - y1), (x2 - x1))
            if theta > 180:
                theta -= 180
            length = math.sqrt(math.pow((x2 - x1), 2) + math.pow((y2 - y1), 2))
            if int(theta) in threshold_angle and length >= threshold_length:
                if y1 > y2:
                    filtered_lines.append([x1, y1, x2, y2, 0])
                else:
                    filtered_lines.append([x2, y2, x1, y1, 0])
    return filtered_lines


def combineLineSegments(lines, image):
    left_points = []
    right_points = []
    threshold_position = int(image.shape[1] / 2)
    USED = 1
    threshold_angle = 2
    threshold_distance = 30
    for i in range(0, len(lines), 1):
        if lines[i][4] == USED:
            continue
        Ix1, Iy1, Ix2, Iy2 = lines[i][0], lines[i][1], lines[i][2], lines[i][3]
        Itheta = int(cv2.fastAtan2((Iy2 - Iy1), (Ix2 - Ix1)))
        if Itheta > 180:
            Itheta -= 180
        for j in range(0, len(lines), 1):
            if lines[i] == lines[j] or lines[j][4] == USED:
                continue
            Jx1, Jy1, Jx2, Jy2 = lines[j][0], lines[j][1], lines[j][2], lines[j][3]
            Jtheta = int(cv2.fastAtan2((Jy2 - Jy1), (Jx2 - Jx1)))
            if Jtheta > 180:
                Jtheta -= 180
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
                    lines[i][4] = USED
                    lines[j][4] = USED

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


def draw(points, image):
    cv2.polylines(image, np.int32([points]), False, (0, 0, 255), 2, cv2.LINE_AA)


def enhanceCurveFitting(left_points, right_points, homo, ipmFrame):
    if left_points.size > 4:
        left_points = curveFit(left_points[:, 0], left_points[:, 1], 1, ipmFrame.shape[0], 10)
        left_points = doInverse(left_points, homo)
    elif left_points.size == 4:
        deltaY = abs(left_points[0, 1] - left_points[1, 1])
        if deltaY > 200:
            left_points = curveFit(left_points[:, 0], left_points[:, 1], 2, ipmFrame.shape[0], 10)
            left_points = doInverse(left_points, homo)
        else:
            left_points = []
    else:
        left_points = []
    if right_points.size > 4:
        right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, ipmFrame.shape[0], 10)
        right_points = doInverse(right_points, homo)
    elif right_points.size == 4:
        deltaY = abs(right_points[0, 1] - right_points[1, 1])
        if deltaY > 200:
            right_points = curveFit(right_points[:, 0], right_points[:, 1], 2, ipmFrame.shape[0], 10)
            right_points = doInverse(right_points, homo)
        else:
            right_points = []
    else:
        right_points = []
    return left_points, right_points
