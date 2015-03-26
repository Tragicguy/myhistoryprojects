#include"stdio.h";
#include"graphics.h"
#include"math.h"
#include"dos.h"
#include"time.h"

/********************/
/*                  */
/*     二维图形     */
/*                  */
/********************/
typedef struct Point      /*  点 */
{
   int x;
   int y;
}Point;

typedef struct Line       /*  线 */
{
    Point  A;/*起点*/
    Point  B;/*终点*/
    int color; /*颜色*/
}Line;



typedef struct Yuan       /*  圆 */
{
   Point  o; /*圆心*/
   int r;    /*半径*/
   int color;/*颜色*/
}Yuan;

typedef struct TuoYuan    /*椭 圆*/
{
   Point  o;/*中心*/
   int a;   /*长轴*/
   int b;   /*短轴*/
   int color;/*颜色*/
}TuoYuan;

typedef struct Qiu        /* 球 */
{
   Point  o;/*中心*/
   int r;
   int color; /*颜色*/
}Qiu;

typedef struct Rect       /* 矩形*/
{
  Point LeftTop;
  Point RightDown;
  int color;
}Rect;


/********************************/
/****     二维基本变换     ******/
/********************************/

/* 点的平移变换 */
void PinYi(int *x,int *y,int tx,int ty)
{
  *x = *x + tx;
  *y = *y + ty;

}



/* 点的旋转变换 */

void XuanZhuan(int *x,int *y,float q)
{
  float m ;
  float n;
  float Q;
  Q = (3.1415926/180)*q;
  m = (*x);
  n = (*y);
  *x = m * cos(Q) - n * sin(Q);
  *y = m * sin(Q) + n * cos(Q);
}

/* 绕任意点的旋转变换 */

void XuanZhuan_RY(int m,int n,int *x,int *y,float q)/* 绕(m,n)的旋转*/
{

  PinYi(x,y,-m,-n);
    XuanZhuan(x,y,q);
  PinYi(x,y,m,n);
}

/* 点的放缩变换 */
void FangSuo(int *x,int *y,float sx,float sy)
{
   *x =*x * sx;
   *y =*y * sy;
}

/* 以任意点的放缩变换 */
void FangSuo_RY(int m,int n,int *x,int *y,float sx,float sy) /*以(m,n)为参考点*/
{
   PinYi(x,y,-m,-n);
     FangSuo(x,y,sx,sy);
   PinYi(x,y,m,n);
}



/* 点的错切变换 */
void CuoQie(int *x,int *y,float q,int flag)
{
  float Q;
  Q = (3.1415926/180) * q;
  if(flag==0)/* y为依赖轴 */
  {
    *x = (float)*x + tan(Q)*((float)(*y));
    *y = *y;
  }
  else
  {
    *y = (float)*y + tan(Q)*((float)(*x));
    *x = *x;
  }

}

/* 以任意点的错切变换 */
void CuoQie_RY(int m,int n,int * x,int * y,float q,int flag) /* 以为(m,n)参考点*/
{

    PinYi(x,y,-m,-n);
    CuoQie(x,y,q,flag);
    PinYi(x,y,m,n);

}



/***********************/
/*                     */
/*      三维图形       */
/*                     */
/***********************/


typedef struct Point3D      /*  点 */
{
   int x;
   int y;
   int z;
}Point3D;

typedef struct Rect3D      /*  矩形面 */
{
   Point3D A;
   int a;
   int b;
   int color;
}Rect3D;


typedef struct CFT        /*长方体*/
{
  Point3D A;/*正方体的中心*/
  int a;    /*    长    */
  int b;    /*    宽    */
  int h;    /*    高    */
}CFT;

typedef struct SLZ        /*四棱锥*/
{
  Point3D A;/*四棱锥底面的中心*/
  int r;    /*    底面边长    */
  int h;    /*    高　　　　　*/
}SLZ;

typedef struct SLZU        /*三棱柱*/
{
  Point3D A;/*三棱锥底面的中心*/
  int r;    /*    底面边长    */
  int h;    /*    高　　　　　*/
}SLZU;

typedef struct depth       /*三维实体深度信息*/
{
  char s;  /*实体名称*/
  int  d;   /*实体深度*/
}Depth;


/********************************/
/****                      ******/
/****     三维基本变换     ******/
/****                      ******/
/********************************/


/* 点的平移变换 */
void PinYi3D(int * x,int * y,int * z,int tx,int ty,int tz)
{
  *x = *x + tx;
  *y = *y + ty;
  *z = *z + tz;

}



/* 点旋转变换 */

void XuanZhuan3D(int * x,int * y,int *z,float q,int flag)
{
  int m,n,t;
  float Q;
  m = (*x);
  n = (*y);
  t = (*z);
  Q = (3.1415926/180)*q;
  if(flag==1)   /* 绕x轴旋转*/
  {
    *x = *x;
    *y = (float)n * cos(Q) - (float)t * sin(Q);
    *z = (float)n * sin(Q) + (float)t * cos(Q);
   }
   if(flag==2)  /* 绕y轴旋转*/
   {
     *x = (float)m * cos(Q) + (float)t * sin(Q);
     *y = *y;
     *z = (float)(-m) * sin(Q) + (float)t * cos(Q);
   }
   if(flag==3)  /* 绕z轴旋转*/
   {
      *x = (float)m * cos(Q) - (float)n * sin(Q);
      *y = (float)m * sin(Q) + (float)n * cos(Q);
      *z = *z;
   }

}

/* 绕任意轴的点的旋转变换 */
void XuanZhuan3D_RY(int * x,int * y,int *z,float q,int i,int j,int k,int flag)
{

  /* if(flag==1)   /* 绕平行于X轴且过点(i,j,k)的直线的旋转*/ */
 /*  { */
  /*   PinYi3D(x,y,z,-i,-j,-k); */
 /*       XuanZhuan3D(x,y,z,q,flag); */
  /*   PinYi3D(x,y,z,i,j,k); */
  /*  } */
  /*  if(flag==2)  /* 绕平行于Y轴且过点(i,j,k)的直线的旋转*/ */
  /*  { */
  /*    PinYi3D(x,y,z,-i,-j,-k); */
  /*      XuanZhuan3D(x,y,z,q,flag); */
  /*    PinYi3D(x,y,z,i,j,k); */
  /*  } */
  /*  if(flag==3)  /* 绕平行于Z轴且过点(i,j,k)的直线的旋转*/ */
  /*  { */
   /*   PinYi3D(x,y,z,-i,-j,-k); */
   /*     XuanZhuan3D(x,y,z,q,flag); */
   /*  PinYi3D(x,y,z,i,j,k); */
  /* } */

    /* 这里可以修改 */

       PinYi3D(x,y,z,-i,-j,-k);
       XuanZhuan3D(x,y,z,q,flag);
       PinYi3D(x,y,z,i,j,k);


}

/* 点放缩变换 */
void FangSuo3D(int * x,int * y,int * z,float sx,float sy,float sz)
{
   *x =*x * sx;
   *y =*y * sy;
   *z =*z * sz;
}

/* 以任意点的放缩变换 */
void FangSuo3D_RY(int m,int n,int t,int * x,int * y,int *z,float sx,float sy,float sz) /*以(m,n,t)为参考点*/
{
   PinYi3D(x,y,z,-m,-n,-t);
   FangSuo3D(x,y,z,sx,sy,sz);
   PinYi3D(x,y,z,m,n,t);
}


/************************/
/*                      */
/* 三维坐标系的平移变换 */
/*                      */
/************************/

void PingYi3D_ZBX(int * x,int * y,int *z,int tx,int ty,int tz)
{
   *x = *x - tx;
   *y = *y - ty;
   *z = *z - tz;
}


/************************/
/*                      */
/* 三维坐标系的旋转变换 */
/*                      */
/************************/

void XuanZhuan3D_ZBX(int * x,int * y,int *z,float q,int flag)
{
  /* int m,n,t; */
  /* float Q; */
 /*  m = (*x); */
 /*  n = (*y); */
  /* t = (*z); */
 /*  Q = (3.1415926/180 )*q; */

   XuanZhuan3D(x,y,z,-q,flag);


  /* if(flag==1)   /* x,y轴的旋转*/ */
  /* {                                 / */
  /*   *x = (float)m * cos(Q) + (float)n * sin(Q); */
  /*   *y = (float)(-m) * sin(Q) + (float)n * cos(Q); */
  /*   *z = *z; */
  /*  } */
  /*  if(flag==2)  /* y,z的轴旋转*/ */
  /*  { */
  /*    *x = *x; */
  /*    *y = (float)n * cos(Q) + (float)t * sin(Q); */
  /*    *z = (float)(-n) * sin(Q) + (float)t * cos(Q); */
  /*  } */
  /*  if(flag==3)  /* z,x的轴旋转*/ */
  /*  { */
  /*     *x = (float)m * cos(Q) - (float)t * sin(Q); */
  /*     *y = *y; */
  /*     *z = (float)m * sin(Q) + (float)t * cos(Q); */
  /*  } */

}


/********************************/
/*                              */
/* 三维空间点到二维平面点的投影 */
/*                              */
/********************************/
void Point3D_2D(int *x,int *y,int *z,Point3D B,float qx,float qy,float qz)/*投影方向为(qx,qy,qz)*/
{
      int t,i,j,k;
      i = B.x; j = B.y; k = B.z;


      FangSuo3D_RY(i,j,k,x,y,z,0.4,0.4,0.4);     /* 相对于参考点B进行比例放缩 */

      PingYi3D_ZBX(x,y,z,i,j,k);                 /* 坐标系平移 */
      XuanZhuan3D(x,y,z,-qx,3);                  /*  XuanZhuan3D_ZBX(x,y,z,qx,3);//绕Z轴旋转 */
      XuanZhuan3D(x,y,z,-qz,2);                  /*  XuanZhuan3D_ZBX(x,y,z,qz,2);//绕Y轴旋转 */
      PingYi3D_ZBX(x,y,z,-i,-j,-k);

      t = *x; *x = *y; *y = t;                   /* 3D到2D转换时的XY坐标轴变换 */

}


/***********************/
/*   直线的DDA 算法    */
/***********************/

