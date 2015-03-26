#include<dos.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#define FALSE 0
#define TRUE  1
#define NULL  0

/* //////////////////////////// */
/* 绘图方式选择标志 */
#define POINT        0
#define LINE         1
#define POLYGON      2
#define CLIPPINGAREA 3
/* /////////////////////////////////// */
/* 点相对于剪裁区的方向标志 */
#define IN_IT        0
#define LEFT_ABOVE   9
#define LEFT         1
#define LEFT_BELOW   5
#define BELOW        4
#define RIGHT_BELOW  6
#define RIGHT        2
#define RIGHT_ABOVE  10
#define ABOVE        8



union REGS regs;     /* 鼠标的变量 */
int  X_max,Y_max;     /* 鼠标活动范围最大值 */
int x_Origin, y_Origin,x_Old,y_Old,x_New,y_New;/* 鼠标点击的初始点,前一点和当前点的坐标 */
int PointNum=0;    /* 判断鼠标是否是第一次按下 */
int LineDrawFlag=FALSE;    /* 随鼠标画线标志 */
int DrawFlag=0;                 /* 定义并初始化画图方式为点 */
int Xleft,Xright,Ybottom,Ytop;   /* 保存剪裁区域的变量 */
int FillFlag=FALSE;               /* 是否填充标志 */
int k;       /* 待剪裁线段的斜率 */
int ISDrawFlag=FALSE;            /* 是否画线标志 */
struct time t;                  /* 获取系统时间的结构体 */
char C_time[50];                 /* 存放当前时间的字符数组 */
int oldtime1,oldtime2;            /* 保存前一秒和当前秒,方便判断是否做以一秒为间隔的动作 */
int msec;                        /* 设置小球移动时间间隔 */
char Run_time[50];              /* 存放程序已运行时间 */
int Secs=0,Mins=0;                /* 定义并初始化程序已经运行的时间 */
static int writecolor=1;           /* 球和欢迎词的颜色 */
int Backcolor=GREEN;               /* 作图区域外围颜色 */
int UpFlag1=FALSE,DownFlag1=TRUE;  /* 第一个小球移动方向 */
int UpFlag2=TRUE,DownFlag2=FALSE;  /* 第二个小球移动方向 */


typedef struct Point              /* 线段的端点数据结构 */
{
  int x;
  int y;
  struct Point *next;
  int DirectionFlag;

}Point;


typedef struct SEG_Of_Line           /* 线段数据结构 */
{
   int x1;
   int y1;
   int x2;
   int y2;
  struct SEG_Of_Line *next;

}SEG_Of_Line;

Point *head;

/* //////////////////////////////////////////////////////////////////////// */
/* 扫描线种子填充算法函数段 */

push(int x,int y)            /* 入栈操作 */
{
    Point *temp;
    temp=(Point *)malloc(sizeof(Point));
    temp->x=x;
    temp->y=y;
    temp->next=head;
    head=temp;

}

pop(int *x,int *y)     /* 出栈操作 */
{
  *x=head->x;
  *y=head->y;
  head=head->next;

}

