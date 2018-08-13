#pragma once
#include "stdafx.h"
#include "overlay.h"
#include <fstream>

YuvOverlay::YuvOverlay(std::string fileName, int w, int h, std::string fmt):
    inFileName_(fileName), width_(w), height_(h), format_(fmt)
{
}

YuvOverlay::~YuvOverlay()
{
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

int YuvOverlay::initialize()
{
    int fileLength = 0;
    std::fstream ifs;
    ifs.open(inFileName_.c_str(), std::ios::in | std::ios::binary);
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

    outFileName_ = "out_overlay.yuv";

    return 0;
}

int YuvOverlay::readFrame(int frameIndex)
{
    std::fstream ifs;
    ifs.open(inFileName_.c_str(), std::ios::in | std::ios::binary);

    if (!ifs.is_open())
        return -1;

    if (frameIndex >= frameNum_)
        return -1;

    int offset = frameSize_ * frameIndex;
    ifs.seekg(offset, ifs.beg);
    ifs.read(pFrame_, frameSize_);
    ifs.close();

    return 0;
}

void YuvOverlay::writeBlock(char* pBuf, int index, int blockSize, int pitch, char val)
{
    for (int y = 0; y < BLOCK_SIZE; y++)
        for (int x = 0; x < BLOCK_SIZE; x++)
            pBuf[y*pitch+x] = val;
}

void YuvOverlay::writeOverlay(int index, int val)
{
    char* pBuf = nullptr;

    // write Y
    pBuf = pY_ + index * BLOCK_SIZE;
    writeBlock(pBuf, index, BLOCK_SIZE, width_, val);

    // write U
    pBuf = pU_ + index * BLOCK_SIZE / 2;
    writeBlock(pBuf, index, BLOCK_SIZE/2, width_/2, (char)0x80);

    // write V
    pBuf = pV_ + index * BLOCK_SIZE / 2;
    writeBlock(pBuf, index, BLOCK_SIZE/2, width_/2, (char)0x80);
}

int YuvOverlay::overlayFrame(int frameIndex, int value)
{
    if (readFrame(frameIndex) != 0)
        return -1;

    for (int n = 0; n < 16; n++)
    {
        bool bin = (value >> n) & 1;
        char val = (bin) ? 0xff : 0;
        writeOverlay(n, val);
    }

    if (writeFrame(frameIndex) != 0)
        return -1;

    return 0;
}

int YuvOverlay::writeFrame(int frameIndex)
{
    std::fstream ofs;
    ofs.open(outFileName_.c_str(), std::ios::out | std::ios::binary | std::ios::app);

    if (!ofs.is_open())
        return -1;

    ofs.write(pFrame_, frameSize_);
    ofs.close();

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
