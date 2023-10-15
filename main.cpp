#include <cmath>
#include <iostream>
#include <thread>
#include <Windows.h>

static const int LEN = 128;
static HDC hdc;

void genRotationMatrix(float dest[], float rX, float rY, float rZ);
void multiplyVectorMatrix(const float vector[], const float matrix[], int accessStart, float dest[], int destStart);
void draw(const float data[], const int indexes[], int indexBound, COLORREF color);
void multiplyVectorScalar(float vector[], float scalar, int accessStart);
void addToAllVectorElements(float vector[], float factor, int accessStart);
void emptyArray(float array[], int len);
void debugArray(const float arr[], int len);

int main() {
    float rotationMatrix[16];
    float cube[] = {
            -1,-1,-1,0,
            -1,1,-1,0,
            -1,1,1,0,
            -1,-1,1,0,
            1,-1,-1,0,
            1,1,-1,0,
            1,1,1,0,
            1,-1,1,0,
    };
    int lineIndexes[] = {
        0,1,1,2,2,3,3,0,
        4,5,5,6,6,7,7,4,
        0,4,1,5,2,6,3,7
    };
    const int indexBound = 24;
    HWND consoleWindow = GetConsoleWindow();
    hdc = GetDC(consoleWindow);
    for (int i = 0; i < 100; ++i) {
        SetPixel(hdc,i,100,RGB(255,255,0));
    }

    float cubeBuffer[32];
    float rX = 0,rY = 0,rZ = 0;
    COLORREF color = RGB(255,0,0);
    COLORREF clearColor = RGB(0,0,0);


    for(int itneration = 0; itneration < 10000; ++itneration){
        emptyArray(cubeBuffer, 32);
        genRotationMatrix(rotationMatrix,rX,rY,rZ);
        for (int i = 0; i < 32; i+=4) {
            multiplyVectorMatrix(cube,rotationMatrix,i,cubeBuffer,i);
            //std::cout << i/4 << ": " << cubeBuffer[i] << ", " << cubeBuffer[i+1] << ", " << cubeBuffer[i+2] << std::endl;
            addToAllVectorElements(cubeBuffer,2.F,i);
            multiplyVectorScalar(cubeBuffer, LEN, i);
        }
        //debugArray(cubeBuffer,32);
        draw(cubeBuffer, lineIndexes, indexBound,color);
        rX += 0.009F;
        rY += 0.012F;
        rZ += 0.015F;
        std::this_thread::sleep_for(std::chrono::milliseconds (10));
        draw(cubeBuffer, lineIndexes, indexBound,clearColor);
    }

}

void emptyArray(float array[], int len){
    for (int i = 0; i < len; ++i) {
        array[i] = 0;
    }
}

void multiplyVectorMatrix(const float vector[], const float matrix[], int accessStart, float dest[], int destStart){
    for (int i = 0; i < 16; i++) {
        int c = i%4, r = i/4;
        dest[r+destStart] += vector[c + accessStart] * matrix[r * 4 + c];
    }
}

void addToAllVectorElements(float vector[], float factor, int accessStart){
    for (int i = 0; i < 3; ++i) {
        vector[i+accessStart]+=factor;
    }
}

void multiplyVectorScalar(float vector[], float scalar, int accessStart){
    for (int i = 0; i < 3; ++i) {
        vector[i+accessStart]*=scalar;
    }
}

void genRotationMatrix(float dest[], float rX, float rY, float rZ){
    float sX = std::sin(rX);
    float sY = std::sin(rY);
    float sZ = std::sin(rZ);
    float cX = std::cos(rX);
    float cY = std::cos(rY);
    float cZ = std::cos(rZ);
    dest[0] = cZ * cY;
    dest[1] = cZ * sY * sX - sZ * cX;
    dest[2] = cZ * sY * cX + sZ * sX;
    dest[3] = 0;
    dest[4] = sZ * cY;
    dest[5] = sZ * sY * sX + cZ * cX;
    dest[6] = sZ * sY * cX - cZ * sX;
    dest[7] = 0;
    dest[8] = -sY;
    dest[9] = cY * sX;
    dest[10] = cY * cX;
    dest[11] = dest[12] = dest[13] = dest[14] = 0;
    dest[15] = 1;
}

void debugArray(const float arr[], int len){
    std::cout << "ARRAY: ";
    for (int i = 0; i < len; ++i) {
        std::cout << i << "=" << arr[i] << " ";
    }
    std::cout << std::endl;
}

void draw(const float data[], const int indexes[], int indexBound, COLORREF color ) {

    for(int i = 0; i < indexBound; i+=2){
        int i1 = indexes[i]*4, i2 = indexes[i+1]*4;
        float l1x = data[i1], l1y = data[i1 + 1], l2x = data[i2], l2y = data[i2 + 1];
        float x1 = l1x,y1 = l1y,x2 = l2x,y2 = l2y;
        float dx = x2-x1, dy = y2-y1;
        /*std::cout << i/2 << ": \ni: " << i1 << "->" << i2 << std::endl
        << x1 << ", " << y1 << " -> " << x2 << ", " << y2 << std::endl << " (" << l1x << "," << l1y << "->"
        << l2x << "," << l2y << ")" << std::endl << std::endl;*/

        if(dx > dy){
            if(x1 > x2){x1 = l2x; y1 = l2y; x2 = l1x; y2 = l2y;}
            for (float x = x1; x < x2; x+=1) {
                float y = y1 + dy * (x - x1)/dx;
                if(x < 0 || y < 0){continue;}
                SetPixel(hdc, x, y, color);
            }
        } else {
            if(y1 > y2){y1 = l2y; x1 = l2x; y2 = l1y; x2 = l1x;}
            for (float y = y1; y < y2; y+= 1) {
                float x = x1 + dx * (y - y1)/dy;
                if(x < 0 || y < 0){continue;}
                SetPixel(hdc, x, y, color);
            }
        }
    }
}
