#pragma once
#include "GameEntry.h"
// ���Դ�ȡ��
#ifndef PI
#define PI (3.1415926f)
#endif

class CGameEntryMoveable :
	public CGameEntry
{
public:
	CGameEntryMoveable();
	CGameEntryMoveable(const CGameEntryMoveable &rhs); //���췽��
	virtual ~CGameEntryMoveable();
	CGameEntryMoveable &operator=(const CGameEntryMoveable &rhs);//���������
/*���ƶ����干ͬ����*/
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0;
	virtual void RotateRight();
	virtual void RotateLeft(); // ��ת
	virtual void Forward(); // ǰ�ƶ�
	virtual void Backward();//����
	virtual float GetXSpeed() const;// ���ƽ������ٶȷ���
	virtual float GetYSpeed() const;// ��ü�ֱ������ٶȷ���
	virtual RectF ForwardNextRect() const;// ��һ��(ǰ����)λ��
	virtual RectF BackwardNextRect() const; // ��һ��(���˵�)λ��
	// ��ȡͷ��λ�� : ͼƬΪ���ͣ�ͷ��λ�� ���ǽǶ�Ϊ0ʱ��
	// �������ĵ�Ϊ�뾶���Խ���/2)���ȣ���ֵ���Լ��� X���λ��
	virtual PointF GetHeadPos() const;
	virtual PointF GetCenterPoint()const;// ��ȡ���ĵ�
	virtual void SetCenterPoint(const PointF &ptCenter);// �������ĵ�
	virtual void SetDirectionArc(float dir);// ���ýǶ� : ��λΪ a*PI
	virtual void SetDirection(float dir);// ���ýǶ�(��λ�� ��)
	virtual float GetDirectionArc() const; // ��õ�ǰ�ĽǶ�(��λ�� a*PI)
	virtual float GetDirection() const;// ��õ�ǰ�ĽǶ�(��λ�� ��)
	virtual void SetDirectionTurnArc(float dir);// ����ÿ����ת�ĽǶ�
	virtual void SetDirectionTurn(float dir);// ����ÿ����ת�ĽǶ�
	virtual float GetDirectionTurnArc() const;// ��õ�ǰ�ĽǶ�(��λ�� PI)
	virtual float GetDirectionTurn() const;// ��õ�ǰ�Ļ���(��λ�Ƕ�)
	virtual  bool IsActive() const;//�ж��Ƿ���Ч
	virtual void SetActive(bool bActive);
	virtual void SetRect(const RectF rect);// ռ�÷�Χ
	virtual RectF GetRect() const;
	virtual void SetSpeed(float speed);// �ƶ��ٶ�
	virtual float GetSpeed() const;

private:
	bool m_bActive{ false };  		// �Ƿ���Ч
	RectF m_rect{ 0, 0, 100, 100 }; // ռ�õ�����
	float m_fSpeed{ 10 }; 			// �ƶ��ٶ�
	float m_direction{ 0 }; 		// ��ǰ�ĽǶ�
	float m_directionTurn{ 5 }; 	// ÿ����ת�ĽǶ�
};

