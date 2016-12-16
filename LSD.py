import cv2
import numpy as np

def LineSegmentDetector(input):
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
    #print lines
    filtered_lines = np.array(filtered_lines)
    new_image = LSD.drawSegments(new_image, filtered_lines)
    return new_image
