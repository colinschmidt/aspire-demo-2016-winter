import hindemith as hm
from hindemith.types import hmarray
from hindemith.core import compose
from hindemith.operations.array import Square
from hindemith.operations.convolve import Convolve2D
import numpy as np
import cv2
from scipy.ndimage.filters import convolve

alpha = 15.0

jacobi = np.array([
    [1.0/12.0, 1.0/6.0, 1.0/12.0],
    [1.0/6.0, 0.0, 1.0/6.0],
    [1.0/12.0, 1.0/6.0, 1.0/12.0]
])

dx = np.array([[-1.0/12.0, -2.0/3.0, 0.0, 2.0/3.0, 1.0/12.0]])
dy = np.array([[-1.0/12.0], [-2.0/3.0], [0.0], [2.0/3.0], [1.0/12.0]])


def np_hs_jacobi(im0, im1, u, v):
    It = im1 - im0
    Iy = convolve(im1, dy)
    Ix = convolve(im1, dx)
    denom = np.square(Ix) + np.square(Iy) + alpha ** 2

    for _ in range(100):
        ubar = convolve(u, jacobi)
        vbar = convolve(v, jacobi)
        t = (Ix * ubar + Iy * vbar + It) / denom
        u = ubar - Ix * t
        v = vbar - Iy * t
    return u, v


alpha2 = alpha ** 2
epsilon = .01

@compose
def hs_jacobi(im0, im1, u, v):
    It = im1 - im0
    Iy = Convolve2D(im1, dy)
    Ix = Convolve2D(im1, dx)
    denom = Square(Ix) + Square(Iy) + alpha2

    for _ in range(100):
        ubar = Convolve2D(u, jacobi)
        vbar = Convolve2D(v, jacobi)
        t = (Ix * ubar + Iy * vbar + It) / denom
        u_new = ubar - Ix * t
        v_new = vbar - Iy * t
        u, v = u_new, v_new
    return u, v


def solve():
    frame0 = cv2.imread('images/frame0.png')
    frame1 = cv2.imread('images/frame1.png')
    # cap = cv2.VideoCapture('ir.mp4')
    # ret, frame0 = cap.read()
    hsv = np.zeros_like(frame0)
    im0 = cv2.cvtColor(frame0, cv2.COLOR_BGR2GRAY).astype(np.float32).view(hmarray)
    hm_u = hm.zeros_like(im0)
    hm_v = hm.zeros_like(im0)
    im1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY).astype(np.float32).view(hmarray)

    hm_u, hm_v = hs_jacobi(im0, im1, hm_u, hm_v)

    hm_u.sync_host()
    hm_v.sync_host()
    mag, ang = cv2.cartToPolar(hm_u, hm_v)
    mag = cv2.normalize(mag, None, 0, 255, cv2.NORM_MINMAX)
    ang = ang*180/np.pi/2
    hsv[..., 1] = 255
    hsv[..., 0] = ang
    hsv[..., 2] = mag
    flow = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
    cv2.imshow('flow', flow)
    cv2.waitKey()

if __name__ == '__main__':
    solve()
