#include "SDK.h"
#include "Menu.h"
/* originals */
EndSceneFn oEndScene;
ResetFn oReset;
WNDPROC oWndProc;

/* pointers / resolved externs */
settings_t g_Settings;
CMenu* g_pMenu = new CMenu();

/* variables */
bool initialized = false;
typedef unsigned int VPANEL;

/*
void __stdcall HookManager::PaintTraverse(unsigned int panel, bool forceRepaint, bool allowForce)
{
	static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverseFn>(index::PaintTraverse);

	oPaintTraverse(panel, forceRepaint, allowForce);

}
*/
/*
void __stdcall HookManager::DrawModelExecute(void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
{
	//static auto ofunc = mdlrender_hook.get_original<DrawModelExecuteFn>(index::DrawModelExecute);
	//ofunc(g_pModelRender, ctx, state, pInfo, pCustomBoneToWorld);

}
*/
/**/
void __stdcall HookManager::OverrideView(CViewSetup* vsView)
{
	static auto ofunc = clientmode_hook.get_original<OverrideViewFn>(index::OverrideView);

	ofunc(vsView);
}

void __stdcall HookManager::FrameStageNotify(ClientFrameStage_t stage)
{
	static auto ofunc = hlclient_hook.get_original<FrameStageNotifyFn>(index::FrameStageNotify);


	ofunc(stage);
}

void __stdcall HookManager::hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	C_BaseEntity* localplayer = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

	auto oCreateMove = hlclient_hook.get_original<CreateMove>(index::CreateMove);
	oCreateMove(g_pClient, sequence_number, input_sample_frametime, active);

	
	auto cmd = g_pInput->GetUserCmd(sequence_number);
	auto verified = g_pInput->GetVerifiedCmd(sequence_number);

	if (!cmd || !cmd->command_number)
		return;
		
	//if (pCmd == nullptr)
		//return;
		
	if (g_Settings.Menu.Bhop) {

		if (localplayer == nullptr)
			return;

		if (localplayer && localplayer->IsAlive())
		{
			if (cmd->buttons & IN_JUMP && !(localplayer->m_fFlags() & FL_ONGROUND))
			{
				cmd->buttons &= ~IN_JUMP;
			}
		}
	}

	//verified->m_cmd = *cmd;
	//verified->m_crc = cmd->GetChecksum();
}
__declspec(naked) void __stdcall HookManager::hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx
		lea  ecx, [esp]
		push ecx
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call HookManager::hkCreateMove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}
/*
void __stdcall HookManager::PlaySoundG(const char* fileName)
{
	//static auto oPlaySound = vguisurf_hook.get_original<PlaySound>(index::PlaySound); <--- original from CSGOSIMPLE
	static auto oPlaySound = vguisurf_hook.get_original<PlaySoundFn>(index::PlaySound);

	oPlaySound(fileName);

	// Auto Accept
	if (strstr(fileName, "UI/competitive_accept_beep.wav")) {
		static auto fnAccept = (void(*)())g_pUtils->PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");
		fnAccept();
	}
}
*/

long __stdcall HookManager::EndScene(IDirect3DDevice9* device)
{
	static bool done = false;

	if (!done)
	{
		g_pUtils->Log(" Initializing drawing manager and allocating fonts...");
		g_pDraw->Init(device);

		ImGui_ImplDX9_Init(g_Globals::hwndWindow, device);

		g_pMenu->InitializeTheme();

		done = initialized = true;
	}
	/* reset here apparently */
	g_pDraw->Reset();

	DWORD dwOld_D3DRS_COLORWRITEENABLE;

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

	// If you're doing ESP in EndScene, I suggest doing it here.
	// e.g: g_pESP->Think();

	ImGui_ImplDX9_NewFrame();

	g_pDraw->String(8, 8, FONT_LEFT, g_pDraw->fonts.esp, true, WHITE(255), CHEAT_NAME);
	g_pDraw->String(8, 20, FONT_LEFT, g_pDraw->fonts.esp_small, true, WHITE(255), "%s", __DATE__);

	g_pMenu->Think();

	ImGuiStyle &style = ImGui::GetStyle();

	/* dope animation for pImGui menu */
	if (g_pMenu->m_opened)
	{
		if (style.Alpha > 1.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
			style.Alpha += 0.01f; 
	}
	else
	{
		if (style.Alpha < 0.f)
			style.Alpha = 0.f;
		else if (style.Alpha != 0.f)
			style.Alpha -= 0.01f;
	}

	ImGui::Render();

	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

	return oEndScene(device);
}

long __stdcall HookManager::Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if(!initialized)
		return oReset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto res = oReset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return res;
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall HookManager::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* this doesn't really have to go here, i just put it here for convenience */
	g_pMenu->ThinkKeyboard();

	/* blocks input to CSGO and points to ImGui */
	if (initialized && g_pMenu->m_opened && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
}
