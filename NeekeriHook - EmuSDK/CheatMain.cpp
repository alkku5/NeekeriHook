#include "SDK.h"

/* globals declarations */
char g_Globals::szDirectoryDLL[MAX_PATH];
char* g_Globals::szLastFunction = "No function was called.";
HMODULE g_Globals::hmDLL = NULL;
C_BaseEntity* g_Globals::ptrLocalPlayer = nullptr;
C_BaseWeapon* g_Globals::ptrLocalWeapon = nullptr;
CUserCmd* g_Globals::ptrCmd = nullptr;
HWND g_Globals::hwndWindow = NULL;

/* exception handling - credits: s0beit */
LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	g_pUtils->Log(" =====================================================================================================");
	g_pUtils->Log(" %s has crashed.", CHEAT_NAME);
	g_pUtils->Log(" Base address: 0x%p.", g_Globals::hmDLL);
	g_pUtils->Log(" Exception at address: 0x%p, last function called: %s", ExceptionInfo->ExceptionRecord->ExceptionAddress, g_Globals::szLastFunction);

	int m_ExceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	int m_exceptionInfo_0 = ExceptionInfo->ExceptionRecord->ExceptionInformation[0];
	int m_exceptionInfo_1 = ExceptionInfo->ExceptionRecord->ExceptionInformation[1];
	int m_exceptionInfo_2 = ExceptionInfo->ExceptionRecord->ExceptionInformation[2];
	switch (m_ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		g_pUtils->Log(" Cause: EXCEPTION_ACCESS_VIOLATION");
		if (m_exceptionInfo_0 == 0)
		{
			// bad read
			g_pUtils->Log(" Attempted to read from: 0x%08x", m_exceptionInfo_1);
		}
		else if (m_exceptionInfo_0 == 1)
		{
			// bad write
			g_pUtils->Log(" Attempted to write to: 0x%08x", m_exceptionInfo_1);
		}
		else if (m_exceptionInfo_0 == 8)
		{
			// user-mode data execution prevention (DEP)
			g_pUtils->Log(" Data Execution Prevention (DEP) at: 0x%08x", m_exceptionInfo_1);
		}
		else
		{
			// unknown, shouldn't happen
			g_pUtils->Log(" Unknown access violation at: 0x%08x", m_exceptionInfo_1);
		}
		break;

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		g_pUtils->Log(" Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
		break;

	case EXCEPTION_BREAKPOINT:
		g_pUtils->Log(" Cause: EXCEPTION_BREAKPOINT");
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		g_pUtils->Log(" Cause: EXCEPTION_DATATYPE_MISALIGNMENT");
		break;

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_DENORMAL_OPERAND");
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO");
		break;

	case EXCEPTION_FLT_INEXACT_RESULT:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_INEXACT_RESULT");
		break;

	case EXCEPTION_FLT_INVALID_OPERATION:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_INVALID_OPERATION");
		break;

	case EXCEPTION_FLT_OVERFLOW:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_OVERFLOW");
		break;

	case EXCEPTION_FLT_STACK_CHECK:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_STACK_CHECK");
		break;

	case EXCEPTION_FLT_UNDERFLOW:
		g_pUtils->Log(" Cause: EXCEPTION_FLT_UNDERFLOW");
		break;

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		g_pUtils->Log(" Cause: EXCEPTION_ILLEGAL_INSTRUCTION");
		break;

	case EXCEPTION_IN_PAGE_ERROR:
		g_pUtils->Log(" Cause: EXCEPTION_IN_PAGE_ERROR");
		if (m_exceptionInfo_0 == 0)
		{
			// bad read
			g_pUtils->Log(" Attempted to read from: 0x%08x", m_exceptionInfo_1);
		}
		else if (m_exceptionInfo_0 == 1)
		{
			// bad write
			g_pUtils->Log(" Attempted to write to: 0x%08x", m_exceptionInfo_1);
		}
		else if (m_exceptionInfo_0 == 8)
		{
			// user-mode data execution prevention (DEP)
			g_pUtils->Log(" Data Execution Prevention (DEP) at: 0x%08x", m_exceptionInfo_1);
		}
		else
		{
			// unknown, shouldn't happen
			g_pUtils->Log(" Unknown access violation at: 0x%08x", m_exceptionInfo_1);
		}

		g_pUtils->Log(" NTSTATUS: 0x%08x", m_exceptionInfo_2);
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		g_pUtils->Log(" Cause: EXCEPTION_INT_DIVIDE_BY_ZERO");
		break;

	case EXCEPTION_INT_OVERFLOW:
		g_pUtils->Log(" Cause: EXCEPTION_INT_OVERFLOW");
		break;

	case EXCEPTION_INVALID_DISPOSITION:
		g_pUtils->Log(" Cause: EXCEPTION_INVALID_DISPOSITION");
		break;

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		g_pUtils->Log(" Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION");
		break;

	case EXCEPTION_PRIV_INSTRUCTION:
		g_pUtils->Log(" Cause: EXCEPTION_PRIV_INSTRUCTION");
		break;

	case EXCEPTION_SINGLE_STEP:
		g_pUtils->Log(" Cause: EXCEPTION_SINGLE_STEP");
		break;

	case EXCEPTION_STACK_OVERFLOW:
		g_pUtils->Log(" Cause: EXCEPTION_STACK_OVERFLOW");
		break;

	case DBG_CONTROL_C:
		g_pUtils->Log(" Cause: DBG_CONTROL_C");
		break;

	default:
		g_pUtils->Log(" Cause: %08x", m_ExceptionCode);
	}

	g_pUtils->Log(" =====================================================================================================");

	g_pUtils->Log(" Dumping ASM registers:");
	g_pUtils->Log(" EAX: 0x%08x || ESI: 0x%08x", ExceptionInfo->ContextRecord->Eax, ExceptionInfo->ContextRecord->Esi);
	g_pUtils->Log(" EBX: 0x%08x || EDI: 0x%08x", ExceptionInfo->ContextRecord->Ebx, ExceptionInfo->ContextRecord->Edi);
	g_pUtils->Log(" ECX: 0x%08x || EBP: 0x%08x", ExceptionInfo->ContextRecord->Ecx, ExceptionInfo->ContextRecord->Ebp);
	g_pUtils->Log(" EDX: 0x%08x || ESP: 0x%08x", ExceptionInfo->ContextRecord->Edx, ExceptionInfo->ContextRecord->Esp);

	return EXCEPTION_CONTINUE_SEARCH;
}

