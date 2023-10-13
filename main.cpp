#include <cmath>
#include <iostream>

void draw(float data[], int len, float boundX, float boundY);
void genRotationMatrix(float dest[], float rX, float rY, float rZ);
void multiplyVectorMatrix(float vector[], float matrix[], int accessStart, float dest[], int destStart);
void draw(float data[],const int indexes[], int len, int indexBound);
void multiplyVectorScalar(float vector[], float scalar, int accessStart);

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
        0,1,2,2,3,4
    };

    float cubeBuffer[32];
    float rX = 0,rY = 0,rZ = 0;
    for(int itneration = 0; itneration < 60000; itneration++){
        genRotationMatrix(rotationMatrix,rX,rY,rZ);
        for (int i = 0; i < 32; i+=4) {
            multiplyVectorMatrix(cube,rotationMatrix,i,cubeBuffer,i);
            //multiplyVectorScalar(cubeBuffer,0.7F,i);
        }
        draw(cubeBuffer,lineIndexes,16,6);
        rX += 0.05F;
        rY += 0.05F;
        rZ += 0.05F;
    }
}

void multiplyVectorMatrix(float vector[], float matrix[], int accessStart, float dest[], int destStart){
    for (int i = 0; i < 16; i++) {
        int c = i%4, r = i/4;
        dest[r+destStart]+= vector[c + accessStart] * matrix[r * 4 + c];
    }
}

void multiplyVectorScalar(float vector[], float scalar, int accessStart){
    for (int i = 0; i < 4; ++i) {
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

void draw(float data[],const int indexes[], int len, int indexBound){
    char buffer[len][len];
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            buffer[i][j] = '-';
        }
    }
    for (int i = 0; i < indexBound; i+=2) {
        int indA = indexes[i]*4, indB = indexes[i+1]*4;
        float x1 = std::fmin(data[indA],data[indB]), x2 = std::fmax(data[indA],data[indB]);
        float y1 = std::fmin(data[indA+1],data[indB+1]), y2 = std::fmin(data[indA+1],data[indB+1]);
        float dX = x2 - x1, dY = y2 - y1;
        for(float l = x1; l < x2;l+=0.5F){
            float y = y1 + dY * (l - x1)/dX;
            buffer[(int)l][(int)y] = '0';
        }
    }
    printf("RENDER:\n");
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            std::cout<<buffer[i][j]<<" ";
        }
        printf("\n");
    }
}

