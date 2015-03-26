
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Des.h"
//---------------------------------------------------------------------------
//http
int (__stdcall *MongateConnect)(const char*, int, const char*, const char*);
void (__stdcall *MongateDisconnect)(int);

int (__stdcall *MongateCsGetSmsEx)(int sock, char *strMsg);
int (__stdcall *MongateCsGetSms)(int sock, char strMsg[500][1500]);
int (__stdcall *MongateCsSendSms)(int sock, const char* pszMobis, const char* pszMsg,
                        int iMobiCount, char* pszSN);
//int (__stdcall *MongateTestMsgEx)(SOCKET sock,const char* pStrMessage,char *keywords);
//int (__stdcall *MongateTestMsg)(int sock,char* pStrMessage);
//---------------------------------------------------------------------------

int main(void)
{
  char buf[50000] = {0};
  char bufnew[500] = {0};
  int sock = 0;
  char selected = 0;
  char** option = NULL;
  int res = 0;
  void* handle = NULL;
  char msg[30][1500] = {0};
  int i = 0;
  
  handle = LoadLibrary("MWGateway.dll");
  if (handle == NULL)
  {   
    puts("can't load the dll.program End");
    return 1;
  }
  if (handle == NULL)
  {
    puts("can't load the dll.program End");
    return 1;
  }
  MongateConnect = (void*)GetProcAddress(handle, "MongateConnect");
  MongateCsSendSms      = (void*)GetProcAddress(handle, "MongateCsSendSms");
  MongateCsGetSms         = (void*)GetProcAddress(handle, "MongateCsGetSms");
  MongateDisconnect     = (void*)GetProcAddress(handle, "MongateDisconnect");
  MongateCsGetSmsEx         = (void*)GetProcAddress(handle, "MongateCsGetSmsEx");

  puts("connecting to the server...");
  MongateCsGetSmsEx(-806,buf);
  MD5String(bufnew,buf);
  MongateCsGetSmsEx(-807,bufnew);
  sock = MongateConnect("61.145.229.26", 8018,"13600000412", "778896");
  printf("%d",sock);
  if (sock < 1)
  {
    printf("fail to connect to the server. Error Numbers is %d\n", sock);
	system("Pause");
    return 1;
  }
		res = MongateCsGetSmsEx(sock, buf);
		printf ("sms %d %s\n", res,buf);
	    res = MongateCsSendSms(sock, "15986756631", "Hello, world!", 1, buf);
       if ( res == 1)
        printf("Send success.SN is %s\n", buf);
      else
        printf("Send failed. Error Numbers is %d\n", res);
		res = MongateCsGetSms(sock, msg);
		if (res == 0)
		{
			puts("There is 0 new sms."); 
		}
		else
		{
			for (i = 0; i < res; ++i)
			{
				printf("(%d):  %s\n", i+1, msg[i]);
			}
		}
   MongateDisconnect(sock);
  return 0;
}