void LineDDA(Point A,Point B,int color)
{
   int x0,y0,x1,y1,x;
   float dy,dx,y,m;

   if(A.x<B.x)
   {
      x0 = A.x;
      y0 = A.y;
      x1 = B.x;
      y1 = B.y;

      dx = x1 - x0;
      dy = y1 - y0;
      m = dy/dx;
      y = y0;
      for(x = x0;x <= x1;x++)
      {
        putpixel(x,(int)(y + 0.5),color);
        y = y + m;
      }
   }

  if(A.x>B.x)
  {
      x0 = B.x;
      y0 = B.y;
      x1 = A.x;
      y1 = A.y;

      dx = x1 - x0;
      dy = y1 - y0;
      m = dy/dx;
      y = y0;
      for(x = x0;x <= x1;x++)
      {
        putpixel(x,(int)(y + 0.5),color);
        y = y + m;
      }
  }
  if(A.x==B.x)
  {
    if(A.y<=B.y)
     {
        x0 = A.x;
        y0 = A.y;
        while(y0<=B.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
     else
     {
        x0 = B.x;
        y0 = B.y;
        while(y0<=A.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
  }

}

/***********************/
/*   直线的中点算法    */
/***********************/

void MidPointLine(Point A,Point B,int color)
{
  int dx,dy,incrE,incrNE,d,x,y,x0,y0,x1,y1;

   if(A.x<B.x)
   {
    if((float)(B.y - A.y)/(B.x - A.x)<1.0)
    {
      x0 = A.x;
      y0 = A.y;
      x1 = B.x;
      y1 = B.y;

      dx = x1 - x0;
      dy = y1 - y0;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(x,y,color);
      while(x <= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y++;
       
          }
        x++;
       putpixel(x,y,color);
       }
      }
     if((float)(B.y - A.y)/(B.x - A.x)>1.0)
     {
        x0 =A.y;
        y0 =A.x;
        x1 =B.y;
        y1 =B.x;

      dx = x1 - x0;
      dy = y1 - y0;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(y,x,color);
      while(x <= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y++;
       
          }
        x++;
       putpixel(y,x,color);
       }

     }
   }

  if(A.x>B.x)
  {
      if((float)(B.y - A.y)/(A.x - B.x)<1.0)
    {
      x0 = A.x;
      y0 = A.y;
      x1 = B.x;
      y1 = B.y;

      dx = x0 - x1;
      dy = y1 - y0;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(x,y,color);
      while(x >= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y++;
       
          }
        x--;
       putpixel(x,y,color);
       }
      }
     if((float)(B.y - A.y)/(A.x - B.x)>1.0)
     {
        x0 =A.y;
        y0 =A.x;
        x1 =B.y;
        y1 =B.x;

      dx = x1 - x0;
      dy = y0 - y1;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(y,x,color);
      while(x <= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y--;
       
          }
        x++;
       putpixel(y,x,color);
       }

     }
  }
  if(A.x==B.x)
  {
    if(A.y<=B.y)
     {
        x0 = A.x;
        y0 = A.y;
        while(y0<=B.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
     else
     {
        x0 = B.x;
        y0 = B.y;
        while(y0<=A.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
  }


}



/*********************/
/*   圆的中点算法    */
/*********************/


/* 圆的8对称点的显示 */
void CirclePoints(int x0,int y0,int x,int y,int color)/* 圆心为（x0,y0);*/
{
  putpixel(x,y,color);                 /*(x,  y) */
  putpixel(x0-y0+y,y0-x0+x,color);     /*(y,  x) */
  putpixel(x0+y0-y,y0-x0+x,color);     /*(-y, x) */
  putpixel(2*x0-x,y,color);            /*(-x, y) */
  putpixel(x0-y0+y,y0+x0-x,color);     /*(y, -x) */
  putpixel(x,2*y0-y,color);            /*(x, -y) */
  putpixel(2*x0-x,2*y0-y,color);       /*(-x,-y) */
  putpixel(x0+y0-y,y0+x0-x,color);     /*(-y,-x) */
}


/* 算法 1：圆的中点算法 */
void MidPointCircle_1(Point Q,int radius,int color)/* 圆心为 Q */
{
  int x,y,x0,y0;
  float d;

  x0 = Q.x;
  y0 = Q.y;

  x = x0;
  y = y0+radius;
  d = 5.0/4.0 - radius;
  CirclePoints(x0,y0,x,y,color);
  while((y-y0)>(x-x0))
  {
    if(d<=0)
      d = d + 2.0*(x-x0) + 3;
    else
      {
        d = d + 2.0*((x-x0)-(y-y0)) + 5;
        y--;
      }
    x++;
    CirclePoints(x0,y0,x,y,color);
  }

}

/* 算法 2 ：消除了浮点运算的中点算法 */

void MidPointCircle_2(Point Q,int radius,int color)/* 圆心为（x0,y0);*/
{
  int x,y,d,x0,y0;

  x0 = Q.x;
  y0 = Q.y;

  x = x0;
  y = y0 + radius;
  d = 5 - 4*radius;
  CirclePoints(x0,y0,x,y,color);
  while((y-y0)>(x-x0))
  {
   if(d<=0)
      d = d + 8*(x-x0) + 12;
    else
      {
        d = d + 8*((x-x0)-(y-y0)) + 20;
        y--;
      }
    x++;
    CirclePoints(x0,y0,x,y,color);
  }
}

/* 算法 3 ：消除了乘法运算的中点算法 */
void MidPointCircle_3(Point Q,int radius,int color)/* 圆心为（x0,y0);*/
{
   int x,y,d,deltaE,deltaSE,x0,y0;

   x0 = Q.x;
   y0 = Q.y;

   x = x0;
   y = y0 + radius;
   d = 5 - 4*radius;
   deltaE = 12;
   deltaSE = 20 - 8*radius;
   CirclePoints(x0,y0,x,y,color);
   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints(x0,y0,x,y,color);
   }

}



/***********************/
/*   椭圆的中点算法    */
/***********************/


/* 椭圆的4对称点的显示 */
void EllipsePoints(int x0,int y0,int x,int y,int color) /*椭圆的中心为:(x0,y0)*/
{
  putpixel(x,y,color);              /*(x,  y) */
  putpixel(2*x0-x,y,color);         /*(-x, y) */
  putpixel(2*x0-x,2*y0-y,color);    /*(-x,-y) */
  putpixel(x,2*y0-y,color);         /*(x, -y) */
}


/* 椭圆的中点算法 */
void MidPointEllipse(Point Q,int a,int b,int color)
{
  int x,y,xP,yP,x0,y0;
  float d,squrarea,squrareb;
   x0 = Q.x;
   y0 = Q.y;


  squrarea = a * a;
  squrareb = b * b;

  /*计算分解点P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* 生成上半部椭圆*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;
  EllipsePoints(x0,y0,x,y,color);
  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;
    EllipsePoints(x0,y0,x,y,color);
  }

  /* 生成下半部椭圆*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;
  EllipsePoints(x0,y0,x,y,color);
  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;
   EllipsePoints(x0,y0,x,y,color);
  }
}


/***********************/
/*     矩形的算法      */
/***********************/
void Rectangle(Point LeftTop,Point RightDown,int color)
{
  int x1,y1,x2,y2,x,y;
  x1 = LeftTop.x;
  x2 = RightDown.x;
  y1 = LeftTop.y;
  y2 = RightDown.y;
  x = x1;
  while(x<=x2)
  {
    putpixel(x,y1,color);
    putpixel(x,y2,color);
    x++;
  }

  y = y1;
  while(y<=y2)
  {
     putpixel(x1,y,color);
     putpixel(x2,y,color);
     y++;
  }

}


/*************************/
/******   网格球     *****/
/*************************/

void QiuEC(Point o,int r,int color) /*  球心为o(x,y) */
{
  float i,X,Y,ra,rb;
  int k,x,y;
  x = o.x;
  y = o.y;
  MidPointEllipse(o,r,15,color);
  MidPointCircle_1(o,r,color);
  for(i=1;i<=(r/5);i++)
  {

     MidPointEllipse(o,5*i,r,color);

  }
  k = (int)(r/4);
  for(i=1;i<4;i++)
  {
     o.y = y-(4-i)*k;
     MidPointEllipse(o,sqrt(2*r*(k*i)-k*i*k*i),i*4,color);
     o.y = y+(4-i)*k;
     MidPointEllipse(o,sqrt(2*r*(k*i)-k*i*k*i),i*4,color);
  }
}





/*****************************/
/**                         **/
/**     矩形的旋转          **/
/**                         **/
/*****************************/
void Rectangle_XuanZhuan(Rect R,float q)
{
  int x1,y1,x2,y2,x,y,color,X,Y;

  color = R.color;

  x1 = R.LeftTop.x;
  y1 = R.LeftTop.y;

  x2 = R.RightDown.x;
  y2 = R.RightDown.y;

  x = x1;
  while(x<=x2)
  {
    X = x;
    Y = y1;
    XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);

    X = x;
    Y = y2;
    XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);

    x++;
  }

  y = y1;
  while(y<=y2)
  {
     X = x1;
     Y = y;
     XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);

     X = x2;
     Y = y;
     XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);
     y++;
  }

}




/******************************/
/*                            */
/*       椭圆的平移           */
/*                            */
/******************************/

/*平移后椭圆的4对称点的显示*/
void EllipsePoints_PinYi(int x0,int y0,int x,int y,int color,int tx,int ty) /*椭圆的中心为:(x0,y0)*/
{
  int X,Y;
  X = x;
  Y = y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);      /*(x,  y)*/

  X = 2*x0-x;
  Y = y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);      /*(-x, y)*/

  X = 2*x0-x;
  Y = 2*y0-y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);      /*(-x,-y)*/

  X = x;
  Y = 2*y0-y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);       /*(x, -y)*/
}

/* 椭圆的平移*/

