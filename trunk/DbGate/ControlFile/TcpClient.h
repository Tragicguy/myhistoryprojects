#pragma once
#define  RECV 0
#define  SEND 1
#define MAX_SMS_LENGTH 1024
#include <Afxsock.h>
class CTcpClient
{
public:
	CTcpClient(void);
	~CTcpClient(void);	
	int MongateCsGetStatusReportEx(SOCKET sock, char *strMsg);
	int MongateConnect(const char* pszIP, int iPort, const char* pszAccount, const char* pszPwd);
	int MongateCsGetSmsEx(SOCKET sock, char *strMsg);
	int MongateTestConn(SOCKET sock);
	int MongateQueryUsed(SOCKET sock);
	int MongateQueryBalance(SOCKET sock);
	void MongateDisconnect(SOCKET sock);
	int MongateCsSPSendSms(SOCKET sock, const char* pszMobis, const char* pszMsg, int iMobiCount,const char* pszSubPort,char* pszSN);
	int MGhexChrTosrcStr(const char* srcMsg,char* retMsg);	
	void SetCmdType(int iCmdType = 25);
private:
	int IsCanSendRecv(SOCKET fd, int sec, int flags)/*flags==READ 读等待 */;
	int code_convert(char *from_charset,char *to_charset,const char *inbuf,int inlen,char *outbuf,int outlen);
	void srcStrTohexChr(const char* strMsg,char* retstr);
	char* hexChrTosrcStr(const char* strMsg,char* retstr);
	bool IsNumeric(const char* pszMobi);
	int TelsCheck(const char* pTels,int iTelNum);	
	int m_iOvertime;
	int m_iSendNumber;
	char m_Os[10];        //操作系统版本
	char m_sVersion[6];   //版本号
	int m_iCmdType;
};
