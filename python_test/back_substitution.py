#coding=utf8
#http://answers.opencv.org/question/66084/using-backgroundsubtractormog2-for-images/

import numpy as np
import cv2
import sys


# both MOG and MOG2 can be used, with different parameter values
backgroundSubtractor = cv2.createBackgroundSubtractorMOG2()

#backgroundSubtractor = cv2.BackgroundSubtractorMOG(history=100, nmixtures=5, backgroundRatio=0.7, noiseSigma=0)
#backgroundSubtractor = cv2.BackgroundSubtractorMOG2(history=500, varThreshold=500)
#backgroundSubtractor = cv2.BackgroundSubtractorMOG2()

# apply the algorithm for background images using learning rate > 0
for i in range(1, -1):
    bgImageFile = "/home/anand/store/git/anandsaha/my.opencv.lab/images/DSC_0138.png" % i
    print ("Opening background", bgImageFile)
    bg = cv2.imread(bgImageFile)
    backgroundSubtractor.apply(bg, learningRate=0.5)

bgImageFile = "/home/anand/store/git/anandsaha/my.opencv.lab/images/DSC_0138.png" 
bg = cv2.imread(bgImageFile)
backgroundSubtractor.apply(bg, learningRate=0.5)

# apply the algorithm for detection image using learning rate 0
stillFrame = cv2.imread("/home/anand/store/git/anandsaha/my.opencv.lab/images/DSC_0138.png")
fgmask = backgroundSubtractor.apply(stillFrame, learningRate=0)

# show both images
cv2.imshow("original", cv2.resize(stillFrame, (0, 0), fx=0.5, fy=0.5))
cv2.imshow("mask", cv2.resize(fgmask, (0, 0), fx=0.5, fy=0.5))
cv2.waitKey()
cv2.destroyAllWindows()

