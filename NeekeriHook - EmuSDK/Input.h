#pragma once
#include "SDK.h"
#define MULTIPLAYER_BACKUP 150

class CInput
{
public:


	inline CUserCmd* GetUserCmd(int sequence_number);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);

	bool                m_bTrackIRAvailable;            //0x04
	bool                m_bMouseInitialized;            //0x05
	bool                m_bMouseActive;                 //0x06
	bool                m_bJoystickAdvancedInit;        //0x07
	char                pad_0x08[0x2C];                 //0x08
	void*               m_pKeys;                        //0x34
	char                pad_0x38[0x64];                 //0x38
	bool                m_bCameraInterceptingMouse;     //0x9C
	bool                m_bCameraInThirdPerson;         //0x9D
	bool                m_bCameraMovingWithMouse;       //0x9E
	Vector              m_vecCameraOffset;              //0xA0
	bool                m_bCameraDistanceMove;          //0xAC
	int                 m_nCameraOldX;                  //0xB0
	int                 m_nCameraOldY;                  //0xB4
	int                 m_nCameraX;                     //0xB8
	int                 m_nCameraY;                     //0xBC
	bool                m_CameraIsOrthographic;         //0xC0
	QAngle              m_angPreviousViewAngles;        //0xC4
	QAngle              m_angPreviousViewAnglesTilt;    //0xD0
	float               m_flLastForwardMove;            //0xDC
	int                 m_nClearInputState;             //0xE0
	char                pad_0xE4[0x8];                  //0xE4
	CUserCmd*           m_pCommands;                    //0xEC
	CVerifiedUserCmd*   m_pVerifiedCommands;            //0xF0
}; extern CInput* g_pInput;

CUserCmd* CInput::GetUserCmd(int sequence_number)
{
	return &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequence_number)
{
	return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
}