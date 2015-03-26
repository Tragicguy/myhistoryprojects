#pragma once
#include "ADOFile/AdoRecordSet.h"
#include "ADOFile/AdoCommand.h"
#include "dataType.h"
#include "ASCIIConver.h"

class CDataBaseCtrl
{
public:
	CDataBaseCtrl(void);
	~CDataBaseCtrl(void);
	void InitParams(CString strdbServer,CString strDbName,CString strDbUser,CString strDbPwd,int iDbType,int iDbPort);
	void ReadSqlParams();
	int AddMoMessage(DeliverPack &mo);
	int UpdateSendResult(ReportPack &report);
	int ReadWaitMessage(KfSubmitPackList &sendBufList,CCriticalSection &sendListLock);
	int AddSendResult(KfSubmitPack &mt,int iFlag);
	void InitArray(CString strSrcIndex,int iFlag);	
	int ConvertSendMessage(KfSubmitPack &mt);
	bool IsSingleSend( CString strDestAddr );
private:
	int GetSendArray(CArray <CString, CString> &strSendList,CString strSrc);
	int ConvertPhoneList( CString &strDestAddr );
	int GetMtString(KfSubmitPack &mt,CString strArry[35],int iFlag,int iSucFlag);
	void GetReportString(ReportPack &report,CString strArry[7]);
	void GetMoString(DeliverPack &mo,CString strArry[7]);
	CString MakeSqlString(KfSubmitPack &mt,CString strSqlSrc,int iFlag,int iSucFalg);
	int InitMoStoreParams(CAdoCommand &adoCmd,DeliverPack &mo);
	int InitReportStoreParams(CAdoCommand &adoCmd,ReportPack &report);
	int InitAddResultStoreParams(CAdoCommand &adoCmd,KfSubmitPack &mt,int iFlag);	
	BOOL ConnectDataBase(CAdoConnection &adoConn,int iFlag);	//连接数据库
	BOOL ConnectDataBase(CAdoConnection &adoConn,CAdoRecordSet &adoRst,int iFlag);
private:
	CString m_strPhoneFilePath;			//strPath
	CAdoConnection m_adoConnect;		//读取数据连接
	CAdoRecordSet  m_adoRcrd;			//读取数据记录集
	CAdoCommand m_adoCmd;				//存储过程对象
	CString m_dbServer;		//数据库服务器
	CString m_dbName;		//数据库名
	CString m_dbUser;		//登录账户
	CString m_dbPwd;		//登录密码
	int m_idbType;
	int m_idbPort;

	int m_iOptFlag;		   //数据库交互方式,2自定义,3存储过程

	CString m_strReadSql;				//读取待发信息
	CString m_strReadIndexList;			//待发映射
	CString m_strSucAddResultSql;		//发送后添加发送结果[成功]
	CString m_strSucAddResultIndexList;	//添加发送结果映射[成功]
	CString m_strFailAddResultSql;		//发送后添加发送结果[失败]
	CString m_strFailAddResultIndexList;//添加发送结果映射[失败]
	CString m_strSucCleanSql;			//发送成功更新待发表
	CString m_strSucCleanIndexList;		//成功更新待发表映射
	CString m_strFailCleanSql;			//发送失败更新待发表
	CString m_strFailCleanIndexList;	//失败更新待发表映射
	CString m_strUpdateSql;				//状态报告更新发送结果表
	CString m_strUpdateIndexList;		//状态报告更新映射
	CString m_strAddMoSql;				//保存上行信息	
	CString m_strAddMoIndexList;		//保存上行信息映射

	CString m_strAddResultStoreName;	//存储过程(保存发送历史)
	CString m_strAddResultStoreIndexList;	//存储过程(保存发送历史序号)
	
	//CString m_strReadNumName;			//存储过程中读取时传入的参数名

	int m_iReadIndex[35];
	int m_iSucAddResultIndex[35];
	int m_iFailAddResultIndex[35];
	int m_iSucCleanIndex[35];
	int m_iFailCleanIndex[35];
	int m_iUpdateIndex[7];
	int m_iAddMoIndex[7];
	int m_iAddResultIndex[35];

	CString m_strReadParams[35];
	CString m_strAddResultParams[35];
	CString m_strSucAddResultParams[35];
	CString m_strFailAddResultParams[35];
	CString m_strSucCleanParams[35];
	CString m_strFailCleanParams[35];

	CString m_strUpdateParams[7];
	CString m_strAddmoParams[7];

	int m_iUpdateParamLen[7];
	int m_iAddmoParamLen[7];
	int m_iAddResultParamLen[35];

	CString m_strSplitString;			//分隔符字符串
	
	CString m_strMsgStatus[2];			//信息发送状态	第一位表示成功的状态,第二位表示失败的状态
	int m_iMsgStatus[2];
	CString m_strRecvStatus[5];		//映射后的接收状态
	int m_iRecvStatus[5];

	CString m_strCharSet;			//指定数据库字符集
};