void ScanlineSeedFill(int Sx,int Sy,int boundarycolor,int fillcolor)
{                           /* 扫描线种子填充 */

    int x_Now,y_Now,x0,y0,Rx,Lx,OldRx,OldLx;
    int RightFlag,LeftFlag; /* 是否到达最右非边界点标志 */
    int i;                  /* 循环控制变量 */
    push(Sx,Sy);           /* 种子点入栈 */
    setcolor(fillcolor);
    while(head!=NULL)         /* 当栈不为空时,做以下事情 */
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

    line(Lx,y_Now,Rx,y_Now);          /* 填充当前扫描线所在行 */

    OldLx=Lx;                  /* 保存最左点,以便在下一条扫描线时用 */

   y0=y_Now+1;               /* 扫描线向上扫描 */

   RightFlag=FALSE;

  while(Lx<=Rx && RightFlag==FALSE)/* 查找上一条线的最右值,并记录为Rx */
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

if(( RightFlag & LeftFlag )==TRUE &&
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

  if(( RightFlag & LeftFlag )==TRUE &&
  (getpixel(Rx,y0)!=fillcolor || getpixel(Lx,y0)!=fillcolor))
   {                         /* 如果找到了最左值和最右值，则执行下面的语句 */
       push(Rx,y0);            /* 将下一条线上的最右点作为种子点压入栈 */

       for(i=Lx;i<Rx;i++)      /* 在LX到RX间找每一像素段的最右像素作为种子压入栈 */
        if(getpixel(i,y0)==boundarycolor && getpixel(i-1,y0)!=boundarycolor)
           push(i-1,y0);

    }
 }
}
/* 扫描线种子填充算法函数段 */
/* ////////////////////////////////////////////////////////////////////////////// */


/* //////////////////////////////////////////////////////////////////////////////////// */
/* 点和线段剪裁函数段 */

SEG_Of_Line *AddSEG(SEG_Of_Line *head)               /* 加入线段链表 */
{
    SEG_Of_Line *p;
    p=(SEG_Of_Line *)malloc(sizeof(SEG_Of_Line));
    if(DrawFlag==POINT)
    {
      p->x1=x_New;
      p->y1=y_New;
      p->x2=x_New;
      p->y2=y_New;
    }
    if(DrawFlag==LINE || DrawFlag==POLYGON)
    {
       p->x1=x_Old;
       p->y1=y_Old;
       p->x2=x_New;
       p->y2=y_New;
    }
    p->next=head;
    head=p;
    return head;
}

Point *IsOnSEG(Point *p,int k)                /* 确保中点在线段上 */
{
  if( k >= 0)               /* 线段斜率大于或等于零 */
  {
    if( getpixel(p->x,p->y) == WHITE )
      {}

    else if( getpixel(p->x+1,p->y+1) == WHITE )
      { p->x++;  p->y++;  }
    else if(getpixel(p->x+1,p->y) == WHITE)
        p->x++;
     else   p->y++;
      return p;
  }
  else              /* 线段斜率小于零 */
  {
      if( getpixel(p->x,p->y) == WHITE )
      {}
      else if( getpixel(p->x+1,p->y-1) == WHITE )
         {
              p->x++; p->y--;
         }
      else if(  getpixel(p->x+1,p->y) == WHITE )
              p->x++;
         else p->y--;
         return p;
  }
}


Point *JudgeDirection(Point *q)             /* 判断线段端点相对于剪裁区域的位置 */
{
   if(q->x < Xleft && q->y > Ytop)
         q->DirectionFlag = LEFT_ABOVE;

   if(q->x < Xleft && q->y <= Ytop && q->y >= Ybottom)
         q->DirectionFlag = LEFT;

   if(q->x < Xleft && q->y < Ybottom)
         q->DirectionFlag = LEFT_BELOW;

   if(q->x >= Xleft && q->x <= Xright && q->y < Ybottom)
         q->DirectionFlag = BELOW;

   if(q->x > Xright && q->y < Ybottom)
         q->DirectionFlag = RIGHT_BELOW;

   if(q->x > Xright && q->y >= Ybottom && q->y <= Ytop)
         q->DirectionFlag = RIGHT;

   if(q->x > Xright && q->y > Ytop)
         q->DirectionFlag = RIGHT_ABOVE;

   if(q->x >= Xleft && q->x <= Xright && q->y > Ytop)
         q->DirectionFlag = ABOVE;

   if(q->x >= Xleft && q->x <= Xright && q->y >= Ybottom && q->y <= Ytop)
         q->DirectionFlag = IN_IT;

     return q;
}


