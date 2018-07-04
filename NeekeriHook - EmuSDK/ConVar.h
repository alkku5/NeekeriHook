#pragma once
#include "SDK.h"

//-----------------------------------------------------------------------------
// ConVar flags
//-----------------------------------------------------------------------------
// The default, no flags at all
#define FCVAR_NONE                0 

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED              (1<<0)  // If this is Set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY           (1<<1)  // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL                   (1<<2)  // defined by the game DLL
#define FCVAR_CLIENTDLL                 (1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN                    (1<<4)  // Hidden. Doesn't appear in GetOffset or auto complete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only                                  
#define FCVAR_PROTECTED                 (1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY                    (1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define FCVAR_ARCHIVE                   (1<<7)  // Set to cause it to be saved to vars.rc
#define FCVAR_NOTIFY                    (1<<8)  // notifies players when changed
#define FCVAR_USERINFO                  (1<<9)  // changes the client's info string

#define FCVAR_PRINTABLEONLY             (1<<10) // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED                  (1<<11) // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING           (1<<12) // never try to print that cvar
#define FCVAR_REPLICATED                (1<<13) // server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_CHEAT                     (1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats
#define FCVAR_SS                        (1<<15) // causes varnameN where N == 2 through max splitscreen slots for mod to be autogenerated
#define FCVAR_DEMO                      (1<<16) // record this cvar when starting a demo file
#define FCVAR_DONTRECORD                (1<<17) // don't record these command in demofiles
#define FCVAR_SS_ADDED                  (1<<18) // This is one of the "added" FCVAR_SS variables for the splitscreen players
#define FCVAR_RELEASE                   (1<<19) // Cvars tagged with this are the only cvars avaliable to customers
#define FCVAR_RELOAD_MATERIALS          (1<<20) // If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES           (1<<21) // If this cvar changes, if forces a texture reload
#define FCVAR_NOT_CONNECTED             (1<<22) // cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD    (1<<23) // Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_XBOX              (1<<24) // cvar written to config.cfg on the Xbox
#define FCVAR_ACCESSIBLE_FROM_THREADS   (1<<25) // used as a debugging tool necessary to check material system thread convars

#define FCVAR_SERVER_CAN_EXECUTE        (1<<28) // the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY       (1<<29) // If this is Set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE     (1<<30) // IVEngineClient::ClientCmd is allowed to execute this command. 
#define FCVAR_MEME_DLL                  (1<<31)

#define FCVAR_MATERIAL_THREAD_MASK ( FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD )    

class ConVar
{
public:
	void SetValue(float value)
	{
		typedef void(__thiscall* OriginalFn)(void*, float);
		return g_pUtils->Emulate< OriginalFn >(this, 15)(this, value);
	}

	char pad_0x0000[0x4]; //0x0000
	ConVar* pNext; //0x0004 
	__int32 bRegistered; //0x0008 
	char* pszName; //0x000C 
	char* pszHelpString; //0x0010 
	__int32 nFlags; //0x0014 
	char pad_0x0018[0x4]; //0x0018
	ConVar* pParent; //0x001C 
	char* pszDefaultValue; //0x0020 
	char* strString; //0x0024 
	__int32 StringLength; //0x0028 
	float fValue; //0x002C 
	__int32 nValue; //0x0030 
	__int32 bHasMin; //0x0034 
	float fMinVal; //0x0038 
	__int32 bHasMax; //0x003C 
	float fMaxVal; //0x0040 
	void* fnChangeCallback; //0x0044 
};//Size=0x0048

/* i was intoxicated while making this class, do not judge */
class ICVar 
{
private:
	virtual void fuck() = 0;
	virtual void pvoid1() = 0;
	virtual void pvoid2() = 0;
	virtual void pvoid3() = 0;
	virtual void pvoid4() = 0;
	virtual void pvoid5() = 0;
	virtual void pvoid6() = 0;	
	virtual void pvoid11() = 0;
	virtual void pvoid21() = 0;
	virtual void pvoid31() = 0;
	virtual void pvoid41() = 0;
	virtual void pvoid51() = 0;
	virtual void pvoid61() = 0;
	virtual void pvoid161() = 0;
	virtual void pvoid261() = 0;
	virtual void pvoid361() = 0;
public:
	virtual ConVar*                    FindVar(const char *var_name) = 0; // 16
private:
	virtual void pvoid123() = 0;
	virtual void pvoid121() = 0;
	virtual void pvoid32() = 0;
	virtual void pvoid2313() = 0;
	virtual void pvoid444() = 0;
	virtual void pvoid55() = 0;
	virtual void pvoid66() = 0;
	virtual void pvoid1f1() = 0;
	virtual void pvoid2a1() = 0;
	virtual void pvoid3q1() = 0;
public:
	virtual void                       ConsoleColorPrintf(const Color clr, const char *pFormat, ...) const = 0; // 25
}; extern ICVar* g_pConsole;