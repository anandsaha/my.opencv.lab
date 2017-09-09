import numpy as np
import cv2
from matplotlib import pyplot as plt
import math


orig_image = cv2.imread(r'3.jpg', 0)
cv2.imshow("Image", orig_image)
cv2.waitKey(0)
cv2.imwrite('asdf.jpg', orig_image)


