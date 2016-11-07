import cv2
import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor
import HT


'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
arg.add_argument('-u', '--use', type=str,help="Type of Hough transform: Standard or Probabilistic")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
if args['video'] is not None: #decide video source
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)
video_out = VideoStreamOut()
video_in.start()    #start thread of streaming in
video_out.start()   #start thread of streaming out
while True:
    if video_in.streamFlag():
        video_out.setEndStream()
    if video_out.getShowFlag():
        video_in.stopStream()
        exit()
    frame = video_in.readFrame()
    if frame is None:
        continue
    '''********************************Algorithm*********************************'''
    '''***********************************IPM************************************'''
    ipmInput = frame
    h, w, channels = frame.shape
    pts = np.array([[0, h / 2],
                    [w, h / 2],
                    [w, h],
                    [0, h]], dtype="float32")
    # apply the four point transform to obtain a "birds eye view" of image
    ipmOutput = IPM.four_point_transform(ipmInput, pts)
    '''********************************Gabor Filter******************************'''
    gaborInput = ipmOutput
    gabor_filter = gabor.build_gabor_filter()  # build gabor filter
    gaborOutput = gabor.process(gaborInput, gabor_filter)  # processing on image
    '''*******************************Hough Transform****************************'''
    temp = ipmOutput
    if args['use'] == 'PHT':
        houghOutput = HT.HoughTransform(gaborOutput, temp, PHT=True, SHT=False)
    elif args['use'] == 'SHT':
        houghOutput = HT.HoughTransform(gaborOutput, temp, PHT=False, SHT=True)
    '''******************************************************************************'''
    video_out.showFrame(houghOutput)


#End of program