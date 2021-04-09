#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "../GlobalStructs.h"
#include <wrl.h>
#include <string>
#include <atomic>

namespace Grafik
{
	struct Vertex
	{
		fvec2 pos;
		fvec4 color;
		fvec2 texPos;
	};
	class Graphix
	{
	public:
		~Graphix();
		void Initialize(HWND hWnd, const ivec2& size);
		void Recreate();

		void BeginDrawing() const;
		void EndDrawing() const;

		ID3D11DeviceContext* GetContext() const;
		ID3D11Device* GetDevice() const;
		IDXGISwapChain* GetSwapChain() const;
		ID3D11RenderTargetView* GetRenderTargetView() const;

	private:
		ID3D11RenderTargetView* pRenderTargetView;

		IDXGISwapChain* pSwapChain;
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
		HWND hWnd;
	};


	class Bindable
	{
	public:
		virtual void Bind(const Graphix& gfx) = 0;
		virtual ~Bindable() = default;
	};
	class Blender : public Bindable
	{
	public:
		Blender() = default;
		virtual ~Blender();
		Blender(const Graphix& gfx, bool blending);
		virtual void Bind(const Graphix& gfx) override;

		Blender& operator=(Blender&& moved);
	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
	};
	class BothShader : public Bindable
	{
	public:
		BothShader() = default;
		BothShader(const Graphix& gfx);
		virtual ~BothShader();
		virtual void Bind(const Graphix& gfx) override;

		BothShader& operator=(BothShader&& moved);
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const Graphix& gfx, const int size, const UINT* pInd);
		virtual ~IndexBuffer();
		virtual void Bind(const Graphix& gfx) override;


		IndexBuffer& operator=(IndexBuffer&& moved);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
	class Sampler : public Bindable
	{
	public:
		Sampler() = default;
		Sampler(const Graphix& gfx);
		virtual ~Sampler();
		virtual void Bind(const Graphix& gfx) override;

		Sampler& operator=(Sampler&& moved);
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
	class Texture : public Bindable
	{
	public:
		Texture() = default;
		Texture(const Graphix& gfx, const unsigned char* fontData);
		Texture(const Graphix& gfx, const fvec4* col, const fvec2& size);
		
		virtual ~Texture();
		virtual void Bind(const Graphix& gfx) override;
		

		Texture& operator=(Texture&& moved);
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const Graphix& gfx, const int size, const Vertex* vert);
		virtual ~VertexBuffer();
		virtual void Bind(const Graphix& gfx) override;
		

		VertexBuffer& operator=(VertexBuffer&& moved);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		UINT stride;
	};

	
	class TexturedRectangle
	{
	public:
		TexturedRectangle(const Graphix& gfx, const fvec4* col, const fvec2& texSize, const fvec2& size, const fvec2& startPos = { -1.0f, 1.0f });
		void Draw(const Graphix& gfx);

	private:
		VertexBuffer vertBuf;
		IndexBuffer indBuf;
		Texture texBuf;
	};
}





class Window
{
public:
	Window(const std::wstring& WindowName, void(_fastcall* DrawFunktion)(Grafik::Graphix& gfx));
	~Window();

	static bool Activ();


	static void mainthread(Window* wnd);
private:
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT HandleWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	std::wstring Name;
	Grafik::Graphix gfx;
	HMENU hMenu;
	RECT winRec;
	HWND hWnd;
	static std::atomic<int> WinRefCounter;
	void(_fastcall* callbackFunktion)(Grafik::Graphix& gfx) = nullptr;
	HANDLE drawThreadHandle;
};