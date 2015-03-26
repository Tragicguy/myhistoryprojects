#if !defined(MSGDEFINES)
#define MSGDEFINES

//------以下宏用于开发人员进行调试,发布模式下编译时，以下宏必须都屏蔽---------
//------调试宏开始--------
//#define TEST_REMOTIE_CONNECT	//用于调试远程，会话连接后循环关闭TCP，建立文件传输
//#define DISABLE_HTML_NAVIGATE //用于调试是否执行内嵌页面跳转,排除内嵌页面内存泄露
//------调试宏结束--------

// RTS Channels
#define SUPPORT_CHANNEL 0
#define	DEMO_CHANNEL 1
#define FILE_CHANNEL 2
#define CHAT_CHANNEL 3
#define VOIP_CHANNEL 4

// RTS Wrapper Messages
#define MYWM_NOTIFYICON (WM_APP + 100)
#define MYWM_EXIT (WM_APP + 110)
#define MYWM_KILLWALLPAPER (WM_APP + 120)
#define MYWM_RESTOREWALLPAPER (WM_APP + 130)
#define MYWM_MULTIPLEXER  (WM_APP + 150)
#define MYWM_SERVERREJECTED (WM_APP + 160)
#define MYWM_IDLE_TIMEOUT (WM_APP + 170)
#define MYWM_IDLE_TIMEOUT_HANDLED (WM_APP + 171)
#define MYWM_WEBACCESSFAILED (WM_APP + 180)
#define MYWM_STATUS_SYNCHRONIZED (WM_APP + 181)
#define MYWM_SYSTEMKICKOUT (WM_APP + 182)
#define MYWM_NOTSESSION (WM_APP + 190)
#define MYWM_FAILEDWEBACCESSTHREAD (WM_APP + 200)
#define MYWM_FAILEDSTARTWEBACCESS (WM_APP + 210)
#define MYWM_INVALIDSESSIONSTATE (WM_APP + 220)
#define MYWM_STOP (WM_APP + 230)
#define MYWM_FAILEDINITIALIZEWEBACCESS (WM_APP + 240)
#define MYWM_CLIENTSTOPPED (WM_APP + 250)
#define MYWM_SESSIONEND (WM_APP + 260)
// RTSTransaction messages
#define MY_NO_VIEWONLY (WM_APP + 261)
#define MY_BACK_TO_VIEWONLY (WM_APP + 262)
#define MY_CONNECTION_PROFILE (WM_APP + 263)

#define MYWM_FILERESPONSE (WM_APP + 270)
#define MYWM_SUPPORTREQUEST (WM_APP + 280) 
#define MYWM_REMOTINGRESPONSE (WM_APP + 290)
#define MYWM_WEBSTARTCHAT (WM_APP + 300)
#define MYWM_WEBTERMINATESESSION (WM_APP + 310)
#define MYWM_SESSIONENDFROMWEB (WM_APP + 320)
#define MYWM_CLIENTREBOOTING (WM_APP + 330)
#define MYWM_CLIENTREBOOTED (WM_APP + 331)
#define MYWM_VOIPRESPONSE (WM_APP + 340)
#define MYWM_VOIPREJECT (WM_APP + 350)
#define MYWM_SUPPORTERSTATUS (WM_APP + 360)
#define MYWM_ADJUSTPIXEL (WM_APP + 370)
#define MYWM_ADJUSTPIXELSTOPPED (WM_APP + 380)
#define MYWM_TIPMESSAGESTOPPED (WM_APP + 390)

#define MYWM_TIPMESSAGECLOSED (WM_APP + 400)
#define MYWM_WEBACCOUNTRATE (WM_APP + 410)
#define MYWM_DIRECTCONNECTION (WM_APP + 420)
#define MYWM_WAITINGRESPONSE (WM_APP + 430)
#define MYWM_CHECKCLIENTPATH (WM_APP + 440)
// Add by ZhuJie 2008-07-07.
#define MYWM_BREAKCONNECTION (WM_APP + 450)

