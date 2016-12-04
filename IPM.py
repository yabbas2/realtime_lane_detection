# python transform_example.py --image images/example_01.png --coords "[(73, 239), (356, 117), (475, 265), (187, 443)]"
# python transform_example.py --image images/example_02.png --coords "[(101, 185), (393, 151), (479, 323), (187, 441)]"
# python transform_example.py --image images/example_03.png --coords "[(63, 242), (291, 110), (361, 252), (78, 386)]"

# import the necessary packages

import numpy as np
import argparse
import cv2


def order_points(pts):
    # initialzie a list of coordinates that will be ordered
    # such that the first entry in the list is the top-left,
    # the second entry is the top-right, the third is the
    # bottom-right, and the fourth is the bottom-left
    rect = np.zeros((4, 2), dtype="float32")

    # the top-left point will have the smallest sum, whereas
    # the bottom-right point will have the largest sum
    s = pts.sum(axis=1)
    rect[0] = pts[np.argmin(s)]
    rect[2] = pts[np.argmax(s)]

    # now, compute the difference between the points, the
    # top-right point will have the smallest difference,
    # whereas the bottom-left will have the largest difference
    diff = np.diff(pts, axis=1)
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
    # 3x-coordiates or the top-right and top-left x-coordinates
    widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
    widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
    maxWidth = max(int(widthA), int(widthB))

    # compute the height of the new image, which will be the
    # maximum distance between the top-right and bottom-right
    # y-coordinates or the top-left and bottom-left y-coordinates
    heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
    heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
    maxHeight = max(int(heightA), int(heightB))
    h, w, channels = image.shape
    dst = np.array([[0, 0],
                [h, 0],
                [h, w],
                [0, w]], dtype="float32")
    # compute the perspective transform matrix and then apply it
    M = cv2.getPerspectiveTransform(pts, dst)
    H = cv2.getPerspectiveTransform(dst, pts)

    warped = cv2.warpPerspective(image, M, (h, w))
    #original2 = cv2.warpPerspective(warped, H, (w, h))
    #cv2.imshow("Original2", original2)
    #image = image - original2
    #o = cv2.bitwise_or(original2, image)
    #cv2.imshow("o", o)
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
                      [0, math.cos(theta), height*math.cos(theta)]])]
        '''
    Hom = np.array([
        [532.80992188, 0., 342.4952186],
        [0., 532.9334642, 233.8879292],
        [0., 0., 1.]], dtype="float32")

    ##########################################################################
    # compute the perspective transform matrix and then apply it
    #M = cv2.getPerspectiveTransform(pts, dst)
    #H = cv2.getPerspectiveTransform(dst, pts)

    #warped = cv2.warpPerspective(image, M, (w, h))
    #original2 = cv2.warpPerspective(warped, H, (maxWidth, maxHeight))
    #cv2.imshow("Original2", original2)
    #row, col = image.shape[:2]

    #image[(row / 2 + row / 7):row, 0:col] = original2
    cv2.imshow("Original", image)
    # cropped = image[0:w,  0:h]

    '''
        bottom= cropped[row-2:row, 0:col]
        mean= cv2.mean(bottom)[0]

        bordersize=10

        border=cv2.copyMakeBorder(cropped, top=bordersize, bottom=bordersize, left=bordersize, right=bordersize, borderType= cv2.BORDER_CONSTANT, value=[mean,mean,mean] )
        '''
    # cv2.imshow("cropped", cropped)
    # original2[0:row, 0:col] = cropped
    # cv2.imshow("o", original2)

    # returned = cropped + original2


    # return the warped image
    return warped


# construct the argument parse and parse the arguments
#ap = argparse.ArgumentParser()
#ap.add_argument("-i", "--image", help="path to the image file")
# ap.add_argument("-c", "--coords",
#	help = "comma seperated list of source points")
#args = vars(ap.parse_args())

# load the image and grab the source coordinates (i.e. the list of
# of (x, y) points)
# NOTE: using the 'eval' function is bad form, but for this example
# let's just roll with it -- in future posts I'll show you how to
# automatically determine the coordinates without pre-supplying them
image = cv2.imread("lane3.png")
# pts = np.array(eval(args["coords"]), dtype = "float32")

h, w, channels = image.shape
'''
pts = np.array([[0, h / 2],
                [w, h / 2],
                [w, h],
                [0, h]], dtype="float32")
'''
pts = np.array([[263, 142],
                [394, 142],
                [485, 209],
                [107, 208]], dtype="float32")
# apply the four point tranform to obtain a "birds eye view" of
# the image
warped = four_point_transform(image, pts)
# show the original and warped images


cv2.imshow("Warped", warped)

cv2.waitKey(0)
