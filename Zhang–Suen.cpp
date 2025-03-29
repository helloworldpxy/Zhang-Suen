#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

typedef vector<vector<int>> Image;
// 计算图像中指定像素的8邻域中非零像素的数量
int countNeighbours(const Image &img, int x, int y) {
    int count = 0;
    // 8 个邻域顺序与 Python 示例中一致
    count += img[x-1][y];   // P2
    count += img[x-1][y+1]; // P3
    count += img[x][y+1];   // P4
    count += img[x+1][y+1]; // P5
    count += img[x+1][y];   // P6
    count += img[x+1][y-1]; // P7
    count += img[x][y-1];   // P8
    count += img[x-1][y-1]; // P9
    return count;
}

int countTransitions(const Image &img, int x, int y) {
    // 按照顺时针顺序取 8 邻域
    int transitions = 0;
    int p2 = img[x-1][y];
    int p3 = img[x-1][y+1];
    int p4 = img[x][y+1];
    int p5 = img[x+1][y+1];
    int p6 = img[x+1][y];
    int p7 = img[x+1][y-1];
    int p8 = img[x][y-1];
    int p9 = img[x-1][y-1];
    int neighbors[9] = {p2, p3, p4, p5, p6, p7, p8, p9, p2}; // 循环构建
    for (int i = 0; i < 8; i++) {
        if (neighbors[i] == 0 && neighbors[i+1] == 1)
            transitions++;
    }
    return transitions;
}
// 实现Zhang-Suen细化算法，用于二值图像的细化处理
void zhangSuenThinning(Image &img) {
    bool changed;
    int rows = img.size();
    int cols = img[0].size();
    do {
        changed = false;
        // 创建临时标记图像
        Image marker(rows, vector<int>(cols, 0));
        // 子迭代 1
        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                if (img[i][j] != 1)
                    continue;
                int Np = countNeighbours(img, i, j);
                int Ap = countTransitions(img, i, j);
                int p2 = img[i-1][j];
                int p3 = img[i-1][j+1];
                int p4 = img[i][j+1];
                int p5 = img[i+1][j+1];
                int p6 = img[i+1][j];
                int p7 = img[i+1][j-1];
                int p8 = img[i][j-1];
                int p9 = img[i-1][j-1];
                if ((Np >= 2 && Np <= 6) && (Ap == 1) &&
                    (p2 * p4 * p6 == 0) &&
                    (p4 * p6 * p8 == 0)) {
                    marker[i][j] = 1;
                    changed = true;
                }
            }
        }
        // 删除在子迭代1中标记的像素
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (marker[i][j] == 1)
                    img[i][j] = 0;
                
        // 子迭代 2
        marker.assign(rows, vector<int>(cols, 0));
        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                if (img[i][j] != 1)
                    continue;
                int Np = countNeighbours(img, i, j);
                int Ap = countTransitions(img, i, j);
                int p2 = img[i-1][j];
                int p3 = img[i-1][j+1];
                int p4 = img[i][j+1];
                int p5 = img[i+1][j+1];
                int p6 = img[i+1][j];
                int p7 = img[i+1][j-1];
                int p8 = img[i][j-1];
                int p9 = img[i-1][j-1];
                if ((Np >= 2 && Np <= 6) && (Ap == 1) &&
                    (p2 * p4 * p8 == 0) &&
                    (p2 * p6 * p8 == 0)) {
                    marker[i][j] = 1;
                    changed = true;
                }
            }
        }
        // 删除在子迭代2中标记的像素
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (marker[i][j] == 1)
                    img[i][j] = 0;
        
    } while (changed);
}

int main() {
    // 假设我们有一个简单的 10x10 二值图像
    Image img = {
        {0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,0,0,0,0,0,0},
        {0,1,1,1,0,0,0,0,0,0},
        {0,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,0},
        {0,0,0,0,0,0,1,1,1,0},
        {0,0,0,0,0,0,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    };
    
    zhangSuenThinning(img);
    
    // 打印处理后的图像
    for (auto &row : img) {
        for (auto val : row)
            cout << (val ? "1 " : "0 ");
        cout << endl;
    }
    
    return 0;
}