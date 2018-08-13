#pragma once

#include "stdafx.h"
#include <string>

class YuvOverlay
{
public:
    YuvOverlay(std::string fileName, int w, int h, std::string fmt);
    ~YuvOverlay();

    int initialize();
    int overlayFrame(int frameIndex, int value);
    int destroy();

private:
    int readFrame(int frameIndex);
    int overlay();
    int writeFrame();

private:
    int width_ = 0;
    int height_ = 0;
    std::string format_ = "";
    std::string fileName_ = "";
    int frameNum_ = 0;
    char* pY_ = nullptr;
    char* pU_ = nullptr;
    char* pV_ = nullptr;
};
