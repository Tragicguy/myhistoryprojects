// ChatLibDefines.h: Common defines for the ChatLib components.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "afxmt.h"

#define DESTINATION_FOLDER "D:\\Web1800S"

//#pragma pack(1)
struct FilePackHeader
{
	short nType;
	short nParam;
};

struct BasicFileInfo
{
	char FileName[MAX_PATH];
	ULONGLONG Size;
	FILETIME CreationTime;
	bool bOverWrite;
	BasicFileInfo()
	{
		memset(FileName, 0, MAX_PATH);
		Size = 0;
		bOverWrite = false;
	}
};

struct FileCount
{
	int nFileCount;
};

struct FileTransProgress
{
	ULONGLONG uDataRead;
};

struct RecvFileResponse
{
	bool ReadToGo;
};

struct FileToSend
{
	char FileName[MAX_PATH];
	ULONGLONG llStart;
	FileToSend()
	{
		memset(FileName, 0, MAX_PATH);
		llStart = 0;
	}
};

struct BasicFileInfoList
{
	BasicFileInfo fileInfo;
	BasicFileInfoList* pNext;
};


//***Add by Pan peng 2008-11-27
struct Driver
{
	char DriverName[MAX_PATH];
	Driver *next;
	Driver()
	{
		next = NULL; 
		memset(DriverName, 0, MAX_PATH);
	}
};

struct RemoteFolderPath
{
	char RemotePath[MAX_PATH];
	RemoteFolderPath(){ memset(RemotePath, 0, MAX_PATH); }
};

struct FileRequest
{
	char FilePath[MAX_PATH];
	char FileName[MAX_PATH];
	FileRequest()
	{
		memset(FilePath, 0, MAX_PATH); 
		memset(FileName, 0, MAX_PATH);
	}
};

typedef enum State
{ 
	Sending = 0, 
	Receiving = 1, 
	CRequestSend = 2, 
	FinishSend = 3, 
	FinishReceive = 4, 
	CRejectReceive = 5,
	SRejectReceive = 6,
	StopSend = 7,
	StopReceive = 8,
	SRequestSend = 9,
	RequestGet = 10,
	RejectGet = 11
} State;

//#pragma pack()


extern CCriticalSection gl_ActiveSection;
extern bool gl_bActive;


//end***

#define CMD_FILEINFO			0x07

#define CMD_FILEFINISH			0x05
#define CMD_FILEABORT			0x06

#define FILE_REQUEST			0x01
#define FILE_RESPONSE			0x02

#define CMD_PARAM_TESTCONNECTION			0x11

#define CMD_PARAM_LISTFILE					0x12
#define CMD_PARAM_TRANSFILE_C2S				0x13
#define CMD_PARAM_TRANSFILE_S2C				0x14
#define CMD_PARAM_TRANSFILE_C2S_DATA		0x15
#define CMD_PARAM_TRANSFILE_S2C_DATA		0x16
#define CMD_PARAM_TRANSFILE_C2S_STOP		0x17
#define CMD_PARAM_TRANSFILE_C2S_FINISH		0x18
#define CMD_PARAM_TRANSFILE_S2C_FINISH		0x19
#define CMD_PARAM_TRANSFILE_S2C_STOP		0x20
#define CMD_PARAM_TRANSFILE_S2C_RESPONSE	0x21


//***Add by Pan peng 2008-11-27
#define CMD_PARAM_DRIVER					0x22
#define CMD_PARAM_GETFOLDERFILES			0x23
//2008-12-01
#define CMD_PARAM_TRANSFILE_C2S_ACEEPT		0x24
#define CMD_PARAM_TRANSFILE_C2S_REJECT		0x25
#define CMD_PARAM_TRANSFILE_S2C_ACEEPT		0x26
#define CMD_PARAM_TRANSFILE_S2C_REJECT		0x27

#define CMD_PARAM_TRANSFILE_C2S_REQUEST		0x28
#define CMD_PARAM_TRANSFILE_S2C_REQUEST		0x29

