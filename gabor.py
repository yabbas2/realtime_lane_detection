import numpy as np
import cv2
 
#def build_filters(x,y):  				#x,y dool kano lel tagreba 3shan a2dar ageeb a7san output .. hayetshalo aslun ^^

 #creating filters , in our case .. gabor filter
def build_gabor_filter():
 ksize = 31 #filter size

 #for theta in np.arange(0, np.pi, np.pi / 16): 	#de zeyada ... 3shan law 3ayz ageb kaza line lehom thetas mo5talefa hatetshal bardo ^^
 
 gabor_filter = cv2.getGaborKernel((ksize, ksize), 4.5,0, 10.0, 2, 0, ktype=cv2.CV_32F) #passing parameters to gabor
 gabor_filter /= 1.5*gabor_filter.sum() 
 return gabor_filter
 
 #function to apply the filter on image 
def process(img, filter):

 #filtered = np.zeros_like(img) 		#initializing matrix as the image size = 0s .. hatetshal

 filtered_img = cv2.filter2D(img, cv2.CV_8UC3, filter) #applying filter on image
 #np.maximum(filtered, fimg, filtered)  #kan beye5tar el maximum benha we been el filtered zero .. lel ta7seen bs mesh shayef far2 .. fa sheltaha 	
 return filtered_img
 

if __name__ == '__main__':
 import sys

 try:
    img_fn = sys.argv[1] 				#input image as argument 1 
 except:
    img_fn = 'test.png' 				#if there is no inputs .. take an test.png as the input
 
 img = cv2.imread(img_fn)
 if img is None:
    print 'Failed to load image file:', img_fn
 
 gabor_filter = build_gabor_filter()  	#build gabor
 res = process(img, gabor_filter)		#processing on image
 cv2.imshow('Original', img)			#printing source image
 cv2.imshow('Filtered', res)			#printing filtered image
 cv2.waitKey(0)							#waiting 0 to exit
 cv2.destroyAllWindows()				#close all windows at pushing 0 button