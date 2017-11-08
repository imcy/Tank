#include "stdafx.h"
#include "Game.h"
#include <algorithm>

CGame::CGame()
{
	GameInitOne2Bot(); //初始化人机大战游戏
}


CGame::~CGame()
{
}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool CGame::EnterFrame(DWORD dwTime) //游戏帧处理
{
	GameRunDraw(); //每次帧处理绘制图片
	GameRunLogic();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuSelect.OnMouseMove(nFlags, point); //再向下调用到menu中的方法
}

void CGame::OnButtonUp(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuSelect.OnLButtonUp(nFlags, point);
}

bool CGame::GameInitOne2Bot()
{
	//游戏初始化
	m_map.LoadMap();//载入地图
	//玩家一
	{
		PointF ptCenter;
		if (!m_map.FindRandomPosition(ptCenter)) {
			AfxMessageBox(_T("调整玩家1位置失败"));
		}
		else
		{
			m_player01 = CPlayer(ptCenter.X, ptCenter.Y, _T("images/tank_player1.png"));
			//m_player01.SetCenterPoint(ptCenter);
		}
	}
	//处理子弹
	m_lstBullets.clear();
	return true;
}

void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//获取客户区大小
	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC *dc = CClientDC::FromHandle(hdc);
	CDC m_dcMemory;  //双缓冲区绘图
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc()); //构造Graphics对象
	gh.Clear(Color::White); //清除背景
	gh.ResetClip();
	//画出一张图片
	//Gdiplus::Image *img = Image::FromFile(_T("./images/menu_background.png"));
	//gh.DrawImage(img, rc.left, rc.top, rc.Width(), rc.Height());
	/*{
		m_menu.getImage(); //先获取背景图片
		m_menu.Draw(gh);
		m_menuSelect.getImage();
		m_menuSelect.Draw(gh); //先获取所有图片
	}*/
	{
		m_map.Draw(gh);
		m_player01.getImage();  //画玩家1
		m_player01.Draw(gh);
		for (auto &blt : m_lstBullets) {
			blt.Draw(gh);
		}
	}
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(),
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();

	return;
}

void CGame::GameRunLogic()
{
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
	//按键处理
	{
		if (KEYDOWN(VK_LEFT))          // 左方向盘按下
		{
			m_player01.RotateLeft(); // 玩家一向左转
		}
		if (KEYDOWN(VK_RIGHT))          			// 右方向盘按下
		{
			m_player01.RotateRight();			// 玩家一向右转
		}
		if (KEYDOWN(VK_UP))            			// 上方向盘按下
		{
			if (m_map.IsHitTheWall(m_player01, true)) {
				m_player01.ChangeDirection(true); //判断到撞墙，需要改变方向
			}
			else
			{
				m_player01.Forward();     			// 玩家一向前走

			}
			

		}
		if (KEYDOWN(VK_DOWN))          			// 下方向盘按下
		{
			if (m_map.IsHitTheWall(m_player01, true)) {
				m_player01.ChangeDirection(true); //判断到撞墙，需要改变方向
			}
			else
			{
				m_player01.Backward(); 		// 玩家一向后退
			}
		}
		if (KEYDOWN('M'))                		// 按下M键
		{
			CBullet blt;
			if (m_player01.Fire(blt)) {        	// 开火
				m_lstBullets.push_back(blt); 	// 加入到地图列表中
			}
		}
		//子弹撞墙处理
		for (auto &blt : m_lstBullets) {
			m_map.HitWallProcess(blt);
			blt.Move();
		}
		//检查子弹是否击中坦克，击中则坦克爆炸
		for (auto &blt : m_lstBullets) {
			if (m_player01.IsHitted(blt)) {
				m_player01.Bomb();
			}
			break;
		}
	}

	for (auto &blt : m_lstBullets) {          	// 处理子弹对象的移动
		blt.Move();                            	// 子弹向前移动
	}

	// 移除超时的子弹
	{
		// 查找超时的子弹
		auto itRemove = std::remove_if(m_lstBullets.begin(),
			m_lstBullets.end(),
			[](CBullet & blt)->bool {
			return blt.IsTimeout();
		});
		for (auto it = itRemove; it != m_lstBullets.end(); ++it) {
			it->SetActive(false);				// 设置为无效
			it->GetOwner()->AddBullet(*it);	// 给对应的坦克增加子弹
		}
		// 从本地 删除 子弹
		m_lstBullets.erase(itRemove, m_lstBullets.end());
	}
}

void CGame::DrawFps(Graphics & gh)
{
	static int fps = 0; //定义静态变量
	m_fps++;  //已经画了多少帧
	static DWORD dwLast = GetTickCount(); //记录上次输出fps时间
	if (GetTickCount() - dwLast >= 1000)  //判断时间间隔是否超过1s
	{

		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();  //更新时刻
	}
	//输出Fps
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);
		SolidBrush brush(Color(0x00, 0x00, 0xFF));//红色画刷
		Gdiplus::Font font(_T("宋体"), 10.0); //创建输出字体
		CRect rc;
		::GetClientRect(m_hWnd, &rc); //还是获取窗口大小，定位文字输出位置
		PointF origin(static_cast<float>(rc.right - 50),
			static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush); //正式输出文字

	}
}