#define MYWM_OPENSCWEBPAGE (WM_APP + 500)
#define MYWM_OPENMGWEBPAGE (WM_APP + 510)
#define MYWM_SYSMENUREQUEST	(WM_APP + 520)
#define MYWM_SWITCHREQUEST (WM_APP + 530)
#define MYWM_SWITCHRESPONSE (WM_APP + 540)
#define MYWM_WEBSWITCHREQUEST (WM_APP + 550)
#define MYWM_WEBSWITCHRESPONSE (WM_APP + 560)
#define MYWM_QUEUELISTRESPONSE (WM_APP + 570)
#define MYWM_SENDCHATFAILED (WM_APP + 580)
#define MYWM_ENDUSERSWITCHING (WM_APP + 590)

#define MYWM_STARTPIXELSETTING (WM_APP + 660)
#define MYWM_STOPEUCHAT (WM_APP + 670)
#define MYWM_UPDATEEUBUTTON (WM_APP + 690)

#define MYWM_UPDATESTATUS (WM_APP + 700)
#define MYWM_UPDATEFILETRANSFER (WM_APP + 710)
#define MYWM_UPDATEEULISTSTATUS (WM_APP + 720)
#define MYWM_UPDATESPLIST (WM_APP + 730)
#define MYWM_UPDATEGPLIST (WM_APP + 731)
#define MYWM_UPDATESPCHATLIST (WM_APP + 740)
#define MYWM_UPDATEEUCHATLIST (WM_APP + 750)
#define MYWM_UPDATEEULIST (WM_APP + 760)
#define MYWM_SYSMENUSTATUS	(WM_APP + 770)
#define MYWM_UPDATEMENUSTATUS (WM_APP + 780)
#define MYWM_COMPARECONFIG	(WM_APP + 790)

#define MYWM_SERVICERESPONSE (WM_APP + 810)
#define MYWM_RTSCOMMANDSENT (WM_APP + 820)
#define MYWM_SUPPORTLOGRESPONSE (WM_APP + 830)
#define MYWM_PRODLISTRESPONSE (WM_APP + 840)
#define MYWM_ENDUSERINFORESPONSE (WM_APP + 860)
// Add by ZhuJie 2008-07-09. Begin
#define MYWM_WEBCOMMAND (WM_APP + 880)
// Add by ZhuJie 2008-07-09. End

// WM_APP + 900 == 20004
#define MYWM_SERVICECODE (WM_APP + 910)
#define MYWM_INFORMEUMESSAGE (WM_APP + 920)

// Message about keyboard hook
#define MYWM_STOPCONTROLHOOK (WM_APP + 1000)

// Message about system information
#define MYWM_GETSYSTEMINFORMATION (WM_APP + 1100)

// Message about Machine information
#define MYWM_MACHINEINFO_REQUEST (WM_APP + 1101)
#define MYWM_MACHINEINFO_RESPONSE (WM_APP + 1102)
#define MYWM_HIDEWEBINFO (WM_APP + 1103)

#define MYWM_SETWEBTITLE (WM_APP + 1104)
#define MYWM_SETLANGUAGEID (WM_APP + 1105)
#define MYWM_STARTREPORTONLINESTATUS (WM_APP + 1106)
#define MYWM_SHOWGPCHATDLG	(WM_APP + 1107)

// Welcome page
#define MYWM_WELCOMEPAGE_EXIT (WM_APP + 1200)

//Closed to icon
#define MYWM_MINIMUMTOICON (WM_APP + 1300)

//Upgrade information
#define MYWM_DOWNLOADSTATE (WM_APP + 1400)
#define MYWM_STARTDOWNLOAD (WM_APP + 1410)
#define MYWM_DOWNLOADPROGRESS (WM_APP + 1420)
#define MYWM_COPYFILEFINISHED (WM_APP + 1430)
#define MYWM_RUNFILESTATE	(WM_APP + 1440)
#define MYWM_DOWNLOADSPEED (WM_APP + 1441)

//Capture information
#define MYWM_SHOWSCREENCAPTURE (WM_APP + 1500)
#define MYWM_SENDSCREENCAPTURE (WM_APP + 1501)
#define MYWM_LOADSCREENCAPTURE (WM_APP + 1501)
#define MYWM_SAVESCREENCAPTURE (WM_APP + 1502)

#define MYWM_STARTSUPERCONTROL (WM_APP + 1550)