void  ClipingSEG(Point *p1,Point *p2)
{

      p1=JudgeDirection(p1);               /* 给P1和P2加上方向标志 */
      p2=JudgeDirection(p2);
    if( p1->DirectionFlag == 0 )             /* 如果P1可见 */
              return;

    else  if( (p1->DirectionFlag & p2->DirectionFlag) !=0)     /* 如果P1P2 都不可见 */
         {
            setwritemode(OR_PUT);
            setcolor(getbkcolor());                            /* 删除 */
            line(p1->x,p1->y,p2->x,p2->y);
            setcolor(WHITE);
            setwritemode(XOR_PUT);
         }
         else                                         /* 如果P1不可见,但P1P2有可见部分 */
MIDPoint:
       {

         Point *pm=(Point *)malloc(sizeof(Point));
         p1=JudgeDirection(p1);
         p2=JudgeDirection(p2);
         pm->x = (p1->x + p2->x)/2;
         pm->y = (p1->y + p2->y)/2;
         k = (p2->y - p1->y)*(p2->x - p1->x);         /* 线段斜率 */
         pm = IsOnSEG(pm,k);
         pm=JudgeDirection(pm);                         /* 取P1P2中点,并给它加上方向标志 */

         if( (pm->x - p2->x)*(pm->x - p2->x) <= 1 && (pm->y - p2->y)*(pm->y - p2->y) <= 1 )

         {                                      /* 当中点与P2点足够接近时 */
            Point *p=(Point *)malloc(sizeof(Point));
            p->x = pm->x;
            p->y = pm->y;
            p->DirectionFlag = pm->DirectionFlag;
            return;/*  p;                                //返回中点 */
         }
        else if( (p1->DirectionFlag & pm->DirectionFlag) !=0)

           {
               setwritemode(OR_PUT);
               setlinestyle(0,1,3);
                setcolor(getbkcolor());
                line(p1->x,p1->y,pm->x,pm->y);

              setcolor(WHITE);
              setlinestyle(0,1,1);
              setwritemode(XOR_PUT);

              p1->x = pm->x;                         /* 如果P1PM不可见,则删除,P1移动到PM点 */
              p1->y = pm->y;
              p1->DirectionFlag = pm->DirectionFlag;
              free(pm);
              goto MIDPoint;                          /* 再求P1P2中点 */
           }

           else                                       /* 如果P1PM可见! */
           {
             p2->x = pm->x;
             p2->y = pm->y;                         /* P2移动到PM点 , 再求P1P2中点 */
             p2->DirectionFlag = pm->DirectionFlag;
             free(pm);
            goto MIDPoint;
           }
      }

}


SEG_Of_Line *Clipping(SEG_Of_Line *head)          /* 对加入线段链表的所有线段进行剪裁 */
{
    SEG_Of_Line *q;
   while(head!=NULL)                         /* 当线段链表不为空,即还有线段未剪裁 ,则做以下工作 */
   {

      Point *p1,*p2,*pold1,*pold2;
      p1=(Point *)malloc(sizeof(Point));
      p2=(Point *)malloc(sizeof(Point));
      pold1=(Point *)malloc(sizeof(Point));
      pold2=(Point *)malloc(sizeof(Point));

      q=head;
      p1->x=q->x1;
      p1->y=q->y1;
      p2->x=q->x2;
      p2->y=q->y2;

      pold1->x=p1->x;       /* 保存 */
      pold1->y=p1->y;
      pold2->x=p2->x;
      pold2->y=p2->y;

       ClipingSEG(p1,p2);
       ClipingSEG(pold2,pold1);
       head=q->next;

   }
   free(q);
   return head;
}

/* 点和线段剪裁函数段 */
/* ////////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////////// */
/* 绘制简易按钮并且给它定上标题 */

