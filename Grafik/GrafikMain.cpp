#include "GrafikMain.h"
#include <iostream>
#include "FontInformation.h"


Grafik::Graphix::~Graphix()
{
    pSwapChain->Release();
    pDevice->Release();
    pContext->Release();
}

void Grafik::Graphix::Initialize(HWND hWnd,const ivec2& size)
{
    this->hWnd = hWnd;
    D3D_FEATURE_LEVEL featLevel;
    DXGI_SWAP_CHAIN_DESC sd{ 0 };
    sd.BufferCount = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.RefreshRate = { 0 , 0 };
    sd.OutputWindow = hWnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, NULL, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featLevel, &pContext);
    _ASSERT(res == S_OK);
   

    ID3D11Texture2D* pBackbuffer = nullptr;
    res = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackbuffer);
    _ASSERT(res == S_OK);
    res = pDevice->CreateRenderTargetView(pBackbuffer, 0, &pRenderTargetView);
    _ASSERT(res == S_OK);
    pBackbuffer->Release();

    pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
}

void Grafik::Graphix::Recreate()
{
}
void Grafik::Graphix::BeginDrawing() const
{
    static int counter = 0;
    const FLOAT clearCol[4] = { 0.0f, 0.0f, 0.0f, 0.2f };
    pContext->ClearRenderTargetView(pRenderTargetView, clearCol);

    counter++;
    if (counter > 100)
    {
        counter = 0;
    }

    RECT winRect;
    GetClientRect(hWnd, &winRect);
    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (FLOAT)(winRect.right - winRect.left), (FLOAT)(winRect.bottom - winRect.top), 0.0f, 1.0f };
    pContext->RSSetViewports(1, &viewport);

    pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
}
void Grafik::Graphix::EndDrawing() const
{
    HRESULT res = pSwapChain->Present(1, 0);
    _ASSERT(res == S_OK);
}

ID3D11DeviceContext* Grafik::Graphix::GetContext() const
{
	return pContext;
}

ID3D11Device* Grafik::Graphix::GetDevice() const
{
	return pDevice;
}

IDXGISwapChain* Grafik::Graphix::GetSwapChain() const
{
	return pSwapChain;
}
ID3D11RenderTargetView* Grafik::Graphix::GetRenderTargetView() const
{
	return pRenderTargetView;
}



