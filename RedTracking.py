# Blob Detection Example
#
# This example shows off how to use the find_blobs function to find color
# blobs in the image. This example in particular looks for dark green objects.

import sensor, image, time

# For color tracking to work really well you should ideally be in a very, very,
# very, controlled enviroment where the lighting is constant...
red_threshold_01 = (0, 35, 0, 50, -10, 40)

#设置红色的阈值，括号里面的数值分别是L A B 的最大值和最小值（minL, maxL, minA,
# maxA, minB, maxB），LAB的值在图像左侧三个坐标图中选取。如果是灰度图，则只需
#设置（min, max）两个数字即可。

# You may need to tweak the above settings for tracking green things...
# Select an area in the Framebuffer to copy the color settings.

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False)
#关闭白平衡。白平衡是默认开启的，在颜色识别中，需要关闭白平衡。

def compareBlob(blob1, blob2):
    # 这里我们选择了pixels作为指标比对二者的代码
    # 你也可以换用其它指标 例如 blob.area()
    tmp = blob1.pixels() - blob2.pixels()
    if tmp == 0:
        return 0;
    elif tmp > 0:
        return 1;
    else:
        return -1;

clock = time.clock() # Tracks FPS.

while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    bigBlob = None #最大的色块
    img = sensor.snapshot() # Take a picture and return the image.
    #  pixels_threshold=100, area_threshold=100
    blobs = img.find_blobs([red_threshold_01], area_threshold=150)

    if len(blobs) == 0:
        continue

    bigBlob = blobs[0]
    #如果找到了目标颜色
    for blob in blobs:
    #迭代找到的目标颜色区域
        if compareBlob(bigBlob, blob) == -1:
            bigBlob = blob

    # 标记识别到的最大色块
    img.draw_rectangle(bigBlob[0:4], color = (255, 0, 0))
    img.draw_cross(bigBlob.cx(), bigBlob.cy())
    error_x = bigBlob.cx() - 80
    error_y = 60 - bigBlob.cy()
    print("error_x:",error_x)
    print("error_y:",error_y)

    print(clock.fps()) # Note: Your OpenMV Cam runs about half as fast while
    # connected to your computer. The FPS should increase once disconnected.
