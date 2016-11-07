import cv2

def build_gabor_filter():
    ksize = 31  # filter size

    # for theta in np.arange(0, np.pi, np.pi / 16): 	#de zeyada ... 3shan law 3ayz ageb kaza line lehom thetas mo5talefa hatetshal bardo ^^

    gabor_filter = cv2.getGaborKernel((ksize, ksize), 4.5, 0, 10.0, 2, 0, ktype=cv2.CV_32F)  # passing parameters to gabor
    gabor_filter /= 1.5 * gabor_filter.sum()
    return gabor_filter


# function to apply the filter on image
def process(img, filter):
    # filtered = np.zeros_like(img) 		#initializing matrix as the image size = 0s .. hatetshal

    filtered_img = cv2.filter2D(img, cv2.CV_8UC3, filter)  # applying filter on image
    # np.maximum(filtered, fimg, filtered)  #kan beye5tar el maximum benha we been el filtered zero .. lel ta7seen bs mesh shayef far2 .. fa sheltaha
    return filtered_img