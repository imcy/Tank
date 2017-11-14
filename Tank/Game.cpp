#include "stdafx.h"
#include "Game.h"
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & 0x8000)  //判断按键是否按下

CGame::CGame()
{
	//GameInitOne2Bot(); //初始化人机大战游戏
	m_menuSelect.m_pParent = this; //设置菜单选择项指向当前对象
	m_menuBackup.m_pParent = this;
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
	return true;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuBackup.getImage();
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnMouseMove(nFlags, point); //再向下调用到menu中的方法
	}
	else
	{
		m_menuBackup.OnMouseMove(nFlags, point);
	}
}

void CGame::OnButtonUp(UINT nFlags, CPoint point)
{
	m_menuSelect.getImage();
	m_menuBackup.getImage();
	if (m_eStep == EGameTypeMenu) {
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else
	{
		m_menuBackup.OnLButtonUp(nFlags, point);
	}
}
// 设置当前游戏所处阶段，根据步骤初始化
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();
}

bool CGame::GameInit()
{
	srand(GetTickCount());  //初始化随机数生成器
	return (this->*m_initFunc[m_eStep])();  //根据不同阶段调用不同处理方法,函数名前加*号
}
//游戏初始化，选择阶段
bool CGame::GameInitMenu()
{
	return true;
}
//游戏初始化，人机大战菜饭阶段
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);				// 设置单人对战keyMenu位置为屏幕正中间
	m_keymenu01.SetStop(false);					// 设置“不”停止播放动画
	return true;
}

bool CGame::GameInitOne2Bot()
{
	for (;;) {
		//游戏初始化
		m_map.LoadMap();//载入地图
		//玩家一
		{
			m_player01 = CPlayer(0, 0, _T("./images/tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {
				AfxMessageBox(_T("调整玩家1位置失败"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		// 机器人
		{
			m_bot = CBot(0, 0, _T("./images/tank_bot.png"));				// 制造机器人对象
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {				// 随机查找地图中的空地
				AfxMessageBox(_T("调整Bot位置失败")); 				// 提示调整位置失败
			}
			else {
				m_bot.SetCenterPoint(ptCenter);
			}
		}
		// 判断是否合法
		{
			// 获取机器人，玩家 所在的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY)) {
				AfxMessageBox(_T("获取坦克位置发生错误"));
				goto __Init_End;
			}
			// 判断玩家和机器人是否连通，如果无法连通，则无法进个游戏，所以需要重新设置
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty()) {
				goto __Init_End; // 可以连通，跳出循环，直接跳到函数尾部，初始化结束
			}
		}
		//处理子弹
		m_lstBullets.clear();
	}
__Init_End:
	return true;
}
// 游戏初始化 : 单人对电脑 结束
bool CGame::GameInitOne2BotEnd()
{
	return true;
}

// 游戏初始化 : 双人对战 菜单阶段
bool CGame::GameInitOne2OneMenu()
{
	// 设置 两个玩家的 keyMenu 位置 : 屏幕正中间
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);							// 设置该菜单项的位置
	m_keymenu01.SetStop(false);								// 设置不停止播放动画


	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu02.SetCenterPoint(pt); 							// 设置该菜单项的位置
	m_keymenu02.SetStop(false); 								// 设置不停止播放动画

	return true;
}

// 游戏初始化 : 双人对战
bool CGame::GameInitOne2One()
{
	for (;;) {
		m_map.LoadMap();										// 载入地图
																//中间放置坦克
		{
			m_player01 = CPlayer(0, 0, _T("./images/tank_player1.png"));	// 构造玩家一对象
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {			// 查找随机的空地位置
				AfxMessageBox(_T("调整Player01位置失败"));		// 提示查找失败
			}
			else {
				m_player01.SetCenterPoint(ptCenter); // 设置玩家一位置到这块空地中心	
			}
		}
		{
			m_player02 = CPlayer(0, 0, _T("./images/tank_player2.png"));	// 构造玩家二
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter)) {			// 随机查找地图中的空地
				AfxMessageBox(_T("调整Player02位置失败"));		// 提示查找失败
			}
			else {
				m_player02.SetCenterPoint(ptCenter);	// 设置玩家二的位置到这块空地中心
			}
		}

		m_lstBullets.clear();							// 清空子弹数组
														// 判断是否合法
		{
			// 查找机器人，玩家 所在的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY)) {
				AfxMessageBox(_T("获取坦克位置发生错误")); 	// 提示查找失败
				break;
			}
			// 判断两个玩家是否可以连通
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty()) {
				break;									// 可以连通跳出循环,初始化完成
			}
			// 不可以连通，说明本次初始化失败，不跳出循环，继续尝试一下初始化
		}
	}
	return true;
}