Grafik::Blender::~Blender()
{
}
Grafik::Blender::Blender(const Graphix& gfx, bool blending)
{
    D3D11_BLEND_DESC bd{};
    auto& brt = bd.RenderTarget[0];
    if (blending)
    {
        brt.BlendEnable = TRUE;
        brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        brt.BlendOp = D3D11_BLEND_OP_ADD;
        brt.SrcBlendAlpha = D3D11_BLEND_ZERO;
        brt.DestBlendAlpha = D3D11_BLEND_ZERO;
        brt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    else
    {
        brt.BlendEnable = FALSE;
        brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    HRESULT res = gfx.GetDevice()->CreateBlendState(&bd, &pBlender);
    _ASSERT(res == S_OK);
}
void Grafik::Blender::Bind(const Graphix& gfx)
{
    gfx.GetContext()->OMSetBlendState(pBlender.Get(), nullptr, 0xFFFFFFFFu);
}
Grafik::Blender& Grafik::Blender::operator=(Blender&& moved)
{
    if (this->pBlender)
    {
        this->pBlender->Release();
    }
    this->pBlender = std::move(moved.pBlender);
    moved.pBlender = nullptr;
    return *this;
}


Grafik::BothShader::BothShader(const Graphix& gfx)
{
    ID3D10Blob* pBlob = nullptr;

    const char* szShadez = R"(
struct PSI
{
	float4 pos : SV_POSITION;
	float4 col : COLOUR;
	float2 tex : TEXPOS;
};

// VertexShader
PSI VS( float2 pos : POSITION, float4 col : COLOR, float2 tex : TexCoord )
{
	PSI psi;
	psi.pos = float4(pos,0.0f,1.0f);
	psi.col = col;
	psi.tex = tex;
    
	return psi;
}

Texture2D texu : register(t0);
SamplerState splr : register(s0);

// PixelShader
float4 PS( PSI psi ) : SV_TARGET
{
	if(psi.col.w > 0.0f)
	{
		return psi.col;
	}
	else
	{
		return texu.Sample(splr, psi.tex) * float4(psi.col.xyz,-psi.col.w);
	}
}
)";
    HRESULT hr = D3DCompile(szShadez, strlen(szShadez), 0, nullptr, nullptr, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pBlob, nullptr);
    if (FAILED(hr))
    {
        std::cout << " failed to load shader " << std::endl;
        std::cout << std::hex << hr << std::endl;
    }

    gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[3] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };;

    gfx.GetDevice()->CreateInputLayout(layout, 3, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

    hr = D3DCompile(szShadez, strlen(szShadez), 0, nullptr, nullptr, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pBlob, nullptr);
    if (FAILED(hr))
    {
        std::cout << " failed to load shader pixel " << std::endl;
        std::cout << std::hex << hr << std::endl;
    }
    gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
    pBlob->Release();

}
Grafik::BothShader::~BothShader()
{

}
void Grafik::BothShader::Bind(const Graphix& gfx)
{
    gfx.GetContext()->PSSetShader(pPixelShader.Get(), 0, 0);
    gfx.GetContext()->VSSetShader(pVertexShader.Get(), 0, 0);
    gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
}
Grafik::BothShader& Grafik::BothShader::operator=(Grafik::BothShader&& moved)
{
    if (this->pPixelShader)
    {
        this->pPixelShader->Release();
        this->pVertexShader->Release();
        this->pInputLayout->Release();
    }
    this->pPixelShader = std::move(moved.pPixelShader);
    this->pVertexShader = std::move(moved.pVertexShader);
    this->pInputLayout = std::move(moved.pInputLayout);

    moved.pPixelShader = nullptr;
    moved.pVertexShader = nullptr;
    moved.pInputLayout = nullptr;

    return *this;
}


Grafik::IndexBuffer::IndexBuffer(const Graphix& gfx, const int size, const UINT* pInd)
{
    D3D11_BUFFER_DESC bd{0};
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * size;
    bd.StructureByteStride = sizeof(UINT);
    D3D11_SUBRESOURCE_DATA sr{};
    sr.pSysMem = pInd;
    HRESULT res = gfx.GetDevice()->CreateBuffer(&bd, &sr, &pIndexBuffer);
    _ASSERT(res == S_OK);
}
Grafik::IndexBuffer::~IndexBuffer()
{
}
void Grafik::IndexBuffer::Bind(const Graphix& gfx)
{
    gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
Grafik::IndexBuffer& Grafik::IndexBuffer::operator=(IndexBuffer&& moved)
{
    if (this->pIndexBuffer)
    {
        this->pIndexBuffer->Release();
    }
    this->pIndexBuffer = std::move(moved.pIndexBuffer);
    moved.pIndexBuffer = nullptr;
    return *this;
}


Grafik::Sampler::Sampler(const Graphix& gfx)
{
    D3D11_SAMPLER_DESC sd = {};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    HRESULT res = gfx.GetDevice()->CreateSamplerState(&sd, &pSampler);
    _ASSERT(res == S_OK);
}
Grafik::Sampler::~Sampler()
{

}
void Grafik::Sampler::Bind(const Graphix& gfx)
{
    gfx.GetContext()->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}

Grafik::Sampler& Grafik::Sampler::operator=(Sampler&& moved)
{
    if (this->pSampler)
    {
        this->pSampler->Release();
    }
    this->pSampler = moved.pSampler;
    moved.pSampler = nullptr;
    return *this;
}


Grafik::Texture::Texture(const Graphix& gfx, const unsigned char* fontData)
{
    fvec4* unwrappedFontData = new fvec4[fontImageSize[1] * fontImageSize[0]];
    for (int i = 0; i < 5376; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            if (font[i] & (1 << k))
            {
                unwrappedFontData[8 * i + k].x = 1.0f;
                unwrappedFontData[8 * i + k].y = 1.0f;
                unwrappedFontData[8 * i + k].z = 1.0f;
                unwrappedFontData[8 * i + k].w = 1.0f;
            }
            else
            {
                unwrappedFontData[8 * i + k].x = 0.0f;
                unwrappedFontData[8 * i + k].y = 0.0f;
                unwrappedFontData[8 * i + k].z = 0.0f;
                unwrappedFontData[8 * i + k].w = 0.0f;
            }
        }

    }
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = fontImageSize[0];
    td.Height = fontImageSize[1];

    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = unwrappedFontData;
    sd.SysMemPitch = fontImageSize[0] * sizeof(fvec4);
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

    HRESULT res = gfx.GetDevice()->CreateTexture2D(&td, &sd, &pTexture);
    _ASSERT(res == S_OK);

    D3D11_SHADER_RESOURCE_VIEW_DESC srd = {};
    srd.Format = td.Format;
    srd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srd.Texture2D.MostDetailedMip = 0;
    srd.Texture2D.MipLevels = td.MipLevels;
    res = gfx.GetDevice()->CreateShaderResourceView(pTexture.Get(), &srd, &pTextureView);
    _ASSERT(res == S_OK);

    delete[] unwrappedFontData;
}
Grafik::Texture::Texture(const Grafik::Graphix& gfx, const fvec4* col, const fvec2& size)
{
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = size.x;
    td.Height = size.y;

    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = col;
    sd.SysMemPitch = size.x * sizeof(fvec4);
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

    HRESULT res = gfx.GetDevice()->CreateTexture2D(&td, &sd, &pTexture);
    _ASSERT(res == S_OK);

    D3D11_SHADER_RESOURCE_VIEW_DESC srd = {};
    srd.Format = td.Format;
    srd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srd.Texture2D.MostDetailedMip = 0;
    srd.Texture2D.MipLevels = td.MipLevels;
    res = gfx.GetDevice()->CreateShaderResourceView(pTexture.Get(), &srd, &pTextureView);
    _ASSERT(res == S_OK);
}
Grafik::Texture::~Texture()
{
}
void Grafik::Texture::Bind(const Graphix& gfx)
{
    gfx.GetContext()->PSSetShaderResources(0, 1u, pTextureView.GetAddressOf());
}
Grafik::Texture& Grafik::Texture::operator=(Texture&& moved)
{
    if (this->pTextureView)
    {
        this->pTextureView->Release();
    }
    this->pTextureView = std::move(moved.pTextureView);
    moved.pTextureView = nullptr;
    return *this;
}


Grafik::VertexBuffer::VertexBuffer(const Graphix& gfx, const int size, const Vertex* vert)
{
    stride = sizeof(Vertex);

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)size * sizeof(Vertex);
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sr = {0};
    sr.pSysMem = vert;
    HRESULT res = gfx.GetDevice()->CreateBuffer(&bd, &sr, &pVertexBuffer);
    _ASSERT(res == S_OK);
}
Grafik::VertexBuffer::~VertexBuffer()
{
}
void Grafik::VertexBuffer::Bind(const Graphix& gfx)
{
    UINT offsets = 0;
    gfx.GetContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offsets);
}
Grafik::VertexBuffer& Grafik::VertexBuffer::operator=(VertexBuffer&& moved)
{
    if (this->pVertexBuffer)
    {
        this->pVertexBuffer->Release();
    }
    this->stride = moved.stride;
    this->pVertexBuffer = std::move(moved.pVertexBuffer);
    moved.pVertexBuffer = nullptr;
    return *this;
}