void DrawButton()
{


  /* 绘制简易按钮 */
  rectangle(0,0,639,475);
  rectangle(10,115,80,135);
  rectangle(10,145,80,165);
  rectangle(10,175,80,195);
  rectangle(10,205,80,225);
  rectangle(10,235,80,255);
  rectangle(10,265,80,285);
  rectangle(10,295,80,315);


  setwritemode(OR_PUT);
  rectangle(100,35,540,475);
  setwritemode(XOR_PUT);

  setfillstyle(1, Backcolor);
  floodfill(2,2,WHITE);
  outtextxy(544,445,"RunningTime:");
  setcolor(BLACK);
  ellipse(325,18,0,360,170,15);             /* 画出欢迎界面处的框架 */
  setfillstyle(1, BLACK);
  floodfill(320,18,BLACK);

  setfillstyle(1, BLUE);                    /* 将按钮初始化成蓝色 */
  floodfill(12,120,GREEN);
  floodfill(12,150,GREEN);
  floodfill(12,180,GREEN);
  floodfill(12,210,GREEN);
  floodfill(12,240,GREEN);
  floodfill(12,270,GREEN);
  floodfill(12,300,GREEN);

  setcolor(WHITE);

  outtextxy(28,122,"Point");                      /* 按钮名称 */
  outtextxy(30,152,"LINE");
  outtextxy(20,182,"POLYGON");
  outtextxy(15,212,"ClipArea");
  outtextxy(30,242,"Clip");
  outtextxy(20,272,"CBoard");
  outtextxy(30,302,"Exit");


}

/* /////////////////////////////////////////////////////////////////// */
/* 画棋盘 */

void DrawChessBoard()
{
  int i,j;
  setcolor(WHITE);
 for(i=100;i<540;i+=55)
     for( j=35;j<475;j+=55)
       {
         line(i,j,i+55,j);
         line(i,j,i,j+55);
       }

      line(i,j,i-440,j);
      line(i,j,i,j-440);

 setwritemode(XOR_PUT);
  for(i=155;i<540;i+=55)
     for( j=90;j<475;j+=55)
      circle(i,j,2);
}


