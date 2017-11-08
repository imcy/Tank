#pragma once
#include "GameEntry.h"
// 属性存取器
#ifndef PI
#define PI (3.1415926f)
#endif

class CGameEntryMoveable :
	public CGameEntry
{
public:
	CGameEntryMoveable();
	CGameEntryMoveable(const CGameEntryMoveable &rhs); //构造方法
	virtual ~CGameEntryMoveable();
	CGameEntryMoveable &operator=(const CGameEntryMoveable &rhs);//运算符重载
/*可移动物体共同属性*/
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0;
	virtual void RotateRight();
	virtual void RotateLeft(); // 左转
	virtual void Forward(); // 前移动
	virtual void Backward();//后退
	virtual float GetXSpeed() const;// 获得平方向的速度分量
	virtual float GetYSpeed() const;// 获得坚直方向的速度分量
	virtual RectF ForwardNextRect() const;// 下一步(前进的)位置
	virtual RectF BackwardNextRect() const; // 下一步(后退的)位置
	// 获取头部位置 : 图片为矩型：头部位置 就是角度为0时，
	// 距离中心点为半径（对角线/2)长度，重值于自己的 X轴的位置
	virtual PointF GetHeadPos() const;
	virtual PointF GetCenterPoint()const;// 获取中心点
	virtual void SetCenterPoint(const PointF &ptCenter);// 设置中心点
	virtual void SetDirectionArc(float dir);// 设置角度 : 单位为 a*PI
	virtual void SetDirection(float dir);// 设置角度(单位是 度)
	virtual float GetDirectionArc() const; // 获得当前的角度(单位是 a*PI)
	virtual float GetDirection() const;// 获得当前的角度(单位是 度)
	virtual void SetDirectionTurnArc(float dir);// 设置每次旋转的角度
	virtual void SetDirectionTurn(float dir);// 设置每次旋转的角度
	virtual float GetDirectionTurnArc() const;// 获得当前的角度(单位是 PI)
	virtual float GetDirectionTurn() const;// 获得当前的弧度(单位是度)
	virtual  bool IsActive() const;//判断是否有效
	virtual void SetActive(bool bActive);
	virtual void SetRect(const RectF rect);// 占用范围
	virtual RectF GetRect() const;
	virtual void SetSpeed(float speed);// 移动速度
	virtual float GetSpeed() const;

private:
	bool m_bActive{ false };  		// 是否有效
	RectF m_rect{ 0, 0, 100, 100 }; // 占用的区域
	float m_fSpeed{ 10 }; 			// 移动速度
	float m_direction{ 0 }; 		// 当前的角度
	float m_directionTurn{ 5 }; 	// 每次旋转的角度
};

