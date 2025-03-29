import numpy as np
import cv2

def neighbours(x, y, image):
    # 返回 8 邻域像素值，顺序为 P2,P3,...,P9
    return [image[x-1, y],     # P2
            image[x-1, y+1],   # P3
            image[x,   y+1],   # P4
            image[x+1, y+1],   # P5
            image[x+1, y],     # P6
            image[x+1, y-1],   # P7
            image[x,   y-1],   # P8
            image[x-1, y-1]]   # P9

def transitions(neighbors):
    # 计算 A(P1)：从 0 到 1 的转变次数（循环相连，第9个与第1个之间也算一次）
    n = neighbors + [neighbors[0]]
    count = 0
    for i in range(len(neighbors)):
        if n[i] == 0 and n[i+1] == 1:
            count += 1
    return count

def zhang_suen_thinning(image):
    # 确保输入为二值图像，前景为1,背景为0
    prev = np.zeros(image.shape, np.uint8)
    diff = None
    
    while True:
        # 第一步：子迭代1，标记删除像素
        marker = np.zeros(image.shape, np.uint8)
        rows, cols = image.shape
        for i in range(1, rows-1):
            for j in range(1, cols-1):
                P1 = image[i, j]
                if P1 != 1:
                    continue
                # 获取 8 邻域像素
                nb = neighbours(i, j, image)
                Np = sum(nb)
                Ap = transitions(nb)
                # 条件判定
                if (2 <= Np <= 6 and Ap == 1 and 
                    nb[0] * nb[2] * nb[4] == 0 and 
                    nb[2] * nb[4] * nb[6] == 0):
                    marker[i, j] = 1
        image = image - marker
        
        # 第二步：子迭代2，标记删除像素
        marker = np.zeros(image.shape, np.uint8)
        for i in range(1, rows-1):
            for j in range(1, cols-1):
                P1 = image[i, j]
                if P1 != 1:
                    continue
                nb = neighbours(i, j, image)
                Np = sum(nb)
                Ap = transitions(nb)
                if (2 <= Np <= 6 and Ap == 1 and 
                    nb[0] * nb[2] * nb[6] == 0 and 
                    nb[0] * nb[4] * nb[6] == 0):
                    marker[i, j] = 1
        image = image - marker
        
        # 若本次迭代中无变化，退出循环
        diff = np.abs(image - prev).sum()
        if diff == 0:
            break
        prev = image.copy()
    
    return image

if __name__ == "__main__":
    # 示例：读取图像并执行骨架提取
    # 注意：确保图像为二值图像，若需要可用 cv2.threshold 处理
    img = cv2.imread("binary_input.png", 0)
    _, binary = cv2.threshold(img, 127, 1, cv2.THRESH_BINARY)
    skeleton = zhang_suen_thinning(binary)
    # 显示骨架图像（放大显示，值为 0 或 1）
    cv2.imshow("Skeleton", skeleton*255)
    cv2.waitKey(0)
    cv2.destroyAllWindows()