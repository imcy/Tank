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
	void SetHandle(HWND hWnd); //������ھ��
	bool EnterFrame(DWORD dwTime); //������Ϸ֡
	void OnMouseMove(UINT nFlags, CPoint point); //��������ƶ�
	void OnButtonUp(UINT nFlags, CPoint point); //�����ʼ�̧���¼�
	//��ǰ��Ϸ�����׶�
	enum EGameType {
		EGameTypeMenu=0,  //ѡ��׶�
		EGameTypeOne2BotMenu, //�˻���ս�˵��׶�
		EGameTypeOne2Bot, //�˻���ս
		EGameTypeOne2BotEnd, //�˻���ս����
		EGameTypeOne2OneMenu, //˫�˴�ս�˵��׶�
		EGameTypeOne2One,    //˫�˴�ս
		EGameTypeOne2OneEnd, //˫�˴�ս����
		EGameTypeCount, //��7���׶�
	};
	//���õ�ǰ��Ϸ�����׶�
	void SetStep(CGame::EGameType step);
private:
	HWND m_hWnd; //����
	/*
	��Ϸ��ʼ��
	������Ϸ���󣬳�ʼ����ͼ������λ�õ�
	*/
	EGameType m_eStep{ EGameTypeMenu };  //��ǰ�׶Σ��˵�ѡ��׶�
	bool GameInit();
	bool GameInitMenu(); //��Ϸ��ʼ����ѡ��׶�
	bool GameInitOne2BotMenu();//��Ϸ��ʼ�����˻���ս�˵��׶�
	bool GameInitOne2Bot();//��Ϸ��ʼ�����˻���ս
	bool GameInitOne2BotEnd();//��Ϸ��ʼ�����˻���ս����
	bool GameInitOne2OneMenu(); //��Ϸ��ʼ����˫�˴�ս�˵��׶�
	bool GameInitOne2One(); //��Ϸ��ʼ����˫�˴�ս
	bool GameInitOne2OneEnd(); //��Ϸ��ʼ����˫�˴�ս����
	//�������뺯��ָ�룬�������
	bool(CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd
	};

	//��Ϸ��ͼ����
	//������Ϸ�еĶ���
	//��¼��Ϸÿ�����֡
	int m_fps{ 0 };

	CGameMenuBackground m_menu; //��ʼ�˵�����ͼ
	CGameMenuPanel m_menuSelect; //��ʼ�˵�
	CPlayer m_player01{ 200,100,_T("images/tank_player1.png") };
	CPlayer m_player02{ 200,100,_T("images/tank_player2.png") };
	CBot m_bot;  //�����ˣ��˻���սʱ�õ�
	CBot m_bit{ 300,200,_T("images/tank_bot.png") };
	std::list<CBullet> m_lstBullets;

	CGameMap m_map{ 10,10,780,580 };//��ͼ����
	CGameMenuPanel2 m_menuBackup; 			// ���ز˵�
	CKeyMenuPlayer01 m_keymenu01; 			// ��ʾ�����Ĳ˵�
	CKeyMenuPlayer02 m_keymenu02;

	/* ��Ϸ�߼�����:
	1. ά���ӵ�״̬
	2. ά�� ������AI���Զ��ƶ�,�Զ������ӵ�
	3. ά�� ��� ̹�˵�״̬
	�Բ�������� ײǽ���ӵ�����̹��...*/
	void GameRunLogic();
	void GameRunLogicOnMenu();				// ��Ϸ�߼����� : ѡ��׶�
	void GameRunLogicOnOne2BotMenu();			// ��Ϸ�߼����� : ���˶Ե��� �˵��׶�
	void GameRunLogicOnOne2Bot();				// ��Ϸ�߼����� : ���˶Ե���
	void GameRunLogicOnOne2BotEnd();			// ��Ϸ�߼����� : ���˶Ե��� ����
	void GameRunLogicOnOne2OneMenu();			// ��Ϸ�߼����� : ˫�˶�ս �˵��׶�
	void GameRunLogicOnOne2One();				// ��Ϸ�߼����� : ˫�˶�ս
	void GameRunLogicOnOne2OneEnd();			// ��Ϸ�߼����� : ˫�˶�ս ����

												// �����������������飬�������
	void(CGame::*m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicOnMenu,         	// ѡ��׶�
		&CGame::GameRunLogicOnOne2BotMenu, 	// �˻���ս������ʾ
		&CGame::GameRunLogicOnOne2Bot,      	// ���˶Ե���
		&CGame::GameRunLogicOnOne2BotEnd,   	// ���˶Ե��� ����
		&CGame::GameRunLogicOnOne2OneMenu,  	// ˫�˶�ս������ʾ
		&CGame::GameRunLogicOnOne2One,      	// ˫�˶�ս
		&CGame::GameRunLogicOnOne2OneEnd    	// ˫�˶�ս ����
	};
	/* ��Ϸ��ͼ����
	���� �滭 ��Ϸ�еĶ���
	*/
	void GameRunDraw();

	void GameRunDrawOnMenu(Graphics &gh); 		// ��Ϸ��ͼ���� : ѡ��׶�

	void GameRunDrawOnOne2BotMenu(Graphics &gh); 	// ��Ϸ��ͼ���� : ���˶Ե��� �˵��׶�

	void GameRunDrawOnOne2Bot(Graphics &gh); 		// ��Ϸ��ͼ���� : ���˶Ե���

	void GameRunDrawOnOne2BotEnd(Graphics &gh); 	// ��Ϸ��ͼ���� : ���˶Ե��� ����

	void GameRunDrawOnOne2OneMenu(Graphics &gh); 	// ��Ϸ��ͼ���� : ˫�˶�ս �˵��׶�

	void GameRunDrawOnOne2One(Graphics &gh); 		// ��Ϸ��ͼ���� : ˫�˶�ս

	void GameRunDrawOnOne2OneEnd(Graphics &gh); 	// ��Ϸ��ͼ���� : ˫�˶�ս ����

													// �����������������飬�������
	void(CGame::*m_drawFunc[EGameTypeCount])(Graphics &) = {
		&CGame::GameRunDrawOnMenu, 				// ѡ��׶�
		&CGame::GameRunDrawOnOne2BotMenu, 		// �˻���ս�׶�
		&CGame::GameRunDrawOnOne2Bot, 			// ���˶Ե���
		&CGame::GameRunDrawOnOne2BotEnd, 			// ���˶Ե��� ����
		&CGame::GameRunDrawOnOne2OneMenu, 		// ˫�˶�ս�׶�
		&CGame::GameRunDrawOnOne2One, 			// ˫�˶�ս
		&CGame::GameRunDrawOnOne2OneEnd 			// ˫�˶�ս ����
	};

	void RemoveTimeoutBullets();  //�Ƴ���ʱ�ӵ�
	void ProcessHitBullets();  //ά���ӵ����м�ײǽ����
	void AI();  //ά�������ӵ�Ѱ·����
	//���fps
	void DrawFps(Graphics &gh);

};

