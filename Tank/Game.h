#pragma once
#include "GameMenuBackground.h"
#include "GameMenuPanel.h"
#include "GameMenuPanel2.h"
#include "KeyMenuPlayer01.h"
#include "KeyMenuPlayer02.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"
#include "GameMap.h"
#include "Wall.h"

class CGame
{
public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd); //输出窗口句柄
	bool EnterFrame(DWORD dwTime); //进入游戏帧
	void OnMouseMove(UINT nFlags, CPoint point); //处理鼠标移动
	void OnButtonUp(UINT nFlags, CPoint point); //处理邮件抬起事件
	//当前游戏所属阶段
	enum EGameType {
		EGameTypeMenu=0,  //选择阶段
		EGameTypeOne2BotMenu, //人机大战菜单阶段
		EGameTypeOne2Bot, //人机大战
		EGameTypeOne2BotEnd, //人机大战结束
		EGameTypeOne2OneMenu, //双人大战菜单阶段
		EGameTypeOne2One,    //双人大战
		EGameTypeOne2OneEnd, //双人大战结束
		EGameTypeCount, //有7个阶段
	};
	//设置当前游戏所处阶段
	void SetStep(CGame::EGameType step);
private:
	HWND m_hWnd; //窗口
	/*
	游戏初始化
	生成游戏对象，初始化地图、对象位置等
	*/
	EGameType m_eStep{ EGameTypeMenu };  //当前阶段：菜单选择阶段
	bool GameInit();
	bool GameInitMenu(); //游戏初始化：选择阶段
	bool GameInitOne2BotMenu();//游戏初始化：人机大战菜单阶段
	bool GameInitOne2Bot();//游戏初始化：人机大战
	bool GameInitOne2BotEnd();//游戏初始化：人机大战结束
	bool GameInitOne2OneMenu(); //游戏初始化：双人大战菜单阶段
	bool GameInitOne2One(); //游戏初始化：双人大战
	bool GameInitOne2OneEnd(); //游戏初始化：双人大战结束
	//方法放入函数指针，方便调用
	bool(CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd
	};

	//游戏绘图处理
	//绘制游戏中的对象
	//记录游戏每秒多少帧
	int m_fps{ 0 };

	CGameMenuBackground m_menu; //开始菜单背景图
	CGameMenuPanel m_menuSelect; //开始菜单
	CPlayer m_player01{ 200,100,_T("images/tank_player1.png") };
	CPlayer m_player02{ 200,100,_T("images/tank_player2.png") };
	CBot m_bot;  //机器人，人机大战时用到
	CBot m_bit{ 300,200,_T("images/tank_bot.png") };
	std::list<CBullet> m_lstBullets;

	CGameMap m_map{ 10,10,780,580 };//地图对象
	CGameMenuPanel2 m_menuBackup; 			// 返回菜单
	CKeyMenuPlayer01 m_keymenu01; 			// 提示按键的菜单
	CKeyMenuPlayer02 m_keymenu02;

	/* 游戏逻辑处理:
	1. 维护子弹状态
	2. 维护 机器人AI的自动移动,自动发射子弹
	3. 维护 玩家 坦克的状态
	以测检测包括： 撞墙，子弹命中坦克...*/
	void GameRunLogic();
	void GameRunLogicOnMenu();				// 游戏逻辑处理 : 选择阶段
	void GameRunLogicOnOne2BotMenu();			// 游戏逻辑处理 : 单人对电脑 菜单阶段
	void GameRunLogicOnOne2Bot();				// 游戏逻辑处理 : 单人对电脑
	void GameRunLogicOnOne2BotEnd();			// 游戏逻辑处理 : 单人对电脑 结束
	void GameRunLogicOnOne2OneMenu();			// 游戏逻辑处理 : 双人对战 菜单阶段
	void GameRunLogicOnOne2One();				// 游戏逻辑处理 : 双人对战
	void GameRunLogicOnOne2OneEnd();			// 游戏逻辑处理 : 双人对战 结束

												// 把上述方法放入数组，方便调用
	void(CGame::*m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicOnMenu,         	// 选择阶段
		&CGame::GameRunLogicOnOne2BotMenu, 	// 人机对战按键提示
		&CGame::GameRunLogicOnOne2Bot,      	// 单人对电脑
		&CGame::GameRunLogicOnOne2BotEnd,   	// 单人对电脑 结束
		&CGame::GameRunLogicOnOne2OneMenu,  	// 双人对战按键提示
		&CGame::GameRunLogicOnOne2One,      	// 双人对战
		&CGame::GameRunLogicOnOne2OneEnd    	// 双人对战 结束
	};
	/* 游戏绘图处理
	负责 绘画 游戏中的对象
	*/
	void GameRunDraw();

	void GameRunDrawOnMenu(Graphics &gh); 		// 游戏绘图处理 : 选择阶段

	void GameRunDrawOnOne2BotMenu(Graphics &gh); 	// 游戏绘图处理 : 单人对电脑 菜单阶段

	void GameRunDrawOnOne2Bot(Graphics &gh); 		// 游戏绘图处理 : 单人对电脑

	void GameRunDrawOnOne2BotEnd(Graphics &gh); 	// 游戏绘图处理 : 单人对电脑 结束

	void GameRunDrawOnOne2OneMenu(Graphics &gh); 	// 游戏绘图处理 : 双人对战 菜单阶段

	void GameRunDrawOnOne2One(Graphics &gh); 		// 游戏绘图处理 : 双人对战

	void GameRunDrawOnOne2OneEnd(Graphics &gh); 	// 游戏绘图处理 : 双人对战 结束

													// 把上述方法放入数组，方便调用
	void(CGame::*m_drawFunc[EGameTypeCount])(Graphics &) = {
		&CGame::GameRunDrawOnMenu, 				// 选择阶段
		&CGame::GameRunDrawOnOne2BotMenu, 		// 人机对战阶段
		&CGame::GameRunDrawOnOne2Bot, 			// 单人对电脑
		&CGame::GameRunDrawOnOne2BotEnd, 			// 单人对电脑 结束
		&CGame::GameRunDrawOnOne2OneMenu, 		// 双人对战阶段
		&CGame::GameRunDrawOnOne2One, 			// 双人对战
		&CGame::GameRunDrawOnOne2OneEnd 			// 双人对战 结束
	};

	void RemoveTimeoutBullets();  //移除超时子弹
	void ProcessHitBullets();  //维护子弹运行及撞墙处理
	void AI();  //维护电脑子弹寻路攻击
	//输出fps
	void DrawFps(Graphics &gh);

};

