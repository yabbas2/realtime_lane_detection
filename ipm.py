# python transform_example.py --image path of image


# import the necessary packages

import numpy as np
import argparse
import cv2


def order_points(pts):
	# initialzie a list of coordinates that will be ordered
	# such that the first entry in the list is the top-left,
	# the second entry is the top-right, the third is the
	# bottom-right, and the fourth is the bottom-left
	rect = np.zeros((4, 2), dtype = "float32")
 
	# the top-left point will have the smallest sum, whereas
	# the bottom-right point will have the largest sum
	s = pts.sum(axis = 1)
	rect[0] = pts[np.argmin(s)]
	rect[2] = pts[np.argmax(s)]
 
	# now, compute the difference between the points, the
	# top-right point will have the smallest difference,
	# whereas the bottom-left will have the largest difference
	diff = np.diff(pts, axis = 1)
	rect[1] = pts[np.argmin(diff)]
	rect[3] = pts[np.argmax(diff)]
 
	# return the ordered coordinates
	return rect

def four_point_transform(image, pts):
	# obtain a consistent order of the points and unpack them
	# individually
	rect = order_points(pts)
	(tl, tr, br, bl) = rect

	# compute the width of the new image, which will be the
	# maximum distance between bottom-right and bottom-left
	# x-coordiates or the top-right and top-left x-coordinates
	#widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
	#widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
	#maxWidth = max(int(widthA), int(widthB))

	# compute the height of the new image, which will be the
	# maximum distance between the top-right and bottom-right
	# y-coordinates or the top-left and bottom-left y-coordinates
	#heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
	#heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
	#maxHeight = max(int(heightA), int(heightB))
        h, w = image.shape[:2]
	# now that we have the dimensions of the new image, construct
	# the set of destination points to obtain a "birds eye view",
	# (i.e. top-down view) of the image, again specifying points
	# in the top-left, top-right, bottom-right, and bottom-left
	# order
	dst = np.array([
		[-w/10, h/2],
		[w+(w/10), h/2],
		[w-(w/3), h],
		[w/3, h]], dtype = "float32")
	
	##########################################################################
	'''
	phi = 0
	theta = 0.6
	height = 200
	fx = 90
	fy = 300
	cx = 379
	cy = 0
	C = np.array([[fx, 0, cx], [0, fy, cy], [0, 0, 1]])
	X = np.array([[math.cos(phi), math.sin(phi)*math.sin(theta), height*math.sin(phi)*math.sin(theta)],
                      [-(math.sin(phi)), math.cos(phi)*math.sin(theta), -height*math.cos(phi)*math.sin(theta)],
                      [0, math.cos(theta), height*math.cos(theta)]])
	Hom = C*X
	'''
	########################################################################## 
	# compute the perspective transform matrix and then apply it
	M = cv2.getPerspectiveTransform(pts, dst)
	H = cv2.getPerspectiveTransform(dst, pts)
	
	
	warped = cv2.warpPerspective(image, M, (w, h))
	original2 = cv2.warpPerspective(warped, H, (w, h))
	cv2.imshow("o", original2)
	# return the warped image
	return warped

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image file")
#ap.add_argument("-c", "--coords",
#	help = "comma seperated list of source points")
args = vars(ap.parse_args())

# load the image and grab the source coordinates (i.e. the list of
# of (x, y) points)
# NOTE: using the 'eval' function is bad form, but for this example
# let's just roll with it -- in future posts I'll show you how to
# automatically determine the coordinates without pre-supplying them
image = cv2.imread(args["image"])
#pts = np.array(eval(args["coords"]), dtype = "float32")

h, w, channels = image.shape
pts = np.array([[0, h/2 + h/7],
		[w, h/2 + h/7],
		[w, h],
		[0, h]], dtype = "float32")

# apply the four point tranform to obtain a "birds eye view" of
# the image
warped = four_point_transform(image, pts)
# show the original and warped images

cv2.imshow("Original", image)
cv2.imshow("Warped", warped)
cv2.waitKey(0)

