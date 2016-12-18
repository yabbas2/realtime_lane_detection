import cv2
import numpy as np

def LineSegmentDetector(input):
    h, w = input.shape[:2]
    out = cv2.cvtColor(input.copy(), cv2.COLOR_BGR2GRAY)
    LSD = cv2.createLineSegmentDetector(cv2.LSD_REFINE_STD)
    lines, width, prec, nfa = LSD.detect(out)
    new_image = input.copy()#np.zeros_like(input)
    filtered_lines = list()
    for line in lines:
        for x1, y1, x2, y2 in line:
            if abs(y1 - y2) < 150:
                continue
            #print x1, y1, x2, y2
            filtered_lines.append([x1, y1, x2, y2])
    '''for i in range(10, w-10, 40):
        counter = int()
        sum_x1 = float()
        sum_x2 = float()
        for x in range(i, i + 40, 1):
            flag, x_1, x_2 = searchForLine(x, filtered_lines)
            if flag:
                counter += 1
                sum_x1 += x_1
                sum_x2 += x_2
        if counter > 0:
            avg_x1 = sum_x1 / counter
            avg_x2 = sum_x2 / counter
            cv2.line(new_image, (int(avg_x1), 0), (int(avg_x2), h), (0, 0, 255), 6)'''
    #print lines
    filtered_lines = np.array(filtered_lines)
    new_image = LSD.drawSegments(new_image, filtered_lines)
    return new_image

def searchForLine(x, filtered_lines):
    for line in filtered_lines:
        if int(line[0]) == x:
            return True, line[0], line[2]
    return False, 0, 0