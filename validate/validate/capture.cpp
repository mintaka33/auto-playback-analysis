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
    HRESULT hr = S_OK;

    // need a sleep here, or will get blank surface. 
    // TODO: need figure out root cause
    Sleep(50);

    // Get frame
    DXGI_OUTDUPL_FRAME_INFO frameInfo = {};
    hr = pOutputDupl_->AcquireNextFrame(500, &frameInfo, &pDesktopResource_);
    RETURN_IF_FAIL(hr);

    // Query interface of IDXGIResource
    hr = pDesktopResource_->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&pDesktopSrc_));
    RETURN_IF_FAIL(hr);

    // Copy desktop data
    pDeviceContext_->CopyResource(pDesktopDst_, pDesktopSrc_);
    D3D11_MAPPED_SUBRESOURCE subRes = {};
    UINT subResIndex = D3D11CalcSubresource(0, 0, 0);
    hr = pDeviceContext_->Map(pDesktopDst_, subResIndex, D3D11_MAP_READ, 0, &subRes);
    RETURN_IF_FAIL(hr);

    BYTE* pSrc = reinterpret_cast<BYTE*>(subRes.pData);

    // Calculate frame height
    pitch_ = subRes.RowPitch;
    DXGI_OUTPUT_DESC outputDesc = {};
    pDxgiOutput_->GetDesc(&outputDesc);
    height_ = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;

    if (pData_ == nullptr)
    {
        pData_ = new BYTE[pitch_ * height_];
    }

    INT size = pitch_ * height_;
    memcpy_s(pData_, size, pSrc, size);

    pDeviceContext_->Unmap(pDesktopDst_, subResIndex);

    prop->pData = pData_;
    prop->pitch = pitch_;
    prop->height = height_;
    prop->size = size;

    return 0;
}

void CaptureDesktop::destroy()
{
    FREE_RESOURCE(pDesktopDst_);
}

