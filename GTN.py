import cv2

def process(input):
    out= cv2.cvtColor(input, cv2.COLOR_BGR2GRAY)
    out = cv2.GaussianBlur(out, (19, 19), 0)
    out = cv2.adaptiveThreshold(out, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 3, 2)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 1))
    out = cv2.morphologyEx(out, cv2.MORPH_OPEN, kernel, iterations=1)
    return out