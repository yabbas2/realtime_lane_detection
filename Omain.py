import numpy as np
import argparse
from Streaming import VideoStreamOut, VideoStreamIn
import IPM
import gabor
import HT
import GTN
import LSD
import time

'''***********Arguments************'''
arg = argparse.ArgumentParser()
arg.add_argument('-v', '--video', type=str,help="Video source")
arg.add_argument('-s', '--show', type=str,help="Choose only one algorithm to be displayed")
arg.add_argument('-m', '--show2', type=str,help="Choose only two algorithms to be displayed")
args = vars(arg.parse_args())
'''**********************************'''

'''***************main code**************'''
print "[INFO] Start of streaming.."
'''*****************************Initialization*******************************'''
if args['show2'] is not None:
    frames = args['show2'].split(',')

if args['video'].startswith("sample7"):
    pts = np.array([[263, 142], [394, 142], [485, 209], [107, 208]], dtype="float32")
elif args['video'].startswith("sample8"):
    pts = np.array([[271, 247], [370, 247], [468, 281], [168, 281]], dtype="float32")
else:
    pts = np.array([[269, 206], [384, 206], [596, 297], [1, 297]], dtype="float32")

if args['video'] is not None:
    video_in = VideoStreamIn(src=args['video'])
else:
    video_in = VideoStreamIn(src=0)
video_out = VideoStreamOut()
video_in.start()
video_out.start()
'''**************************************************************************'''
'''********************************Algorithm*********************************'''
while True:
    '''**************************Start time**********************************'''
    start = time.time()
    '''**************************Reading Frames******************************'''
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
    ipmOutput = IPM.four_point_transform(ipmInput, pts)
    '''****************************Gabor Filter******************************'''
    gaborInput = ipmOutput
    gabor_filter = gabor.build_gabor_filter()
    gaborOutput = gabor.process(gaborInput, gabor_filter)
    '''****************Gaussian blur - threshold - noise removal*************'''
    gtnInput = gaborOutput.copy()#lsdOutput.copy()
    gtnOutput = GTN.process(gtnInput)
    '''***************************Hough Transform****************************'''
    try:
        houghInput1 = gtnOutput
        houghInput2 = ipmOutput.copy()
        houghOutput = HT.HoughTransform(houghInput1, houghInput2)
    except:
        pass
    '''**********************Line Segment Detector***************************'''
    lsdInput = houghOutput.copy()
    lsdOutput = LSD.LineSegmentDetector(lsdInput)
    '''*************************Final Step***********************************'''
    # @amal @amany: call the required function here - your input is houghOutput
    #finalOutput = IPM.inverse(ipmInput,houghOutput,pts)
    finalOutput = IPM.inverse(ipmInput, lsdOutput, pts)
    '''**************************Displaying Videos***************************'''
    display = {'gabor': gaborOutput, 'ht': houghOutput, 'lsd': lsdOutput, 'gtn': gtnOutput, 'ipm': ipmOutput,
               'original': frame, 'all': finalOutput}
    if args['show'] is not None:
        video_out.showFrame(display[args['show']])
    elif args['show2'] is not None:
        video_out.showTwoFrames(display[frames[0]], display[frames[1]])
    else:
        print 'No algorithm\'s output is selected!'
    '''**************************End time - Calculate time*******************'''
    end = time.time()
    print 'Time is:', end - start

#End of program