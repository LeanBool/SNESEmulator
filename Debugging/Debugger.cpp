#include "Debugger.h"
#include <vector>
#include <iostream>
#include "../CPU.h"
#include "../Grafik/FontInformation.h"


std::string CreateHexString(int feinheit, uint32_t Mem)
{
	std::string str;
	static constexpr char hexdingens[16] = { '0', '1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	for (int i = 0; i < feinheit; i++)
	{
		int res = ((Mem & (0xF << 4 * (feinheit - i - 1))) >> 4 * (feinheit - i - 1));
		str += hexdingens[res];
	}
	return str;
}

struct DebugInstruction
{
	std::string name;
	ivec3 length;
};
static int currentBank = 0;
//Die LookUpTable enthält die Informationen über die länger der Instruction: length.x = NORMAL; length.y = M(flag)==1; length.z = INDEX(flag)==1; Falls INDEX(flag) == 1 && M(flag) == 1 ==> length = max(length.y,length.z);
static const DebugInstruction lookup[] =	{	{"BRK", {2,2,2}},{"ORA", {2,2,2}},{"COP", {2,2,2}},{"ORA", {2,2,2}},{"TSB", {2,2,2}},{"ORA", {2,2,2}},{"ASL", {2,2,2}},{"ORA", {2,2,2}},{"PHP", {1,1,1}},{"ORA", {2,3,2}},{"ASL", {1,1,1}},{"PHD", {1,1,1}},{"TSB", {3,3,3}},{"ORA", {3,3,3}},{"ASL", {3,3,3}},{"ORA", {4,4,4}},
												{"BPL", {2,2,2}},{"ORA", {2,2,2}},{"ORA", {2,2,2}},{"ORA", {2,2,2}},{"TRB", {2,2,2}},{"ORA", {2,2,2}},{"ASL", {2,2,2}},{"ORA", {2,2,2}},{"CLC", {1,1,1}},{"ORA", {3,3,3}},{"INC", {1,1,1}},{"TCS", {1,1,1}},{"TRB", {3,3,3}},{"ORA", {3,3,3}},{"ASL", {3,3,3}},{"ORA", {4,4,4}},
												{"JSR", {3,3,3}},{"AND", {2,2,2}},{"JSL", {4,4,4}},{"AND", {2,2,2}},{"BIT", {2,2,2}},{"AND", {2,2,2}},{"ROL", {2,2,2}},{"AND", {2,2,2}},{"PLP", {1,1,1}},{"AND", {2,3,2}},{"ROL", {1,1,1}},{"PLD", {1,1,1}},{"BIT", {3,3,3}},{"AND", {3,3,3}},{"ROL", {3,3,3}},{"AND", {4,4,4}},
												{"BMI", {2,2,2}},{"AND", {2,2,2}},{"AND", {2,2,2}},{"AND", {2,2,2}},{"BIT", {2,2,2}},{"AND", {2,2,2}},{"ROL", {2,2,2}},{"AND", {2,2,2}},{"SEC", {1,1,1}},{"AND", {3,3,3}},{"DEC", {1,1,1}},{"TSC", {1,1,1}},{"BIT", {3,3,3}},{"AND", {3,3,3}},{"ROL", {3,3,3}},{"AND", {4,4,4}},
												{"RTI", {1,1,1}},{"EOR", {2,2,2}},{"WDM", {2,2,2}},{"EOR", {2,2,2}},{"MVP", {3,3,3}},{"EOR", {2,2,2}},{"LSR", {2,2,2}},{"EOR", {2,2,2}},{"PHA", {1,1,1}},{"EOR", {2,3,2}},{"LSR", {1,1,1}},{"PHK", {1,1,1}},{"JMP", {3,3,3}},{"EOR", {3,3,3}},{"LSR", {3,3,3}},{"EOR", {4,4,4}},
												{"BVC", {2,2,2}},{"EOR", {2,2,2}},{"EOR", {2,2,2}},{"EOR", {2,2,2}},{"MVN", {3,3,3}},{"EOR", {2,2,2}},{"LSR", {2,2,2}},{"EOR", {2,2,2}},{"CLI", {1,1,1}},{"EOR", {3,3,3}},{"PHY", {1,1,1}},{"TCD", {1,1,1}},{"JMP", {4,4,4}},{"EOR", {3,3,3}},{"LSR", {3,3,3}},{"EOR", {4,4,4}},
												{"RTS", {1,1,1}},{"ADC", {2,2,2}},{"PER", {3,3,3}},{"ADC", {2,2,2}},{"STZ", {2,2,2}},{"ADC", {2,2,2}},{"ROR", {2,2,2}},{"ADC", {2,2,2}},{"PLA", {1,1,1}},{"ADC", {2,3,2}},{"ROR", {1,1,1}},{"RTL", {1,1,1}},{"JMP", {3,3,3}},{"ADC", {3,3,3}},{"ROR", {3,3,3}},{"ADC", {4,4,4}},
												{"BVS", {2,2,2}},{"ADC", {2,2,2}},{"ADC", {2,2,3}},{"ADC", {2,2,2}},{"STZ", {2,2,2}},{"ADC", {2,2,2}},{"ROR", {2,2,2}},{"ADC", {2,2,2}},{"SEI", {1,1,1}},{"ADC", {3,3,3}},{"PLY", {1,1,1}},{"TDC", {1,1,1}},{"JMP", {3,3,3}},{"ADC", {3,3,3}},{"ROR", {3,3,3}},{"ADC", {4,4,4}},
												{"BRA", {2,2,2}},{"STA", {2,2,2}},{"BRL", {3,3,3}},{"STA", {2,2,2}},{"STY", {2,2,2}},{"STA", {2,2,2}},{"STX", {2,2,2}},{"STA", {2,2,2}},{"DEY", {1,1,1}},{"BIT", {2,3,2}},{"TXA", {1,1,1}},{"PHB", {1,1,1}},{"STY", {3,3,3}},{"STA", {3,3,3}},{"STX", {3,3,3}},{"STA", {4,4,4}},
												{"BCC", {2,2,2}},{"STA", {2,2,2}},{"STA", {2,2,2}},{"STA", {2,2,2}},{"STY", {2,2,2}},{"STA", {2,2,2}},{"STX", {2,2,2}},{"STA", {2,2,2}},{"TYA", {1,1,1}},{"STA", {3,3,3}},{"TXS", {1,1,1}},{"TXY", {1,1,1}},{"STZ", {3,3,3}},{"STA", {3,3,3}},{"STZ", {3,3,3}},{"STA", {4,4,4}},
												{"LDY", {2,2,3}},{"LDA", {2,2,2}},{"LDX", {2,2,3}},{"LDA", {2,2,2}},{"LDY", {2,2,2}},{"LDA", {2,2,2}},{"LDX", {2,2,2}},{"LDA", {2,2,2}},{"TAY", {1,1,1}},{"LDA", {2,3,2}},{"TAX", {1,1,1}},{"PLB", {1,1,1}},{"LDY", {3,3,3}},{"LDA", {3,3,3}},{"LDX", {3,3,3}},{"LDA", {4,4,4}},
												{"BCS", {2,2,2}},{"LDA", {2,2,2}},{"LDA", {2,2,2}},{"LDA", {2,2,2}},{"LDY", {2,2,2}},{"LDA", {2,2,2}},{"LDX", {2,2,2}},{"LDA", {2,2,2}},{"CLV", {1,1,1}},{"LDA", {3,3,3}},{"TSX", {1,1,1}},{"TYX", {1,1,1}},{"LDY", {3,3,3}},{"LDA", {3,3,3}},{"LDX", {3,3,3}},{"LDA", {4,4,4}},
												{"CPY", {2,2,3}},{"CMP", {2,2,2}},{"REP", {2,2,2}},{"CMP", {2,2,2}},{"CPY", {2,2,2}},{"CMP", {2,2,2}},{"DEC", {2,2,2}},{"CMP", {2,2,2}},{"INY", {1,1,1}},{"CMP", {2,3,2}},{"DEX", {1,1,1}},{"WAI", {1,1,1}},{"CPY", {3,3,3}},{"CMP", {3,3,3}},{"DEC", {3,3,3}},{"CMP", {4,4,4}},
												{"BNE", {2,2,2}},{"CMP", {2,2,2}},{"CMP", {2,2,2}},{"CMP", {2,2,2}},{"PEI", {2,2,2}},{"CMP", {2,2,2}},{"DEC", {2,2,2}},{"CMP", {2,2,2}},{"CLD", {1,1,1}},{"CMP", {3,3,3}},{"PHX", {1,1,1}},{"STP", {1,1,1}},{"JML", {3,3,3}},{"CMP", {3,3,3}},{"DEC", {3,3,3}},{"CMP", {4,4,4}},
												{"CPX", {2,2,3}},{"SBC", {2,2,2}},{"SEP", {2,2,2}},{"SBC", {2,2,2}},{"CPX", {2,2,2}},{"SBC", {2,2,2}},{"INC", {2,2,2}},{"SBC", {2,2,2}},{"INX", {1,1,1}},{"SBC", {2,3,2}},{"NOP", {1,1,1}},{"XBA", {1,1,1}},{"CPX", {3,3,3}},{"SBC", {3,3,3}},{"INC", {3,3,3}},{"SBC", {4,4,4}},
												{"BEQ", {2,2,2}},{"SBC", {2,2,2}},{"SBC", {2,2,2}},{"SBC", {2,2,2}},{"PEA", {3,3,3}},{"SBC", {2,2,2}},{"INC", {2,2,2}},{"SBC", {2,2,2}},{"SED", {1,1,1}},{"SBC", {3,3,3}},{"PLX", {1,1,1}},{"XCE", {1,1,1}},{"JSR", {3,3,3}},{"SBC", {3,3,3}},{"INC", {3,3,3}},{"SBC", {4,4,4}} };


static DebugWindow* dieses = nullptr;
static float usedFontSize = 0.08f;
class DebugRegisters
{
public:
	DebugRegisters() = default;
	DebugRegisters(const CPU& Cpu) : Cpu(Cpu)
	{
		bottomleftfirst = { -1.0f, 1.0f - usedFontSize };
	}
	void Draw(Grafik::Graphix& gfx)
	{
		fvec2 temp = bottomleftfirst;
		if (Cpu.status & (1 << 0))
		{
			Grafik::String stri(gfx, ActivFlagCol, "C", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "C", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if (Cpu.status & (1 << 1))
		{
			Grafik::String stri(gfx, ActivFlagCol, "Z", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "Z", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if (Cpu.status & (1 << 2))
		{
			Grafik::String stri(gfx, ActivFlagCol, "I", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "I", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if (Cpu.status & (1 << 3))
		{
			Grafik::String stri(gfx, ActivFlagCol, "D", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "D", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if (Cpu.status & (1 << 4))
		{
			Grafik::String stri(gfx, ActivFlagCol, "IDX", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "IDX", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 5 * usedFontSize;

		if (Cpu.status & (1 << 5))
		{
			Grafik::String stri(gfx, ActivFlagCol, "M", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "M", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if (Cpu.status & (1 << 6))
		{
			Grafik::String stri(gfx, ActivFlagCol, "V", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "V", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if (Cpu.status & (1 << 7))
		{
			Grafik::String stri(gfx, ActivFlagCol, "N", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "N", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 3 * usedFontSize;

		if(Cpu.emulation_mode)
		{
			Grafik::String stri(gfx, ActivFlagCol, "EMU", temp, usedFontSize);
			stri.Draw(gfx);
		}
		else
		{
			Grafik::String stri(gfx, InActivFlagCol, "EMU", temp, usedFontSize);
			stri.Draw(gfx);
		}
		temp.x += breiteZusatz * 6 * usedFontSize;

		Grafik::String PCString(gfx, { 1.0f,1.0f,1.0f,1.0f }, "PC = 0x" + CreateHexString(4,Cpu.PC), temp,usedFontSize);
		PCString.Draw(gfx);
		temp.x += breiteZusatz * 14 * usedFontSize;
		Grafik::String AString(gfx, { 1.0f,1.0f,1.0f,1.0f }, "A = 0x" + CreateHexString(4,Cpu.A), temp, usedFontSize);
		AString.Draw(gfx);
		temp.x += breiteZusatz * 13 * usedFontSize;
		Grafik::String XString(gfx, { 1.0f, 1.0f, 1.0f, 1.0f }, "X = 0x" + CreateHexString(4,Cpu.X), temp, usedFontSize);
		XString.Draw(gfx);
		//Erste Zeile vorbei

		temp = { bottomleftfirst.x, bottomleftfirst.y - usedFontSize };
		Grafik::String YString(gfx, { 1.0f,1.0f,1.0f,1.0f }, "Y = 0x" + CreateHexString(4,Cpu.Y), temp, usedFontSize);
		YString.Draw(gfx);
		temp.x += breiteZusatz * 13 * usedFontSize;
		Grafik::String SPString(gfx, { 1.0f,1.0f,1.0f,1.0f }, "SP = 0x" + CreateHexString(4,Cpu.SP), temp, usedFontSize);
		SPString.Draw(gfx);
		temp.x += breiteZusatz * 14 * usedFontSize;
		Grafik::String DPString(gfx, { 1.0f, 1.0f,1.0f,1.0f }, "DP = 0x" + CreateHexString(4,Cpu.DP), temp, usedFontSize);
		DPString.Draw(gfx);
		temp.x += breiteZusatz * 14 * usedFontSize;
		Grafik::String DBRString(gfx, { 1.0f, 1.0f,1.0f,1.0f }, "DBR = 0x" + CreateHexString(2,Cpu.DBR), temp, usedFontSize);
		DBRString.Draw(gfx);
		temp.x += breiteZusatz * 13 * usedFontSize;
		Grafik::String PBRString(gfx, { 1.0f, 1.0f,1.0f,1.0f }, "PBR = 0x" + CreateHexString(2,Cpu.PBR), temp, usedFontSize);
		PBRString.Draw(gfx);
	}
	const CPU& Cpu;
private:
	fvec2 bottomleftfirst;
	std::string str;
	static constexpr fvec4 ActivFlagCol = { 0.0f, 1.0f, 0.0f, 1.0f };
	static constexpr fvec4 InActivFlagCol = { 1.0f, 0.0f, 0.0f, 1.0f };
};
class DebugZeile
{
public:
	DebugZeile() = default;
	DebugZeile(int ID)
	{
		this->ID = ID;
		col = { 1.0f, 1.0f, 1.0f, 1.0f };
		size = { 1.9f, 0.08f };
		float numZeilen = 1.0f / usedFontSize;
		DebugZeile::BetrachteteStartAddresse = ID / 2 - numZeilen + 1;
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
	
	void Draw(Grafik::Graphix& gfx)
	{
		bottomleft = { -1.0f, 1.00f - 0.08f * (ID + 3) };
		if (ID == SelectedZeile)
		{
			col = SelectedCol;
		}
		if ((BetrachteteStartAddresse + ID) >= 0 && (BetrachteteStartAddresse + ID) < dieses->Cpu.memory.at(currentBank).size())
		{				
			int addr = BetrachteteStartAddresse;
			if (addr < 0)
			{
				addr = 0;
			}
			
			for (int i = 0; i < ID; i++)
			{
				if (addr < dieses->Cpu.memory.at(currentBank).size())
				{
					addr += lookup[dieses->Cpu.memory.at(currentBank).at(addr)].length.x;
				}
				
			}
			if (addr < dieses->Cpu.memory.at(currentBank).size())
			{
				if (((dieses->Cpu.PBR << 16) | dieses->Cpu.PC) >= ((currentBank << 16) | addr) && ((dieses->Cpu.PBR << 16) | dieses->Cpu.PC) < ((currentBank << 16) | addr + 1))
				{
					col = PCColor;
				}
				Grafik::Vertex vertices[4] = { {{bottomleft.x, bottomleft.y},col, {}},{{bottomleft.x + size.x, bottomleft.y},col,{}}, {{bottomleft.x, bottomleft.y + size.y},col,{}},{{bottomleft.x + size.x, bottomleft.y + size.y},col,{}} };
				UINT inds[6] = { 0,2,3,3,1,0 };
				Grafik::VertexBuffer vertBuf(gfx, 4, vertices);
				Grafik::IndexBuffer indBuf(gfx, 6, inds);
				vertBuf.Bind(gfx);
				indBuf.Bind(gfx);
				gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				gfx.GetContext()->DrawIndexed(6, 0, 0);


				std::string str = CreateHexString(6, (currentBank << 16) | addr) + ":  " + lookup[dieses->Cpu.memory.at(currentBank).at(addr)].name + "  ";
				for (int i = 1; i < lookup[dieses->Cpu.memory.at(currentBank).at(addr)].length.x; i++)
				{
					if ((addr + i) < dieses->Cpu.memory.at(currentBank).size())
					{
						str += CreateHexString(2,dieses->Cpu.memory.at(currentBank).at(addr + i)) + " ";
					}
				}
				if (addr < dieses->Cpu.memory.at(currentBank).size())
				{
					Grafik::String stri(gfx, { 0.0f, 0.0f, 0.0f, 1.0f }, str, bottomleft, usedFontSize);
					stri.Draw(gfx);
				}
			}
		}
	}

	static int NumVisibleZeilen;
	static int BetrachteteStartAddresse;
	static int SelectedZeile;
private:
	int ID;
	fvec4 col;
	fvec2 bottomleft;
	fvec2 size;
	static constexpr fvec4 SelectedCol = { 0.25f, 0.25f, 0.4136f, 1.0f };
	static constexpr fvec4 PCColor = { 0.5f, 0.5f, 0.0f, 1.0f };
};
int DebugZeile::NumVisibleZeilen = 2.0f/usedFontSize + 1;
int DebugZeile::BetrachteteStartAddresse = 0;
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
		y_aktuell = 1.0f - 2.0f/(float)dieses->Cpu.memory.at(0).size() * (DebugZeile::BetrachteteStartAddresse + numZeilen/2 + 1);
	}
	void SetZeileFromScrollBarPos()
	{
		float numZeilen = 1.0f / usedFontSize;
		DebugZeile::BetrachteteStartAddresse = (1.0f - y_aktuell) * (float)dieses->Cpu.memory.at(currentBank).size() / 2.0f - numZeilen/2;
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
		y_size = numZeilen / (float)dieses->Cpu.memory.at(currentBank).size();
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
	static DebugRegisters registers(dieses->Cpu);
	for (int i = 0; i < DebugZeile::NumVisibleZeilen; i++)
	{	
		zeilen.at(i).Draw(gfx);
	}
	registers.Draw(gfx);
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
		zeilen.at(i).isOverlapped(mousePos);
	}
	bar.SetScrollBarPosMouse(mousePos);
	switch (msg)
	{
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			DebugZeile::BetrachteteStartAddresse--;
		}
		else
		{
			DebugZeile::BetrachteteStartAddresse++;
		}
		bar.SetScrollBarPosFromZeile();
		break;
	}
}
void DebugWindow::Dissassemble()
{
	for (int i = 0; i < DebugZeile::NumVisibleZeilen; i++)
	{
		zeilen.push_back(DebugZeile(i));
	}
}
DebugWindow::DebugWindow(const CPU& cpu) : wnd(L"Debugger", DebugWindow::DrawFunktion, DebugWindow::msgFunk), Cpu(cpu)
{
	dieses = this;
	Dissassemble();
	
	wnd.initialized = true;
}