// 游戏初始化 : 双人对战 结束
bool CGame::GameInitOne2OneEnd()
{
	return true;	// 不需要初始化动作，直接返回true表示初始化成功
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
	

	(this->*m_drawFunc[m_eStep])(gh);  //调用相应画图函数
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(),
		m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc); 							// 释放

	return;
}
// 选择阶段
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.getImage();
	m_menuSelect.Draw(gh);
}

//单人对电脑 : 菜单阶段
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.getImage(); 
	m_menu.Draw(gh); 									// 画背景
	m_keymenu01.getImage();
	m_keymenu01.Draw(gh); 								// 画菜单
}

//单人对电脑
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.getImage();
	m_menuBackup.Draw(gh); 								// 画菜单
	m_map.Draw(gh); 										// 画墙

	m_player01.Draw(gh); 								// 画玩家一

	m_bot.Draw(gh); 										// 画机器人

	for (auto b : m_lstBullets) { 						// 画子弹 :  己经发射的
		b.Draw(gh);
	}

	DrawFps(gh); 										// 输出 : FPS
}

//单人对电脑 结束
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh); 								// 菜单
	m_map.Draw(gh); 										// 墙
	m_player01.Draw(gh); 								// 玩家
	m_bot.Draw(gh); 										// 机器人

	DrawFps(gh); 										// 输出 : FPS

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd()) { 	// 判断游戏整体结束
		m_eStep = EGameTypeMenu;							// 设置为菜单状态
	}
}

// 双人对战 : 菜单阶段
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.getImage();
	m_menu.Draw(gh); 									// 画菜单背景
	m_keymenu01.getImage();
	m_keymenu01.Draw(gh); 								// 画菜单项一
	m_keymenu02.getImage();
	m_keymenu02.Draw(gh); 								// 画菜单项二
}

// 双人对战
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
	m_menuBackup.getImage();
	m_menuBackup.Draw(gh); 								// 画菜单

	m_map.Draw(gh); 										// 画墙

	m_player01.Draw(gh); 								// 画玩家一
	m_player02.Draw(gh); 								// 画玩家二

	for (auto b : m_lstBullets) { 						// 画子弹 :  己经发射的
		b.Draw(gh);
	}

	DrawFps(gh); 											// 输出 : FPS
}

// 双人对战 结束
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
	m_menuBackup.getImage();
	m_menuBackup.Draw(gh); 									// 菜单
	m_map.Draw(gh); 											// 墙
	m_player01.Draw(gh); 									// 玩家一
	m_player02.Draw(gh); 									// 玩家二
	for (auto b : m_lstBullets) { 							// 画子弹 :  己经发射的
		b.Draw(gh);
	}
	DrawFps(gh);	 											// 输出 : FPS

	if (m_player01.IsBombEnd() || m_player02.IsBombEnd()) { // 判断游戏整体结束
		m_eStep = EGameTypeMenu;								// 设置游戏状态为菜单状态
	}
}

void CGame::GameRunLogic()
{
	// 根据不同阶段调用不同的处理方法
	(this->*m_logicFunc[m_eStep])();
}
// 游戏逻辑处理 : 选择阶段
void CGame::GameRunLogicOnMenu()
{
	// 什么也不做，还没开始游戏
}

// 游戏逻辑处理 : 单人对电脑 菜单阶段
void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M')) { 				// 如果按下了M键，停止动画状态
		m_keymenu01.SetStop();
	}

	if (m_keymenu01.GetStop()) { 		// 如果都按下了，正式开始游戏
		SetStep(EGameTypeOne2Bot);
	}
}

