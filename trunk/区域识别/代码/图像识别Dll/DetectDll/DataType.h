#pragma once
#include <afxtempl.h>
struct BMPAreaNode
{
	int Xleft;
	int Ytop;
	int Xright;
	int Ybottom;
};

struct RGBTable
{
	int iAverRed;
	int iAverGreen;
	int iAverBlue;
	
};

struct MyTemplate
{
	int iThreShold;
	int iWidth;
	int iHeight;
	float fAverGray;
	float fBlackPercent;
	float fAverGrayDiffer;
	float fBlackPerDiffer;
	int iCheckType;
	RGBTable myRGBTable;
};




typedef CList<BMPAreaNode,BMPAreaNode&> BMPAreaList;//区域链表

typedef CList<MyTemplate,MyTemplate&> MyTemplateList;//模板表