import numpy as np
import cv2


def order_points(pts):
    rect = np.zeros((4, 2), dtype="float32")
    s = pts.sum(axis=1)
    rect[0] = pts[np.argmin(s)]
    rect[2] = pts[np.argmax(s)]
    diff = np.diff(pts, axis=1)
    rect[1] = pts[np.argmin(diff)]
    rect[3] = pts[np.argmax(diff)]
    return rect


def four_point_transform(image, pts):
    global h, w
    h, w, channels = image.shape
    dst = np.array([[0, 0],
                [h, 0],
                [h, w],
                [0, w]], dtype="float32")
    HomographyToInversed = cv2.getPerspectiveTransform(pts, dst)
    global HomographyToOriginal
    HomographyToOriginal = cv2.getPerspectiveTransform(dst, pts)
    warped = cv2.warpPerspective(image, HomographyToInversed, (h, w))
    return warped


def inverse(inputPts):
    outputPts = list()
    for row in inputPts:
        Z = 1 / (HomographyToOriginal[2][0] * row[0] + HomographyToOriginal[2][1] * row[1] + HomographyToOriginal[2][2])
        px1 = np.float32(((HomographyToOriginal[0][0] * row[0] + HomographyToOriginal[0][1] * row[1] + HomographyToOriginal[0][2]) * Z))
        py1 = np.float32(((HomographyToOriginal[1][0] * row[0] + HomographyToOriginal[1][1] * row[1] + HomographyToOriginal[1][2]) * Z))
        Z = 1 / (HomographyToOriginal[2][0] * row[2] + HomographyToOriginal[2][1] * row[3] + HomographyToOriginal[2][2])
        px2 = np.float32(((HomographyToOriginal[0][0] * row[2] + HomographyToOriginal[0][1] * row[3] + HomographyToOriginal[0][2]) * Z))
        py2 = np.float32(((HomographyToOriginal[1][0] * row[2] + HomographyToOriginal[1][1] * row[3] + HomographyToOriginal[1][2]) * Z))
        outputPts.append([px1, py1, px2, py2])
    return outputPts

def DrawLanes(lines, image):
    redFlag = True
    for line in lines:
        [x1, y1, x2, y2] = line
        try:
            if (x1 - x2) == 0:
                x = np.float32(x1)
                if redFlag: cv2.line(image, (x, y1), (x, h), (0, 0, 255), 6)
                else: cv2.line(image, (x, y1), (x, h), (0, 255, 0), 6)
            else:
                slope = (y2 - y1) / (x2 - x1)
                new_x2 = np.float32((h - y1) / slope + x1)
                if redFlag: cv2.line(image, (x1, y1), (new_x2, h), (0, 0, 255), 6)
                else: cv2.line(image, (x1, y1), (new_x2, h), (0, 255, 0), 6)
            redFlag = False
        except:
            pass
    return image



