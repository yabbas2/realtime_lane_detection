import cv2
import numpy as np
import math


def determinePtsAndDst(args, ext):
    [width, height] = [int(args['res'].split('x')[0]), int(args['res'].split('x')[1])]
    dst = np.array([[0, 0], [height, 0], [height, width], [0, width]], dtype="float32")
    if args['video'].endswith("sample1"+ext):
        pts = np.array([[253, 219], [352, 219], [444, 288], [60, 288]], dtype="float32")
    elif args['video'].endswith("sample2"+ext):
        # pts = np.array([[197, 212], [326, 212], [348, 230], [106, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif args['video'].endswith("sample3"+ext):
        # pts = np.array([[220, 200], [322, 200], [361, 230], [85, 230]], dtype="float32")
        pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
    elif args['video'].endswith("sample4"+ext):
        # pts = np.array([[230, 211], [342, 211], [373, 238], [160, 238]], dtype="float32")
        # pts = np.array([[250, 222], [370, 222], [440, 290], [214, 290]], dtype="float32")
        pts = np.array([[290, 200], [334, 200], [440, 290], [214, 290]], dtype="float32")
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
    outputLines = list()
    for row in points:
        Z = 1 / (HomographyToOriginal[2][0] * row[0] + HomographyToOriginal[2][1] * row[1] + HomographyToOriginal[2][2])
        px1 = np.float32(((HomographyToOriginal[0][0] * row[0] + HomographyToOriginal[0][1] * row[1] +
                           HomographyToOriginal[0][2]) * Z))
        py1 = np.float32(((HomographyToOriginal[1][0] * row[0] + HomographyToOriginal[1][1] * row[1] +
                           HomographyToOriginal[1][2]) * Z))
        outputLines.append([px1, py1])
    assert size == len(outputLines)
    return np.array(outputLines)


def eliminateFalseDetection(lines):
    threshold_length = 60
    threshold_angle = range(80, 100)
    filtered_lines = []
    for line in lines:
        for x1, y1, x2, y2 in line:
            theta = cv2.fastAtan2((y2-y1), (x2-x1))
            if theta > 180:
                theta -= 180
            length = math.sqrt(math.pow((x2-x1), 2) + math.pow((y2-y1), 2))
            if int(theta) in threshold_angle and length >= threshold_length:
                filtered_lines.append([x1, y1, x2, y2])
    return filtered_lines


def combineLineSegments(lines, threshold_position, image):
    # lines = sorted(lines, key=lambda l: l[0], reverse=False)
    left_lines = []
    right_lines = []
    threshold_angle = 1
    threshold_distance = 30
    threshold_height = 5
    for lineI in lines:
        Ix1, Iy1, Ix2, Iy2 = lineI[0], lineI[1], lineI[2], lineI[3]
        Itheta = int(cv2.fastAtan2((Iy2-Iy1), (Ix2-Ix1)))
        if Itheta > 180:
            Itheta -= 180
        for lineJ in lines:
            if lineI == lineJ:
                continue
            Jx1, Jy1, Jx2, Jy2 = lineJ[0], lineJ[1], lineJ[2], lineJ[3]
            Jtheta = int(cv2.fastAtan2((Jy2 - Jy1), (Jx2 - Jx1)))
            if Jtheta > 180:
                Jtheta -= 180
            if abs(Jy1 - Iy1) <= threshold_height and abs(Jy2 - Iy2) <= threshold_height:
                if abs(Jx1-Ix1) <= threshold_distance and abs(Jx2-Ix2) <= threshold_distance and \
                        abs(Itheta-Jtheta) <= threshold_angle:
                    avg_x1 = (Jx1+Ix1)/2
                    avg_x2 = (Jx2+Ix2)/2
                    avg_y1 = Jy1
                    avg_y2 = Jy2
                    # print(image[int(avg_y1)][int(avg_x1)])
                    if image[int(avg_y1)][int(avg_x1)][0] in range(110, 255) and \
                            image[int(avg_y1)][int(avg_x1)][1] in range(110, 255) and \
                            image[int(avg_y1)][int(avg_x1)][2] in range(110, 255):
                        if avg_x1 < threshold_position and avg_x2 < threshold_position:
                            left_lines.append([avg_x1, avg_y1, avg_x2, avg_y2])
                        else:
                            right_lines.append([avg_x1, avg_y1, avg_x2, avg_y2])
                        lines.remove(lineJ)
            else:
                if abs(Jx1 - Ix2) <= threshold_distance and abs(Jx2 - Ix1) <= threshold_distance and \
                                abs(Itheta - Jtheta) <= threshold_angle:
                    avg_x1 = (Jx1 + Ix2) / 2
                    avg_x2 = (Jx2 + Ix1) / 2
                    avg_y1 = Jy1
                    avg_y2 = Jy2
                    # print(image[int(avg_y1)][int(avg_x1)])
                    if image[int(avg_y1)][int(avg_x1)][0] in range(110, 255) and \
                            image[int(avg_y1)][int(avg_x1)][1] in range(110, 255) and \
                            image[int(avg_y1)][int(avg_x1)][2] in range(110, 255):
                        if avg_x1 < threshold_position and avg_x2 < threshold_position:
                            left_lines.append([avg_x1, avg_y1, avg_x2, avg_y2])
                        else:
                            right_lines.append([avg_x1, avg_y1, avg_x2, avg_y2])
                        lines.remove(lineJ)

    left_points = []
    right_points = []
    # left_lines = sorted(left_lines, key=lambda l: l[1], reverse=True)
    for line in left_lines:
        left_points.append([line[0], line[1]])
        left_points.append([line[2], line[3]])
    # right_lines = sorted(right_lines, key=lambda l: l[1], reverse=True)
    for line in right_lines:
        right_points.append([line[0], line[1]])
        right_points.append([line[2], line[3]])

    return left_points, right_points
