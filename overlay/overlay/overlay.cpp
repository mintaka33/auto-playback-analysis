#pragma once
#include "stdafx.h"
#include "overlay.h"

YuvOverlay::YuvOverlay(std::string fileName, int w, int h, std::string fmt):
    fileName_(fileName), width_(w), height_(h), format_(fmt)
{
}

YuvOverlay::~YuvOverlay()
{
}

int YuvOverlay::initialize()
{
    return 0;
}

int YuvOverlay::overlayFrame(int frameIndex, int value)
{
    return 0;
}

int YuvOverlay::destroy()
{
    return 0;
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

