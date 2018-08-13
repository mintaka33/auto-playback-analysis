#pragma once
#include "stdafx.h"
#include "overlay.h"
#include <fstream>

YuvOverlay::YuvOverlay(std::string fileName, int w, int h, std::string fmt):
    fileName_(fileName), width_(w), height_(h), format_(fmt)
{
}

YuvOverlay::~YuvOverlay()
{
}

int YuvOverlay::initialize()
{
    int fileLength = 0;
    std::fstream ifs;
    ifs.open(fileName_.c_str(), std::ios::in | std::ios::binary);
    if (!ifs.is_open())
        return -1;

    // get length of file:
    ifs.seekg(0, ifs.end);
    fileLength = (int)ifs.tellg();
    ifs.seekg(0, ifs.beg);
    if (fileLength <= 0)
        return -1;
    ifs.close();

    // calculate frame size and frame number
    frameSize_ = calcFrameSize();
    if (fileLength % frameSize_ != 0)
        return -1;
    frameNum_ = fileLength / frameSize_;

    pFrame_ = new char[frameSize_];
    if (pFrame_ == nullptr)
        return -1;
    pY_ = pFrame_;
    pU_ = pY_ + width_ * height_;
    pV_ = pU_ + width_ * height_ / 4;

    return 0;
}

int YuvOverlay::overlayFrame(int frameIndex, int value)
{
    return 0;
}

int YuvOverlay::destroy()
{
    if (pFrame_)
    {
        delete[] pFrame_;
        pFrame_ = nullptr;
    }
    return 0;
}

int YuvOverlay::calcFrameSize()
{
    int size = 0;
    if (format_ == "yuv420p")
    {
        size = width_ * height_;
        size += width_ * height_ / 2;
    }
    else
    {
        size = 0;
    }
    return size;
}

int YuvOverlay::readFrame(int frameIndex)
{
    return 0;
}

int YuvOverlay::overlay()
{
    return 0;
}

int YuvOverlay::writeFrame()
{
    return 0;
}

