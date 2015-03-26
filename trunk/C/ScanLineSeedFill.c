#include<dos.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#define FALSE 0
#define TRUE  1
#define NULL  0

union REGS regs;       /* 鼠标的变量 */
int  X_max,Y_max;     /* 鼠标活动范围最大值 */
int x_Origin, y_Origin,x_Old,y_Old,x_New,y_New;/* 鼠标点击的初始点,前一点和当前点的坐标 */
int PointNum=0;    /* 判断鼠标是否是第一次按下 */
int LineDrawFlag=FALSE;    /* 随鼠标画线标志 */

int FillFlag=FALSE;
                                                    /* 会在程序退出时报错,不清楚原因!! */

typedef struct Point
{  int x;
   int y;
   struct Point *next;
}Point;

Point *head1;     /* 栈首结点 */
void push(int x,int y)            /* 入栈操作 */
{
    Point *temp;
    temp=(Point *)malloc(sizeof(Point));
    temp->x=x;
    temp->y=y;
    temp->next=head1;
    head1=temp;

}

void pop(int *x,int *y)     /* 出栈操作 */
{
 Point  *temp;
 temp=head1;
 *x=head1->x;
 *y=head1->y;
 head1=head1->next;
 free(temp);
}

void ScanlineSeedFill(int Sx,int Sy,int boundarycolor,int fillcolor)
{                           /* 扫描线种子填充 */

    int x_Now,y_Now,x0,y0,Rx,Lx,OldRx,OldLx;
    int RightFlag,LeftFlag; /* 是否到达最右非边界点标志 */
    int i;                  /* 循环控制变量 */
    push(Sx,Sy);           /* 种子点入栈 */

    while(head1!=NULL)         /* 当栈不为空时,做以下事情 */
    {
      pop(&x_Now,&y_Now);        /* 头结点出栈 */

      x0=x_Now+1;
      while(getpixel(x0,y_Now)!=boundarycolor)  /* 找最右边的非边界点! */
          x0++;

      Rx=x0-1;
      OldRx=Rx;                     /* 保存最右点,以便在下一条扫描线时 */
     x0=x_Now-1;

  while(getpixel(x0,y_Now)!=boundarycolor) /*  找最左边的非边界点 */
       x0--;


    Lx=x0+1;
   setcolor(fillcolor);
   setwritemode(OR_PUT);
   line(Lx,y_Now,Rx,y_Now);          /* 填充当前扫描线所在行 */
   setwritemode(XOR_PUT);
   OldLx=Lx;                  /* 保存最左点,以便在下一条扫描线时用 */

   y0=y_Now+1;               /* 扫描线向上扫描 */

   RightFlag=FALSE;

  while(Lx<=Rx &&RightFlag==FALSE)/* 查找上一条线的最右值,并记录为Rx */
  {
   if(getpixel(Rx,y0)==boundarycolor)

       Rx--;
       else
   RightFlag=TRUE;                      /* 如果找到最右值,则置最右标志为真 */
  }



  LeftFlag=FALSE;
  while(Lx<=Rx && LeftFlag==FALSE) /* 查找上一条线的最左值，并记录为Lx */
  {
   if(getpixel(Lx,y0)==boundarycolor)
          Lx++;
   else
          LeftFlag=TRUE;                          /* 如果找到最左值,则置最左标志为真 */
  }

if(RightFlag==TRUE&&LeftFlag==TRUE && getpixel(Rx,y0)!=boundarycolor &&
              (getpixel(Rx,y0)!=fillcolor || getpixel(Lx,y0)!=fillcolor))
   {                                /* 如果找到了最左值和最右值，则执行下面的语句 */
     push(Rx,y0);           /* 将上一条线上的最右点作为种子点入栈 */
     for(i=Lx;i<Rx;i++)      /* 在LX到RX间找每一像素段的最右像素作为种子压入栈 */
        if(getpixel(i,y0)==boundarycolor && getpixel(i-1,y0)!=boundarycolor)
            push(i-1,y0);
    }


  y0=y_Now-1;              /* 扫描线向下扫描 */

  Rx=OldRx;
  Lx=OldLx;

  RightFlag=FALSE;
  while(Lx<=Rx && RightFlag==FALSE)/* 查找下一条线的最右值,并记录为RX */
  {
   if(getpixel(Rx,y0)==boundarycolor)
        Rx--;
   else
        RightFlag=TRUE;             /* 如果找到最右值,则置最右标志为真 */
  }

  LeftFlag=FALSE;
  while( Lx<=Rx &&LeftFlag==FALSE) /* 查找下一条线的最左值，并记录为LX */
  {

    if(getpixel(Lx,y0)==boundarycolor)
         Lx++;
    else
         LeftFlag=TRUE;      /* 如果找到最左值,则置最左标志为真 */
  }

  if(RightFlag==TRUE&&LeftFlag==TRUE && getpixel(Rx,y0)!=boundarycolor &&
       (getpixel(Rx,y0)!=fillcolor || getpixel(Lx,y0)!=fillcolor))
   {                         /* 如果找到了最左值和最右值，则执行下面的语句 */
       push(Rx,y0);            /* 将下一条线上的最右点作为种子点压入栈 */

       for(i=Lx;i<Rx;i++)      /* 在LX到RX间找每一像素段的最右像素作为种子压入栈 */
        if(getpixel(i,y0)==boundarycolor && getpixel(i-1,y0)!=boundarycolor)
            push(i-1,y0);
    }
 }        /* end while(head1!=null) */
}        /* end scanlineseedfill */


