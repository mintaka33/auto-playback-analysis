#include "stdafx.h"
#include "capture.h"

#define RETURN_IF_FAIL(hr) \
if (!SUCCEEDED(hr)) \
{\
    return -1;\
}

#define FREE_RESOURCE(res) \
if (res != nullptr) \
{ \
    res->Release(); \
    res = nullptr; \
}

CaptureDesktop::CaptureDesktop()
{
}

CaptureDesktop::~CaptureDesktop()
{
}

INT CaptureDesktop::initialize()
{
    HRESULT hr = S_OK;
    D3D_FEATURE_LEVEL fl = {};

    // Create D3D11 Device
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &pD3D11Device_, &fl, &pDeviceContext_);
    RETURN_IF_FAIL(hr);

    // Get DXGI device
    hr = pD3D11Device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDxgiDevice_));
    RETURN_IF_FAIL(hr);

    // Get DXGI adapter
    hr = pDxgiDevice_->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDxgiAdapter_));
    RETURN_IF_FAIL(hr);

    // Get output
    hr = pDxgiAdapter_->EnumOutputs(0, &pDxgiOutput_);
    RETURN_IF_FAIL(hr);

    // Get output description
    DXGI_OUTPUT_DESC outputDesc = {};
    pDxgiOutput_->GetDesc(&outputDesc);

    // Query interface of Output1
    hr = pDxgiOutput_->QueryInterface(__uuidof(pDxgiOutput1_), reinterpret_cast<void**>(&pDxgiOutput1_));
    RETURN_IF_FAIL(hr);

    // Create desktop duplication
    hr = pDxgiOutput1_->DuplicateOutput(pD3D11Device_, &pOutputDupl_);
    RETURN_IF_FAIL(hr);

    // Create a surface to hold desktop
    D3D11_TEXTURE2D_DESC desc = {};
    DXGI_OUTDUPL_DESC outputDuplDesc = {};
    pOutputDupl_->GetDesc(&outputDuplDesc);
    desc.Width = outputDuplDesc.ModeDesc.Width;
    desc.Height = outputDuplDesc.ModeDesc.Height;
    desc.Format = outputDuplDesc.ModeDesc.Format;
    desc.ArraySize = 1;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.MipLevels = 1;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.Usage = D3D11_USAGE_STAGING;
    hr = pD3D11Device_->CreateTexture2D(&desc, NULL, &pDesktopDst_);
    RETURN_IF_FAIL(hr);

    return 0;
}

INT CaptureDesktop::getFrame(FrameProp* prop)
{
    return 0;
}

void CaptureDesktop::destroy()
{
    FREE_RESOURCE(pDesktopDst_);
}

