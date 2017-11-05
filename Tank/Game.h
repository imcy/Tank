#pragma once
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
};

