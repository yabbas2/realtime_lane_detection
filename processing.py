import cv2
import numpy as np
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
    left_lines = []
    right_lines = []
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
                avg_theta = (Jtheta + Itheta) / 2
                # print(image[int(avg_y1)][int(avg_x1)])
                if image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)][0] in range(110, 255) and \
                        image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)][1] in range(110, 255) and \
                        image[int((avg_y1 + avg_y2) / 2)][int((avg_x1 + avg_x2) / 2)][2] in range(110, 255):
                    if avg_x1 < threshold_position and avg_x2 < threshold_position:
                        left_lines.append([avg_x1, avg_y1, avg_x2, avg_y2, avg_theta])
                    else:
                        right_lines.append([avg_x1, avg_y1, avg_x2, avg_y2, avg_theta])
                    lines[i][4] = USED
                    lines[j][4] = USED

    left_points = []
    right_points = []
    if len(left_lines) > 1:
        sum_theta = 0
        for line in left_lines:
            sum_theta += ((line[3] - line[1]) / (line[2] - line[0]))
        avg_theta = int(sum_theta / len(left_lines))
        if abs(avg_theta - math.tan(left_lines[0][4])) <= math.tan(threshold_angle):
            # slope = (left_lines[0][3]-left_lines[0][1]) / (left_lines[0][2]-left_lines[0][0])
            new_x1 = (0 - left_lines[0][1]) / avg_theta + left_lines[0][0]
            new_x2 = (image.shape[0] - left_lines[0][1]) / avg_theta + left_lines[0][0]
            left_points = [[new_x1, 0], [new_x2, image.shape[0]]]
        else:
            for line in left_lines:
                left_points.append([line[0], line[1]])
                left_points.append([line[2], line[3]])
    elif len(left_lines) == 1:
        left_points.append([left_lines[0][0], left_lines[0][1]])
        left_points.append([left_lines[0][2], left_lines[0][3]])

    if len(right_lines) > 1:
        sum_theta = 0
        for line in right_lines:
            sum_theta += ((line[3] - line[1]) / (line[2] - line[0]))
        avg_theta = int(sum_theta / len(right_lines))
        if abs(avg_theta - math.tan(right_lines[0][4])) <= math.tan(threshold_angle):
            new_x1 = (0 - right_lines[0][1]) / avg_theta + right_lines[0][0]
            new_x2 = (image.shape[0] - right_lines[0][1]) / avg_theta + right_lines[0][0]
            right_points = [[new_x1, 0], [new_x2, image.shape[0]]]
        else:
            for line in right_lines:
                right_points.append([line[0], line[1]])
                right_points.append([line[2], line[3]])
    elif len(right_lines) == 1:
        right_points.append([right_lines[0][0], right_lines[0][1]])
        right_points.append([right_lines[0][2], right_lines[0][3]])

    return left_points, right_points
