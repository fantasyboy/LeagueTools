#pragma once
#include <mutex>
#include <memory>
#include "SkillService.h"
#include "BufferService.h"
#include "Monster.h"
namespace Player
{
	//读取玩家名字
	char* GetName();
	//读取英雄名字
	char* GetHeroName();
	//获取当前血量
	float GetCurrHp();
	//获取最大血量
	float GetMaxHp();
	//获取当前蓝量
	float GetCurrMp();
	//获取最大蓝量
	float GetMaxMp();
	//获取当前坐标
	PEP_POINT_3D GetCurrPostion();
	//获取是否死亡
	bool BDead() ;
	//获取是否可见
	bool BVisableSee() ;
	//获取是否移动
	bool BMoving() ;
	//获取怪物朝向
	PEP_POINT_3D GetOrientation() ;
	//获取怪物阵营
	DWORD GetCamp() ;
	//获取怪物类型
	DWORD GetType() ;
	//获取玩家等级
	DWORD GetLevel() ;
	//获取攻击力
	float GetAttackPower() ;
	//获取移动速度
	float GetMoveSpeed() ;
	//获取攻击速度
	float GetAttackSpeed() ;
	//获取攻击距离
	float GetAttackDistance() ;
	//获取技能数组
	Skill* GetSkillByIndex(DWORD index);
	//获取穿甲值 +58
	float GetArmour() ;
	//获取护甲穿透 +5C
	float GetArmourPenetration();
	//获取模型大小
	float GetMonsModelSize() ;
	//获取法术强度
	float GetSpellPower() ;
	//获取泽拉斯蓄力时间
	float GetXerathPowerTime();
	//获取泽拉斯当前技能距离
	float GetXerathSkillCurrDistance();
	//获取Object给技能使用
	DWORD GetObjectForSkill();
	//计算预判坐标
	EP_POINT_3D ClacAnticipationPnt(DWORD mons, float skDelay, float MissileSpeed);
	//走砍
	void ZouA2Mons(DWORD target, bool tag);
	//计算攻击间隔
	float sub_10008CC4(float a1);
	//使用技能1 怪物对象 技能对象 技能延迟 技能速度 是否接平A
	bool UseSkillToMons(DWORD mons, DWORD skIndex,float skDelay, float MissileSpeed,bool bAttack);
	//使用技能2
	void UseSkillToMons2(DWORD mons, DWORD skIndex, float MissileSpeed, float skDelay);
	//滑板鞋自动E
	bool KalistaAutoE2Object(DWORD mons);
};

