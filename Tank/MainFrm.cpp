
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Tank.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define GAME_WIN_W (800) //窗口宽
#define GAME_WIN_H (600) //窗口高

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
#define MY_STYLE (WS_OVERLAPPED| WS_CAPTION| WS_SYSMENU|\
				WS_MINIMIZEBOX|FWS_ADDTOTITLE)
//创建窗口
	Create(NULL, _T("坦克大战"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
//设置客户区大小
	{
		CRect rcCli;
		GetClientRect(rcCli); //获取客户区大小
		RECT reFrame = { 0,0, //计算边框大小
					m_iWidth + m_iWidth - rcCli.right,
					m_iHeight + m_iHeight - rcCli.bottom }; 
		MoveWindow(&reFrame, TRUE); //设置窗口位置和大小
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case ETimerIdGameLoop: {
		static DWORD dwLastUpdate = GetTickCount();  //记录本次时刻
		if (GetTickCount() - dwLastUpdate >= 20)  //判断时间间隔
		{
			m_game.EnterFrame(GetTickCount());  //进入游戏帧处理
			dwLastUpdate = GetTickCount();  //更新时刻
		}
	}
	default:
		break;
	}
	CMainFrame::OnTimer(nIDEvent);
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
	SetTimer(ETimerIdGameLoop, 0, NULL); //启动定时器每次都会进入游戏帧
	m_game.SetHandle(GetSafeHwnd()); //设置主窗口句柄
}

