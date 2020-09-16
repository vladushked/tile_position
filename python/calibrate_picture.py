from __future__ import print_function
import numpy as np
import cv2
import glob
from matplotlib import pyplot as plt
import os

img_names_undistort = [img for img in glob.glob("../images/*.jpg")]
print(img_names_undistort)
new_path = "./calib/"

camera_matrix = np.array([[666.26647899, 0.00000000e+00, 388.78781901],
                          [0.00000000e+00, 644.32316834, 274.19962853],
                          [0.00000000e+00, 0.00000000e+00, 1.00000000e+00]]);
dist_coefs = np.array([-0.39800662,  0.20574202, -0.00278414,  0.00056211, -0.07206232]);

i = 0

#for img_found in img_names_undistort:
while i < len(img_names_undistort):
    img = cv2.imread(img_names_undistort[i])
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    h,  w = img.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(camera_matrix, dist_coefs, (w, h), 1, (w, h))

    dst = cv2.undistort(img, camera_matrix, dist_coefs, None, newcameramtx)

    dst = cv2.cvtColor(dst, cv2.COLOR_BGR2RGB)

    # crop and save the image
    x, y, w, h = roi
    dst = dst[y:y+h-50, x+70:x+w-20]

    name = img_names_undistort[i].split("/")
    print(name)
    name = name[2].split(".")
    name = name[0]
    full_name = new_path + name + '.jpg'

    #outfile = img_names_undistort + '_undistorte.png'
    print('Undistorted image written to: %s' % full_name)
    cv2.imwrite(full_name, dst)
    i = i + 1