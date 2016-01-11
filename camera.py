import sys
import numpy as np
import cv2

def print_2d_arr(arr):
    for y in range(arr.shape[0]):
        for x in range(arr.shape[1]):
            print arr[y, x],
    print ""

def print_2d_arr_spaces(arr):
    for y in range(arr.shape[0]):
        for x in range(arr.shape[1]):
            print arr[y, x],
            print " ",
    print ""

cap = cv2.VideoCapture(0)

print(240)
print(320)
cv2.namedWindow('input')

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
      print >> sys.stderr, "FAIL"
      break

    small_frame = cv2.resize(frame, (320, 240))
    cv2.imshow('input', small_frame)
    print >> sys.stderr, small_frame.shape
    #cv2.imwrite('images/small_camera.png',small_frame)
    #break
    cv2.waitKey(50)

    # Our operations on the frame come here
    im0 = cv2.cvtColor(small_frame, cv2.COLOR_BGR2GRAY).astype(np.float32)
    print_2d_arr(im0)
    print >> sys.stderr, "END FRAME"
    #cv2.imshow('frame',small_frame)


# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
