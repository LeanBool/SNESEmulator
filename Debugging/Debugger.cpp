#include "Debugger.h"
#include <vector>
#include <iostream>
#include "../CPU.h"

static DebugWindow* dieses = nullptr;
static float usedFontSize = 0.08f;
class DebugZeile
{
public:
	DebugZeile() = default;
	DebugZeile(const std::string& stri, int ID)
	{
		this->ID = ID;
		col = { 1.0f, 1.0f, 1.0f, 1.0f };
		this->str = stri;
		size = { 1.9f, 0.08f };
		float numZeilen = 1.0f / usedFontSize;
		DebugZeile::BetrachteteZeile = ID / 2 - numZeilen + 1;
	}
	void isOverlapped(const fvec2& mousep)
	{
		if (mousep.x > bottomleft.x && mousep.x < bottomleft.x + size.x)
		{
			if (mousep.y > bottomleft.y && mousep.y < bottomleft.y + size.y)
			{
				col = { 0.3f, 0.3f, 0.3f,1.0f };
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					SelectedZeile = ID;
					col = SelectedCol;
				}
			}
			else
			{
				col = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}
	}
	void OverrideZeile(WPARAM wParam)
	{
		static bool activ = false;
		if (wParam == VK_RETURN)
		{
			activ = !activ;
		}
		if (activ)
		{
			for (int i = 0x41; i < 0x5A; i++)
			{
				if (wParam == i)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						str.push_back('A' + i - 0x41);
					}
					else
					{
						str.push_back('a' + i - 0x41);
					}
				}
			}
			for (int i = 0; i < 10; i++)
			{
				if (wParam == i + 0x30)
				{
					if(GetAsyncKeyState(VK_SHIFT))
					{
						if (i == 0)
						{
							str.push_back('=');
						}
						else
						{
							str.push_back('!' + i-1);
						}
					}
					else
					{
						str.push_back('0' + i);
					}
				}
			}
			if (wParam == VK_BACK && str.size() > 0)
			{
				str.pop_back();
			}
			if (wParam == VK_SPACE)
			{
				str.push_back(' ');
			}
		}
	}
	
	void Draw(Grafik::Graphix& gfx)
	{
		bottomleft = { -1.0f, 1.00f - 0.08f * (ID - BetrachteteZeile) };
		if (ID == SelectedZeile)
		{
			col = SelectedCol;
		}
		Grafik::Vertex vertices[4] = { {{bottomleft.x, bottomleft.y},col, {}},{{bottomleft.x + size.x, bottomleft.y},col,{}}, {{bottomleft.x, bottomleft.y + size.y},col,{}},{{bottomleft.x + size.x, bottomleft.y + size.y},col,{}} };
		UINT inds[6] = { 0,2,3,3,1,0 };
		Grafik::VertexBuffer vertBuf(gfx, 4, vertices);
		Grafik::IndexBuffer indBuf(gfx, 6, inds);
		vertBuf.Bind(gfx);
		indBuf.Bind(gfx);
		gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx.GetContext()->DrawIndexed(6, 0, 0);
		if (str.size() > 0)
		{
			Grafik::String stri(gfx, { 0.0f, 0.0f, 0.0f, 1.0f }, str, bottomleft, usedFontSize);
			stri.Draw(gfx);
		}
	}

	static int NumVisibleZeilen;
	static int BetrachteteZeile;
	static int SelectedZeile;
private:
	int ID;
	fvec4 col;
	fvec2 bottomleft;
	fvec2 size;
	std::string str;
	static constexpr fvec4 SelectedCol = { 0.25f, 0.25f, 0.4136f, 1.0f };
};
int DebugZeile::NumVisibleZeilen = 2.0f/usedFontSize + 1;
int DebugZeile::BetrachteteZeile = 0;
int DebugZeile::SelectedZeile = 0;
static std::vector<DebugZeile> zeilen;

class ScrollBar
{
public:
	ScrollBar()
	{
		vertices[0] = { {BarBaseTopLeft},BarBaseCol,{0.0f, 0.0f} };
		vertices[1] = { {BarBaseTopLeft.x, BarBaseTopLeft.y - BarBaseSize.y}, BarBaseCol, {0,0} };
		vertices[2] = { {BarBaseTopLeft.x + BarBaseSize.x, BarBaseTopLeft.y - BarBaseSize.y },BarBaseCol, {0,0} };
		vertices[3] = { {BarBaseTopLeft.x + BarBaseSize.x, BarBaseTopLeft.y}, BarBaseCol, {0,0} };
		vertices[4].color = BarSliderCol;
		vertices[5].color = BarSliderCol;
		vertices[6].color = BarSliderCol;
		vertices[7].color = BarSliderCol;
	}