// 1 to n chat.
#define MAX_EUBUTTON 5
#define MYWM_UPDATECHATSESSIONS (WM_APP + 1600)
#define MYWM_UPDATECHATLISTBYSP (WM_APP + 1610)
#define MYWM_FREECHANNEL (WM_APP + 1630)
#define MYWM_SERVICELOGEXIT (WM_APP + 1640)

//Define a Message to show Main dialog
#define SHOW_MAIN_DIALOG (WM_APP + 9999)

//***Add by Pan peng 2008-11-11
#define	MYWM_ASSIGNREQUEST (WM_APP + 10000)
#define MYWM_REJECTASSIGNQUEUEUSER (WM_APP + 10001)
#define MYWM_ACCEPQUEUEDUSER (WM_APP + 10003)
#define MYWM_ASSIGNQUEUEUSER (WM_APP + 10004)


//2008-12-06
#define MYWM_VNCERROR (WM_APP + 10005)

//2009-1-14
#define MYWM_PIXELDLGCLOSE (WM_APP + 10007)
#define MYWM_ADDPRECHATRECORD (WM_APP + 10006)
#define MYWM_SHOWSERVICELOGDLG (WM_APP + 10007)
#define MYWM_CHECKMSNMSG (WM_APP + 10008)
#define MYWM_FLASHICON (WM_APP + 10009)
#define MYWM_SHOWQUUEMSNDLG (WM_APP + 10010)
#define MYWM_MSNTIMEOUT	100
#define MYWM_MSNCLOSE	101
#define MYWM_SAVESERVICEMODE (WM_APP + 10011)
#define MYWM_GETCHATNUM (WM_APP + 10012)


#define MYWM_WRITEASSIGNHISTORY	(WM_APP + 10013)
#define MYWM_LEAFAFTERDONE (WM_APP + 10014)
#define MYWM_GETCONFIGPARAMS (WM_APP + 10015)


#define MYWM_READWEBCHATCOMMANDFAILD (WM_APP + 10016)
#define MYWM_READWEBCHATCOMMANDSUCCESS (WM_APP + 10017)
#define MYWM_WRITEWEBCHATCOMMANDFAILD (WM_APP + 10018)
#define MYWM_WRITEWEBCHATCOMMANDSUCCESS (WM_APP + 10019)
#define MYWM_SETWEBPARAM (WM_APP + 10020)

#define MYWM_ADDGROUPID (WM_APP + 10025)

#define MYWM_REJECTASSIGNQUEUEUSERAWAY (WM_APP + 10026)
#define MYWM_MAINMENUSHOWMSG (WM_APP + 10027)

#define WM_STARTTHREADS (WM_APP + 10028)
#define MYWM_SHOWFIRSTLOGINDLG (WM_APP + 10029)

#define MYWM_NETTESPEEDLOW (WM_APP + 10030)
#define MYWM_SHOWASSIGNWND (WM_APP + 10031)

#define MYWM_STARTWEBCHATEND (WM_APP + 10032)
#define MYWM_ACEEPASSINGUSEROK (WM_APP + 10033)

#define MYWM_ADDCOMMANDXML (WM_APP + 10034)

#define MYWM_REFRESHGPMEMBERLIST (WM_APP + 10035)
#define MYWM_SHOWINVITEMSN (WM_APP + 10036)
#define MYWM_CLIENTREQUEST (WM_APP + 10037)
#define MYWM_GPSPCHAT (WM_APP + 10038)
#define MYWM_LOADPOPEDOM (WM_APP + 10039)
#define MYWM_SHOWNEWMESSAGE (WM_APP + 10040)
#define MYWM_SHOWMESSAGEPAGE (WM_APP + 10041)
#define MYWN_INVITECUSTOMER (WM_APP+10042)
#define MYWM_WEBINFO_SIZE (WM_APP + 10040)

#define MYWM_STARTDEMODRAW (WM_APP + 10050)

#define MYWM_SETDISPLAYSETTINGMSG (WM_APP + 10051)


#define WM_SAVEBITMAP (WM_APP + 15000)
#define WM_SENDBITMAP (WM_APP + 15001)

#define MYWM_REJECTMSG (WM_APP + 15002)
#define MYWM_DELETENODE (WM_APP + 15003)

#define MYWM_DISPATCHCOMMAND (WM_APP + 16000)

