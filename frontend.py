import numpy as np
import cv2
import sys

frame0 = cv2.imread('images/frame0.png')
frame1 = cv2.imread('images/frame1.png')

im0 = cv2.cvtColor(frame0, cv2.COLOR_BGR2GRAY).astype(np.float32)
im1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY).astype(np.float32)

print(im0.shape[0])
print(im0.shape[1])

def print_2d_arr(arr):
    for y in range(arr.shape[0]):
        for x in range(arr.shape[1]):
            print arr[y, x],

print_2d_arr(im0)
print_2d_arr(im1)
