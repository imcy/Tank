#pragma once
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
};