/*
* Die Eingentlichen Objecte...
* 
* 
*/


Grafik::TexturedRectangle::TexturedRectangle(const Grafik::Graphix& gfx, const fvec4* col, const fvec2& texSize, const fvec2& size, const fvec2& startPos) : texBuf(gfx, col, texSize)
{
    fvec4 color = { 1.0f, 1.0f, 1.0f, -1.0f };
    Vertex vertices[4] = { {{startPos}, color, {0.0f, 0.0f}}, {{startPos.x, startPos.y - size.y},color,{0.0f, 1.0f}},{{startPos.x + size.x, startPos.y - size.y},color,{1.0f,1.0f}},{{startPos.x + size.x, startPos.y},color,{1.0f,0.0f}} };
    UINT indices[6] = { 0,3,2,2,1,0 };
    
    vertBuf = std::move(VertexBuffer(gfx, 4, vertices));
    indBuf = std::move(IndexBuffer(gfx, 6, indices));
}
void Grafik::TexturedRectangle::Draw(const Grafik::Graphix& gfx)
{
    vertBuf.Bind(gfx);
    indBuf.Bind(gfx);
    texBuf.Bind(gfx);

    gfx.GetContext()->DrawIndexed(6, 0, 0);
}




std::atomic<int> Window::WinRefCounter;
LRESULT CALLBACK Window::StaticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* wnd = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    return wnd->HandleWindowMessage(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
void Window::mainthread(Window* wnd)
{
    const int DefaultWidth = 640;
    const int DefaultHeight = 480;

    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

    WCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    HICON hIcon = ExtractIcon(hInstance, szExePath, 0);

    WNDCLASSEX wndClass{ 0 };
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_OWNDC;
    wndClass.lpfnWndProc = Window::StaticWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = hIcon;
    wndClass.hCursor = NULL;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = wnd->Name.c_str();
    wndClass.hIconSm = nullptr;

    RegisterClassEx(&wndClass);

    wnd->winRec = { 0, 0, DefaultWidth, DefaultHeight };

    AdjustWindowRect(&wnd->winRec, WS_OVERLAPPEDWINDOW, FALSE);


    wnd->hWnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, wnd->Name.c_str(), wnd->Name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, (wnd->winRec.right - wnd->winRec.left), (wnd->winRec.bottom - wnd->winRec.top), 0, 0,hInstance, wnd);
    _ASSERT(wnd->hWnd != NULL);
  
    wnd->gfx.Initialize(wnd->hWnd, { DefaultWidth, DefaultHeight });
   
    Grafik::BothShader standardShader(wnd->gfx);
    Grafik::Blender blender(wnd->gfx, true);
    Grafik::Sampler sampler(wnd->gfx);
    Grafik::Texture fontTexture(wnd->gfx, font);

    ShowWindow(wnd->hWnd, SW_SHOWDEFAULT);

    while (IsWindowEnabled(wnd->hWnd))
    {
        MSG message = {};
        while (PeekMessageW(&message, wnd->hWnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        wnd->gfx.BeginDrawing();
        wnd->gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
        sampler.Bind(wnd->gfx);
        standardShader.Bind(wnd->gfx);
        blender.Bind(wnd->gfx);
        fontTexture.Bind(wnd->gfx);

        wnd->callbackFunktion(wnd->gfx);

        wnd->gfx.EndDrawing();
    }
    WinRefCounter--;
}
bool Window::Activ()
{
    if (WinRefCounter > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Window::Window(const std::wstring& WindowName, void(_fastcall* DrawFunktion)(Grafik::Graphix& gfx)) : Name(WindowName)
{
    WinRefCounter++;
    this->callbackFunktion = DrawFunktion;
    drawThreadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainthread, this, 0, 0);
}

Window::~Window()
{
   
}