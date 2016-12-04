import numpy as np
import cv2

def build_gabor_filter():
    ksize = 31
    gabor_filter = cv2.getGaborKernel((ksize, ksize), 4,0, 10, 0, 0, ktype=cv2.CV_32F)
    gabor_filter /= 1.5*gabor_filter.sum()
    return gabor_filter
 

def process(img, filter):
    filtered_img = cv2.filter2D(img, cv2.CV_8UC3, filter)
    return filtered_img