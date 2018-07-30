#pragma once

#include "stdafx.h"
#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>

struct FrameProp
{
    BYTE* pData;
    INT pitch;
    INT height;
    INT size;
};

class CaptureDesktop
{
public:
    CaptureDesktop();
    ~CaptureDesktop();

    INT initialize();
    INT getFrame(FrameProp* prop);
    void destroy();

private:
    ID3D11Device* pD3D11Device_ = nullptr;
    ID3D11DeviceContext* pDeviceContext_ = nullptr;
    IDXGIDevice* pDxgiDevice_ = nullptr;
    IDXGIAdapter* pDxgiAdapter_ = nullptr;

    IDXGIOutput* pDxgiOutput_ = nullptr;
    IDXGIOutput1* pDxgiOutput1_ = nullptr;
    IDXGIOutputDuplication* pOutputDupl_ = nullptr;

    IDXGIResource* pDesktopResource_ = nullptr;
    ID3D11Texture2D* pDesktopSrc_ = nullptr;
    ID3D11Texture2D* pDesktopDst_ = nullptr;

    BYTE* pData_ = nullptr;
    INT pitch_ = 0;
    INT height_ = 0;
};

