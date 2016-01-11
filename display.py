import sys
import cv2
import numpy as np
frame0 = cv2.imread('images/small_camera.png')
hsv = np.zeros_like(frame0)
cv2.namedWindow('flow')
cv2.moveWindow('flow',640,0)

while True:
    u = np.fromstring(sys.stdin.readline(), sep=" ")
    v = np.fromstring(sys.stdin.readline(), sep=" ")
    print(frame0.shape)
    print(u.shape)
    u = u.reshape(frame0.shape[0:2])
    v = v.reshape(frame0.shape[0:2])
    mag, ang = cv2.cartToPolar(u, v)
    mag = cv2.normalize(mag, None, 0, 255, cv2.NORM_MINMAX)
    ang = ang*180/np.pi/2
    hsv[..., 1] = 255
    hsv[..., 0] = ang
    hsv[..., 2] = mag
    flow = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
    cv2.imshow('flow', flow)
    cv2.waitKey(500)
