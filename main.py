import cv2
import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor
import HT
import GTN

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
'''*****************************Initialization*******************'''
pts = np.array([[263, 142],
                [394, 142],
                [485, 209],
                [107, 208]], dtype="float32")
houghOutput = np.zeros((200, 200, 3), dtype=np.uint8)
if args['video'] is not None: #decide video source
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)
video_out = VideoStreamOut()
video_in.start()    #start thread of streaming in
video_out.start()   #start thread of streaming out
'''**************************************************************'''
'''********************************Algorithm*********************************'''
while True:
    '''**************************Reading Frames*******************************'''
    if video_in.streamFlag():
        video_out.setEndStream()
    if video_out.getShowFlag():
        video_in.stopStream()
        exit()
    frame = video_in.readFrame()
    if frame is None:
        continue
    '''*******************************IPM************************************'''
    ipmInput = frame
    # apply the four point transform to obtain a "birds eye view" of image
    ipmOutput = IPM.four_point_transform(ipmInput, pts)
    '''****************************Gabor Filter******************************'''
    gaborInput = ipmOutput
    gabor_filter = gabor.build_gabor_filter()  # build gabor filter
    gaborOutput = gabor.process(gaborInput, gabor_filter)  # processing on image
    '''****************Gaussian blur - threshold - noise removal*************'''
    gtnInput = gaborOutput
    gtnOutput = GTN.process(gtnInput)
    '''***************************Hough Transform****************************'''
    try:
        temp = ipmOutput
        houghOutput = HT.HoughTransform(gtnOutput, temp)
    except:
        pass
    '''**************************Displaying Videos***************************'''
    video_out.showFrame(frame, ipmOutput)

#End of program