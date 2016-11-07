import cv2
import numpy as np

def HoughTransform(input, houghOutput, PHT=False, SHT=False):
    houghInput = cv2.cvtColor(input, cv2.COLOR_BGR2GRAY)
    houghInput = cv2.GaussianBlur(houghInput, (19, 19), 0)
    houghInput = cv2.adaptiveThreshold(houghInput, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 3, 2)
    # kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 1))
    # houghInput = cv2.morphologyEx(houghInput, cv2.MORPH_OPEN, kernel, iterations=1)
    # mu, sigma = cv2.meanStdDev(houghInput)
    # houghInput = cv2.Canny(houghInput, mu - sigma, mu + sigma)
    # using PHT
    if PHT:
        minLineLength = 60  # 50
        maxLineGap = 5  # 5
        maxVotes = 60  # 50
        lines = cv2.HoughLinesP(houghInput, 1, np.pi / 180, maxVotes, minLineLength, maxLineGap)
        for line in lines:
            for x1, y1, x2, y2 in line:
                if not (abs(x1 - x2) == 0):  # 0 and abs(x1 - x2) < ):  #catch vertical lines
                    continue
                print x1, x2, y1, y2  # debugging
                cv2.line(houghOutput, (x1, y1), (x2, y2), (0, 0, 255), 2)
    # using SHT
    if SHT:
        lines = cv2.HoughLines(houghInput, 1, np.pi / 180, 100)
        for line in lines:
            for rho, theta in line:
                if not (theta < 4 * np.pi / 180 and theta > -4 * np.pi / 180):  # between 100 degree and 80 degree
                    continue
                print rho, theta  # debugging
                a = np.cos(theta)
                b = np.sin(theta)
                x0 = a * rho
                y0 = b * rho
                x1 = int(x0 + 400 * (-b))
                y1 = int(y0 + 400 * (a))
                x2 = int(x0 - 400 * (-b))
                y2 = int(y0 - 400 * (a))
                cv2.line(houghOutput, (x1, y1), (x2, y2), (0, 0, 255), 1)
    return houghOutput