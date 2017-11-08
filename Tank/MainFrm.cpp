
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "Tank.h"
#include <direct.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define GAME_WIN_W (800) //���ڿ�
#define GAME_WIN_H (600) //���ڸ�

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED| WS_CAPTION| WS_SYSMENU|\
				WS_MINIMIZEBOX|FWS_ADDTOTITLE)
//��������
	Create(NULL, _T("̹�˴�ս"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
//���ÿͻ�����С
	{
		CRect rcCli;
		GetClientRect(rcCli); //��ȡ�ͻ�����С
		RECT reFrame = { 0,0, //����߿��С
					m_iWidth + m_iWidth - rcCli.right,
					m_iHeight + m_iHeight - rcCli.bottom }; 
		MoveWindow(&reFrame, TRUE); //���ô���λ�úʹ�С
	}
}
CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	SetTimer(ETimerIdGameLoop, 0, NULL); //������ʱ��ÿ�ζ��������Ϸ֡
	m_game.SetHandle(GetSafeHwnd()); //����Ϸ�����������ھ��
	char buff[1000];
	_getcwd(buff, 1000);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case ETimerIdGameLoop: {
		static DWORD dwLastUpdate = GetTickCount();  //��¼����ʱ��
		if (GetTickCount() - dwLastUpdate >= 20)  //�ж�ʱ����
		{
			m_game.EnterFrame(GetTickCount());  //������Ϸ֡����
			dwLastUpdate = GetTickCount();  //����ʱ��
		}
	}
	default:
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	/*ֱ�ӵ���CGame�ķ���*/
	m_game.OnMouseMove(nFlags, point); 
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	/*ֱ�ӵ���CGame�ķ���*/
	m_game.OnButtonUp(nFlags, point);
	CFrameWnd::OnLButtonUp(nFlags, point);
}
