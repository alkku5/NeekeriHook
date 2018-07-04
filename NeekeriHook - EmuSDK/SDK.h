#pragma once


//DONT INCLUDE STUPID SHIT FROM PLAYER STRUCTS OR ANYTHING OTHERWISE YOU WILL GET 1000000 ERRORS (FOR EXAMPLE BASEENTITY.H)...

/* cheat stuff */
#define CHEAT_NAME "NeekeriHook : CSGO"
#define assert(expression) ((void)0)
//DONT INCLUDE STUPID SHIT FROM PLAYER STRUCTS OR ANYTHING OTHERWISE YOU WILL GET 1000000 ERRORS (FOR EXAMPLE BASEENTITY.H)...

/* default imports */
#include <Windows.h>
#include <Psapi.h>
#include "vfunc.h"
#include "IPanel.h"
//DONT INCLUDE STUPID SHIT FROM PLAYER STRUCTS OR ANYTHING OTHERWISE YOU WILL GET 1000000 ERRORS (FOR EXAMPLE BASEENTITY.H)...

/* standard libraries */
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <array>
#include <deque>
#include <intrin.h>
#include <atomic>
#include <mutex>
using namespace std;
//DONT INCLUDE STUPID SHIT FROM PLAYER STRUCTS OR ANYTHING OTHERWISE YOU WILL GET 1000000 ERRORS (FOR EXAMPLE BASEENTITY.H)...

/* sdk macros */
#define FUNCTION_GUARD g_Globals::szLastFunction = __FUNCTION__ /* put this before every function: if the exceptionhandler is called, it will say the function name */
#define HOOK_GUARD(x) g_Globals::szLastFunction = x /* same goes for hooks ^^ */
 
/* d3d shit */
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
 
#define XorStr(x) (x)
 
/* utils class */
class KeyValues;
class CUtils
{
public:
	void Log(const char *fmt, ...);
	DWORD FindPatternIDA(std::string modName, std::string patContent); // learn_more
	DWORD FindPatternByte(std::string modName, PBYTE patContent, char* szMask);
	HMODULE GetHandleSafe(std::string modName);
	std::uint8_t* PatternScan(void* module, const char* signature);
	void LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = nullptr, void* pfnEvaluateSymbolProc = nullptr);
	void InitKeyValues(KeyValues* pKeyValues, const char* name);
 
	template<typename FuncType>
	static FuncType Emulate(void* ppClass, int index)
	{
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}
private:
	bool ByteCompare(const BYTE* Data, const BYTE* Mask, const char* szMask);
}; extern CUtils* g_pUtils;
 

namespace index
{
	constexpr auto EndScene = 42;
	constexpr auto Reset = 16;
	constexpr auto PaintTraverse = 41;
	constexpr auto CreateMove = 21;
	constexpr auto PlaySound = 82;
	constexpr auto FrameStageNotify = 36;
	constexpr auto DrawModelExecute = 21;
	constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool = 13;
	constexpr auto OverrideView = 18;
}

/* forwards declarations */
class C_BaseEntity;
class C_BaseWeapon;
class CUserCmd;
 
/* globals */
namespace g_Globals
{
	extern char szDirectoryDLL[MAX_PATH];
	extern char* szLastFunction;
	extern HMODULE hmDLL;
	extern C_BaseEntity* ptrLocalPlayer;
	extern C_BaseWeapon* ptrLocalWeapon;
	extern CUserCmd* ptrCmd;
	extern HWND hwndWindow;
}
 
/* our sdk */
#include "VMT.h"
 
#include "IBaseClientDLL.h"
#include "CUserCmd.h"
#include "Color.h"
#include "PlayerInfo.h"
#include "ConVar.h"
 
/* main class */
class CSDK
{
public:
	// TODO: add Destroy() function
	void		  Initialize();
}; extern CSDK* g_pSDK;
 
/* shoutout to @namazso for being a cutie */
using fnv_t = unsigned;
 
class FnvHash
{
	static const fnv_t FNV_PRIME = 16777619u;
	static const fnv_t OFFSET_BASIS = 2166136261u;
 