#define MYWM_RTSCLIENTEXITED (WM_APP + 17000)
#define MYWM_DEMOAGENTOK (WM_APP + 17001)
#define MYWM_LOCALAGENTFAILED (WM_APP + 17002)
#define MYWM_RECORDTIMER (WM_APP + 17003)
#define MYWM_STARTMULTIPLEXER (WM_APP + 17004)
#define MYWM_STARTDIRECTCONNECT (WM_APP + 17005)
#define MYWM_STARTCOMSVRCONNECT (WM_APP + 17006)
#define MYWM_GETCONNECTINFO (WM_APP + 17007)
#define MYWM_ADDCOMMANDLOGFAILED (WM_APP + 17008)
#define MYWM_REPORTMODE	(WM_APP + 17009)

#define WM_VNCERRORMSG (WM_APP + 20000)
#define MYWM_OPENWEBWINDOW (WM_APP + 20001)
#define MYWM_SETSURVEY (WM_APP + 20002)

#define MYWM_RESUCEERROR (WM_APP + 20003)
#define MYWM_RESUCE_WAITMSG (WM_APP + 20004)

#define  MYWM_SPPHOTO_DOWNLOAD (WM_APP + 20005)
#define  MYWM_GET_VERSIONTEXT (WM_APP + 20006)

#define WM_NOTIECE_VNCHANDLE (WM_APP + 25654)
#define WM_NOTICE_VNCSTATUS (WM_APP + 25655)
#define MYWM_SAVESECURITYPATH (WM_APP + 25656)

#define DEFAULTMSNTIMEOUT 20

#define STATUS_NONE 'X'
#define COMMAND_NONE 'X'
#define SERVICEMODE_NONE 0
#define RTSMODE_NONE 0

#define RTS_HELP_PROCESSFLAG "come_from_web1800_help"

enum VncErrorType
{
	InitDesktopFail = 0, // in function Startup
	InitBitmapFail,
	ThunkBitmapInfoFail,
	SetPixFormatFail,
	CreateBuffersFail,
	SetPixShiftsFail,
	SetPaletteFail,
	InitWindowFail,
	GlobalAddAtomFail,
	ShutdownFail = 10,//in function CheckUpdates
	StartupFail
};


class CSession
{
public:
	enum ESessionStatus
	{
		Out = 'A',
		Downloading = 'B',
		Ready = 'C',
		Pending = 'D',
		Busy = 'E',
		Stopping = 'F',
		// SessionHolding = 'G',
		None = 'X',
		Unknown = 'Z',
		Leaving = 'L',
	};
};

class CSupporter
{
public:
	enum ESupporterStatus
	{
		SignIn = 'A',
		Pending = 'B',
		Accepted = 'C',
		Busy = 'D', 
		IdledOut = 'E',
		Rejected = 'F',
		Stopping = 'G',
		SessionEnd = 'H',
		SessionHolding = 'I',
		ClosingSession = 'J',
		Waiting = 'K',
		Listening = 'L',
		DirectConnectionFailed = 'M',
		DirectConnecting = 'N',
		None = 'X',
		Downloading = 'Z',
	};
};

class CEndUser
{
public:
	enum EEndUserStatus
	{
		Ready = 'A',
		Stopping = 'B',
		Downloading = 'C',
		Rebooting = 'D',
		Waiting = 'E',
		Listening = 'F',
		DirectConnecting = 'G',
		None = 'X'
	};
};