	void SetScrollBarPosFromZeile()
	{
		float numZeilen = 1.0f / usedFontSize;
		y_aktuell = 1.0f - 2.0f/(float)zeilen.size() * (DebugZeile::BetrachteteZeile + numZeilen/2 + 1);
	}
	void SetZeileFromScrollBarPos()
	{
		float numZeilen = 1.0f / usedFontSize;
		DebugZeile::BetrachteteZeile = (1.0f - y_aktuell) * (float)zeilen.size() / 2.0f - numZeilen/2;
	}
	void SetScrollBarPosMouse(const fvec2& mousep)
	{
		if (mousep.x > BarBaseTopLeft.x && mousep.x < BarBaseTopLeft.x + BarBaseSize.x)
		{
			if (mousep.y > BarBaseTopLeft.y - BarBaseSize.y && mousep.y < BarBaseTopLeft.y)
			{
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					y_aktuell = mousep.y;
					SetZeileFromScrollBarPos();
				}
			}
		}
	}

	void Draw(Grafik::Graphix& gfx)
	{
		float numZeilen = 2.0f / usedFontSize;
		y_size = numZeilen / (float)zeilen.size();
		if (y_size > 2.0f)
		{
			y_size = 2.0f;
		}
		if(y_size < 0.01f)
		{
			y_size = 0.01f;
		}

		vertices[4].pos = { BarBaseTopLeft.x, y_aktuell + y_size};
		vertices[5].pos = { BarBaseTopLeft.x ,y_aktuell + -y_size };
		vertices[6].pos = { BarBaseTopLeft.x + BarBaseSize.x,y_aktuell + -y_size };
		vertices[7].pos = { BarBaseTopLeft.x + BarBaseSize.x, y_aktuell + y_size };

		UINT pInds[12] = { 0,3,2,2,1,0,4,7,6,6,5,4 };
		Grafik::VertexBuffer vertBuf(gfx, 8, vertices);
		Grafik::IndexBuffer indBuf(gfx, 12, pInds);

		vertBuf.Bind(gfx);
		indBuf.Bind(gfx);

		gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx.GetContext()->DrawIndexed(12, 0, 0);
	}

private:
	float y_size = 0.0f;
	float y_aktuell = 0.0f;
	static constexpr fvec2 BarBaseTopLeft = { 0.9f, 1.0f };
	static constexpr fvec2 BarBaseSize = { 0.1f, 2.0f };
	static constexpr fvec4 BarBaseCol = { 0.258824f, 0.654902f, 0.960784f, 1.0f };
	static constexpr fvec4 BarSliderCol = { 0.058824f, 0.015686f, 0.431373f, 1.0f };
	static constexpr fvec4 SelectedCol = { 0.103f, 0.035f, 0.4136f, 1.0f };
	Grafik::Vertex vertices[8];
};
static ScrollBar bar;


void _fastcall DebugWindow::DrawFunktion(Grafik::Graphix& gfx)
{
	for (int i = 0; i < DebugZeile::NumVisibleZeilen; i++)
	{	
		if (DebugZeile::BetrachteteZeile + i < zeilen.size())
		{
			zeilen.at(DebugZeile::BetrachteteZeile + i).Draw(gfx);
		}
	}
	bar.Draw(gfx);
}
void _fastcall  DebugWindow::msgFunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT mp;
	GetCursorPos(&mp);
	ScreenToClient(hWnd, &mp);
	RECT winRect;
	GetWindowRect(hWnd, &winRect);
	fvec2 mousePos = { (float)mp.x / (float)(winRect.right - winRect.left - 20) * 2.0f - 1.0f, -(float)mp.y / (float)(winRect.bottom - winRect.top - 40) * 2.0f + 1.0f };

	for (int i = 0; i < DebugZeile::NumVisibleZeilen; i++)
	{
		if (DebugZeile::BetrachteteZeile + i < zeilen.size())
		{
			zeilen.at(DebugZeile::BetrachteteZeile + i).isOverlapped(mousePos);
		}
	}
	bar.SetScrollBarPosMouse(mousePos);
	switch (msg)
	{
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			DebugZeile::BetrachteteZeile--;
		}
		else
		{
			DebugZeile::BetrachteteZeile++;
		}
		bar.SetScrollBarPosFromZeile();
		break;
	}
}
void DebugWindow::Dissassemble()
{
	for (int i = 0; i < Cpu.memory.at(0).size(); i++)
	{
		zeilen.push_back(DebugZeile("Hallo " + std::to_string(i), i));
	}
}
DebugWindow::DebugWindow(const CPU& cpu) : wnd(L"Debugger", DebugWindow::DrawFunktion, DebugWindow::msgFunk), Cpu(cpu)
{
	dieses = this;

	Dissassemble();
	
	wnd.initialized = true;
}