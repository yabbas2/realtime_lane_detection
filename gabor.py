import numpy as np
import cv2

def build_gabor_filter():
    ksize = 31
    gabor_filter = cv2.getGaborKernel((ksize, ksize), 4,0, 10, 0, 0, ktype=cv2.CV_32F)
    gabor_filter /= 1.5*gabor_filter.sum()
    return gabor_filter
 

def process(img, filter):
    filtered_img = cv2.filter2D(img, cv2.CV_8UC3, filter)

    filtered_img = cv2.cvtColor(filtered_img, cv2.COLOR_BGR2GRAY)
    filtered_img = cv2.GaussianBlur(filtered_img, (19, 19), 0)
    filtered_img = cv2.adaptiveThreshold(filtered_img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 3, 2)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 1))
    filtered_img = cv2.morphologyEx(filtered_img, cv2.MORPH_OPEN, kernel, iterations=1)

    return filtered_img