class CWebCommand
{
public:
	enum EWebCommand
	{
		StartWebChat = 'C',	// For Supporter Fly-Mode only
		StopRemoting = 'C', // For Supporter installed version
		StartRemoteDesktop = 'D',
		StartRemoteDemo = 'E',
		StartRemoteControl = 'F',
		RejectRequest = 'G',
		TransferRequest = 'H',
		AcceptTransfer = 'I',
		RejectTransfer = 'J',
		AcceptRemoteDesktop = 'K',
		RejectRemoteDesktop = 'L',
		AcceptRemoteDemo = 'M',
		RejectRemoteDemo = 'N',
		AcceptRemoteControl = 'O',
		RejectRemoteControl = 'P',
		StopRemoteControl = 'Q',
		SuspendService = 'U',
		ResumeService = 'V',
		StopService = 'S',
		RtsClientStarted = 'W',
		TerminateSession = 'T',
		StartFileTransfer = 'R',
		AcceptFileTransfer = 'Y',
		RejectFileTransfer = 'Z',
		RequestMachineInfo = '1',
		AcceptMachineInfo = '2',
		RejectMachineInfo = '3',
		CancelRemoting = '9', 
		None = 'X',
		//***Add by pan peng 2008-12-06 for voice.
		RequestVoice = 'a',
		AcceptVoice = 'b',
		RejectVoice = 'c',
		NotOnlineVoice = 'd',			//not Login.
		StopVoice = 'e',				//stop voice.
		CreateVoiceHandFaild = 'f',		//Create HandShake faile.
		TestVoiceFaild = 'g',			//Test Voice hardware faile.
		OpenVoiceDeviceFailed = 'h',	//test device of voice failed.
		GetVoiceInfoFailed = 'i',		//get information from webservice failed.
		InitVoiceFailed = 'j',		
		StopFileTransfer = 'k',
		InitVoice = 'l', 
		InitVoiceOK = 'm', 
		ScreenCapture = 'n',
		MicroPointInstalled = 'o',
		AcceptVoiceFiled = 'p',
		CancelFileTransfer = 'q',
		ClientStarted = 'r',
		VNCError = 's',
		RtsClientExit = 't',
		ClientSurvey = 'u',
		RequestReboot = 'v',
		AcceptReboot = 'w',
		RejectReboot = '4',
		CancelReboot = 'y',
		Rebooting = 'z',
		RebootOk = 'A',
		RebootFaile = 'B',
		RebootRegistFaile = '0',
		RebootCreateShotCutFaile = '5',
		ClientWaitingVoicePackage = '6',
		ClientDownloadedVoicePackage = '7',
		ClientTimeoutExit = '8',
		StartRemoting = '~',
		RestartRemoting = '!'
		//end***
	};
};

typedef enum RemoteCommand
{
	DirectConnect = 'A',
	DirectConnectFaild = 'B',
	UseComserverConnect = 'C',
	UserComserverConnectFailed = 'D',
	UseUdpConnect = 'E'
}RemoteCommand;

class CServiceMode
{
public:
	enum EServiceMode
	{
		None = 0,
		RemoteDesktop = 1,
		RemoteControl = 2,
		Demonstration = 4
	};
};

typedef enum SessionState
{
        SS_None = 0, 
        SS_Starting, 
        SS_Started, 
        SS_Stopped,
        SS_SessionTimeout, 
		SS_StartedUnattendedClient
} SessionState; 


class CRTSMode
{
public:
	enum ERTSMode
	{
		None = 0,
		RTSClient,
		RTSServer,
		RTSServerLite
	};
};

class CRTSType
{
public:
	enum ERTSType
	{
		RTSServer = 0,
		RTSClient = 1,
		RTSService = 2
	};
};

class CRTSDataType
{
public:
	enum ERTSDataType
	{
		RTSNone = 0,
		RTSString,
		RTSBoolean,
		RTSInteger,
		RTSBase64Binary
	};
};

class CMachineInfoType
{
public:
	enum EMachineInfoType
	{
		MIT_All = 0,
		MIT_SystemInfo = 11,
		MIT_CPUInfo = 12,
		MIT_MemoryInfo = 13,
		MIT_LogicalDiskInfo = 14,
		MIT_DisplayInfo = 15,
		MIT_NetworkInfo = 16,
		MIT_SoftwareInfo = 21,
		MIT_StartupInfo = 22,
		MIT_ProcessInfo = 23,
		MIT_ServicesInfo = 24
	};
};

class QueryInfoResult
{
public:
	enum _QueryInfoResult
	{
		AssignQueueInfo = 1,
		ChatSessions = 2,
		MsnMsgList = 4,
		SpChatList = 8, 
		ChatListBySp = 16, 
		GroupList = 32,
		SupporterInfo = 64, 
		StaffConfigList = 128,
		ServiceAllow = 256,
		GetSpGroupList = 512,
	};
};
//common part
const int CFWM_EXIT = WM_USER + 2000;
const int CFWM_PARTNER_EXITED = WM_USER + 2001;
// const int ExitVNCViewerMSG = WM_USER + 2000;

