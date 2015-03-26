#ifndef  _ORDER_DEFINE__
#define  _ORDER_DEFINE__

#define MUTEX_DEFINE "_amazonrobot_yutone_copyright"
#define AMAZON_CONFIG_FILE "amazonset.ini"
#define AMAZON_ROBOTNAME "AmazonRobot.exe"
#define FLAG_STRING "source=eqifa|140382|1|"
#define HAVE_ALREADY_RUN  555	//只能运行一个程序
#define USER_OR_PWD_WRONG  666	//账号或者密码错误 [暂时没用]
#define USER_TIMEOUT  777		//超时
#define ORDER_SUCESS  888		//下单成功[暂时没用]
#define ORDER_ERROR   999		//下单失败
#define ORDER_DUPLICATE 1111	//重复下单
#define LOGIN_FAILED 2222		//登录失败
#define PARAM_ERROR 3333		//参数错误
#define ADD_TOCARTERROR 4444	//添加到购物车失败
#define PROCEEDTOCHECKOUT_ERROR 5555	//进入计费中心失败
#define LOGINPAGE_ERROR 6666	//加载登录页面失败
#define GETSNDADDRINPUT_ERROR 7777	//加载填写地址页面失败
#define SETADDR_ERROR 8888		//填写地址失败
#define GETQUERY_ERROR 9999 //加载订单页面失败

#define GETCHANGE_ERROR 11111 //加载修改支付方式页面失败
#define CHANGE_ERROR 22222 //加载修改支付方式页面失败

#define NOSPEEDSEND 33333 //无快递

#define BOOKNOTFOUND 44444 //
#define NOMATCHPRICEBOOK 55555 //

#define ADDR_ERROR 66666 //
#include<vector>
using namespace std;
enum _SendType		
{
	WorkDayOnly = 1,
	HolidayOnly,
	NightOrOther,
	AddDays
};
enum _QueryType
{
	Last30 = 1,
	Months6,
	Year2011
};

class AmazonOrderStru
{
public:
	CString strGoodNos;	//商品ID字符串
	CString strUserID;
	CString strPwd;
	CString strProvince;
	CString strCity;
	CString strTown;	
	CString strRecver;
	CString strAddr;
	CString strPostCode;
	CString strPhone;
	_SendType eSendType;
	bool bBill;
	CString strBillHeader;
	CString strBillContent;
	int iShowFlag; //是否显示
	int iAutoRun;
	int iDebug;	//如果是调试状态,则失败了,不退出程序,可观察
	int iOpFlag;	//1表示查询，2表示下单,3修改支付方式为中国邮政
	_QueryType eQueryType;
	BOOL bCanNormalSend;
CString strLianmengName;
	CString strOrderNo;	//用于修改订单支付方式为邮局
	//_PayType ePayType;
	CString strTaoBaoOrder;
};

typedef struct _BookInfo
{
	CString strNo;	//书号或者商品号,根据iType来定,如果iType是1,则表示商品号,如果是0,则表示书号
	float fPrice;	
}BookInfo;
class XinHuaOrderStr : public AmazonOrderStru
{
public:
	CArray<BookInfo,BookInfo> bookInfo;
	vector<int>iType;
	vector<CString>num;
	vector<CString>res;
	CString strEmail;
	CString xinhuaOrderNum;
	//CString strTaoBaoOrder;
	 

};

class JingDongOrderStr : public AmazonOrderStru
{
public:
	CArray<BookInfo,BookInfo> bookInfo;
	vector<int>iType;
	vector<CString>num;
	vector<CString>res;
	CString strEmail;
	CString xinhuaOrderNum;
	//CString strTaoBaoOrder;


};
#endif
