#pragma once

const DWORD Base_RoleSelfAddr = 0x0338A5F0;    //玩家基地址     偏移:2    //OD地址:0x00B02AB2
const DWORD Base_MonsterCurrentHpOffset = 0x0000066C;    //当前血量偏移     偏移:5    //OD地址:0x00AA59DF
const DWORD Base_MonsterCurrentMpOffset = 0x000002D8;    //当前蓝量偏移     偏移:5    //OD地址:0x00AA59DF
const DWORD Base_MonsterMoveSpeedOffset = 0xA94;  //移动速度
const DWORD Base_MonsterAttackSpeedOffset = 0xA9C; //攻击距离
const DWORD Base_MonsterDefenseOffset = 0xA7C; //防御力
const DWORD Base_MonsterFixedAttackPowerOffset = 0xA54; //固定攻击力
const DWORD Base_MonsterDynamicAttackPowerOffset = 0x9DC; //动态攻击力
const DWORD Base_MonsterDeadTagOffset = 0x100; //是否死亡标志位
const DWORD Base_GameWndHwndAddr = 0x33864F0;    //游戏窗口基地址     偏移:2    //OD地址:0x0081AD1E
const DWORD Base_SkillTravseOffset1 = 0x00001F60;    //技能数组第一层偏移     偏移:1    //OD地址:0x006C141D
const DWORD Base_SkillTravseOffset2 = 0x00000590;    //技能数组第二层偏移     偏移:2    //OD地址:0x006BAD50
const DWORD Base_SkillOffset_Object = 0x10C;    //技能对象偏移     偏移:2    //OD地址:0x006C1434
const DWORD Base_SkillAggressivityOffset = 0x0d0;//技能基础攻击力
const DWORD Base_SkillOffset_Range = 0x388; //技能范围偏移
const DWORD Base_SkillMpExpendOffset = 0x574; //技能蓝耗
const DWORD  Base_SkillOffset_Type1 = 0x108;  //技能类型偏移1
const DWORD  Base_SkillOffset_Type2 = 0x4c;  //技能类型偏移2
const DWORD Base_GameStartTime = 0x1AD8AD0;    //游戏开局时间     偏移:1    //OD地址:0x00836C62
const DWORD Base_MonsterArrayAddr = 0x1ADADF4;    //怪物数组基地址     偏移:2    //OD地址:0x006A2502
const DWORD Base_SkillCallAddr2 = 0x03950285; //技能CALL2
const DWORD Base_FindWayCallAddr = 0x005DEF90;    //寻路call     偏移:1    //OD地址:0x00AE5662
const DWORD Base_SkillCallAddr = 0x009402C0; //技能CALL 
const DWORD Base_MousePointAddr = 0x1ADAA60;      //鼠标位置
const DWORD Base_MonsterBMovingOffset = 0x3978;  //是否移动
const DWORD Base_MonsterOrientationXOffset = 0x17AC;    //朝向偏移     偏移:5    //OD地址:0x00AA59DF 
const DWORD Base_UIAddr = 0x2727E64;  //UI界面基地址
const DWORD Base_BufferOffset = 0x1668;    //buff偏移
const DWORD Base_BufferCountOffset = 0x70; //BUFF层数
const DWORD Base_MonsterBVisableOffset = 0x1e8; //是否可见偏移1
const DWORD Base_MonsterBVisableOffset2 = 0x0D8; //是否可见偏移1
const DWORD Base_DirectDeviceAddr = 0x033BDB6C; //设备对象指针基地址
const DWORD Base_CassiopeiaAggressivity = 0x6C; //蛇女E技能基础伤害便宜
const DWORD Base_XerathPowerTimeOffset = 0x0D8; //泽拉斯Q蓄力时间

