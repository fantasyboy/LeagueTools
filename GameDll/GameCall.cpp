#include "stdafx.h"
#include "GameCall.h"
#include "utils.h"

//处理技能校验



float GameCall::GetGameTickTime()
{
	return utils::read<float>(Base_GameStartTime + 0x30);
}

PEP_POINT_3D GameCall::GetMousePoint()
{
	auto obj = utils::read<DWORD>(Base_MousePointAddr);
	if (obj)
	{
		auto offset = utils::read<DWORD>(obj + 0x14);
		if (offset)
		{
			return (PEP_POINT_3D)(offset + 0x10);
		}
	}

	return nullptr;
}

void GameCall::FindWay(PEP_POINT_3D pt)
{

	VMProtectBegin(__FUNCTION__);
	__asm
	{
		MOV EDX, 0x1;
		PUSH EDX;
		PUSH 0x0;
		PUSH 0x0;
		PUSH 0x0;
		MOV EDI, pt;
		PUSH EDI;
		PUSH 0x2;
		MOV EAX, Base_MousePointAddr;
		MOV EAX, [EAX];
		MOV ESI, DWORD PTR DS : [EAX + 0x30];
		MOV ECX, Base_RoleSelfAddr;
		MOV ECX, [ECX];
		MOV EAX, FindWaySkipCrc;
		CALL EAX;
	}
	VMProtectEnd();

}

void GameCall::Attack(DWORD dwNodeBase)
{
	VMProtectBegin(__FUNCTION__);
	__asm
	{
		MOV EAX, Base_MousePointAddr;
		MOV EAX, [EAX];
		MOV ESI, DWORD PTR DS : [EAX + 0x30];
		MOV EDI, 0x1;
		PUSH EDI;
		PUSH 0x100;
		PUSH 0x0;
		MOV EBX, dwNodeBase;
		PUSH EBX;		
		LEA EAX, [EBX + 0x104];
		PUSH EAX;
		PUSH 3;
		MOV ECX, Base_RoleSelfAddr;
		MOV ECX, [ECX];
		MOV EAX, FindWaySkipCrc;
		CALL EAX;
	}
	VMProtectEnd();
}

void GameCall::UseSkill(DWORD dwtargetId, PEP_POINT_3D pCurrPnt, PEP_POINT_3D pDestPnt, DWORD dwSkillIndex, DWORD dwSkillBase)
{
	VMProtectBegin(__FUNCTION__);
	__asm
	{
		MOV ESI, Base_RoleSelfAddr;
		MOV ESI, [ESI];
		MOV ECX, ESI;
		ADD ECX, Base_SkillTravseOffset1;
		MOV EAX, dwtargetId;
		PUSH EAX;
		PUSH pCurrPnt;
		PUSH pDestPnt;
		MOV EAX, dwSkillIndex;
		PUSH EAX;
		MOV EDI, dwSkillBase;
		PUSH EDI;
		MOV EAX, SkillSkilCrc;
		CALL EAX;
	}
	VMProtectEnd();
}

void GameCall::useSkill2(PEP_POINT_3D pnt, DWORD index, DWORD skObj)
{
	VMProtectBegin(__FUNCTION__);
	__asm {
		MOV ESI, Base_RoleSelfAddr;
		MOV ESI, [ESI];
		PUSH 1;
		PUSH pnt;
		PUSH index;
		PUSH skObj;
		MOV ECX, ESI;
		ADD ECX, Base_SkillTravseOffset1;
		MOV EAX, Base_SkillCallAddr2;
		CALL EAX;
	}
	VMProtectEnd();
}

bool GameCall::Defense()
{
	VMProtectBegin(__FUNCTION__);
	__asm
	{
		MOV EAX, Base_MousePointAddr;
		MOV EAX, [EAX];
		MOV ESI, DWORD PTR DS : [EAX + 0x30];
		PUSH 1;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		MOV ECX, Base_RoleSelfAddr;
		MOV ECX, [ECX];
		LEA EAX, DWORD PTR DS : [ECX + 0x104];
		PUSH EAX;
		PUSH 0x0A;
		MOV EAX, FindWaySkipCrc;
		CALL EAX;
	}
	VMProtectEnd();
}


void __declspec(naked) SkillSkilCrc()
{
	__asm 
	{
		PUSH EBP;
		MOV EBP, ESP;
		PUSH - 1;
		PUSH 0x137BCD0;
		MOV EAX, DWORD PTR FS : [0];
		PUSH EAX;
		SUB ESP, 0x34;
		PUSH EBX;
		PUSH ESI;
		PUSH EDI;
		MOV EAX, DWORD PTR DS : [0x1ACC278];
		XOR EAX, EBP;
		PUSH EAX;
		LEA EAX, DWORD PTR SS : [EBP - 0xC];
		//MOV DWORD PTR FS : [0], EAX;
		MOV DWORD PTR SS : [EBP - 0x10], ECX;
		MOV EDI, DWORD PTR SS : [EBP + 0x8];
		PUSH 0x009402EE;
		RETN;
	}
}

void __declspec(naked) FindWaySkipCrc()
{
	__asm 
	{
		PUSH EBP;
		LEA EBP, DWORD PTR SS : [ESP - 0x1C];
		SUB ESP, 0x1C;
		PUSH - 1;
		PUSH 0x1359DAB;
		MOV EAX, DWORD PTR FS : [0];
		PUSH EAX;
		SUB ESP, 0x94;
		PUSH EBX;
		PUSH ESI;
		PUSH EDI;
		MOV EAX, DWORD PTR DS : [0x1ACC278];
		XOR EAX, EBP;
		PUSH EAX;
		LEA EAX, DWORD PTR SS : [EBP - 0xC];
		//MOV DWORD PTR FS : [0], EAX;
		MOV EBX, ECX;
		MOV DWORD PTR SS : [EBP - 0x78], EBX;
		PUSH 0x05DEFC5;
		RETN;
	}
}
