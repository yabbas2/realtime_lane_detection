#!usrbinenv python
 
import numpy as np
import cv2
 
def build_filters(x,y):
 filters = []
 ksize = 31
 #for theta in np.arange(0, np.pi, np.pi / 16):
 kern = cv2.getGaborKernel((ksize, ksize), x,0, 10.0, y, 0, ktype=cv2.CV_32F)
 kern /= 1.5*kern.sum()
 filters.append(kern)
 return filters
 
def process(img, filters):
 accum = np.zeros_like(img)
 for kern in filters:
    fimg = cv2.filter2D(img, cv2.CV_8UC3, kern)
 np.maximum(accum, fimg, accum)
 return accum
 
if __name__ == '__main__':
 import sys
 
 #print __doc__
 try:
    img_fn = sys.argv[1]
 except:
    img_fn = '1.png'
 
 img = cv2.imread(img_fn)
 if img is None:
    print 'Failed to load image file:', img_fn
 #sys.exit(1)
 
 filters1 = build_filters(5,0.5)
 filters2 = build_filters(4.5,2)
 res1 = process(img, filters1)
 res2 = process(img, filters2)
 cv2.imshow('source image', img)
 cv2.imshow('segma 5', res1)
 cv2.imshow('segma 4.5', res2)
 cv2.waitKey(0)
 cv2.destroyAllWindows()