# Zhang-Suen
Python and C++ implementation of Zhang Suen thinning algorithm
Zhang Suen细化算法的Python和C++实现

## 代码优化说明

### 1. 并行计算优化
可以采用分块处理策略，将图像划分为多个独立区域，如图：
```
┌───┬───┬───┐
│ 1 │ 2 │ 3 │
├───┼───┼───┤
│ 4 │ 5 │ 6 │ 
├───┼───┼───┤
│ 7 │ 8 │ 9 │
└───┴───┴───┘

```
- 奇数块和偶数块交替处理
- 使用OpenMP实现多线程并行

### 2. 内存访问优化

以上文中C++代码为例，我们可以采用行优先存储+缓存预取技术来实现内存访问优化：
```
// 优化前
for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
        access pixel[y][x]
    }
}

// 优化后
for(int x=0; x<width; x++){
    prefetch_column(x); // 预取整列数据
    for(int y=0; y<height; y++){
        access pixel[y][x] 
    }
}
```

### 3. 使用GPU实现硬件加速

此处以CUDA为例，我们可以使用GPU实现硬件加速：
```

__global__ void thinning_kernel(uchar* img, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    
    if(x <=0 || x >= width-1 || y <=0 || y >= height-1) return;
    
    // 邻域加载到共享内存
    __shared__ uchar neighborhood[34][34]; // 32x32块 + 边界
    load_neighborhood(img, neighborhood);
    
    // 判断条件
    if(meet_conditions(neighborhood)){
        img[y*width + x] = 0;
    }
}
```
## 算法关键点理解与应用
- 由于删除操作仅在满足特定条件时进行，且严格保证边缘像素的删除不会破坏全局连通性，因此最终获得的骨架既细长又保持目标原有的结构。对连通性和形态保持性的要求使得算法非常适用于字符识别和形状分析。
- 通过计算邻域内像素数和 0→1 的转变次数，算法能区分目标区域的内部像素与边缘像素，进而只删除过于“冗余”的像素。两个子迭代中不同的乘积条件（如p2p4p6与p4p6p8的差异）确保在不同方向上都不会一次性删除过多像素，以防止骨架断裂。
- 虽然Zhang–Suen算法已经广泛用于二值图像细化，但它也可以作为其他骨架提取算法的基础。对于噪声较多的图像，通常需在细化前进行平滑、二值化、降噪处理（比如可以新增条件如： (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) ≥ 3 ）；另外也有其它改进版本可以适用于更多复杂形态的图像。
