#pragma once
#include <gdiplus.h>
#include <math.h>
#include <cmath>
/*
定义游戏中所有对象的父类
*/
class CGameEntry
{
public:
	CGameEntry() //构造函数
	{
	}
	virtual ~CGameEntry()  //析构函数
	{

	}
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0; //用于绘制自己的虚函数
};