void MidPointEllipse_PinYi(TuoYuan E,int tx,int ty)
{
  int x,y,xP,yP,x0,y0,a,b,color;
  float d,squrarea,squrareb;

   a = E.a;
   b = E.b;
   color = E.color;
   x0 = E.o.x;
   y0 = E.o.y;


  squrarea = a * a;
  squrareb = b * b;

  /*计算分解点P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* 生成上半部椭圆*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;

  EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;

    EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  }

  /* 生成下半部椭圆*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;

  EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;

   EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  }
}

/******************************/
/*                            */
/*       椭圆的旋转           */
/*                            */
/******************************/

/* 椭圆旋转后的4对称点的显示 */
void EllipsePoints_XuanZhuan(int m,int n,int x0,int y0,int x,int y,int color,float q) /*椭圆的中心为:(x0,y0),绕点(m,n)旋转*/
{
  int X,Y;
  X = x;
  Y = y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);               /*(x,  y) */


  X = 2*x0-x;
  Y = y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);              /*(-x, y) */


  X = 2*x0-x;
  Y = 2*y0-y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);             /*(-x,-y) */

  X = x;
  Y = 2*y0-y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);             /*(x, -y) */

}

/*        椭圆的旋转    */

MidPointEllipse_XuanZhuan(TuoYuan E,Point A,float q) /*绕A点旋转*/
{
  int x,y,xP,yP,x0,y0,a,b,color,m,n;
  float  d,squrarea,squrareb;
  m = A.x;
  n = A.y;
  x0 = E.o.x;
  y0 = E.o.y;
  a = E.a;
  b = E.b;
  color  = E.color;


  squrarea = a * a;
  squrareb = b * b;

  /*计算分解点P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* 生成上半部椭圆*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;

  EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);

  while((x - x0)<=xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;

  EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);

  }
  /* 生成下半部椭圆*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;

 EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);

  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;

  EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);
  }
}


/******************************/
/*                            */
/*       椭圆的放缩           */
/*                            */
/******************************/

/* 椭圆放缩后的4对称点的显示 */
void EllipsePoints_FangSuo(int m,int n,int x0,int y0,int x,int y,int color,float sx,float sy) /*椭圆的中心为:(x0,y0),以(m,n)为参考点放缩*/
{
  int X,Y;
  X = x;
  Y = y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);               /*(x,  y) */


  X = 2*x0-x;
  Y = y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);              /*(-x, y) */


  X = 2*x0-x;
  Y = 2*y0-y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);             /*(-x,-y) */

  X = x;
  Y = 2*y0-y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);             /*(x, -y) */

}

/*        椭圆的放缩     */

void MidPointEllipse_FangSuo(TuoYuan E,Point A,float sx,float sy) /*以A点为参考点放缩*/
{
  int x,y,xP,yP,x0,y0,a,b,color,m,n;
  float  d,squrarea,squrareb;
  m = A.x;
  n = A.y;
  x0 = E.o.x;
  y0 = E.o.y;
  a = E.a;
  b = E.b;
  color  = E.color;


  squrarea = a * a;
  squrareb = b * b;

  /*计算分解点P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* 生成上半部椭圆*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;

  EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);

  while((x - x0)<=xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;

  EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);

  }
  /* 生成下半部椭圆*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;

 EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);

  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;

  EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);
  }
}


/************************/
/**                    **/
/**   圆的错切变换     **/
/**                    **/
/************************/


/* 错切后圆的8对称点的显示 */
void CirclePoints_CuoQie(int m,int n,int x0,int y0,int x,int y,int color,float q,int flag)/* 圆心为（x0,y0);以(m,n)为参考点*/
{
  int X,Y;
  X = x;
  Y = y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);                /*(x,  y) */

  X = x0-y0+y;
  Y = y0-x0+x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);               /*(y,  x) */

  X = x0+y0-y;
  Y = y0-x0+x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);              /*(-y, x) */

  X = 2*x0-x;
  Y = y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);              /*(-x, y) */

  X = x0-y0+y;
  Y = y0+x0-x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);              /*(y, -x) */

  X = x;
  Y = 2*y0-y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);               /*(x, -y) */

  X =2*x0-x;
  Y =2*y0-y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);               /*(-x,-y) */

  X =x0+y0-y;
  Y =y0+x0-x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);               /*(-y,-x) */
}

/*  圆的错切 */
void MidPointCircle_CuoQie(Yuan C,Point A,float q,int flag)/* 圆心为（x0,y0);*/
{
   int x,y,d,deltaE,deltaSE,x0,y0,radius,color,m,n;
   m = A.x;
   n = A.y;
   x0 = C.o.x;
   y0 = C.o.y;
   radius = C.r;
   color = C.color;

   x = x0;
   y = y0 + radius;
   d = 5 - 4*radius;
   deltaE = 12;
   deltaSE = 20 - 8*radius;

     CirclePoints_CuoQie(m,n,x0,y0,x,y,color,q,flag);
   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_CuoQie(m,n,x0,y0,x,y,color,q,flag);
   }

}


/***************************/
/**                       **/
/**     3D空间矩形面      **/
/**    平行于X-Y平面      **/
/***************************/
void Rect_3DXY(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   Point3D B1,B2,B3,B4;

   B1.x = A.x + b/2.0; B1.y = A.y - a/2.0; B1.z = A.z;
   B2.x = A.x + b/2.0; B2.y = A.y + a/2.0; B2.z = A.z;
   B3.x = A.x - b/2.0; B3.y = A.y + a/2.0; B3.z = A.z;
   B4.x = A.x - b/2.0; B4.y = A.y - a/2.0; B4.z = A.z;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; j = Y;
   while(j <= B2.y)
   {
      while(i >= B4.x)
      {
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

         i--;
         X = i; Y = j; Z = B1.z;
      }
      j++;
      X = B1.x; Y = j; Z = B1.z;
      i = X;
   }

}


/***************************/
/**                       **/
/**     3D空间矩形面      **/
/**     平行于X-Z平面     **/
/***************************/
void Rect_3DXZ(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   Point3D B1,B2,B3,B4;

   B1.x = A.x + a/2.0; B1.y = A.y; B1.z = A.z - b/2.0;
   B2.x = A.x - a/2.0; B2.y = A.y; B2.z = A.z - b/2.0;
   B3.x = A.x - a/2.0; B3.y = A.y; B3.z = A.z + b/2.0;
   B4.x = A.x + a/2.0; B4.y = A.y; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; k = Z;
   while(i >= B2.x)
   {
      while(k <= B4.z)
      {
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

         k++;
         X = i; Y =A.y; Z = k;
      }
      i--;
      X = i; Y = A.y; Z = B1.z; k = Z;
   }

}

/***************************/
/**                       **/
/**     3D空间矩形面      **/
/**     平行于Y-Z平面     **/
/***************************/
void Rect_3DYZ(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   Point3D B1,B2,B3,B4;

   B1.x = A.x; B1.y = A.y - a/2.0; B1.z = A.z - b/2.0;
   B2.x = A.x; B2.y = A.y + a/2.0; B2.z = A.z - b/2.0;
   B3.x = A.x; B3.y = A.y + a/2.0; B3.z = A.z + b/2.0;
   B4.x = A.x; B4.y = A.y - a/2.0; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   j = Y; k = Z;
   while(j <= B2.y)
   {
      while(k <= B4.z)
      {
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

         k++;
         X = A.x; Y =j; Z = k;
      }
      j++;
      X = A.x; Y = j; Z = B1.z; k = Z;
   }

}

/***************************/
/**                       **/
/**  3D空间矩形面的旋转   **/
/**    平行于X-Y平面      **/
/***************************/
void Rect_3DXY_XuanZhuan(Point3D A,int a,int b,int color,int q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   int ii,jj,kk;
   Point3D B1,B2,B3,B4;

   ii = D.x; jj = D.y; kk = D.z;

   B1.x = A.x + b/2.0; B1.y = A.y - a/2.0; B1.z = A.z;
   B2.x = A.x + b/2.0; B2.y = A.y + a/2.0; B2.z = A.z;
   B3.x = A.x - b/2.0; B3.y = A.y + a/2.0; B3.z = A.z;
   B4.x = A.x - b/2.0; B4.y = A.y - a/2.0; B4.z = A.z;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; j = Y;
   while(j <= B2.y)
   {
      while(i >= B4.x)
      {
         XuanZhuan3D_RY(&X,&Y,&Z,q,ii,jj,kk,flag);
            Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
              putpixel(X,Y,color);

         i--;
         X = i; Y = j; Z = B1.z;
      }
      j++;
      X = B1.x; Y = j; Z = B1.z; i = X;
   }

}

/***************************/
/**                       **/
/**  3D空间矩形面的旋转   **/
/**    平行于X-Z平面      **/
/***************************/
void Rect_3DXZ_XuanZhuan(Point3D A,int a,int b,int color,int q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   int ii,jj,kk;
   Point3D B1,B2,B3,B4;

   ii = D.x; jj = D.y; kk = D.z;

   B1.x = A.x + a/2.0; B1.y = A.y; B1.z = A.z - b/2.0;
   B2.x = A.x - a/2.0; B2.y = A.y; B2.z = A.z - b/2.0;
   B3.x = A.x - a/2.0; B3.y = A.y; B3.z = A.z + b/2.0;
   B4.x = A.x + a/2.0; B4.y = A.y; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; k = Z;
   while(i >= B2.x)
   {
      while(k <= B4.z)
      {
         XuanZhuan3D_RY(&X,&Y,&Z,q,ii,jj,kk,flag);
            Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
              putpixel(X,Y,color);

         k++;
         X = i; Y =A.y; Z = k;
      }
      i--;
      X = i; Y = A.y; Z = B1.z; k = Z;
   }

}
/***************************/
/**                       **/
/**  3D空间矩形面的旋转   **/
/**    平行于Y-Z平面      **/
/***************************/
void Rect_3DYZ_XuanZhuan(Point3D A,int a,int b,int color,int q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   int ii,jj,kk;
   Point3D B1,B2,B3,B4;

   ii = D.x; jj = D.y; kk = D.z;

   B1.x = A.x; B1.y = A.y - a/2.0; B1.z = A.z - b/2.0;
   B2.x = A.x; B2.y = A.y + a/2.0; B2.z = A.z - b/2.0;
   B3.x = A.x; B3.y = A.y + a/2.0; B3.z = A.z + b/2.0;
   B4.x = A.x; B4.y = A.y - a/2.0; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   j = Y; k = Z;
   while(j <= B2.y)
   {
      while(k <= B4.z)
      {
         XuanZhuan3D_RY(&X,&Y,&Z,q,ii,jj,kk,flag);
            Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
              putpixel(X,Y,color);
         k++;
         X = A.x; Y =j; Z = k;
      }
      j++;
      X = A.x; Y = j; Z = B1.z; k = Z;
   }

}