	template <unsigned int N>
	static constexpr fnv_t fnvHashConst(const char(&str)[N], unsigned int I = N)
	{
		return static_cast<fnv_t>(1ULL * (I == 1 ? (OFFSET_BASIS ^ str[0]) : (fnvHashConst(str, I - 1) ^ str[I - 1])) * FNV_PRIME);
	}
 
	static fnv_t fnvHash(const char* str)
	{
		const auto length = strlen(str) + 1;
		auto hash = OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash ^= *str++;
			hash *= FNV_PRIME;
		}
		return hash;
	}
 
	struct Wrapper
	{
		Wrapper(const char* str) : str(str) { }
		const char* str;
	};
 
	fnv_t hash_value;
public:
	FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { }
 
	template <unsigned int N>
	constexpr FnvHash(const char(&str)[N]) : hash_value(fnvHashConst(str)) { }
 
	constexpr operator fnv_t() const { return this->hash_value; }
};
 
class CNetVarManager
{
private:
	struct StoredPropData
	{
		RecvProp* prop_ptr;
		uint16_t class_relative_offset;
	};
 
public:
	static CNetVarManager& Get()
	{
		static CNetVarManager instance;
		return instance;
	}
 
	unsigned short GetOffset(unsigned int hash) { return m_props[hash]->m_Offset; }
	RecvProp* GetPropPtr(unsigned int hash) 
	{ return m_props[hash]; }
 
	// Prevent instruction cache pollution caused by automatic
	// inlining of Get and GetOffset every netvar usage when there
	// are a lot of netvars
	__declspec(noinline) static uint16_t GetOffsetByHash(fnv_t hash)
	{
		return Get().GetOffset(hash);
	}
 
private:
	CNetVarManager();
	void DumpRecursive(RecvTable* table);
 
private:
	std::map<unsigned int, RecvProp*> m_props;
};
 
/* gay sdk imports */
#include "BaseEntity.h"
#include "BaseWeapon.h"
#include "EngineTrace.h"
#include "Prediction.h"
#include "PhysicsAPI.h"
#include "ConVar.h"
#include "Engine.h"
#include "IPaint.h"
#include "StudioDefinitions.h"
#include "MaterialDefinitions.h"
#include "GameEvent.h"
#include "ModelMaterial.h"
#include "Input.h"
 
/* menu bs */
#include "ImGUI\imgui.h"
#include "ImGUI\DX9\imgui_impl_dx9.h"
 
#define me g_pEngine->GetLocalPlayer()


enum FontOrient_t
{
	FONT_LEFT,
	FONT_CENTER,
	FONT_RIGHT
};
 
#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)
#define BLACK(alpha)         D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define RED(alpha)           D3DCOLOR_ARGB(alpha, 255, 0, 0)
#define GREEN(alpha)         D3DCOLOR_ARGB(alpha, 0, 128, 0)
#define LAWNGREEN(alpha)     D3DCOLOR_ARGB(alpha, 124, 252, 0)
#define BLUE(alpha)          D3DCOLOR_ARGB(alpha, 0, 200, 255)
#define DEEPSKYBLUE(alpha)   D3DCOLOR_ARGB(alpha, 0, 191, 255)
#define SKYBLUE(alpha)       D3DCOLOR_ARGB(alpha, 0, 122, 204)
#define YELLOW(alpha)        D3DCOLOR_ARGB(alpha, 255, 255, 0)
#define ORANGE(alpha)        D3DCOLOR_ARGB(alpha, 255, 165, 0)
#define DARKORANGE(alpha)    D3DCOLOR_ARGB(alpha, 255, 140, 0)
#define PURPLE(alpha)        D3DCOLOR_ARGB(alpha, 125, 0, 255)
#define CYAN(alpha)          D3DCOLOR_ARGB(alpha, 0, 255, 255)
#define PINK(alpha)          D3DCOLOR_ARGB(alpha, 255, 20, 147)
#define GRAY(alpha)          D3DCOLOR_ARGB(alpha, 128, 128, 128)
#define DARKGRAY(alpha)      D3DCOLOR_ARGB(alpha, 73, 73, 73)
#define DARKERGRAY(alpha)    D3DCOLOR_ARGB(alpha, 31, 31, 31)
 
