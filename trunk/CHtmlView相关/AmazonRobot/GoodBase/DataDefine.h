#ifndef  _ORDER_DEFINE__
#define  _ORDER_DEFINE__
#include <map>
using std::map;
//#include "stdafx.h"
#define MUTEX_DEFINE "_amazonrobot_yutone_copyright"
#define AMAZON_CONFIG_FILE "amazonset.ini"
#define AMAZON_ROBOTNAME "AmazonRobot.exe"
#define FLAG_STRING "source=eqifa|140382|1|"
#define AMAZON_TITILE "%E4%BA%9A%E9%A9%AC%E9%80%8A%E7%BD%91%E7%AB%99"
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

#define NONORMAL -1 //无普邮
#define NOEMS 1 //无EMS
#define NOEXPRESS 2 //无快递
#define NOSUPEREXPRESS 3 //无超级快递
#define CONFIG_ERROR	4
#define PAYTYPE_ERROR	5	//支付类型无效
#define BILL_CONTENT_ERROR 6	//发票类型错
#define NOSUCHPAYTYPE	  7		/*您的配送地址不在MPOS及现金可配送范围内.您选择的送货方式是平邮或国内特快专递（EMS）*/

#define ONLY_ALL_COMPLETE 8	//只能选择\"等待所有商品到货一起发货

#define NOT_ENUGH_GOOD 9	//此商品的购买数量有限
#define ORDER_TIMEOUT 10	//此商品的购买数量有限
#define ORDER_UNKNOWN_STATE 11
#define PARAM_EXCEPTION 12
#define NO_GIFT_CARD 13
#define LOW_GIFTCARD_BALANCE 14
#define BE_SET_QUICK_ORDER 15

#define BOOKNOTFOUND 44444 //
#define NOMATCHPRICEBOOK 55555 //

#define ADDR_ERROR 66666 //
#define SEND_TYPE_ERROR 77777
#define PAY_TYPE_ERROR 88888
enum _SendTime		
{
	NoneSendTime = 0,
	WorkDayOnly,
	HolidayOnly,
	NightOrOther,
	AddDays
};

enum _ShipType		
{
	Normal = 1,
	Ems,
	Express,
	SuperExpress,
	Scheduled
};

enum _QueryType
{
	Last30 = 1,
	Months6,
	Year2011
};

class GoodOrderStru
{
public:
	CString strGoodNos;	//商品ID字符串
	map<CString, int> mapGoodNos;
	CString strUserID;
	CString strPwd;
	CString strProvince;
	CString strCity;
	CString strTown;	
	CString strRecver;
	CString strAddr;
	CString strPostCode;
	CString strPhone;
	
	_ShipType eShipType;
	BOOL bCanNormalSend;
	BOOL bShipWaitType;	//0单独发货, 1等货齐后一并发货
	_SendTime eSendTime;
	CString strSendRemark;
	CString strPayType;	//付款方式ID
	CString strPayTypeEx;	//付款类型

	bool bBill;
	CString strBillHeader;
	CString strBillContent;

	CString strClaimCode;	//优惠码
	BOOL bUseGiftCard;
	int iShowFlag; //是否显示
	int iAutoRun;
	int iDebug;	//如果是调试状态,则失败了,不退出程序,可观察
	int iOpFlag;	//1表示查询，2表示下单,3修改支付方式为中国邮政
	_QueryType eQueryType;	

	
	
	
	CString strOrderNo;	//用于修改订单支付方式为邮局
	//_PayType ePayType;
	GoodOrderStru()
	{
		bCanNormalSend = FALSE;
		bShipWaitType = TRUE;
		eShipType = Express;
		eSendTime = NightOrOther;
		strPayType = "支付宝";
		bBill = false;
		bUseGiftCard = FALSE;
	}
};

typedef struct _BookInfo
{
	CString strNo;	//书号或者商品号,根据iType来定,如果iType是1,则表示商品号,如果是0,则表示书号
	float fPrice;	
}BookInfo;

#endif