#define CMD_PARAM_TRANSFILE_C2S_FAILED		0x30
//end***

//Begin YFW 2008-12-24 Interaction for sending file from client to server
#define CMD_PARAM_TRANSFILE_C2S_QUERYSTATUS	0x31
#define CMD_PARAM_TRANSFILE_C2S_STATUSOK	0x32
#define CMD_PARAM_TRANSFILE_C2S_STATUSERROR	0x33
//End  YFW 2008-12-24 


#define CMD_PARAM_TRANSFILE_S2C_SENDFILEREQUEST	0x34
#define CMD_PARAM_TRANSFILE_S2C_SENDFILEACCEPT  0x35
#define CMD_PARAM_TRANSFILE_S2C_SENDFILEREJECT	0x36
#define CMD_PARAM_TRANSFILE_SSTOPRECEIVE		0x37
#define CMD_REJECTWHENNOTFREE					0x38


#define WM_FILE_LISTFILE_MESSAGE (WM_USER + 100)
#define WM_FILE_SENDFILETOSERVER_MESSAGE (WM_USER + 101)
#define WM_FILE_SENDFILETOSERVER_FINISH_MESSAGE (WM_USER + 102)
#define WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE (WM_USER + 103)
#define WM_FILE_GETLOCALPATH_MESSAGE (WM_USER + 104)
#define WM_FILE_REFRESHLOCAL_MESSAGE (WM_USER + 105)
#define WM_FILE_SETRANGE_MESSAGE (WM_USER + 106)
#define WM_FILE_GETRECVSTATUS_MESSAGE (WM_USER + 107)
#define WM_FILE_ENABLEBUTTON_MESSAGE (WM_USER + 108)
#define WM_FILE_DISABLEBUTTON_MESSAGE (WM_USER + 109)
#define WM_FILE_ADDLOG_MESSAGE (WM_USER + 110)
#define WM_FILE_FILETOCLIENTFINISH_MESSAGE (WM_USER + 111)
#define WM_FILE_FILETOSERVERSTOP_MESSAGE (WM_USER + 112)
//***Add by Pan peng 2008-11-28
#define WM_FILE_LISTDRIVERS_MESSAGE (WM_USER + 113)
#define WM_FILE_SENDFILETOSERVER_FINISHED_MESSAGE (WM_USER + 114)
#define	WM_SHOWSTOPBUTTON (WM_USER + 115)
#define WM_CLOSECURRENTFILE (WM_USER + 116)
#define WM_GETSENDFILEPATHNAME (WM_USER + 117)
#define WM_SETSENDING (WM_USER + 118)
#define WM_SETRECEIVE (WM_USER + 119)
#define WM_ADDLISTITEM (WM_USER + 120)
#define WM_SETLISTITEM (WM_USER + 121)
#define WM_GETSTOPSTATE (WM_USER + 122)
#define WM_SETSTOPSTATE (WM_USER + 123)
#define WM_GETSTOPBYMYSEFLT (WM_USER + 124)
#define WM_SETSTOPBYMYSEFLT (WM_USER + 125)
#define WM_GETCONTINUESTATUS (WM_USER + 126)
#define WM_SETSTSTUS_TEXT (WM_USER + 127)

#define WM_FILE_DESKTOP_MESSAGE (WM_USER + 127)
#define WM_REJECT_SENDREQUEST (WM_USER + 128)
#define WM_FILE_ERROR (WM_USER + 129)
#define WM_SETMSGTEXT (WM_USER + 130)
#define WM_SHOWMSGTEXT (WM_USER + 131)
#define WM_FILE_STOP (WM_USER + 132)

#define WM_MAKETUNNELEND (WM_USER + 133)
#define WM_MAKETUNNEL	(WM_USER + 134)
#define WM_MAKETUNNELWAIT (WM_USER + 135)

#define FF_REBUILDCHANNEL	100
#define FF_REBUILDCHANNELAk 101
#define FF_FILECLOSE		102


//end***
