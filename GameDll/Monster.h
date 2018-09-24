#pragma once


namespace Monster
{
	//读取玩家名字
	char* GetName(DWORD obj);
	//读取英雄名字
	char* GetHeroName(DWORD obj);
	//获取当前血量
	float GetCurrentHealth(DWORD obj);
	//获取最大血量
	float GetMaxHealth(DWORD obj);
	//获取当前蓝量
	float GetCurrentMagic(DWORD obj);
	//获取最大蓝量
	float GetMaxMagic(DWORD obj);
	//获取当前坐标
	PEP_POINT_3D GetCurrentPos(DWORD obj);
	//获取模型坐标
	PEP_POINT_3D GetModelPos(DWORD obj);
	//获取是否死亡标志位
	bool BDead(DWORD obj);
	//获取是否移动标志位
	bool BMoving(DWORD obj);
	//获取是否可见标志位
	bool BVisable(DWORD obj);
	//获取怪物类型
	DWORD GetType(DWORD obj);
	//获取怪物阵营
	DWORD GetCamp(DWORD obj);
	//获取怪物朝向
	PEP_POINT_3D GetOrientation(DWORD obj);
	//获取防御力
	float GetDefensePower(DWORD obj);
	//获取移动速度
	float GetMoveSpeed(DWORD obj);
	//获取技能参数
	DWORD GetObjectForSkill(DWORD obj);
	//获取模型大小
	float GetModelWidth(DWORD obj);
}