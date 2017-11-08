#pragma once
#include "GameMenuBackground.h"
#include "GameMenuPanel.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"
#include "GameMap.h"

class CGame
{
public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd); //输出窗口句柄
	bool EnterFrame(DWORD dwTime); //进入游戏帧
	void OnMouseMove(UINT nFlags, CPoint point); //处理鼠标移动
	void OnButtonUp(UINT nFlags, CPoint point); //处理邮件抬起事件
private:
	HWND m_hWnd; //窗口
	//游戏绘图处理
	//绘制游戏中的对象
	CGameMenuBackground m_menu; //开始菜单背景图
	CGameMenuPanel m_menuSelect; //开始菜单
	CPlayer m_player01{ 100,100,_T("images/tank_player1.png") };
	//CPlayer m_player02{ 200,100,_T("images/tank_player2.png") };
	CBot m_bit{ 300,200,_T("images/tank_bot.png") };
	std::list<CBullet> m_lstBullets;
	CGameMap m_map{ 10,10,780,580 };//地图对象
	bool CGame::GameInitOne2Bot();//游戏初始化

	void GameRunDraw();
	//游戏逻辑处理
	void GameRunLogic();
	//输出fps
	void DrawFps(Graphics &gh);
	//记录游戏每秒多少帧
	int m_fps{ 0 };
};

