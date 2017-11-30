# Tank
坦克大战游戏项目

照着书上的教程写的一个坦克大战的游戏项目。有两个部分一个是人机大战一个是双人对战。其中人机大战用到了一些自动寻路的代码。
完成图片：<br>
![](https://github.com/imcy/Tank/blob/master/%E5%AE%8C%E6%88%90%E5%9B%BE%E7%89%87/2017-11-30_140126.png) <br>
菜单选择:<br>
![](https://github.com/imcy/Tank/blob/master/%E5%AE%8C%E6%88%90%E5%9B%BE%E7%89%87/2017-11-30_140138.png)<br>
确认进入游戏:<br>
![](https://github.com/imcy/Tank/blob/master/%E5%AE%8C%E6%88%90%E5%9B%BE%E7%89%87/2017-11-30_144959.png)<br>
游戏界面:<br>
![](https://github.com/imcy/Tank/blob/master/%E5%AE%8C%E6%88%90%E5%9B%BE%E7%89%87/2017-11-30_140211.png)<br>

### MainFrm
主界面，MFC主程序,构造函数中设置窗口
```cpp
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
```
onTimer定时器函数进入Game类的帧处理函数
```cpp
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
	CFrameWnd::OnTimer(nIDEvent);
}
```
添加鼠标处理函数，鼠标处理方法向下传递
```cpp
void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	/*直接调用CGame的方法*/
	m_game.OnMouseMove(nFlags, point); 
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	/*直接调用CGame的方法*/
	m_game.OnButtonUp(nFlags, point);
	CFrameWnd::OnLButtonUp(nFlags, point);
}
```
### CGame
游戏主框架，用于初始化菜单界面，设置和控制游戏的各个状态，根据不同状态调用不同类封装的函数,阶段控制用枚举类型，并设置一个设置函数
```cpp
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
```
根据枚举类型调用函数，用一个函数指针控制,游戏中不同模式都有init,draw,logic方法。每次根据游戏阶段，调用不同函数。
```cpp
	void(CGame::*m_drawFunc[EGameTypeCount])(Graphics &) = {
		&CGame::GameRunDrawOnMenu, 				// 选择阶段
		&CGame::GameRunDrawOnOne2BotMenu, 		// 人机对战阶段
		&CGame::GameRunDrawOnOne2Bot, 			// 单人对电脑
		&CGame::GameRunDrawOnOne2BotEnd, 			// 单人对电脑 结束
		&CGame::GameRunDrawOnOne2OneMenu, 		// 双人对战阶段
		&CGame::GameRunDrawOnOne2One, 			// 双人对战
		&CGame::GameRunDrawOnOne2OneEnd 			// 双人对战 结束
	};
```
调用时候：
```cpp
(this->*m_drawFunc[m_eStep])(gh);  //调用相应画图函数
(this->*m_logicFunc[m_eStep])();// 根据不同阶段调用不同的处理方法
(this->*m_initFunc[m_eStep])();  //根据不同阶段调用不同处理方法,函数名前加*号  
```
### CGameTimer
游戏中的时间管理类，封装所有游戏中要用到的时间管理方法

### CGameEntry
游戏中所有对象父类，包括一个每个对象的绘图虚函数
```cpp
virtual void Draw(Gdiplus::Graphics &gh) const = 0; //用于绘制自己的虚函数
```
### CGameEntryMoveable
一个游戏中所有可移动物体的父类，父类中包括全部可移动物体的共同属性，包括前进后退，设置位置等等。
### CTankEntry
坦克基类，继承CGameEntryMoveable类，构造函数中，包括读取坦克图片和坦克爆炸图片
```cpp
    m_imgTank = Image::FromFile(szImgName);
	if (m_imgTank != NULL) {
		for (int i = 0; i < _countof(m_imgExplosion); ++i) {
			TCHAR szPath[MAX_PATH];
			_stprintf_s(szPath, _T("./images/%d.gif"), i);
			m_imgExplosion[i] = Image::FromFile(szPath);
		}
		int width = m_imgTank->GetWidth();
		int height = m_imgTank->GetHeight();
```
重写调用Graphics的画图函数
```cpp
// 画自己
void CTankEntry::Draw(Gdiplus::Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }
    //旋转绘图平面
    PointF center = GetCenterPoint();

    // 1.平移变换 : 移动坐标点到 坦克中心
    gh.TranslateTransform(center.X, center.Y);
    // 2.旋转变换 : 使作标系，跟坦克的角度一致
    gh.RotateTransform(GetDirection());
    // 3.恢复原点
    gh.TranslateTransform(-center.X, -center.Y);

    switch(m_status) {
        case 1: {
            //绘制坦克
            DrawTank(gh);
            DrawTankTail(gh);
            break;
        }
        case 2: {
            // 绘制爆制的坦克
            DrawExplosion(gh);
            break;
        }
        default:
            break;
    };
#if 1
    // 重置坐标变换 : 精度是否够呢？
    // 1.移动到中心点
    gh.TranslateTransform(center.X, center.Y);
    // 2.逆向转
    gh.RotateTransform(-GetDirection());
    // 3.再移动回去
    gh.TranslateTransform(-center.X, -center.Y);
    //ScaleTransform : 缩放
#else
    // 第二种恢复的方式 : 缺点：直接重置，不好啊
    gh.ResetTransform();
#endif
}
```
### player
玩家类，继承自CTankEntry，用于创建玩家对象，CGame类中创建两个玩家,构造函数调用父类CTankEntry的构造函数
```cpp
CPlayer m_player01{ 200,100,_T("images/tank_player1.png") };
CPlayer m_player02{ 200,100,_T("images/tank_player2.png") };
```
### CBullet类
子弹类，同样继承自CGameEntryMoveable,重写draw函数
```cpp
// 画自己
void CBullet::Draw(Graphics &gh) const
{
    if(!IsActive()) {
        return;
    }
    // 画一个圆...
    gh.FillEllipse(&SolidBrush(Color::OrangeRed), GetRect());
}
```
### CKeyMenuPanle01、CKeyMenuPanel02
主界面的菜单类，画一个人机大战按钮，一个双人对战按钮，封装鼠标处理函数

### CKeyMenuPlayer01、CKeyMenuPlayer02
Player菜单类,继承自CGameEntryMoveable，画菜单项构造函数找不到图片，添加一个getImage函数
```cpp
CKeyMenuPlayer01::CKeyMenuPlayer01()
{
    m_img[0] = Image::FromFile(_T("./images/menu_key_player01_0.png"));
}

void CKeyMenuPlayer01::getImage()
{
	if (m_img[0] == NULL) {
		m_img[0] = Image::FromFile(_T("./images/menu_key_player01_0.png"));
		m_img[1] = Image::FromFile(_T("./images/menu_key_player01_1.png"));
		if (m_img[0]) {
			RectF rc = { 800 - 100, 600 - 40
				, static_cast<float>(m_img[0]->GetWidth())
				, static_cast<float>(m_img[0]->GetHeight())
			};
			SetRect(rc);
		}
	}
}
```
### CGameMenuBackground
背景类，继承自CGameEntryMoveable，用于绘制背景

### CGameMap
用于绘制游戏中的迷宫

### CGameHit
碰撞检测函数

### CBot
机器人类，用于人机对战，创建机器人