/***************************/
/**                       **/
/**     3D空间三角形      **/
/**    平行于X-Y平面      **/
/***************************/
void SanJiaoxing_3DXY(Point3D A,int a,int h,int color,Point3D B,float qx,float qy,float qz)
{
  int X,Y,Z,i,j,k;
  Point3D B1,B2,B3;
  Point C1,C2,C3,T;

  B1.x = A.x + h/2.0; B1.y = A.y - a/2.0; B1.z = A.z;
  B2.x = A.x + h/2.0; B2.y = A.y + a/2.0; B1.z = A.z;
  B1.x = A.x - h/2.0; B1.y = A.y; B1.z = A.z;

  X = B1.x; Y = B1.y; Z = B1.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  C1.x = X; C1.y = Y;

  X = B2.x; Y = B2.y; Z = B2.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  C2.x = X; C2.y = Y;

  X = B3.x; Y = B3.y; Z = B3.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  C3.x = X; C3.y = Y;

  X = B1.x; Y = B1.y; Z = B1.z;
  j = B1.y; i = B1.x;
  while(j <= B2.y)
  {
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
     T.x = X; T.y = Y;
     LineDDA(C3,T,color);
     j++;
  }


}

/***************************/
/**                       **/
/**      3D空间圆         **/
/**     平行于X-Y平面     **/
/***************************/

/* 3D圆的8对称点的显示 */
void CirclePoints_3DXY(Point3D A,int x,int y,int color,Point3D B,float qx,float qy,float qz)/* 圆心为A;*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                /*(x,  y) */
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);
  X = A.x-A.y+y; Y = A.y-A.x+x; Z = A.z;/*(y,  x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);
  X = A.x+A.y-y; Y = A.y-A.x+x; Z = A.z; /*(-y, x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2*A.x-x; Y = y; Z = A.z;          /*(-x, y) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x-A.y+y; Y = A.y+A.x-x; Z = A.z; /*(y, -x) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);
  X = x; Y = 2*A.y-y; Z = A.z;          /*(x, -y) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2*A.x-x; Y = 2*A.y-y; Z = A.z;    /*(-x,-y) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x+A.y-y; Y = A.y+A.x-x; Z = A.z;/*(-y,-x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

}

void Yuan_3DXY(Point3D A,int r,int color,Point3D B,float qx,float qy,float qz)
{

   int x,y,d,deltaE,deltaSE,x0,y0,X,Y,Z;

   x0 = A.x;
   y0 = A.y;

   x = x0;
   y = y0 + r;
   d = 5 - 4*r;
   deltaE = 12;
   deltaSE = 20 - 8*r;


   CirclePoints_3DXY(A,x,y,color,B,qx,qy,qz);


   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_3DXY(A,x,y,color,B,qx,qy,qz);
   }

}

/***************************/
/**                       **/
/**      3D空间圆         **/
/**     平行于X-Z平面     **/
/***************************/

/* 3D圆的8对称点的显示 */
void CirclePoints_3DXZ(Point3D A,int x,int z,int color,Point3D B,float qx,float qy,float qz)/* 圆心为A;*/
{
  int X,Y,Z;
  X = x; Y = A.y; Z = z;                         /*(x,  z) */
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x - A.z + z; Y = A.y; Z = A.z - A.x + x; /*(z,  x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x + A.z - z; Y = A.y; Z = A.z - A.x + x;/*(-z, x) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = A.y; Z = z;               /*(-x, z) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x - A.z + z; Y = A.y; Z = A.z + A.x - x; /*(z, -x) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = x; Y = A.y; Z = 2 * A.z - z;               /*(x, -z) */
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = A.y; Z = 2 * A.z - z;     /*(-x,-z) */
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x + A.z - z; Y = A.y; Z = A.z + A.x - x; /*(-z,-x) */
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);



}

void Yuan_3DXZ(Point3D A,int r,int color,Point3D B,float qx,float qy,float qz)
{

   int x,z,d,deltaE,deltaSE,x0,z0,X,Y,Z;

   x0 = A.x;
   z0 = A.z;

   x = x0;
   z = z0 + r;
   d = 5 - 4*r;
   deltaE = 12;
   deltaSE = 20 - 8*r;


   CirclePoints_3DXZ(A,x,z,color,B,qx,qy,qz);


   while((z-z0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         z--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_3DXZ(A,x,z,color,B,qx,qy,qz);
   }

}

/***************************/
/**                       **/
/**3D平行于X-Y平面的空间圆**/
/**       的旋转          **/
/***************************/
/* 3D圆的8对称点的显示 */
void CirclePoints_3DXY_XuanZhuan(Point3D A,int x,int y,int color,float q,int i,int j,int k,int flag,Point3D B,float qx,float qy,float qz)/* 圆心为A;*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                /*(x,  y) */
     XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x-A.y+y; Y = A.y-A.x+x; Z = A.z;/*(y,  x) */
      XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x+A.y-y; Y = A.y-A.x+x; Z = A.z; /*(-y, x) */
      XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);


  X = 2*A.x-x; Y = y; Z = A.z;          /*(-x, y) */
     XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x-A.y+y; Y = A.y+A.x-x; Z = A.z; /*(y, -x) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);


  X = x; Y = 2*A.y-y; Z = A.z;          /*(x, -y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = 2*A.x-x; Y = 2*A.y-y; Z = A.z;    /*(-x,-y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x+A.y-y; Y = A.y+A.x-x; Z = A.z;/*(-y,-x) */
      XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);
}

void Yuan_3DXY_XuanZhuan(Point3D A,int r,int color,float q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{

   int x,y,d,deltaE,deltaSE,x0,y0,X,Y,Z,i,j,k;

   i = D.x; j = D.y; k = D.z;

   x0 = A.x;
   y0 = A.y;

   x = x0;
   y = y0 + r;
   d = 5 - 4*r;
   deltaE = 12;
   deltaSE = 20 - 8*r;


   CirclePoints_3DXY_XuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);


   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_3DXY_XuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
   }

}




/***************************/
/*                         */
/*  3D空间椭圆的中点算法   */
/*     平行于X - Y         */
/***************************/

/* 椭圆的4对称点的显示 */
void EllipsePoints_3DXY(Point3D A,int x,int y,int color,Point3D B,float qx,float qy,float qz) /*椭圆的中心为:(x0,y0)*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                    /*(x,  y) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = y; Z = A.z;          /*(-x, y) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = 2 * A.y - y; Z = A.z;/*(-x,-y) */
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = x; Y = 2 * A.y - y; Z = A.z;          /*(x, -y) */
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

}


/* 椭圆的中点算法 */
void MidPointEllipse_3DXY(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
  int x,y,xP,yP,x0,y0;
  float d,squrarea,squrareb;
   x0 = A.x;
   y0 = A.y;


  squrarea = a * a;
  squrareb = b * b;

  /*计算分解点P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* 生成上半部椭圆*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;
  EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;
    EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  }

  /* 生成下半部椭圆*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;
  EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;
   EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  }
}


/***************************/
/*                         */
/*    3D空间椭圆的旋转     */
/*                         */
/***************************/

/* 椭圆旋转后的4对称点的显示 */
void EllipsePoints_3DXY_XhuanZhuan(Point3D A,int x,int y,int color,float q,int i,int j,int k,int flag,Point3D B,float qx,float qy,float qz) /*椭圆的中心为:A*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                    /*(x,  y) */
     XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);


  X = 2 * A.x - x; Y = y; Z = A.z;          /*(-x, y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = 2 * A.x - x; Y = 2 * A.y - y; Z = A.z;/*(-x,-y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);
  X = x; Y = 2 * A.y - y; Z = A.z;          /*(x, -y) */
        XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
          Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);
}