//Server Part (in Client.exe)
const int CreateFileServerSuccessMSG = WM_USER + 1000;
const int CreateFileServerFailMSG = WM_USER + 1001;
const int CreateChatServerSuccessMSG = WM_USER + 1002;
const int CreateChatServerFailMSG = WM_USER + 1003;
const int ChatServerCloseWindowMSG = WM_USER + 1004;
const int CreateVoIPServerSuccessMSG = WM_USER + 1005;
const int CreateVoIPServerFailMSG = WM_USER + 1006;
const int VoIPServerCloseWindowMSG = WM_USER + 1007;
const int FileServerCloseWindowMSG = WM_USER + 1008;

//Client Part (in Server.exe)
const int CreateFileClientSuccessMSG = WM_USER + 1000;
const int CreateFileClientFailMSG = WM_USER + 1001;
const int CreateChatClientSuccessMSG = WM_USER + 1002;
const int CreateChatClientFailMSG = WM_USER + 1003;
const int ChatClientStopMSG = WM_USER + 1004;
const int FileClientStatusMSG = WM_USER + 1005;
const int VoIPClientStopMSG = WM_USER + 1008;
const int CreateVoIPClientSuccessMSG = WM_USER + 1006;
const int CreateVoIPClientFailMSG = WM_USER + 1007;

//VNCViewer Part 
const int CreateVNCViewerSuccessMSG = WM_USER + 1100;
const int StartRSSServerMSG = WM_USER + 1101;
const int StopRSSServerMSG = WM_USER + 1102;
const int CentreWindowMSG = WM_USER + 1103;
const int SetWorkingModeMSG = WM_USER + 1104;
const int SetMainDlgMSG = WM_USER + 1105;
const int StopHelpProcMSG = WM_USER + 1106;
const int SetConfigFileNameMSG = WM_USER + 1107;
const int StopClientMSG = WM_USER + 1108;
const int GenerateHelpMSG = WM_USER + 1109;
const int ConnectionStateMSG = WM_USER + 1200;

// Timers
#define NOTIFICATION_TIMEOUT_TIMER_ID	1
#define WEB_ACCESS_TIMER				2
#define SNOOP_TIMER_ID					3
#define DEMO_TIMER_ID					4
#define TIP_TIMER_ID					5
#define TIP_START_TIMER_ID				6
#define TIP_CLOSE_TIMER_ID				7
#define ICON_FLICKER_TIMER_ID			8
#define ACCESSCODE_TIMER_ID				9
#define RTSDEMO_LIMIT_TIMER_ID			10
#define WEB1800CLIENT_TIMER_ID			11
#define REGISTRATION_TIMER_ID			12
#define SYSTEMINFO_TIMER_ID				13
#define UPGRADE_TIMER_ID				14
#define IDLETIME_ID						15
#define FINDVNCWIN_ID					16
#define REPORTVNCSTATUS_TIMER_ID		17

//***added by pan peng 2008-10-16
#define RECORDWIN_ID					18	//Screen record timer
#define UPLOADFILE_ID				19	//upload avi files timer
#define HAVEQUEUE_ID					20  //queue up timer
#define FLASHTAYICON_ID					21  //Flash the tray icon.
#define CHECKVNC_ID						22	//VNC Checking timer 2008-12-01
#define CHECKQUEUE						23	//2008-12-23
#define VNCCONNECTIMER					24	//2008-12-25
#define ACCEPTQUEUE_ID					25
#define RELEASECHATDLG_ID				26
#define EMPTYWORKINGSET_ID				27	//释放程序占用的虚拟页面
//end ***
// Notification timeout should same as web side notification timeout value -- unit(ms)
#define NOTIFICATION_TIMEOUT			120000
//***Mydified by Pan Peng 2008-11-06 
#define WEB_ACCESS_TIMEOUT				90000	//old 900000 
//end***
#define WEB_ACCESS_INTERVAL				5000
#define SNOOP_TIMEOUT					2000
#define TIP_TIMEOUT						120000
#define TIP_CLOSE_TIMEOUT				10000
#define TIP_START_TIMEOUT				120000
#define ICON_FLICKER_TIMEROUT			500
#define ACCESSCODE_TIMEOUT				20000	//***modify by Pan Peng 2008-11-10 change 10000 to 20000
#define RTSDEMO_LIMIT_TIMEOUT			7200000
#define WEB1800CLIENT_TIMEOUT			100
#define REGISTRATION_TIMEROUT			2000
#define SYSTEMINFO_TIMEROUT				1000
#define UPGRADE_TIMEROUT				500
#define IDLETIME_TIMEOUT				2000
#define FINDVNCWIN_TIMEOUT				1000

