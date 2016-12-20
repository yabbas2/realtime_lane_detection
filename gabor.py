import numpy as np
import cv2

def build_gabor_filter():
	filters = []
	ksize = 31
	for theta in np.arange(359, 361, 1):
		theta = theta % 360;
		gabor_filter = cv2.getGaborKernel((ksize, ksize), 4,theta, 10, 0, 0, ktype=cv2.CV_32F)
		gabor_filter /= 1.5*gabor_filter.sum()
		filters.append(gabor_filter)
	return filters

def process(img, filters):
	accum = np.zeros_like(img)
	for gabor_filter in filters:
		filtered_img = cv2.filter2D(img, cv2.CV_8UC3, gabor_filter)
		np.maximum(accum, filtered_img, accum)
	return accum