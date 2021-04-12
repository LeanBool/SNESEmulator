#pragma once
#include "../Grafik/GrafikMain.h"

class CPU;
class DebugWindow
{
public:
	DebugWindow(const CPU& Cpu);

	void Dissassemble();
	const CPU& Cpu;
private:
	static void _fastcall DrawFunktion(Grafik::Graphix& gfx);
	static void _fastcall msgFunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window wnd;
};