// 游戏逻辑处理 : 单人对电脑
void CGame::GameRunLogicOnOne2Bot()
{
	// 状态维护
	// 移动除列表中无效的子弹,并给相应的坦克增加子弹
	RemoveTimeoutBullets();

	// 检查子弹是否击中坦克 :  击中要使坦克爆炸(子弹可不用移除了，
	// 因为游戏都结束了)
	for (auto &blt : m_lstBullets) {
		if (m_bot.IsHitted(blt)) {			// 击中机器人
			m_bot.Bomb();						// 机器人爆炸
			m_eStep = EGameTypeOne2BotEnd; 	// 游戏结束
			blt.SetActive(false);				// 使子弹不再有效
		}
		if (m_player01.IsHitted(blt)) { 		// 击中玩家一
			m_player01.Bomb();				// 玩家一爆炸
			m_eStep = EGameTypeOne2BotEnd; 	// 游戏结束
			blt.SetActive(false);				// 使子弹不再有效
		}
		break;
	}

	ProcessHitBullets();  					// 子弹运动维护

	AI();									// 使机器人自动攻击玩家

											//按键处理
	{
		if (KEYDOWN(VK_LEFT)) {				// 左方向键按下
			m_player01.RotateLeft();			// 玩家一向左旋转
		}
		if (KEYDOWN(VK_RIGHT)) {				// 右方向键按下
			m_player01.RotateRight();			// 玩家一向右旋转
		}
		if (KEYDOWN(VK_UP)) {					// 上方向键按下
												// 坦克撞墙检测试	
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// 如果撞墙了
					m_player01.ChangeDirection(true);			// 改变坦克方向
				}
				else {
					m_player01.Forward();						// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// 下方向键按下
															// 坦克撞墙检测试
			{
				if (m_map.IsHitTheWall(m_player01, false)) {	// 如果撞墙了
					m_player01.ChangeDirection(true);		  	// 改变坦克方向
				}
				else {
					m_player01.Backward();				  	// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN('M')) {									// M键按下
			CBullet blt;
			if (m_player01.Fire(blt)) {						// 发射子弹
				m_lstBullets.push_back(blt);					// 加入到地图列表中
			}
		}
		if (KEYDOWN('I')) {						// 按下键盘上面的I键，机器人步进(测试功能)
												// 机器人，玩家 所在的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY)) {
				return;
			}
			float fDirNext = 0; //机器人下一步的方向
			if (!m_map.FindNextDirection(&fDirNext, startX, startY,
				targetX, targetY)) {
				return;
			}
			//获取机器人坦克的中心点
			PointF ptTankCenter = m_bot.GetCenterPoint();
			PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
			RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


			if (!rc.Contains(ptTankCenter)) { 		// 判断 坦克是否己经走到了中心点位置了
				m_bot.Forward(); 					// 没有到达中心点，继续前进
				return;
			}
			else {
				m_bot.SetDirection(fDirNext);		// 设置机器人的方向			
				m_bot.Forward();					// 机器人前进
			}
		}
	}

}

// 游戏逻辑处理 : 单人对电脑 结束
void CGame::GameRunLogicOnOne2BotEnd()
{
	//按键处理
	// 不再接受按键


	// 状态维护
	// 移动除列表中无效的子弹,并给相应的坦克增加子弹
	RemoveTimeoutBullets();


	// 子弹是否撞上墙 : 如果撞上了，改变方向等等
	ProcessHitBullets();
}

// 游戏逻辑处理 : 双人对战 菜单阶段
void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M')) { 									// 如果按下了M键，停止动画状态
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q')) { 									// 如果按下了Q键，停止动画状态
		m_keymenu02.SetStop();
	}

	if (m_keymenu01.GetStop() && m_keymenu02.GetStop()) { 	// 如果都按下了，正式开始游戏
		SetStep(EGameTypeOne2One);
	}
}

