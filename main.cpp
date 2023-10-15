#include <cmath>
#include <iostream>
#include <thread>
#include <Windows.h>

static const int LEN = 128;
static HDC hdc;

void genRotationMatrix(float dest[], float rX, float rY, float rZ);
void multiplyVectorMatrix(const float vector[], const float matrix[], int accessStart, float dest[], int destStart);
void draw(float data[], const int indexes[], int indexBound, COLORREF color);
void multiplyVectorScalar(float vector[], float scalar, int accessStart);
void addToAllVectorElements(float vector[], float factor, int accessStart);
void fillArray(float array[], int len);

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
        0,1,1,5,5,4,4,0
    };
    const int indexBound = 8;
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
        fillArray(cubeBuffer,32);
        genRotationMatrix(rotationMatrix,rX,rY,rZ);
        for (int i = 0; i < 32; i+=4) {
            multiplyVectorMatrix(cube,rotationMatrix,i,cubeBuffer,i);
            addToAllVectorElements(cubeBuffer,2.F,i);
            multiplyVectorScalar(cubeBuffer, LEN, i);

        }
        draw(cubeBuffer, lineIndexes, indexBound,color);
        rX += 0.005F;
        rY += 0.005F;
        rZ += 0.005F;
        std::this_thread::sleep_for(std::chrono::milliseconds (10));
        draw(cubeBuffer, lineIndexes, indexBound,clearColor);
    }
}

void fillArray(float array[], int len){
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

void draw(float data[], const int indexes[], int indexBound, COLORREF color ) {

    for(int i = 0; i < indexBound; i+=2){
        int i1 = indexes[i]*4, i2 = indexes[i+1]*4;
        float l1x = data[i1], l1y = data[i1 + 1], l2x = data[i2], l2y = data[i2 + 1];
        //l1x*=0.5F*len;l1y*=0.5F*len;l2x*=0.5F*len;l2y*=0.5F*len;
        float x1 = std::min(l1x,l2x),y1 = std::min(l2x,l2y),x2 = std::max(l1x,l2x),y2 = std::max(l1y,l2y);

        float dx = x2-x1, dy = y2-y1;
        if(dx > dy){
            for (int x = x1; x < x2; ++x) {
                float y = y1 + dy * (x - x1)/dx;
                if(x < 0 || y < 0){continue;}
                SetPixel(hdc, x, y, color);
            }
        } else {
            for (int y = y1; y < y2; ++y) {
                float x = x1 + dx * (y - y1)/dy;
                if(x < 0 || y < 0){continue;}
                SetPixel(hdc, x, y, color);
            }
        }
    }
}
