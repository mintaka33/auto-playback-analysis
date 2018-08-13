#pragma once

#include "stdafx.h"

class YuvOverlay
{
public:
    YuvOverlay();
    ~YuvOverlay();

private:
    int32_t width;
    int32_t height;
    int32_t frameNum;
};
