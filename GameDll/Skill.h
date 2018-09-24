#pragma once

enum SK_SKILL_TYPE
{
	SK_SKILL_LINE = 0x009529A0,   //指向性技能，大嘴Q
	SK_SKILL_LOCK = 0x00959E00,   //锁定技能，比如安妮Q 蛇女E
	SK_SKILL_SECTOR = 0x00952780, //扇形技能，比如安妮W,蛇女R
	SK_SKILL_ROUND = 0x00959C40, //圆形技能，大嘴R ,安妮R
	SK_SKILL_ROUND2 = 0x00959C70, //泽拉斯R , 逆羽Q
	SK_SKILL_TOSELF = 0x00959DD0, //对自己释放的技能
	SK_SKILL_VIKTOR = 0x00952C70, //维克托的E
};

class Skill
{
public:
	Skill(DWORD dwNodeBase, DWORD _index);
	~Skill();
	//获取技能索引
	DWORD GetIndex() const;
	//获取技能名字
	char* GetName() const;
	//获取技能等级
	DWORD GetLevel() const;
	//技能是否冷却
	bool BCoolDown() const;
	//获取技能消耗的蓝量
	float GetExpendMp() const;
	//获取技能范围
	float GetDistance() ;
	//获取技能范围2
	float GetDistance1() ;
	//获取技能范围2
	float GetDistance2() ;
	//获取技能宽度
	float GetBreadth() ;
	//获取技能攻击力
	float GetSkillAggressivity() const;
	//获取技能攻击力加成比例
	//获取技能类型
	DWORD GetSkillType();
	//获取蛇女E技能基础伤害
	float GetBaseAggressivity() const;
	//获取泽拉斯蓄力时间
	float GetXerathPowerTime();
	DWORD GetNodeBase();

	//设置技能范围
	void SetDistance(float dis);


	//设置技能宽度
	void SetWidth(float width);
	float GetWidth();

	//是否判断遮挡
	void SetJudgmentCover(bool _b);
	bool GetJudgmentCover();
private:
	DWORD m_index;
	WORD m_skillType;
	DWORD m_dwNodeBase;
	float m_Distance;
	//技能范围
	float m_skDistance1[7];
	float m_skDistance2[7];
	float m_skBreadth[7];
	//技能攻击力
	float m_skAggressivity[7];
	//技能宽度
	float m_skWidth;
	//是否判断技能遮挡
	bool m_bJudgmentCover;
};