/* for your OG ValveSDK colours xdxdxdxd */
#define GetAlpha(col) (((col)&0xff000000)>>24)
#define GetRed(col) (((col)&0x00ff0000)>>16)
#define GetGreen(col) (((col)&0x0000ff00)>>8)
#define GetBlue(col) ((col)&0x000000ff)
 
enum circle_type { full, half, quarter };
 
struct CUSTOMVERTEX {
	FLOAT X, Y, Z; DWORD COLOR;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
 
struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};
 
class CRender
{
public:
	struct sScreen
	{
		float Width;
		float Height;
		float x_center;
		float y_center;
	} Screen;
 
 
	LPDIRECT3DDEVICE9 device;
	LPDIRECT3DVERTEXBUFFER9 vb;    // Buffer to hold vertices
	LPDIRECT3DINDEXBUFFER9  ib;    // Buffer to hold indices
 
	LPD3DXLINE line;
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 white_tex;
 
	struct sFonts
	{
		LPD3DXFONT esp;
		LPD3DXFONT esp_small;
	}fonts;
 
	void Init(IDirect3DDevice9* pDevice);
	void Reset();
 
	int StringWidth(ID3DXFont* font, char* string);
	void Line(int x, int y, int x2, int y2, D3DCOLOR color);
	void FilledBox(int x, int y, int width, int height, D3DCOLOR color);
	void FilledBoxOutlined(int x, int y, int width, int height, D3DCOLOR colour, D3DCOLOR outlinecolor, int thickness = 1.0f);
	void BorderedBox(int x, int y, int width, int height, D3DCOLOR color, int thickness = 1.0f);
	void BorderedBoxOutlined(int x, int y, int width, int height, D3DCOLOR colour, D3DCOLOR outlinecolor, int thickness = 1.0f);
	void GradientBox(int x, int y, int width, int height, D3DCOLOR color, D3DCOLOR color2, bool vertical);
	void GradientBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR colour, D3DCOLOR color2, D3DCOLOR outlinecolor, bool vertical);
	void Circle(int x, int y, int radius, int points, D3DCOLOR color);
	void FilledCircle(int x, int y, int radius, int points, D3DCOLOR color);
 
	void Text(char *text, float x, float y, int orientation, LPD3DXFONT pFont, bool bordered, DWORD color, DWORD bcolor);
	void String(float x, float y, int orientation, LPD3DXFONT pFont, bool bordered, DWORD color, const char *input, ...);
	void Message(char *text, float x, float y, LPD3DXFONT pFont, int orientation);
 
	void Sprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution_x, float resolution_y, float scale_x, float scale_y, float rotation, DWORD color);
 
	int FrameRate();
}; extern CRender* g_pDraw;
 
/* hooking shit */
using OverrideViewFn = void(__stdcall*)(CViewSetup*);
using FrameStageNotifyFn = void(__stdcall*)(ClientFrameStage_t);
using PaintTraverseFn = void(__stdcall*)(unsigned int, bool, bool);
typedef long(__stdcall* ResetFn)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
using EndSceneFn = long(__stdcall*)(IDirect3DDevice9* device);
extern EndSceneFn oEndScene;
extern ResetFn oReset;
using CreateMove = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
using PlaySoundFn = void(__stdcall*)(const char*);
extern WNDPROC oWndProc;
using DrawModelExecuteFn = void*(__stdcall*)(void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
extern DrawModelExecuteFn oDrawModelExecute;

 
/* prototypes*/
namespace HookManager
{
	void Initialize();
	void UnloadHooks();

	//extern void __stdcall DrawModelExecute(void* context, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	//extern void __stdcall PaintTraverse(unsigned int panel, bool forceRepaint, bool allowForce);
	extern void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket, CUserCmd* pCmd);
	extern void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active);
	extern void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	extern void __stdcall OverrideView(CViewSetup* vsView);
	//extern void __stdcall PlaySoundG(const char* fileName);
	extern long __stdcall EndScene(IDirect3DDevice9* pDevice);
	extern long __stdcall Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




	extern vfunc_hook hlclient_hook;
	extern vfunc_hook direct3d_hook;
	extern vfunc_hook vguipanel_hook;
	extern vfunc_hook vguisurf_hook;
	extern vfunc_hook mdlrender_hook;
	extern vfunc_hook viewrender_hook;
	extern vfunc_hook clientmode_hook;
}