// Multiplexer calling timeout -- unit(s)
//#define SPCALLINGTIMEOUT				900
#define SPCALLINGTIMEOUT				300
#define EUCALLINGTIMEOUT				180
#define DIRECTCONNECTIONTIMEOUT			15

// WebAccessManager status timeout -- unit(s)
//#define SPLISTENINGTIMEOUT				900
#define SPLISTENINGTIMEOUT				600
#define EULISTENINGTIMEOUT				30
#define SPWAITINGTIMEOUT				30

// WebStatus for installlable RSTServer
#define CONNECTION_BREAK 100
#define VERSION_OUTDATE 101
#define USER_SUSPEND 102
#define USER_INEXIST 103
#define USER_EXIST 104
#define PASSWORD_WRONG 105
#define USER_LOGIN 106
#define WEB_RESETSTATU 107
// Add by ZhuJie 2008-08-05.
#define USER_CONCURRENT 108
#define CORP_INEXIST 110
#define CORP_OUTDATE 111
#define USER_OUTDATE 200

#define PRT_INT		0
#define PRT_STRING	1
#define PRT_VOID	2


#define  MM_MSGSPLITTER '\n'
#define  MM_MSGINNERSPLITTER '\t'
enum ProxyAuthType {
	proxy_auth_unknown = 0, 
	proxy_auth_basic, 
	proxy_auth_ntlm_auto, 
	proxy_auth_ntlm, 
	proxy_auth_none, 
	proxy_auth_error, 
};

// Proxy settings
typedef struct MyProxySet
{
	bool m_bProxyEnable;
	ProxyAuthType m_ProxyAuthType; 
	bool m_bBeginWithAutoNtlm;
	bool m_bBasicAuthSupported; 
	bool m_bNtlmAuthSupported; 
	char m_strServerAddress[64];
	int m_nServerPort;
	char m_strProxyUser[64];
	char m_strProxyPassword[64];
	char m_strDomainName[64]; 
	MyProxySet(){
		m_bProxyEnable = false;
		m_bBeginWithAutoNtlm = true;
	}
} MyProxySet;

extern MyProxySet gl_MyProxySet;

#define CHINESE "2052"
#define ENGLISH "1033"
#define CLIENT_DLG_HIDE_FLAG "type="
//***Add by Pan peng 2008-11-18, the mode of the current running programe.ticket #970
#define SUPPORT_INSTALL_MODE	0
#define SUPPORT_UNINSTALL_MODE	1
#define CLIENT_INSTALL_MODE		2
#define CLIENT_UNINSTALL_MODE	3
//end***

enum PackageType
{
	T_None = 0,
	T_Demo,
	T_FileTransfer,
	T_Desktop,
	T_Voice,
	T_IniFile
};

#define FF_REBUILDCHANNEL	100
#define FF_REBUILDCHANNELAk 101
#define FF_FILECLOSE		102

typedef enum RTSStatus
{
	Online = 'A',
	Busy = 'B',
	Leave = 'C',
	Offline = 'D'
}RTSStatus;

typedef enum _ESocketType
{
	Sock_Unknown = 0,
	Sock_UseComSvr,
	Sock_TcpConnect,
	Sock_UdtConnect,
	Sock_NoTcpConnect,
	Sock_NoUdtConnect,
	Sock_NoTcpUdtConnect
}ESocketType;

typedef enum _EClientType
{
	Common_Client = 0,
	Rescue_Client,
	Auto_Client
}EClientType;

#define SECURITY_IGNORE_ERROR_MASK  (INTERNET_FLAG_IGNORE_CERT_CN_INVALID   |  \
	INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  \
	SECURITY_FLAG_IGNORE_UNKNOWN_CA        |  \
	SECURITY_FLAG_IGNORE_REVOCATION    )
#endif
