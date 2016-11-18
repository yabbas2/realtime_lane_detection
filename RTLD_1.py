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
arg.add_argument('-r', '--res', type=str,help="Width and Height (resolution) of video frames")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
'''*****************************Initialization*******************'''
res = args['res'].split('x')
h = int(res[1])
w = int(res[0])
pts = np.array([[0, h / 2],
                [w, h / 2],
                [w, h],
                [0, h]], dtype="float32")
houghOutput = np.zeros((h, w, 3), dtype=np.uint8)
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
    '''***************************Hough Transform****************************'''
    temp = ipmOutput
    if args['use'] == 'PHT':
        houghOutput = HT.HoughTransform(gaborOutput, temp, PHT=True, SHT=False)
    elif args['use'] == 'SHT':
        houghOutput = HT.HoughTransform(gaborOutput, temp, PHT=False, SHT=True)
    '''**************************Displaying Videos***************************'''
    video_out.showFrame(frame, houghOutput)

#End of program