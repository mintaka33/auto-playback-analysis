#pragma once

#include "stdafx.h"
#include <string>

#define BLOCK_SIZE 16

class YuvOverlay
{
public:
    YuvOverlay(std::string fileName, int w, int h, std::string fmt);
    ~YuvOverlay();

    int initialize();
    int overlayFrame(int frameIndex, int value);
    int destroy();

private:
    int calcFrameSize();
    int readFrame(int frameIndex);
    void writeBlock(char* pBuf, int index, int blockSize, int pitch, char bin);
    void writeOverlay(int index, int bin);
    int writeFrame(int frameIndex);

private:
    int width_ = 0;
    int height_ = 0;
    int frameSize_ = 0;
    int frameNum_ = 0;
    std::string format_ = "";
    std::string inFileName_ = "";
    std::string outFileName_ = "";
    char* pFrame_ = nullptr;
    char* pY_ = nullptr;
    char* pU_ = nullptr;
    char* pV_ = nullptr;
};
