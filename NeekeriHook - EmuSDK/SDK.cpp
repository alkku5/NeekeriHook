#include "SDK.h"

/* externs getting resolved here */
CSDK* g_pSDK = new CSDK();
IBaseClientDLL * g_pClient = nullptr;
IClientMode * g_pClientMode = nullptr;
IVEngineClient * g_pEngine = nullptr;
CInput * g_pInput = nullptr;
IGameMovement * g_pGameMovement = nullptr;
IPhysicsAPI * g_pPhysicsAPI = nullptr;
CGlobalVarsBase * g_pGlobals = nullptr;
IEngineTrace * g_pEngineTrace = nullptr;
ICVar * g_pConsole = nullptr;
IClientEntityList * g_pEntList = nullptr;
IMoveHelper * g_pMoveHelper = nullptr;
IVModelRender * g_pModelRender = nullptr;
IMaterialSystem * g_pMaterialSystem = nullptr;
IGameEventManager2 * g_pGameEventManager = nullptr;
ISurface * g_pVGUISurface = nullptr;
IVDebugOverlay * g_pDebugOverlay = nullptr;
IVModelInfo * g_pModelInfo = nullptr;
IPrediction * g_pPrediction = nullptr;
IVRenderView * g_pRenderView = nullptr;
IPanel * g_VGuiPanel = nullptr;

void OnFailedInterface(const char* iface, void* ptr)
{
	g_pUtils->Log(" -- Faulty interface pointer for '%s' ==> 0x%x", iface, ptr);
}

template< typename T >
T* SeekInterface(char* mod, char* interf, bool seek = true)
{
	typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(mod), ("CreateInterface"));

	std::string sInterface, sInterfaceVersion = "0";
	
	if (seek)
	{
		for (auto i = 0; i <= 99; i++)
		{
			sInterface = interf;
			sInterface += sInterfaceVersion;
			sInterface += std::to_string(i);

			T* funcPtr = CreateInterface(sInterface.c_str(), NULL);

			if (funcPtr != NULL)
			{
				g_pUtils->Log(" -- Found interface pointer for '%s' ==> 0x%x", sInterface.c_str(), funcPtr);
				return funcPtr;
			}

			if (i == 99 && sInterfaceVersion == "0")
			{
				sInterfaceVersion = "00";
				i = 0;
			}
			else if (i == 99 && sInterfaceVersion == "00")
			{
				OnFailedInterface(interf, funcPtr);
				return nullptr;
			}
		}
	}
	else
	{
		T* ptr = CreateInterface(interf, 0);

		if (ptr)
		{
			g_pUtils->Log(" -- Found interface pointer for '%s' ==> 0x%x", interf, ptr);
			return ptr;
		}
		else
		{
			OnFailedInterface(interf, ptr);
			return nullptr;
		}
	}

	return nullptr;
}

void CSDK::Initialize()
{
	FUNCTION_GUARD;	

	g_pUtils->Log(" ===================================================================================");
	g_pUtils->Log(" Initializing %s, built on %s at %s.", CHEAT_NAME, __DATE__, __TIME__);

	g_pUtils->Log(" Attempting to locate and attach interfaces to the game engine...");

	g_pClient = SeekInterface<IBaseClientDLL>("client.dll", "VClient");
	g_pEngine = SeekInterface<IVEngineClient>("engine.dll", "VEngineClient");
	g_pEntList = SeekInterface<IClientEntityList>("client.dll", "VClientEntityList");
	g_pEngineTrace = SeekInterface<IEngineTrace>("engine.dll", "EngineTraceClient");
	g_pModelInfo = SeekInterface<IVModelInfo>("engine.dll", "VModelInfoClient");
	g_pDebugOverlay = SeekInterface <IVDebugOverlay > ("engine.dll", "VDebugOverlay");
	g_pConsole = SeekInterface<ICVar>("vstdlib.dll", "VEngineCvar");
	g_pVGUISurface = SeekInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface");
	g_pPhysicsAPI = SeekInterface<IPhysicsAPI>("vphysics.dll", "VPhysicsSurfaceProps");
	g_pPrediction = SeekInterface<IPrediction>("client.dll", "VClientPrediction");
	g_pGameMovement = SeekInterface<IGameMovement>("client.dll", "GameMovement");
	g_pMaterialSystem = SeekInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
	g_pGameEventManager = SeekInterface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002", false);
	g_pModelRender = SeekInterface<IVModelRender>("engine.dll", "VEngineModel");
	g_pRenderView = SeekInterface<IVRenderView>("engine.dll", "VEngineRenderView");
	g_VGuiPanel = SeekInterface<IPanel>("vgui2.dll", "VGUI_Panel009");
	g_pClientMode = **reinterpret_cast<IClientMode***>((*(uintptr_t**)g_pClient)[10] + 0x5);
	g_pGlobals = **reinterpret_cast<CGlobalVarsBase***>((*(uintptr_t**)g_pClient)[0] + 0x1B);
	g_pInput = **reinterpret_cast<CInput***>((*(uintptr_t**)g_pClient)[15] + 0x1); // IN_something + 1 = input ptr
	g_pMoveHelper = **reinterpret_cast<IMoveHelper***>(g_pUtils->FindPatternIDA("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 2);

	if (!g_pMoveHelper)
	{
		OnFailedInterface("g_pMoveHelper", g_pMoveHelper);

		/* hook failed (well obviously) */
		exit(0);
	}

	g_pUtils->Log(" Hooking game engine functions...");

	DWORD device = **(uintptr_t**)(g_pUtils->FindPatternIDA("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

	if (device)
	{
		CVMTHookManager* hdevice = new CVMTHookManager((PDWORD*)device);

		oEndScene = (EndSceneFn)(hdevice->HookMethod((DWORD)HookManager::EndScene, 42));
		oReset = (ResetFn)(hdevice->HookMethod((DWORD)HookManager::Reset, 16));

		oWndProc = (WNDPROC)SetWindowLongA(g_Globals::hwndWindow, GWL_WNDPROC, (LONG_PTR)HookManager::WndProc);
	}
	else
	{
		g_pUtils->Log(" %s failed to completely initialize...", CHEAT_NAME);
	}

	g_pUtils->Log(" %s was successfully initialized...", CHEAT_NAME);

	/* audible loading end signal for the blind */
	Beep(400, 700);
}



namespace HookManager
{
	vfunc_hook hlclient_hook;
	//vfunc_hook vguipanel_hook;
	//vfunc_hook vguisurf_hook;
	//vfunc_hook mdlrender_hook;
	vfunc_hook clientmode_hook;

	//fix painttraverse
	void Initialize()
	{
		hlclient_hook.setup(g_pClient, "client.dll");
		//vguipanel_hook.setup(g_VGuiPanel);
		//vguisurf_hook.setup(g_pVGUISurface);
		//mdlrender_hook.setup(g_pModelRender, "engine.dll");
		clientmode_hook.setup(g_pClientMode, "client.dll");

		hlclient_hook.hook_index(index::FrameStageNotify, HookManager::FrameStageNotify);
		hlclient_hook.hook_index(index::CreateMove, HookManager::hkCreateMove_Proxy);
		//vguipanel_hook.hook_index(index::PaintTraverse, HookManager::PaintTraverse);
		//vguisurf_hook.hook_index(index::PlaySoundA, HookManager::PlaySoundG);
		//mdlrender_hook.hook_index(index::DrawModelExecute, HookManager::DrawModelExecute);
	}

	void UnloadHooks()
	{
		hlclient_hook.unhook_all();
		//vguipanel_hook.unhook_all();
		clientmode_hook.unhook_all();
	}
	
}