void __stdcall on_attach( HINSTANCE instance ) // initialize stuff here
{
    /* find window for wndproc hook etc */
    while ( !( g_Globals::hwndWindow = FindWindowA( "Valve001", 0 ) ) )
        std::this_thread::sleep_for( 50ms );

    /* this is for all the p100 uffja1tap p2c loaders that require you to */
    /* be in the main menu to inject because otherwise clientmode is a nullptr*/
    g_pUtils->GetHandleSafe( "serverbrowser.dll" );

    /* init our sdk */
    g_pSDK->Initialize();
	
	//initialize our p100 new VMTHooks
	HookManager::Initialize();

    g_Globals::hmDLL = (HINSTANCE)instance;

    /* unhandled exception */
    SetUnhandledExceptionFilter(ExceptionFilter);
}

void __stdcall on_detach(HINSTANCE instance)
{ 
	// shutdown stuff here
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    fclose((FILE*)stderr);
	HookManager::UnloadHooks(); //unload our hooks
    std::this_thread::sleep_for( 100ms );
}

bool __stdcall DllMain(HINSTANCE instance, unsigned long reason_for_call, void *reserved)
{
    switch (reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(instance);
        CreateThread( nullptr, 0, LPTHREAD_START_ROUTINE(on_attach), instance, 0, nullptr);

        return true;
        break;

    default:
        return false;
        break;
    }
}