// 游戏逻辑处理 : 双人对战
void CGame::GameRunLogicOnOne2One()
{
	//按键处理
	{
		if (KEYDOWN(VK_LEFT)) { 								// 左方向键按下
			m_player01.RotateLeft();							// 玩家向左旋转
		}
		if (KEYDOWN(VK_RIGHT)) {								// 右方向键按下
			m_player01.RotateRight();							// 玩家一向右旋转
		}
		if (KEYDOWN(VK_UP)) {									// 上方向键按下
																// 坦克撞墙检测试
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// 判断玩家一是否撞墙
					m_player01.ChangeDirection(true);			// 撞墙，玩家一改变方向
				}
				else {
					m_player01.Forward();						// 玩家一没撞墙，继续前进
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// 下方向键按下
			{
				// 坦克撞墙检测试
				{
					if (m_map.IsHitTheWall(m_player01, false)) {// 判断是否撞墙
						m_player01.ChangeDirection(false);
					}
					else {
						m_player01.Backward();
					}
				}
			}
		}
		if (KEYDOWN('M')) {								// 开火键M按下
			CBullet blt;
			if (m_player01.Fire(blt)) {					// 调用玩家一开火函数
				m_lstBullets.push_back(blt);				// 把发射的子弹存入子弹数组
			}
		}
		// 玩家二
		if (KEYDOWN('A')) {								// A键按下
			m_player02.RotateLeft();						// 玩家二向左旋至
		}
		if (KEYDOWN('D')) {								// D键按下
			m_player02.RotateRight();						// 玩家二向右旋转
		}
		if (KEYDOWN('W')) {								// W键按下
														// 坦克撞墙检测试
			{
				if (m_map.IsHitTheWall(m_player02, true)) {// 判断玩家二是否撞墙
					m_player02.ChangeDirection(true);		// 玩家二改变方向
				}
				else {
					m_player02.Forward();					// 玩家二继续向前进
				}
			}
		}
		if (KEYDOWN('S')) {								// S键按按下
			{
				// 坦克撞墙检测试
				{
					if (m_map.IsHitTheWall(m_player02, false)) {// 判断玩家二是否撞墙
						m_player02.ChangeDirection(false);	// 玩家二改变方向
					}
					else {
						m_player02.Backward();				// 玩家二继续向前
					}
				}
			}
		}
		if (KEYDOWN('Q')) {								// Q键按下，开火键
			CBullet blt;
			if (m_player02.Fire(blt)) {					// 调用玩家二开火函数
				m_lstBullets.push_back(blt);				// 把发射的子弹存入子弹数组
			}
		}
		if (KEYDOWN('Z')) {								// 调试用的代码，正式工程无效
			if (m_map.IsCanKillTarget(m_player01, m_player02)) {
				AfxMessageBox(_T("可以打到"));
			}
		}
	}

	// 先判断状态
	// 移动除列表中无效的子弹,并给相应的坦克增加子弹
	RemoveTimeoutBullets();

	// 检查子弹是否击中坦克 :  击中要使坦克爆炸(子弹可不用移除了，因为游戏都结束了)
	for (auto &blt : m_lstBullets) {
		if (!blt.IsActive()) {
			continue;
		}
		if (m_player01.IsHitted(blt)) {	// 击中玩家一
			m_player01.Bomb();			// 玩家一爆炸
			m_eStep = EGameTypeOne2OneEnd;// 游戏结束
			blt.SetActive(false);
		}
		if (m_player02.IsHitted(blt)) { 	// 击中玩家二
			m_player02.Bomb();			// 玩家二爆炸
			m_eStep = EGameTypeOne2OneEnd;// 游戏结束
			blt.SetActive(false);
		}
	}

	ProcessHitBullets();					// 子弹撞墙处理
}

// 游戏逻辑处理 : 双人对战 结束
void CGame::GameRunLogicOnOne2OneEnd()
{
	//按键处理
	// 不需要按键处理

	RemoveTimeoutBullets();				// 移动除列表中无效的子弹,并给相应的坦克增加子弹

	ProcessHitBullets();					// 子弹撞墙处理
}


void CGame::RemoveTimeoutBullets()
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

void CGame::ProcessHitBullets()
{
	//子弹撞墙处理
	for (auto &blt : m_lstBullets) {
		m_map.HitWallProcess(blt);
		blt.Move();
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
// 维护电脑的自动寻路攻击
void CGame::AI()
{
	// 电脑运动状态态维护
	static CGameTimer acTimer(-1, 150);
	if (acTimer.IsTimeval()) {
		// 获得机器人，玩家 所在的位置
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) ||
			!m_map.FindObjPosition(m_player01, targetX, targetY)) {
			return;
		}
		float fDirNext = 0; //机器人下一步的方向
		if (!m_map.FindNextDirection(&fDirNext,
			startX, startY,
			targetX, targetY)) {
			return;
		}
		// 获得机器人、地图中空格的中心点
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);


		// 判断 坦克是否己经走到了中心点位置了
		if (!rc.Contains(ptTankCenter)) {
			m_bot.Forward(); // 没有到达中心点，继续前进
			return;
		}
		else {
			m_bot.SetDirection(fDirNext);	// 设置机器人下一步的运行方向
			float dir;
			// 判断是否可以打玩家一
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir)) {
				CBullet blt;
				if (m_bot.Fire(blt)) {		// 机器人开火，进行主动攻击
					m_lstBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();					// 机器人前进
		}
	}
}