/* ///////////////////////////////////////////////////////// */
/* 鼠标操作函数段 */

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
   static float xxx=30;                           /* 第一个小球的初始坐标 */
   static float yyy=430;                          /* 第二个小球的初始坐标 */
   do
   {
     /*
     if(kbhit())
     {
      ch=getch();
      if(ch==13)
       { *mstate=1;
         return -1;
       }
      else return ch;
      }  */
    regs.x.ax=3;
    int86(0x33,&regs,&regs);
    xnew=regs.x.cx;
    ynew=regs.x.dx;
    *mstate=regs.x.bx;

    gettime(&t);
    oldtime2=t.ti_sec;
    msec=t.ti_hund;
    if(msec%2==0)
    {

      if(DownFlag1==TRUE)                        /* 第一个小球先往下运动,到底后又向上运动,循环 */
      {
       setcolor(GREEN);
       circle(570,xxx-1,8);
       setfillstyle(1, GREEN);
       floodfill(570,xxx-1,GREEN);

       setcolor(writecolor);
       circle(570,xxx,8);
       setfillstyle(1,writecolor);
       floodfill(570,xxx,writecolor);
       xxx+=1;
       if(xxx==430)
       {
          setcolor(GREEN);
          circle(570,xxx,8);
          setfillstyle(1, GREEN);
          floodfill(570,xxx,GREEN);

           DownFlag1=FALSE;
           UpFlag1=TRUE;
        }
      }
      if(UpFlag1==TRUE)
       {
         setcolor(GREEN);
         circle(570,xxx+1,8);
         setfillstyle(1,GREEN);
         floodfill(570,xxx+1,GREEN);

       setcolor(writecolor);
       circle(570,xxx,8);
       setfillstyle(1,writecolor);
       floodfill(570,xxx,writecolor);
       xxx-=1;
       if(xxx==40)
       {
          setcolor(GREEN);
          circle(570,xxx,8);
          setfillstyle(1, GREEN);
          floodfill(570,xxx,GREEN);
           DownFlag1=TRUE;
           UpFlag1=FALSE;
        }
       }

       if(DownFlag2==TRUE)                     /* 第二个小球先往上运动,到顶后又向下运动,循环 */
      {
       setcolor(GREEN);
       circle(610,yyy-1,8);
       setfillstyle(1, GREEN);
       floodfill(610,yyy-1,GREEN);

       setcolor(writecolor);
       circle(610,yyy,8);
       setfillstyle(1, writecolor);
       floodfill(610,yyy,writecolor);
       yyy+=1;
       if(yyy==430)
       {
          setcolor(GREEN);
          circle(610,yyy,8);
          setfillstyle(1, GREEN);
          floodfill(610,yyy,GREEN);

           DownFlag2=FALSE;
           UpFlag2=TRUE;
        }
      }
      if(UpFlag2==TRUE)
       {
         setcolor(GREEN);
         circle(610,yyy+1,8);
         setfillstyle(1,GREEN);
         floodfill(610,yyy+1,GREEN);

       setcolor(writecolor);
       circle(610,yyy,8);
       setfillstyle(1,writecolor);
       floodfill(610,yyy,writecolor);
       yyy-=1;
       if(yyy==40)
       {
          setcolor(GREEN);
          circle(610,yyy,8);
          setfillstyle(1, GREEN);
          floodfill(610,yyy,GREEN);
           DownFlag2=TRUE;
           UpFlag2=FALSE;
        }
       }

       setcolor(WHITE);

     if(oldtime2!=oldtime1)
     {

      setcolor(writecolor);
      outtextxy(180,15,"Welcome to graphic precessing program!");
        writecolor++;
      while(writecolor==2||writecolor==3||writecolor==7||writecolor==8||writecolor==10)
                 writecolor++;                  /* 按秒改变欢迎词和小球颜色,并去掉几种不适合的颜色 */

       if(writecolor==15) writecolor=1;

      setcolor(Backcolor);
      outtextxy(560,15,C_time);                  /* 显示系统当前时间 */
      sprintf(C_time,"%2d:%02d:%02d",t.ti_hour, t.ti_min, t.ti_sec);
      setcolor(WHITE);
      outtextxy(560,15,C_time);
      if(oldtime1++==59) oldtime1=0;
      if(Secs++==60)
       {
        Secs=0;Mins++;
       }
      setcolor(Backcolor);                   /* 显示本程序已运行时间 */
      outtextxy(570,460,Run_time);
      sprintf(Run_time,"%02d:%02d",Mins,Secs);
      setcolor(WHITE);
      outtextxy(570,460,Run_time);
     }
    }
   }while(xnew==x0&&ynew==y0&&*mstate==state);
    state=*mstate;
    x0=xnew;
    y0=ynew;
    *m_x=xnew;
    *m_y=ynew;
    return -1;  
}


void DrawCursor(int x,int y)    /* 在鼠标当前位置画鼠标指针 和 跟随鼠标移动的直线 */
{
  int color;
  char str[50];

  line(x-6,y,x-2,y);            /* 鼠标形状 */
  line(x,y-6,x,y-3);
  line(x+2,y,x+6,y);
  line(x,y+3,x,y+6);

 if(LineDrawFlag==TRUE)
  {
    if(DrawFlag==CLIPPINGAREA)
    {

        setlinestyle(DOTTED_LINE ,1,1);     /* 随鼠标绘制剪裁区域 */
        setcolor(GREEN);
         rectangle(x_Old,y_Old,x,y);

         setlinestyle(0,1,1);
         setcolor(WHITE);

     }
    else
  line(x_New,y_New,x,y);

  }
  color=getcolor();
  setcolor(Backcolor);
  outtextxy(10,15,str);
  sprintf(str,"(%d,%d)",x,y);  /* 显示鼠标当前的坐标值 */
  setcolor(WHITE);
  outtextxy(10,15,str);
  setcolor(color);
}