void Initgr()                  /* 初始化图形模式 */
{ 
  int gdriver=DETECT,gmode;
  registerbgidriver(EGAVGA_driver);
  initgraph(&gdriver,&gmode,"");
  X_max=getmaxx();
  Y_max=getmaxy();         /* 鼠标活动范围最大值 */
}



int MouseInit(int Xp0,int Xp1,int Yp0,int Yp1)    /* 初始化鼠标 */
{    /* 这里的参数是鼠标活动范围的左上角坐标和右下角坐标 */
  int retcode;
  regs.x.ax=0;
  int86(0x33,&regs,&regs);
retcode=regs.x.ax;
if(retcode==0) return 0;
   regs.x.ax=7;
regs.x.cx=Xp0;
regs.x.dx=Xp1;
int86(0x33,&regs,&regs);
regs.x.ax=3;
regs.x.cx=Yp0;
regs.x.dx=Yp1;
int86(0x33,&regs,&regs);
return retcode;
}


int MouseState(int *m_x,int *m_y,int *mstate)    /* 获取鼠标状态和位置 */
{  
   static int x0=10,y0=10,state=0;
   int xnew,ynew,ch;

   do
   {
     if(kbhit())
     {
      ch=getch();
      if(ch==13)
       { *mstate=1;
         return -1;
       }
      else return ch;
      }
    regs.x.ax=3;
    int86(0x33,&regs,&regs);
    xnew=regs.x.cx;
    ynew=regs.x.dx;
    *mstate=regs.x.bx;
    }while(xnew==x0&&ynew==y0&&*mstate==state);
    state=*mstate;
    x0=xnew;
    y0=ynew;
    *m_x=xnew;
    *m_y=ynew;
    return -1;  
}


void DrawCursor(int x,int y)    /* 在鼠标当前位置画鼠标指针 和 跟随鼠标移动的直线 */
{  int color;
   char str[50];
  line(x-6,y,x-2,y);
  line(x,y-6,x,y-3);
  line(x+2,y,x+6,y);
  line(x,y+3,x,y+6);

 if(LineDrawFlag==TRUE)
  {

  line(x_New,y_New,x,y);

  }
  color=getcolor();
  setcolor(getbkcolor());
  outtextxy(10,20,str);
  sprintf(str,"(%d,%d)",x,y);  /* 显示鼠标当前的坐标值 */
  setcolor(WHITE);
  outtextxy(10,20,str);
  setcolor(color);
}

main()
{
 int X,Y,m_state,y,a,b,i,j;


  Initgr();                        /* BGI初始化 */

  setcolor(WHITE);

 setwritemode(XOR_PUT);      /* 设定输入模式为异或模式 */
 MouseInit(0,X_max,0,Y_max);  /* 初始化鼠标 */
 a=X_max;b=Y_max;
 m_state=0;                   /* 初始化鼠标状态为移动状态 */
 DrawCursor(a,b);
 while(m_state!=2)             /* 如果没有点击右键 */
 {
   MouseState(&X,&Y,&m_state);     /* 获取鼠标当前状态与坐标值 */
   DrawCursor(a,b);                /* 通过异或输入模式删除之前的鼠标指针 */
   if(m_state==1)                   /* 如果鼠标左键点击 */
    {
       LineDrawFlag=TRUE;            /* 将跟随鼠标画线标志置为真 */
       if(0==PointNum)               /* 如果是第一次点击左键 */
         {
          x_Origin=a; y_Origin=b;
          x_Old=a;    y_Old=b;
          x_New=a;    y_New=b;
         }
         else                         /* 如果不是第一次点击鼠标左键 */
         {
          x_Old=x_New;
          y_Old=y_New;
          x_New=a;
          y_New=b;
          }


         PointNum++;               /* 记录鼠标左键点击次数,以便确定是否要跟随鼠标画线 */

         if((x_Origin-x_New) > -10 && (x_Origin-x_New) < 10 && (y_Origin-y_New) > -10
          && (y_Origin-y_New) < 10 &&  ((x_Origin-x_New)!=0 || (y_Origin-y_New)!=0))
           {                      /* 如果线画到离初始点足够近的地方 */

             LineDrawFlag=FALSE;     /* 将跟随鼠标画线标志置为假 */
             PointNum=0;              /* 鼠标点击次数清零 */
             x_New=x_Origin; y_New=y_Origin;    /* 将初始点的坐标值赋给当前点 */
           }

           setwritemode(OR_PUT);
           line(x_Old,y_Old,x_New,y_New);       /*  从前一点到当前点画线 */
           setwritemode(XOR_PUT);
    }

   DrawCursor(X,Y);   /* 在当前位置画鼠标指针 */
   a=X;
   b=Y;
}

  DrawCursor(X,Y);
  ScanlineSeedFill(X,Y,WHITE,12);

 getch();
closegraph();
}