/* 椭圆的中点算法 */
void MidPointEllipse_3DXY_XhuanZhuan(Point3D A,int a,int b,int color,float q,Point3D D,int flag,Point3D B,float qx,float qy,float qz) /* 绕通过 D 点直线的旋转 */
{
  int x,y,xP,yP,x0,y0,i,j,k;
  float d,squrarea,squrareb;
   x0 = A.x;
   y0 = A.y;
   i = D.x; j = D.y; k = D.z;

  squrarea = a * a;
  squrareb = b * b;

  /*计算分解点P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* 生成上半部椭圆*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;
  EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;
    EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  }

  /* 生成下半部椭圆*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;
  EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;
   EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  }
}
/***************************/
/**                       **/
/**    圆柱的算法         **/
/**                       **/
/***************************/
void YuanZhu3D(Point3D A,int r,int h,Point3D B,float qx,float qy,float qz)
{
   Point3D T;
   T.x = A.x; T.y = A.y; T.z = A.z -h/2.0;
   while(T.z <= A.z + h/2.0)
   {
     Yuan_3DXY(T,r,6,B,qx,qy,qz);
     T.z++;
   }

   while(r>=0)
   {
      Yuan_3DXY(T,r,2,B,qx,qy,qz);
      r--;
   }
}

/******************************************/
/**                                      **/
/**   长方体的算法(包含隐藏面的消隐算法) **/
/**                                      **/
/******************************************/
void Cft3D(Point3D A,int a,int b,int h,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z,i,j;
   Point3D B1,B2,B3,B4,B5,B6;
   Depth dp[6],temp;

   B1.x = A.x + b/2.0; B1.y = A.y;         B1.z = A.z;             /* 六个面的深度信息(各面矩形的中心点) */

   B2.x = A.x;         B2.y = A.y + a/2.0; B2.z = A.z;

   B3.x = A.x - b/2.0; B3.y = A.y;         B3.z = A.z;

   B4.x = A.x;         B4.y = A.y - a/2.0; B4.z = A.z;

   B5.x = A.x;         B5.y = A.y;         B5.z = A.z - h/2.0;

   B6.x = A.x;         B6.y = A.y;         B6.z = A.z + h/2.0;

                                    /*深度信息初始化*/
   X = B1.x; Y = B1.y; Z = B1.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = '1'; dp[0].d = Z;          /*第 1 面的深度*/

    X = B2.x; Y = B2.y; Z = B2.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = '2'; dp[1].d = Z;          /*第 2 面的深度*/

    X = B3.x; Y = B3.y; Z = B3.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = '3'; dp[2].d = Z;          /*第 3 面的深度*/


   X = B4.x; Y = B4.y; Z = B4.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = '4'; dp[3].d = Z;          /*第 4 面的深度*/


   X = B5.x; Y = B5.y; Z = B5.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = '5'; dp[4].d = Z;          /*第 5 面的深度*/

   X = B6.x; Y = B6.y; Z = B6.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = '6'; dp[5].d = Z;          /*第 6 面的深度*/

    for(i = 0; i <=4; i++)                 /*  深度排序  */
   {
      for(j = i+1;j<=5;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 5)      /*画家算法*/
  {
    switch(dp[i++].s)
    {


    case'1': Rect_3DYZ(B1,a,h,4,B,qx,qy,qz);
             break;

    case'2': Rect_3DXZ(B2,b,h,8,B,qx,qy,qz);
             break;

    case'3': Rect_3DYZ(B3,a,h,6,B,qx,qy,qz);
             break;

    case'4': Rect_3DXZ(B4,b,h,12,B,qx,qy,qz);
             break;

    case'5': Rect_3DXY(B5,a,b,3,B,qx,qy,qz);
             break;

    case'6': Rect_3DXY(B6,a,b,1,B,qx,qy,qz);
             break;
    }
  }
}

/***************************/
/**                       **/
/**   长方体的旋转        **/
/**                       **/
/***************************/
void Cft3D_XuanZhuan(Point3D A,int a,int b,int h,float q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z,i,j;
   Depth dp[6],temp;
   Point3D B1,B2,B3,B4,B5,B6;

   B1.x = A.x + b/2.0; B1.y = A.y;         B1.z = A.z;

   B2.x = A.x;         B2.y = A.y + a/2.0; B2.z = A.z;

   B3.x = A.x - b/2.0; B3.y = A.y;         B3.z = A.z;

   B4.x = A.x;         B4.y = A.y - a/2.0; B4.z = A.z;

   B5.x = A.x;         B5.y = A.y;         B5.z = A.z - h/2.0;

   B6.x = A.x;         B6.y = A.y;         B6.z = A.z + h/2.0;

   /*深度信息初始化*/
   X = B1.x; Y = B1.y; Z = B1.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
   Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = '1'; dp[0].d = Z;          /*第 1 面的深度*/

    X = B2.x; Y = B2.y; Z = B2.z;
    XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = '2'; dp[1].d = Z;          /*第 2 面的深度*/

    X = B3.x; Y = B3.y; Z = B3.z;
    XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = '3'; dp[2].d = Z;          /*第 3 面的深度*/


   X = B4.x; Y = B4.y; Z = B4.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = '4'; dp[3].d = Z;          /*第 4 面的深度*/


   X = B5.x; Y = B5.y; Z = B5.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = '5'; dp[4].d = Z;          /*第 5 面的深度*/

   X = B6.x; Y = B6.y; Z = B6.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = '6'; dp[5].d = Z;          /*第 6 面的深度*/


   for(i = 0; i <=4; i++)                 /*  深度排序  */
   {
      for(j = i+1;j<=5;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }
   i = 0;
   while(i <= 5)      /*画家算法*/
  {
    switch(dp[i++].s)
    {

      case'1': Rect_3DYZ_XuanZhuan(B1,a,h,4,q,D,flag,B,qx,qy,qz);
               break;

      case'2': Rect_3DXZ_XuanZhuan(B2,b,h,12,q,D,flag,B,qx,qy,qz);
               break;

      case'3': Rect_3DYZ_XuanZhuan(B3,a,h,4,q,D,flag,B,qx,qy,qz);
               break;

      case'4': Rect_3DXZ_XuanZhuan(B4,b,h,12,q,D,flag,B,qx,qy,qz);
               break;

      case'5': Rect_3DXY_XuanZhuan(B5,a,b,1,q,D,flag,B,qx,qy,qz);
               break;

      case'6': Rect_3DXY_XuanZhuan(B6,a,b,1,q,D,flag,B,qx,qy,qz);
               break;
     }
  }
}

/***************************************/
/***                                 ***/
/***      常见的实物实体画法         ***/
/***                                 ***/
/***************************************/


/*****************************/
/**    栏杆(平行于X轴)      **/
/*****************************/
void LanGan_X(Point3D A,int L,int h,Point3D BB,float qx,float qy,float qz)
{
  int X,Y,Z,i,j;
  Point3D B;
  Depth dp[7],temp;

  X = A.x - L/2.0; Y = A.y; Z = A.z;    /*五条竖线的深度*/
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[0].s = 'A'; dp[0].d = Z;

  X = A.x - L/2.0+ L/4.0; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[1].s = 'B'; dp[1].d = Z;

  X = A.x - L/2.0 + L/2.0; Y = A.y; Z = A.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[2].s = 'C'; dp[2].d = Z;

  X = A.x - L/2.0 + 3*L/4.0; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[3].s = 'D'; dp[3].d = Z;

  X = A.x - L/2.0 + L; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[4].s = 'E'; dp[4].d = Z;

  for(i = 0; i <=3; i++)                 /*  深度排序  */
   {
      for(j = i+1;j<=4;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 4)      /*画家算法*/
  {
    switch(dp[i++].s)
    {


  case'A': B.x = A.x - L/2.0; B.y = A.y; B.z = A.z;  /*四条竖线*/
           Cft3D(B,15,15,h + 40,BB,qx,qy,qz);
           break;

  case'B': B.x = A.x - L/2.0 + L/4.0; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'C': B.x = A.x - L/2.0 + L/2.0; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'D': B.x = A.x - L/2.0 + 3*L/4.0; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'E': B.x = A.x - L/2.0 + L; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h + 40,BB,qx,qy,qz);
           break;


   }
  }

  B.x = A.x; B.y = A.y; B.z = A.z + h/2.0;        /*两条横线*/
           Cft3D(B,15,L - 15,15,BB,qx,qy,qz);


  B.x = A.x; B.y = A.y; B.z = A.z - h/2.0;
           Cft3D(B,15,L - 15,15,BB,qx,qy,qz);

}

/*****************************/
/**    栏杆(平行于Y轴)      **/
/*****************************/
void LanGan_Y(Point3D A,int L,int h,Point3D BB,float qx,float qy,float qz)
{
  int X,Y,Z,i,j;
  Point3D B;
  Depth dp[7],temp;

  X = A.x - L/2.0; Y = A.y; Z = A.z;    /*五条竖线的深度*/
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[0].s = 'A'; dp[0].d = Z;

  X = A.x - L/2.0+ L/4.0; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[1].s = 'B'; dp[1].d = Z;

  X = A.x - L/2.0 + L/2.0; Y = A.y; Z = A.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[2].s = 'C'; dp[2].d = Z;

  X = A.x - L/2.0 + 3*L/4.0 - 10; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[3].s = 'D'; dp[3].d = Z;

  X = A.x - L/2.0 + L; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[4].s = 'E'; dp[4].d = Z;


  for(i = 0; i <=3; i++)                 /*  深度排序  */
   {
      for(j = i+1;j<=4;j++)
      {
         if(dp[i].d>=dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 4)      /*画家算法*/
  {
    switch(dp[i++].s)
    {


  case'A': B.x = A.x; B.y = A.y - L/2.0; B.z = A.z;  /*四条竖线*/
           Cft3D(B,15,15,h +40,BB,qx,qy,qz);
           break;

  case'B': B.x = A.x; B.y = A.y - L/2.0 + L/4.0; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'C': B.x = A.x ; B.y = A.y- L/2.0 + L/2.0; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'D': B.x = A.x ; B.y = A.y- L/2.0 + 3*L/4.0; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'E': B.x = A.x ; B.y = A.y- L/2.0 + L; B.z = A.z;
           Cft3D(B,15,15,h + 40,BB,qx,qy,qz);
           break;


   }
  }
    B.x = A.x; B.y = A.y; B.z = A.z + h/2.0;
           Cft3D(B,L - 15,15,15,BB,qx,qy,qz);


    B.x = A.x; B.y = A.y; B.z = A.z - h/2.0;
           Cft3D(B,L - 15,15,15,BB,qx,qy,qz);


}





/*****************************/
/**          树             **/
/*****************************/
void Shu(Point3D A,int r,Point3D BB,float qx,float qy,float qz)
{
   int k;
   Point3D B;
   setbkcolor(2);
   B.x = A.x + r; B.y = A.y; B.z = A.z;
   k=0;
   while(k<360)
   {
     Rect_3DXY_XuanZhuan(B,40,20,7,k,A,3,BB,qx,qy,qz);
     k = k+30;
   }

   B.x = A.x; B.y = A.y; B.z = A.z + 50; /* 树干 */
   YuanZhu3D(B,15,100,BB,qx,qy,qz);

   B.x = A.x; B.y = A.y; B.z = A.z + 15;
   k = 15;
   while(B.z>=A.z)
   {
       if((int)B.z%2==0)
          Yuan_3DXY(B,k,2,BB,qx,qy,qz);
       else
          Yuan_3DXY(B,k,4,BB,qx,qy,qz);
       B.z--;
       k = k + 1;
   }



   B.x = A.x; B.y = A.y; B.z = A.z + 100; /*树叶*/
   k=80;
   while(k>=0)
   {
       Yuan_3DXY(B,k--,8,BB,qx,qy,qz);
   }

   k=100;            /*树叶1*/
   while(k>=0)
   {
      if(k%2==0)
        Yuan_3DXY(B,k,3,BB,qx,qy,qz);
      else
          Yuan_3DXY(B,k,4,BB,qx,qy,qz);

      B.z = B.z + 2;
      k--;

   }

   B.x = A.x; B.y = A.y; B.z = A.z + 200;
   k=80;
   while(k>=0)
   {
       Yuan_3DXY(B,k--,8,BB,qx,qy,qz);
   }

   k=80;            /*树叶2*/
   while(k>=0)
   {
      if(k%2==0)
        Yuan_3DXY(B,k,3,BB,qx,qy,qz);
      else
          Yuan_3DXY(B,k,4,BB,qx,qy,qz);

      B.z = B.z + 2;
      k--;

   }


}



/*****************************/
/**          凳子           **/
/*****************************/
void DengZi(Point3D A,int a,int b,int h,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z,i,j;
   Point3D D;
   Depth dp[12],temp;
   /*  Rect_3DXY(A,a + 100,b + 100,8,B,qx,qy,qz);*/



                                              /* 深度初始化*/
   X =  A.x - b/2.0; Y = A.y - a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;


    X =  A.x - b/2.0; Y = A.y + a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;

   X =  A.x + b/2.0; Y = A.y - a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = 'C'; dp[2].d = Z;


   X =  A.x + b/2.0; Y = A.y + a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;

   X =  A.x - b/2.0; Y = A.y; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;

   X =  A.x + b/2.0; Y = A.y; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;

   X = A.x; Y = A.y - a/2.0; Z = A.z + h;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;

   X = A.x; Y = A.y - a/2.0 + a/5.0; Z = A.z + h;
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;

   X = A.x; Y = A.y - a/2.0 + 2*a/5.0; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[8].s = 'J'; dp[8].d = Z;

   X = A.x; Y = A.y - a/2.0 + 3*a/5.0; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[9].s = 'K'; dp[9].d = Z;

   X = A.x; Y = A.y - a/2.0 + 4*a/5.0; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[10].s = 'L'; dp[10].d = Z;

   X = A.x; Y = A.y - a/2.0 + a; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[11].s = 'M'; dp[11].d = Z;




    for(i = 0; i <=11; i++)                 /*  深度排序  */
   {
      for(j = i+1;j<=12;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 11)      /*画家算法*/
  {
    switch(dp[i++].s)
    {
    case 'A': D.x = A.x - b/2.0; D.y = A.y - a/2.0; D.z = A.z + h/2.0;  /* 四个脚 */
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;


    case 'B': D.x = A.x - b/2.0; D.y = A.y + a/2.0; D.z = A.z + h/2.0;
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;

    case 'C': D.x = A.x + b/2.0; D.y = A.y - a/2.0; D.z = A.z + h/2.0;
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;


    case 'D': D.x = A.x + b/2.0; D.y = A.y + a/2.0; D.z = A.z + h/2.0;
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;


    case 'E': D.x = A.x - b/2.0; D.y = A.y; D.z = A.z + h;  /*上面的条木*/
              Cft3D(D,a+20,10,10,B,qx,qy,qz);
              break;


    case 'F': D.x = A.x + b/2.0; D.y = A.y; D.z = A.z + h;
              Cft3D(D,a+20,10,10,B,qx,qy,qz);
              break;

    case 'H': D.x = A.x; D.y = A.y - a/2.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    case 'I': D.x = A.x; D.y = A.y - a/2.0 + a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;


    case 'J': D.x = A.x; D.y = A.y - a/2.0 + 2*a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;


    case 'K': D.x = A.x; D.y = A.y - a/2.0 + 3*a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    case 'L': D.x = A.x; D.y = A.y - a/2.0 + 4*a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    case 'M': D.x = A.x; D.y = A.y - a/2.0 + a; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    }
  }
}

/*****************************/
/**          房子           **/
/*****************************/
void FangZi(Point3D A,int a,int b,int h,Point3D BB,float qx,float qy,float qz)
{
   int X,Y,Z,i,j,d;
   Point3D B,C;
   Depth dp[9],temp;

   X = A.x; Y = A.y; Z = A.z;         /*深度信息初始化*/
    Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;        /* 长方体的深度*/

   X = A.x + b/2.0 + 30; Y = A.y; Z = A.z - h/4.0;
     Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;        /*门框的深度*/

   X = A.x + b/2.0 + a/12.0 + 60; Y = A.y - a/6.0; Z = A.z - h/4.0;
     Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[2].s = 'C'; dp[2].d = Z;        /*门1的深度*/

   X = A.x + b/2.0 + a/12.0 + 60; Y = A.y + a/6.0; Z = A.z - h/4.0;
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;        /*门2的深度*/

   X = A.x; Y = A.y + a/2.0; Z = A.z;
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;       /* 窗框1的深度 */

   X = A.x; Y = A.y + a/2.0 + 25; Z = A.z - h/8.0;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;       /* 窗台1的深度 */

   X = A.x; Y = A.y - a/2.0; Z = A.z;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;       /* 窗框2的深度 */

   X = A.x; Y = A.y - a/2.0 - 25; Z = A.z - h/8.0;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;         /* 窗台2的深度 */

   X = A.x; Y = A.y; Z = A.z + h/2.0;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[8].s = 'J'; dp[8].d = Z;         /*房顶的深度*/

    for(i = 0; i <=8; i++)                 /*  深度排序  */
   {
      for(j = i+1;j<=9;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

  i = 0;
  while(i <= 9)      /*画家算法*/
  {
    switch(dp[i++].s)
    {

       case'A': Cft3D(A,a,b,h,BB,qx,qy,qz); /* 长方体 */
                break;

       case'B': B.x = A.x + b/2.0; B.y = A.y; B.z = A.z - h/4.0; /* 画框 */
                Rect_3DYZ(B,a/3.0,h/2.0,15,BB,qx,qy,qz);
                break;

       case'C': B.x = A.x + b/2.0 + a/12.0; B.y = A.y - a/6.0; B.z = A.z - h/4.0; /* 画门1*/
                C.x = A.x + b/2.0; C.y = B.y; C.z = B.z;
                Cft3D_XuanZhuan(B,5,a/6.0,h/2.0,0,C,3,BB,qx,qy,qz);
                break;

       case'D': B.x = A.x + b/2.0 + a/12.0; B.y = A.y + a/6.0; B.z = A.z - h/4.0;  /* 画门2*/
                C.x = A.x + b/2.0; C.y = B.y; C.z = B.z;
                Cft3D_XuanZhuan(B,5,a/6.0,h/2.0,0,C,3,BB,qx,qy,qz);
                break;

       case'E': B.x = A.x; B.y = A.y + a/2.0; B.z = A.z; /* 画窗框1 */
                Rect_3DXZ(B,b/2.0,h/4.0,15,BB,qx,qy,qz);
                Rect_3DXZ(B,5,h/4.0,12,BB,qx,qy,qz); /* 竖线 */
                break;

       case'F': B.x = A.x; B.y = A.y + a/2.0 + 25; B.z = A.z - h/8.0; /* 窗台1*/
                Cft3D(B,50,b/2.0,5,BB,qx,qy,qz);
                break;

       case'H': B.x = A.x; B.y = A.y - a/2.0; B.z = A.z; /* 画窗框2 */
                Rect_3DXZ(B,b/2.0,h/4.0,15,BB,qx,qy,qz);
                Rect_3DXZ(B,5,h/4.0,12,BB,qx,qy,qz); /* 竖线 */
                break;

       case'I': B.x = A.x; B.y = A.y - a/2.0 - 25; B.z = A.z - h/8.0; /* 窗台2*/
                Cft3D(B,50,b/2.0,5,BB,qx,qy,qz);
                break;

       case'J':  B.x = A.x; B.y = A.y; B.z = A.z + h/2.0;/* 房顶 */
                 Rect_3DXY(B,a + 100,b + 100,8,BB,qx,qy,qz);

                 B.x = A.x - b/2.0 - 50; B.y = A.y; B.z = A.z + h/2.0 + (b+100)/4.0;
                 C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                 d = (b/2.0 + 50) * 2.0/sqrt(3);
                 Rect_3DYZ_XuanZhuan(B,a + 100,d,9,70,C,2,BB,qx,qy,qz);

                 B.x = A.x + b/2.0 + 50; B.y = A.y; B.z = A.z + h/2.0 + (b+100)/4.0;
                 C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                 d = (b/2.0 + 50) * 2.0/sqrt(3);
                 Rect_3DYZ_XuanZhuan(B,a + 100,d,9,-70,C,2,BB,qx,qy,qz);

                                                           /* 瓦*/
                 B.x = A.x - b/2.0 - 50; B.y = A.y - a/2.0 - 50; B.z = A.z + h/2.0 + (b+100)/4.0;
                 C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                 while(B.y <= A.y + a/2.0 + 50)
                {
                 Rect_3DYZ_XuanZhuan(B,5,d,12,70,C,2,BB,qx,qy,qz);
                 B.y = B.y + 40;
                 C.y = C.y + 40;

                 }

                  B.x = A.x + b/2.0 + 50; B.y = A.y - a/2.0 - 50; B.z = A.z + h/2.0 + (b+100)/4.0;
                  C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                  while(B.y <= A.y + a/2.0 + 50)
                    {
                    Rect_3DYZ_XuanZhuan(B,5,d,12,-70,C,2,BB,qx,qy,qz);
                    B.y = B.y + 40;
                    C.y = C.y + 40;

                    }
                                                       /* 屋脊 */
                  B.x = A.x; B.y = A.y; B.z = A.z + h/2.0 + 45;
                  Cft3D(B,a + 100,10,10,BB,qx,qy,qz);
                  break;
        }
    }

}


/***************************/
/**　　　　　　　　　　　 **/
/**       3D综合画面      **/
/**                       **/
/***************************/
void My3DGraphics_A(Point3D A,Point3D BB,float qx,float qy,float qz) /* 观察方向为(qx,qy,qz)*/
{
   float a,b,h,d;
   int X,Y,Z,i,j;
   Point3D B,C;
   Depth dp[20],temp;
   setbkcolor(2);

   a = 220;
   b = 150;
   h = 200;

   B.x = A.x + b/2.0 + 250; B.y = A.y + a/2.0 + 100; B.z = A.z - 100;
   C.x = A.x + b/2.0 + 600; C.y = A.y; C.z = A.z - 100;
   i = 0;
   while(B.y>=A.x - a/2.0 - 1000)   /*亭前小路*/
   {
       Rect_3DXY_XuanZhuan(B,50,100,7,i,C,3,BB,qx,qy,qz);
       B.y = B.y - 80;
       i = i + 5;
   }


                                  /*深度信息初始化*/

   X = A.x; Y = A.y; Z = A.z;            /*底台的深度*/
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;

   X = A.x; Y = A.y; Z = A.z + 100 + h/2.0;    /*房子的深度*/
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;

   X = A.x - a/2.0 - 500; Y = A.y + b/2.0 + 350; Z = A.z+250;
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz); /* 树1的深度*/
   dp[2].s = 'C'; dp[2].d = Z;

   X = A.x + b/2.0 + 150;Y = A.y; Z = A.z + 100 + 60; /*四条栏杆的深度*/
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;

   X = A.x - b/2.0 - 150; Y = A.y; Z = A.z + 100 + 60;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;

   X = A.x;Y = A.y - a/2.0 - 200; Z = A.z + 100 + 60;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;

   X = A.x;Y = A.y + a/2.0 + 200; Z = A.z + 100 + 60;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;

   X = A.x; Y = A.y; Z = A.z + 20; /*台上面的深度*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;

   X = A.x;Y = A.y + a/2.0 + 400; Z = A.z - 50; /*右边小台的深度*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[8].s = 'J'; dp[8].d = Z;

   X = A.x;Y = A.y + a/2.0 + 450; Z = A.z + 100;    /*凳子*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[9].s = 'K'; dp[9].d = Z;

   X = A.x; Y = A.y + a/2.0 + 300; Z = A.z + 150; /*中间的梯子*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[10].s = 'M'; dp[10].d = Z;

   X = A.x + a/2.0 + 900; Y = A.y; Z = A.z; /*树2*/
        Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[11].s = 'N'; dp[11].d = Z;

   X = A.x + 400; Y = A.y + a/2.0 + 400; Z = A.z + 75;   /*小台上的栏杆*/
        Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[12].s = 'O'; dp[12].d = Z;

   X = A.x - 500;Y = A.y + a/2.0 + 400; Z = A.z + 75;
        Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[13].s = 'P'; dp[13].d = Z;

   X = A.x;Y = A.y + a/2.0 + 580; Z = A.z + 65;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[14].s = 'Q'; dp[14].d = Z;

   X = A.x + a/2.0 + 900; Y = A.y - 200; Z = A.z;  /*凳子2的深度*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[15].s = 'R'; dp[15].d = Z;



   for(i = 0; i <=14; i++)                 /*  深度排序*/
   {
      for(j = i+1;j<=15;j++)
      {
         if(dp[i].d>=dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

  i = 0;
  while(i <= 15)      /*画家算法*/
  {
    switch(dp[i++].s)
    {
       case'A':  Cft3D(A,a + 400,b + 300,h,BB,qx,qy,qz);    /* 底台*/
                 break;

       case'B':  B.x = A.x; B.y = A.y; B.z = A.z + 200;
                 FangZi(B,a,b,h,BB,qx,qy,qz);                  /* 房子 */
                 break;

       case'C':  B.x = A.x - a/2.0 - 700; B.y = A.y + b/2.0 + 350; B.z = A.z;
                 Shu(B,100,BB,qx,qy,qz);                                   /*树1*/
                 break;

       case'D':  B.x = A.x + b/2.0 + 150;B.y = A.y; B.z = A.z + 100 + 55;/*四条栏杆*/
                 LanGan_Y(B,a + 380,65,BB,qx,qy,qz);
                 break;

       case'E':  B.x = A.x - b/2.0 - 150;B.y = A.y; B.z = A.z + 100 + 55;
                 LanGan_Y(B,a + 380,65,BB,qx,qy,qz);
                 break;

       case'F':  B.x = A.x;B.y = A.y - a/2.0 - 200; B.z = A.z + 100 + 55;
                 LanGan_X(B,b + 280,65,BB,qx,qy,qz);
                 break;

       case'H':  B.x = A.x + b/3.0 + 100;B.y = A.y + a/2.0 + 200; B.z = A.z + 100 + 55;
                 LanGan_X(B,(b + 280)/3.0,65,BB,qx,qy,qz);

                 B.x = A.x - b/3.0 - 100;B.y = A.y + a/2.0 + 200; B.z = A.z + 100 + 55;
                 LanGan_X(B,(b + 280)/3.0,65,BB,qx,qy,qz);
                 break;

       case'I':  B.x = A.x;B.y = A.y; B.z = A.z + 100;     /*台上的面*/
                 Rect_3DXY(B,a + 400,b + 300,6,BB,qx,qy,qz);
                 break;

       case'J':  B.x = A.x;B.y = A.y + a/2.0 + 400; B.z = A.z - 50; /*右边的小台*/
                 Cft3D(B,400,1000,100,BB,qx,qy,qz);

                 B.x = A.x;B.y = A.y + a/2.0 + 400; B.z = A.z; /*右边台上的面*/
                 Rect_3DXY(B,400,1000,6,BB,qx,qy,qz);
                 break;

       case'K':  B.x = A.x;B.y = A.y + a/2.0 + 450; B.z = A.z; /*凳子1*/
                 DengZi(B,100,250,50,BB,qx,qy,qz);
                 break;

       case'M':  B.x = A.x;B.y = A.y + a/2.0 + 250; B.z = A.z + 25; /*中间的梯子*/
                 Cft3D(B,100,b/3.0 + 100,50,BB,qx,qy,qz);

                 B.x = A.x;B.y = A.y + a/2.0 + 250; B.z = A.z + 50;
                 Rect_3DXY(B,100,b/3.0 + 100,6,BB,qx,qy,qz);
                 break;


       case'N':  B.x = A.x + a/2.0 + 900; B.y = A.y; B.z = A.z;
                 Shu(B,100,BB,qx,qy,qz);                                   /*树2*/
                 break;

       case'O':  B.x = A.x + 480;B.y = A.y + a/2.0 + 400; B.z = A.z + 65; /*小台上的梯子*/
                 LanGan_Y(B,380,60,BB,qx,qy,qz);
                 break;

       case'P':  B.x = A.x - 480;B.y = A.y + a/2.0 + 400; B.z = A.z + 65;
                 LanGan_Y(B,380,60,BB,qx,qy,qz);
                 break;

       case'Q':  B.x = A.x;B.y = A.y + a/2.0 + 600; B.z = A.z + 65;
                 LanGan_X(B,980,60,BB,qx,qy,qz);
                 break;

       case'R':  B.x = A.x + a/2.0 + 900; B.y = A.y - 200; B.z = A.z; /*凳子2*/
                 DengZi(B,100,250,50,BB,qx,qy,qz);
                 break;

    }
  }

}


void My3DGraphics_B(Point3D A,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z;
   Depth dp[20],temp;
   CFT Cft1,Cft2,Cft3,Cft4,Cft5,Cft6;
   Point3D D1,D2,T;
   Point B1,B2;
   int a,b,i,j,k;

   setbkcolor(2);


   a = 400; b = 800;

   Rect_3DXY(A,a,b,15,B,qx,qy,qz);    /*球场*/
   Rect_3DXY(A,a - 10,b - 10,6,B,qx,qy,qz);

   Rect_3DXY(A,a,5,15,B,qx,qy,qz); /* 中间的一条横线*/

   k = 80;

   while(k>=75)  /* 中间的圆 */
   {
     Yuan_3DXY(A,k,15,B,qx,qy,qz);
     k--;
   }

   D2.x = A.x + b/4.0; D2.y = A.y; D2.z = A.z;/* 篮筐1下的的三分线及其斜线 */
   k = 70;
   while(k>=65)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   D2.x = A.x + b/4.0 + 35; D2.y = A.y; D2.z = A.z;
   Rect_3DXY(D2,200,70,6,B,qx,qy,qz);

   D2.x = A.x + 3 * b/8.0; D2.y = A.y - 85; D2.z = A.z;  /* 斜线 */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,-10,D2,3,B,qx,qy,qz);


   D2.x = A.x + 3 * b/8.0; D2.y = A.y + 85; D2.z = A.z;  /* 斜线 */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,10,D2,3,B,qx,qy,qz);



   D2.x = A.x + b/4.0; D2.y = A.y; D2.z = A.z;  /* 篮筐1下的圆 */
   k = 50;
   while(k>=45)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   Rect_3DXY(D2,100,5,15,B,qx,qy,qz);


   D2.x = A.x - b/4.0; D2.y = A.y; D2.z = A.z;/* 篮筐2下的的三分线及其斜线 */
   k = 70;
   while(k>=65)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   D2.x = A.x - b/4.0 - 35; D2.y = A.y; D2.z = A.z;
   Rect_3DXY(D2,200,70,6,B,qx,qy,qz);

   D2.x = A.x - 3 * b/8.0; D2.y = A.y - 85; D2.z = A.z;  /* 斜线 */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,10,D2,3,B,qx,qy,qz);


   D2.x = A.x - 3 * b/8.0; D2.y = A.y + 85; D2.z = A.z;  /* 斜线 */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,-10,D2,3,B,qx,qy,qz);


   D2.x = A.x - b/4.0; D2.y = A.y; D2.z = A.z;  /* 篮筐2下的圆*/
   k = 50;
   while(k>=45)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   Rect_3DXY(D2,100,5,15,B,qx,qy,qz);
                                               /*深度信息初始化*/

   X = A.x - b/2.0 + 20; Y = A.y; Z = A.z + 105;   /*篮板2支架的深度*/
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;

   X = A.x - b/2.0 + 40; Y = A.y;  Z = A.z + 200;   /* 篮板2的深度 */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;


   X = A.x - b/2.0 + 60; Y = A.y; Z = A.z + 200;   /* 篮框2处接柱1的深度 */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = 'C'; dp[2].d = Z;


   X = A.x - b/2.0 + 90; Y = A.y; Z = A.z + 205;   /* 篮筐2的深度 */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;


   X = A.x + b/2.0 - 80; Y = A.y; Z = A.z + 200;   /* 篮筐1的深度 */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;

   X = A.x + b/2.0 - 50;  Y = A.y; Z = A.z + 220;   /* 篮框1处接柱1的深度 */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;

   X = A.x + b/2.0 - 30; Y = A.y; Z = A.z + 200;     /* 篮板1的深度 */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;

   X = A.x + b/2.0 - 10; Y = A.y;  Z = A.z + 100;  /* 篮板1的支架1的深度  */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;

   for(i = 0; i <=6; i++)                 /*  深度排序*/
   {
      for(j = i+1;j<=7;j++)
      {
         if(dp[i].d>=dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

  i = 0;
  while(i <= 7)      /*画家算法*/
  {
    switch(dp[i++].s)
    {


     case'A': Cft4.A.x = A.x - b/2.0 + 30;            /*篮板2的支架*/
              Cft4.A.y = A.y;
              Cft4.A.z = A.z + 105;
              Cft4.a = 10;
              Cft4.b = 10;
              Cft4.h = 200;
              Cft3D(Cft4.A,Cft4.a,Cft4.b,Cft4.h,B,qx,qy,qz);
              break;

     case'B': Cft2.A.x = A.x - b/2.0 + 40;   /* 篮板2 */
              Cft2.A.y = A.y;
              Cft2.A.z = A.z + 200;
              Cft2.a = 80;
              Cft2.b = 10;
              Cft2.h = 80;
              Cft3D(Cft2.A,Cft2.a,Cft2.b,Cft2.h,B,qx,qy,qz);

              break;

     case'C': Cft1.A.x = A.x - b/2.0 + 50;     /* 篮框2处接柱1 */
              Cft1.A.y = A.y;
              Cft1.A.z = A.z + 210;
              Cft1.a = 10;
              Cft1.b = 15;
              Cft1.h = 10;
              Cft3D(Cft1.A,Cft1.a,Cft1.b,Cft1.h,B,qx,qy,qz);
              break;



    case'D':  D1.x = A.x - b/2.0 + 80; D1.y = A.y; D1.z = A.z + 205; /* 篮筐2 */
              k = 20;
              while(k>=17)
              {
                Yuan_3DXY(D1,k,14,B,qx,qy,qz);
                   k--;
              }
              break;

    case'E':  D1.x = A.x + b/2.0 - 80; D1.y = A.y; D1.z = A.z + 200;
              k = 20;
               while(k>=17)        /* 篮筐1 */
               {
                Yuan_3DXY(D1,k,14,B,qx,qy,qz);
                 k--;
               }

             break;

    case'F': Cft1.A.x = D1.x + 30;     /* 篮框1处接柱1 */
             Cft1.A.y = D1.y;
             Cft1.A.z = D1.z+5;
             Cft1.a = 10;
             Cft1.b = 15;
             Cft1.h = 10;
             Cft3D(Cft1.A,Cft1.a,Cft1.b,Cft1.h,B,qx,qy,qz);

             break;

    case'H': Cft2.A.x = D1.x + 40;   /* 篮板1 */
             Cft2.A.y = D1.y;
             Cft2.A.z = D1.z;
             Cft2.a = 80;
             Cft2.b = 10;
             Cft2.h = 80;
             Cft3D(Cft2.A,Cft2.a,Cft2.b,Cft2.h,B,qx,qy,qz);
             break;

    case'I': Cft3.A.x = D1.x + 720;    /* 篮板1的支架 */
             Cft3.A.y = D1.y+30;
             Cft3.A.z = D1.z - 100;
             Cft3.a = 10;
             Cft3.b = 10;
             Cft3.h = 200;
             Cft3D(Cft3.A,Cft3.a,Cft3.b,Cft3.h,B,qx,qy,qz);
             break;
    }
  }

}

void My3DGraphics_C(Point3D A,Point3D B,float qx,float qy,float qz)       /* 还没写! */
{

}

/***************************/
/****                   ****/
/****   图形显示函数    ****/
/****                   ****/
/***************************/

/** 显示一条直线 **/
void Show_L(Line L)
{
   LineDDA(L.A,L.B,L.color);
}

/** 显示一个圆     **/


void Show_C(Yuan C)
{
   MidPointCircle_3((C.o),C.r,C.color);
}

/** 显示一个椭圆 **/
void Show_E(TuoYuan E)
{
  MidPointEllipse(E.o,E.a,E.b,E.color);
}

/** 显示一个球 **/
void Show_Q(Qiu Q)
{
    QiuEC(Q.o,Q.r,Q.color);
}

/** 显示一个矩形 **/
void Show_R(Rect R)
{
   Rectangle(R.LeftTop,R.RightDown,R.color);
}

/** 显示一个3D矩形面  **/
Show3D_R(Rect3D R,Point3D B,float qx,float qy,float qz)
{
   Rect_3DXY(R.A,R.a,R.b,R.color,B,qx,qy,qz);

}

/* 显示一个长方体*/
void Show3D_Cft(CFT C,Point3D B,float qx,float qy,float qz)/* 观察方向为(qx,qy,qz)   */
{
   Cft3D(C.A,C.a,C.b,C.h,B,qx,qy,qz);
}



main()                  /* MAIN函数体 */
{
  int i;
  float k,h;

  Point PointA,PointB;
  Point * P;
  Line L1,L2;
  Yuan C1,C2;
  TuoYuan E1,E2,E3,E4;
  Qiu Q1,Q2,Q3;
  Rect R1,R2;

  Point3D D1,D2,D3;
  CFT Cft1,Cft2;
  SLZ S1,S2;
  SLZU Sz1,Sz2;


  int graphdriver = DETECT,graphmode;
  registerbgidriver(EGAVGA_driver);
  initgraph(&graphdriver,&graphmode,"");

  D1.x = 200;
  D1.y = 250;
  D1.z = -150;

   D2.x = 250;
   D2.y = 320;
   D2.z = -150;

My3DGraphics_A(D1,D1,-45,45,70);
/* Cft3D(D1,220,150,200,D1,-45,45,70); */
/* My3DGraphics_B(D2,D2,35,90,45); */
/* LanGan_X(D1,400,80,D1,-30,45,70); */
/* LanGan_Y(D1,200,80,D1,-45,45,70); */
/* FangZi(D1,300,200,400,D1,-45,45,70); */
/* Shu(D1,80,D1,-45,45,70); */
/* DengZi(D1,200,100,50,D1,-45,45,70); */
  getch();
  closegraph();
}
                  /* 程序结束 */


 /* PointA.x = 550,PointA.y = 100;PointB.x = 100,PointB.y = 100;

  L1.A = PointA;                     /* 测试部分 */
  L1.B = PointB;
  L1.color = 2;

    Show_L(L1);
MidPointLine(L1.A,L1.B,15);


  C1.o.x = 300;
  C1.o.y = 300;
  C1.r = 60;

  C1.color = 4;

    Show_C(C1);
    PointA.x = 300;
    PointA.y = 240;
    k = 0.001;
       while(0)
       {
         C1.color = 0;
         MidPointCircle_CuoQie(C1,PointA,k - 0.001,0);
         C1.color = 4;
         MidPointCircle_CuoQie(C1,PointA,k,0);
         k = k + 0.001;
       }

  E1.o.x = 250;
  E1.o.y = 250;
  E1.a = 60; E1.b = 30;
  E1.color = 11;
     Show_E(E1);


  E2.o.x = 380;
  E2.o.y = 250;
  E2.a = 60; E2.b = 30;
  E2.color = 11;
      Show_E(E2);


  E3.o.x = 315;
  E3.o.y = 180;
  E3.a = 30; E3.b = 60;
  E3.color = 11;
       Show_E(E3);

  E4.o.x = 315;
  E4.o.y = 320;
  E4.a = 30; E4.b = 60;
  E4.color = 11;
       Show_E(E4);

  PointA.x = 315;
  PointA.y = 248;

  R1.LeftTop.x = 313;
  R1.LeftTop.y = 248;
  R1.RightDown.x = 319;
  R1.RightDown.y = 450;
  R1.color = 2;
    Show_R(R1);

      k = 1;
      while(0)
      {
         E1.color = 0;
         E2.color = 0;
         E3.color = 0;
         E4.color = 0;
         MidPointEllipse_XuanZhuan(E1,PointA,k-1);
         MidPointEllipse_XuanZhuan(E2,PointA,k-1);
         MidPointEllipse_XuanZhuan(E3,PointA,k-1);
         MidPointEllipse_XuanZhuan(E4,PointA,k-1);
         Show_R(R1);
         E1.color = 11;
         E2.color = 11;
         E3.color = 11;
         E4.color = 11;
         MidPointEllipse_XuanZhuan(E1,PointA,k);
         MidPointEllipse_XuanZhuan(E2,PointA,k);
         MidPointEllipse_XuanZhuan(E3,PointA,k);
         MidPointEllipse_XuanZhuan(E4,PointA,k);
         k = k + 1;
      }
     PointB.x =190 ;
     PointB.y = 250;
        E1.color = 4;
         MidPointEllipse_FangSuo(E1,PointB,2,2);
       MidPointEllipse_PinYi(E1,150,0);




  Q1.o.x = 500;
  Q1.o.y = 400;
  Q1.r = 60;
  Q1.color = 11;

    Show_Q(Q1);

  R1.LeftTop.x = 50;
  R1.LeftTop.y = 150;
  R1.RightDown.x = 300;
  R1.RightDown.y = 200;
  R1.color = 11;
    Show_R(R1);

    Rectangle_XuanZhuan(R1,32);




  S1.A.x = 350;
  S1.A.y = 250;
  S1.A.z = 100;
  S1.r = 400;
  S1.h = 100;

  Show3D_S(S1,45,45,60);

  D1.x = 200;
  D1.y = 250;
  D1.z = -150;

  /*  D2.x = 250; */
  /*  D2.y = 320; */
  /*  D2.z = -150; */

My3DGraphics_A(D1,D1,-45,45,70);

/* My3DGraphics_B(D2,D2,35,90,45); */


/* My3DGraphics_C(D1,40,50,70); */

/* My3DGraphics_D(D1,45,45,45); */
  getch();
  closegraph();

}*/

