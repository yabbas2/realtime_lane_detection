import cv2
import numpy as np
import argparse

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str, default = 0,help="Your video")
arg.add_argument('-t', '--threading', type=str, default="no", help="Enable or disable Multi-thread mode")
args = vars(arg.parse_args())
'''**********************************'''


cv2.destroyAllWindows() #End of code