/* ///////////////////////////////////////////////////////////////////////////// */
/* ///////////////////////////////////////////////////////////////////////////// */

main()
{
 int X,Y,m_state,y,a,b,i,j;
  static int fillcolor=1;  /* 棋盘填充颜色 */
  SEG_Of_Line *head;       /* 待剪裁线段的头结点 */

  head=NULL;

 Initgr();                    /* BGI初始化 */

 DrawButton();
 setwritemode(XOR_PUT);       /* 设定输入模式为异或模式 */
 MouseInit(0,X_max,0,Y_max);  /* 初始化鼠标 */
 a=X_max;b=Y_max;
 m_state=0;                   /* 初始化鼠标状态为移动状态 */
 DrawCursor(a,b);
 gettime(&t);                 /* 获取系统当前时间 */
 sprintf(C_time,"%2d:%02d:%02d",t.ti_hour, t.ti_min, t.ti_sec);
 setcolor(WHITE);
 outtextxy(560,15,C_time);
 oldtime1=t.ti_sec;                /* 前一秒值 */

 sprintf(Run_time,"%02d:%02d",Mins,Secs);
 outtextxy(544,445,"RunningTime:");
 outtextxy(570,460,Run_time);
 while(TRUE)                     /* 循环条件置为真,使程序一直检测鼠标事件 */
 {
   MouseState(&X,&Y,&m_state);     /* 获取鼠标当前状态与坐标值 */
   DrawCursor(a,b);                /* 通过异或输入模式删除之前的鼠标指针 */
   if(m_state==1)                   /* 如果鼠标左键点击 */
    {
     if(a>100&&a<540&&b>35&&b<475)                /* 如果在绘图区域点击鼠标左键 */
      {
        if(FillFlag==TRUE)
        {
         if( getpixel(a,b)==getbkcolor())
         {
           setwritemode(OR_PUT);
           ScanlineSeedFill(a,b,WHITE,fillcolor);  /* 点左键,对棋盘方格进行填充 */
           setwritemode(XOR_PUT);
           fillcolor++;                             /* 设置填充棋盘的颜色,并填充一次颜色改变一次 */
           if(fillcolor==15)  fillcolor=1;
           setcolor(writecolor);                    /* 填充后再次输出欢迎词,屏蔽因填充产生的错误 */
           outtextxy(180,15,"Welcome to graphic precessing program!");
         }
        }
       if(ISDrawFlag==TRUE)
        {
        if(DrawFlag==POINT)                   /* 如果选择画点 */
        {
          x_New=a;
          y_New=b;
          putpixel(x_New,y_New,WHITE);
          head=AddSEG(head);
        }


        if(DrawFlag==LINE)                /* 如果选择画线 */
        {


         if(0==PointNum)               /* 如果是第一次点击左键 */
         {
          LineDrawFlag=TRUE;
           x_New=a;    y_New=b;
           x_Old=a;    y_Old=b;
           PointNum++;
         }

          else
          {
          x_Old=x_New;
          y_Old=y_New;
          x_New=a;
          y_New=b;

          LineDrawFlag=FALSE;     /* 将跟随鼠标画线标志置为假 */
          PointNum=0;              /* 鼠标点击次数清零 */
          }

           setwritemode(OR_PUT);
           line(x_Old,y_Old,x_New,y_New);
           setwritemode(XOR_PUT);
           head=AddSEG(head);
        }

       if(DrawFlag==POLYGON)               /* 如果选择画多边形 */
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
           line(x_Old,y_Old,x_New,y_New);
           setwritemode(XOR_PUT);

        }

        if(DrawFlag==CLIPPINGAREA)            /* 如果选择画剪裁区域 */
        {

            if(0==PointNum)               /* 如果是第一次点击左键 */
            {
              x_Old=a;    y_Old=b;
              x_New=a;
              y_New=b;
              LineDrawFlag=TRUE;
              PointNum++;
            }
           else                         /* 如果不是第一次点击鼠标左键 */
            {
              x_Old=x_New;
              y_Old=y_New;
              x_New=a;
              y_New=b;
              LineDrawFlag=FALSE;     /* 将跟随鼠标画线标志置为假 */
              PointNum=0;
            }

           setwritemode(OR_PUT);            /* 绘制剪裁区域 */
           setlinestyle(DOTTED_LINE ,1,1);

           setcolor(GREEN);

           rectangle(x_Old,y_Old,x_New,y_New);

           setcolor(WHITE);
           setlinestyle(0,1,1);
           setwritemode(XOR_PUT);
           if(x_Old > x_New)                       /* 保存剪裁区域左上,右下两点的坐标 */
            {   Xleft=x_New;  Xright=x_Old; }
           else
            {   Xleft=x_Old;  Xright=x_New; }

           if(y_Old > y_New)
            {  Ytop=y_Old;   Ybottom=y_New;}
           else
            {  Ytop=y_New;   Ybottom=y_Old;}

      }
      }

    }

     if(a>10&&a<80&&b>115&&b<135)         /* 点击画点选择按钮 */
        {
           if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);    /* 让欢迎词和其它画面同时出来,消除一秒后才显示的误差,下同 */
               outtextxy(180,15,"Welcome to graphic precessing program!");

              }

               setfillstyle(1, BLUE);              /* 设置当前选中按钮颜色为红,其它为蓝,下同 */
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,120,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");

           ISDrawFlag=TRUE;
           DrawFlag=POINT;
         }
     if(a>10&&a<80&&b>145&&b<165)          /* 点击画线选择按钮 */
        {
           if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);
               outtextxy(180,15,"Welcome to graphic precessing program!");
              }

              setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,150,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
           ISDrawFlag=TRUE;
           DrawFlag=LINE;
         }
     if(a>10&&a<80&&b>175&&b<195)            /* 点击画多边形选择按钮 */
       {
         if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);
               outtextxy(180,15,"Welcome to graphic precessing program!");
              }

              setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,180,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
         ISDrawFlag=TRUE;
         DrawFlag=POLYGON;
       }
     if(a>10&&a<80&&b>205&&b<225)              /* 点击画剪裁区域选择按钮 */
       {
         if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);
               outtextxy(180,15,"Welcome to graphic precessing program!");
              }

              setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,210,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
         ISDrawFlag=TRUE;
         DrawFlag=CLIPPINGAREA;
       }
     if(a>10&&a<80&&b>235&&b<255)                /* 点击剪裁按钮 */
       {
          head=Clipping(head);
          setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,240,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
       }
     if(a>10&&a<80&&b>265&&b<285)                 /* 点击棋盘按钮 */
     {
       setbkcolor(BLACK);
       cleardevice();
       ISDrawFlag=FALSE;
       DrawChessBoard();
       DrawButton();
       LineDrawFlag=FALSE;
       PointNum=0;
       head=NULL;
       setcolor(writecolor);
       outtextxy(180,15,"Welcome to graphic precessing program!");
       FillFlag=TRUE;
       setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,270,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
     }
     if(a>10&&a<80&&b>295&&b<315)                 /* 点击退出按钮 */
       {
          setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,300,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
          setcolor(BLACK);
          outtextxy(180,15,"Welcome to graphic precessing program!");   /* 消除欢迎词 */
          setcolor(RED);
          outtextxy(240,15,"The program is exiting...");
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          closegraph();
        }
   }

   if(m_state==2&&DrawFlag==POLYGON&&a>100&&a<540&&b>35&&b<475)
    {
      setwritemode(OR_PUT);                      /* 在多边形填充模式下,点右键进行填充 */
      ScanlineSeedFill(a,b,WHITE,fillcolor);
      setwritemode(XOR_PUT);

    }
   setcolor(WHITE);
   DrawCursor(X,Y);   /* 在当前位置画鼠标指针 */
   a=X;
   b=Y;
}

}
