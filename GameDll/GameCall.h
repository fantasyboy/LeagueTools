#pragma once
#include "baseaddr.h"
#include "Monster.h"

namespace GameCall
{
	//获取游戏时间
	float GetGameTickTime();
	//获取鼠标坐标
	PEP_POINT_3D GetMousePoint();
	//寻路CALL
	void FindWay(PEP_POINT_3D pt);
	//普攻CALL
	void Attack(DWORD obj);
	//使用技能CALL (参数为 怪物对象+0F0 当前坐标 目的坐标 技能索引 技能对象)
	void UseSkill(DWORD dwtargetId, PEP_POINT_3D pCurrPnt, PEP_POINT_3D pDestPnt, DWORD dwSkillIndex, DWORD dwSkillBase);
	//使用技能CALL 2
	void useSkill2(PEP_POINT_3D pnt, DWORD index, DWORD skObj);
	//防御CALL
	bool Defense();
};


void   SkillSkilCrc();
void  FindWaySkipCrc();