#include <cmath>
#include <iostream>
#include <Windows.h>

static const int ARRAY_LEN = 16;

void draw(float data[], int len, float boundX, float boundY);
void genRotationMatrix(float dest[], float rX, float rY, float rZ);
void multiplyVectorMatrix(float vector[], float matrix[], int accessStart, float dest[], int destStart);
void draw(float data[], const int indexes[], const int len, int indexBound);
void multiplyVectorScalar(float vector[], float scalar, int accessStart);
void addToAllVectorElements(float vector[], float factor, int accessStart);

static HANDLE console;

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
    int indexBound = 8;
    console = CreateConsoleScreenBuffer(0,0x2,0,CONSOLE_TEXTMODE_BUFFER,0);
    SetConsoleActiveScreenBuffer(console);

    float cubeBuffer[32];
    float rX = 0,rY = 0,rZ = 0;
    wchar_t * screen = new wchar_t[ARRAY_LEN*ARRAY_LEN];
    DWORD dwBytesWritten = 0;
    for(int itneration = 0; itneration < 1000 || true; itneration++){
        genRotationMatrix(rotationMatrix,rX,rY,rZ);
        for (int i = 0; i < 32; i+=4) {
            multiplyVectorMatrix(cube,rotationMatrix,i,cubeBuffer,i);
            addToAllVectorElements(cubeBuffer,2.5F,i);
            multiplyVectorScalar(cubeBuffer, .02F * ARRAY_LEN, i);
        }
        draw(cubeBuffer, lineIndexes, ARRAY_LEN, indexBound);
        rX += 0.005F;
        rY += 0.005F;
        rZ += 0.005F;
    }
}

void multiplyVectorMatrix(float vector[], float matrix[], int accessStart, float dest[], int destStart){
    for (int i = 0; i < 16; i++) {
        int c = i%4, r = i/4;
        dest[r+destStart]+= vector[c + accessStart] * matrix[r * 4 + c];
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

void draw(float data[], const int indexes[], const int len, int indexBound) {
    char buffer[len][len];
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            buffer[i][j] = '-';
        }
    }
    /*for (int i = 0; i < indexBound; i+=2) {
        int indA = indexes[i]*4, indB = indexes[i+1]*4;
        float x1 = std::fmin(data[indA],data[indB]), x2 = std::fmax(data[indA],data[indB]);
        float y1 = std::fmin(data[indA+1],data[indB+1]), y2 = std::fmin(data[indA+1],data[indB+1]);
        float dX = x2 - x1, dY = y2 - y1;
        for(float x = x1; x < x2; x+=.5F){
            float y = y1 + dY * (x - x1) / dX;
            y = fmin(fmax(y,0),len);
            x = fmin(fmax(x,0),len);
            buffer[(int)x][(int)y] = '0';
        }
        std::cout << "v: " << x1 << ", " << y1 << " : " << x2 << ", " << y2 << std::endl;
    }*/

    for(int i = 0; i < indexBound; i+=2){
        int i1 = indexes[i]*4, i2 = indexes[i+1]*4;
        //std::cout << "indexes: " << i1 << "," << i2 << std::endl;
        float l1x = data[i1], l1y = data[i1 + 1], l2x = data[i2], l2y = data[i2 + 1];
        //std::cout << "index data: " << l1x << ":" << l1y << " -> "  << l2x << ":" << l2y  << std::endl;
        l1x*=0.5F*len;l1y*=0.5F*len;l2x*=0.5F*len;l2y*=0.5F*len;
        float x1 = std::min(l1x,l2x),y1 = std::min(l2x,l2y),x2 = std::max(l1x,l2x),y2 = std::max(l1y,l2y);

        float dx = x2-x1, dy = y2-y1;
        if(dx>dy){
            for (int x = x1; x < x2; ++x) {
                float y = y1 + dy * (x - x1)/dx;
                if(x >= len || y >= len || x < 0 || y < 0){continue;}
                buffer[(int)x][(int)y] = 'G';
            }
        } else {
            for (int y = y1; y < y2; ++y) {
                float x = x1 + dx * (y - y1)/dy;
                if(x >= len || y >= len || x < 0 || y < 0){continue;}
                buffer[(int)x][(int)y] = 'G';
            }
        }
    }

    //std::cout << "\e[A (0x1B 0x91 0x41)" << "H" << std::endl;
    for (int x = 0; x < len; ++x) {
        for (int y = 0; y < len; ++y) {
            std::cout << buffer[x][y] << buffer[x][y];
        }
        std::cout << std::endl;
    }
    //std::cout.flush();*/
}
