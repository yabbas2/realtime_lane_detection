import cv2

def process(input):
    out = cv2.cvtColor(input, cv2.COLOR_BGR2GRAY)
    out = cv2.GaussianBlur(out, (11, 11), 0)
    out = cv2.adaptiveThreshold(out, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 5, 4)
    erosion_kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 12))
    dilation_kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 15))
    out = cv2.erode(out, erosion_kernel, iterations=1)
    out = cv2.dilate(out, dilation_kernel, iterations=1)
    mu, sigma = cv2.meanStdDev(out)
    out = cv2.Canny(out, mu - sigma, mu + sigma, L2gradient=True)
    return out