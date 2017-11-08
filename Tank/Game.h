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
	void SetHandle(HWND hWnd); //������ھ��
	bool EnterFrame(DWORD dwTime); //������Ϸ֡
	void OnMouseMove(UINT nFlags, CPoint point); //��������ƶ�
	void OnButtonUp(UINT nFlags, CPoint point); //�����ʼ�̧���¼�
private:
	HWND m_hWnd; //����
	//��Ϸ��ͼ����
	//������Ϸ�еĶ���
	CGameMenuBackground m_menu; //��ʼ�˵�����ͼ
	CGameMenuPanel m_menuSelect; //��ʼ�˵�
	CPlayer m_player01{ 100,100,_T("images/tank_player1.png") };
	//CPlayer m_player02{ 200,100,_T("images/tank_player2.png") };
	CBot m_bit{ 300,200,_T("images/tank_bot.png") };
	std::list<CBullet> m_lstBullets;
	CGameMap m_map{ 10,10,780,580 };//��ͼ����
	bool CGame::GameInitOne2Bot();//��Ϸ��ʼ��

	void GameRunDraw();
	//��Ϸ�߼�����
	void GameRunLogic();
	//���fps
	void DrawFps(Graphics &gh);
	//��¼��Ϸÿ�����֡
	int m_fps{ 0 };
};

