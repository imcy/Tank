#pragma once
#include <gdiplus.h>
#include <math.h>
#include <cmath>
/*
������Ϸ�����ж���ĸ���
*/
class CGameEntry
{
public:
	CGameEntry() //���캯��
	{
	}
	virtual ~CGameEntry()  //��������
	{

	}
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0; //���ڻ����Լ����麯��
};
