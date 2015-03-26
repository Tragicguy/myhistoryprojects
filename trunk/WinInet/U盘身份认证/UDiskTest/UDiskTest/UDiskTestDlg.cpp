// UDiskTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDiskTest.h"
#include "UDiskTestDlg.h"
#include <setupapi.h>
#include <WinIoCtl.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GDI_VOLUMENAME 0
#define GDI_VOLUMESERIALNUMBER 1
#define GDI_VOLUMEFILESYSTEM 2
#define GDI_VOLUMETYPE 3
#define GDI_VOLUMESIZE 4
#define GDI_VOLUMEFREESIZE 5
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUDiskTestDlg 对话框




CUDiskTestDlg::CUDiskTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDiskTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDiskTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUDiskTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CUDiskTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUDiskTestDlg 消息处理程序

BOOL CUDiskTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDiskTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDiskTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CUDiskTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY    Query;    
	DWORD dwOutBytes;                
	BOOL bResult;                    

	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	bResult = ::DeviceIoControl(hDevice,            
		IOCTL_STORAGE_QUERY_PROPERTY,            
		&Query, sizeof(STORAGE_PROPERTY_QUERY),    
		pDevDesc, pDevDesc->Size,                
		&dwOutBytes,                            
		(LPOVERLAPPED)NULL);                    

	return bResult;
}*/

#define MEDIA_INFO_SIZE    sizeof(GET_MEDIA_TYPES)+15*sizeof(DEVICE_MEDIA_INFO)

BOOL GetDriveGeometry(const TCHAR * filename, DISK_GEOMETRY * pdg)
{
	HANDLE hDevice;         // 设备句柄
	BOOL bResult;           // DeviceIoControl的返回结果
	GET_MEDIA_TYPES *pmt;   // 内部用的输出缓冲区
	DWORD dwOutBytes;       // 输出数据长度

	// 打开设备
	hDevice = ::CreateFile(filename,           // 文件名
		GENERIC_READ,                          // 软驱需要读盘
		FILE_SHARE_READ | FILE_SHARE_WRITE,    // 共享方式
		NULL,                                  // 默认的安全描述符
		OPEN_EXISTING,                         // 创建方式
		0,                                     // 不需设置文件属性
		NULL);                                 // 不需参照模板文件

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		// 设备无法打开...
		return FALSE;
	}

	// 用IOCTL_DISK_GET_DRIVE_GEOMETRY取磁盘参数
	bResult = ::DeviceIoControl(hDevice,       // 设备句柄
		IOCTL_DISK_GET_DRIVE_GEOMETRY,         // 取磁盘参数
		NULL, 0,                               // 不需要输入数据
		pdg, sizeof(DISK_GEOMETRY),            // 输出数据缓冲区
		&dwOutBytes,                           // 输出数据长度
		(LPOVERLAPPED)NULL);                   // 用同步I/O

	// 如果失败，再用IOCTL_STORAGE_GET_MEDIA_TYPES_EX取介质类型参数
	if (!bResult)
	{
		pmt = (GET_MEDIA_TYPES *)new BYTE[MEDIA_INFO_SIZE];

		bResult = ::DeviceIoControl(hDevice,    // 设备句柄
			IOCTL_STORAGE_GET_MEDIA_TYPES_EX,   // 取介质类型参数
			NULL, 0,                            // 不需要输入数据
			pmt, MEDIA_INFO_SIZE,               // 输出数据缓冲区
			&dwOutBytes,                        // 输出数据长度
			(LPOVERLAPPED)NULL);                // 用同步I/O

		if (bResult)
		{
			// 注意到结构DEVICE_MEDIA_INFO是在结构DISK_GEOMETRY的基础上扩充的
			// 为简化程序，用memcpy代替如下多条赋值语句：
			// pdg->MediaType = (MEDIA_TYPE)pmt->MediaInfo[0].DeviceSpecific.DiskInfo.MediaType;
			// pdg->Cylinders = pmt->MediaInfo[0].DeviceSpecific.DiskInfo.Cylinders;
			// pdg->TracksPerCylinder = pmt->MediaInfo[0].DeviceSpecific.DiskInfo.TracksPerCylinder;
			// ... ...
			::memcpy(pdg, pmt->MediaInfo, sizeof(DISK_GEOMETRY));
		}

		delete pmt;
	}

	// 关闭设备句柄
	::CloseHandle(hDevice);

	return (bResult);

}
void CUDiskTestDlg::ShowMessage(CString strInfo)
{
	AfxMessageBox(strInfo);
}
void CUDiskTestDlg::OnBnClickedOk()
{
	CString strDisk = "";
	GetDlgItemText(IDC_EDIT1,strDisk);
	CString strRet = GetDriverInfomation(strDisk,GDI_VOLUMESERIALNUMBER);
	AfxMessageBox(strRet);
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);

	for (TCHAR* s = szBuf; *s; s += _tcslen(s)+1)
	{
		LPCTSTR sDrivePath = s;
		UINT uType = GetDriveType(sDrivePath);
		if (uType == DRIVE_REMOVABLE)
		{
			DISK_GEOMETRY dg;
			TCHAR szPath[100] = _T("\\\\.\\");
			::_tcscat(szPath,sDrivePath);
			int nSize = ::_tcslen(szPath);
			szPath[nSize-1] = '\0';
			BOOL bRetVal = GetDriveGeometry(szPath,&dg);

			if(dg.MediaType == RemovableMedia)
			{
				//dg.Cylinders
			}
		}
		
	}
	/*
	CString strDisk = "";
	HANDLE hDevice = CreateFile(strDisk, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	CString str = "";
	if (hDevice != INVALID_HANDLE_VALUE)
	{

		PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];

		pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;

		if(GetDisksProperty(hDevice, pDevDesc))
		{
			szMoveDiskName[k] = chFirstDriveFromMask(temp);
			szMoveDiskName[0]=k;
			k++;
			/*ShowMessage("\r\n找到U盘，名称:\r\n");
			p= (char*)pDevDesc;
			str = (pDevDesc->VendorIdOffset ? &p[pDevDesc->VendorIdOffset]:"(NULL)");
			ShowMessage(str);

			ShowMessage("\r\n产品ID:\r\n");
			str=(pDevDesc->ProductIdOffset ? &p[pDevDesc->ProductIdOffset]:"(NULL)");
			ShowMessage(str);
			ShowMessage("\r\n产品版本:\r\n");
			str = (pDevDesc->ProductRevisionOffset ? &p[pDevDesc->ProductRevisionOffset] : "(NULL)");
			ShowMessage(str);
			ShowMessage("\r\n序列号:\r\n");
			str = (pDevDesc->SerialNumberOffset ? &p[pDevDesc->SerialNumberOffset] : "(NULL)");
			ShowMessage(str);
		}

		delete pDevDesc;
		CloseHandle(hDevice);
	}
	*/
}
/*
bool Test()
{
	char   USBSN[23];

	TCHAR   buf[MAX_PATH]   =   {0};
	TCHAR   VID[MAX_PATH]   =   {0};
	TCHAR   PID[MAX_PATH]   =   {0};
	TCHAR   SN[MAX_PATH]   =   {0};
	DWORD   nSize   =   0;
	HDEVINFO   hDevInfo;
	SP_DEVINFO_DATA   DeviceInfoData;
	hDevInfo   =   SetupDiGetClassDevs((LPGUID)0,   0,   0,   (DIGCF_PRESENT|DIGCF_DEVICEINTERFACE));
	if   (hDevInfo   ==   INVALID_HANDLE_VALUE)
	{
		//   no   found   USB   device
		return   false;
	}
	DeviceInfoData.cbSize   =   sizeof(SP_DEVINFO_DATA);
	for   (int   j=0;SetupDiEnumDeviceInfo(hDevInfo,   j,   &DeviceInfoData);j++)
	{
		if   (   !SetupDiGetDeviceInstanceId(hDevInfo,   &DeviceInfoData,   buf,   sizeof(buf),   &nSize)   )
			continue;


		for   (int   i=0;   i <4;   i++)
		{
			VID[i]   =   buf[8+i];
		}

		for   (int i=0;   i <4;   i++)
		{
			PID[i]   =   buf[17+i];  
		}


		for   (int i=0;   i <20;   i++)
		{
			USBSN[i]   =   buf[22+i];
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return true;
}
*/
/************************************************************
//函数名称：GetDriverInfomation
//函数功能：得到驱动器的信息
//参数1：bAlpha - BYTE型，驱动器的代号A-Z(a-z)
//参数2：iGTI_TYPE - int，将要获取的驱动器信息类型
// GDI_VOLUMENAME 得到驱动器名字
// GDI_VOLUMESERIALNUMBER 得到驱动器序列号
// GDI_VOLUMEFILESYSTEM 得到驱动器文件系统
// GDI_VOLUMETYPE 得到驱动器类型
// GDI_VOLUMESIZE 得到驱动器总大小
// GDI_VOLUMEFREESIZE 得到驱动器剩余大小
//返回值：CString型，所要得到的驱动器信息的字符串表达
//例子：得到C盘的名字GetDriverInfomation('C',GDI_VOLUMENAME)
************************************************************/

CString CUDiskTestDlg::GetDriverInfomation(CString bAlpha,int iGTI_TYPE)
{
CString strResult = _T("");
{
/**********获取驱动器名字、序列号和文件系统部分**********/
CString strRootPathName;
strRootPathName.Format(_T("%s:\\"),bAlpha);
LPCTSTR lpRootPathName = strRootPathName;
LPTSTR lpVolumeNameBuffer = new char[_MAX_FNAME];
DWORD nVolumeNameSize = _MAX_FNAME;
DWORD nVolumeSerialNumber = 0;//便于驱动器无效时做判断
DWORD nMaximumComponentLength;
DWORD nFileSystemFlags;
LPTSTR lpFileSystemNameBuffer = new char[20];//文件系统(NTFS,FAT)多大有定义好的宏吗
DWORD nFileSystemNameSize = 20;
GetVolumeInformation(
lpRootPathName,
lpVolumeNameBuffer,
nVolumeNameSize,
&nVolumeSerialNumber, 
&nMaximumComponentLength,
&nFileSystemFlags,
lpFileSystemNameBuffer,
nFileSystemNameSize);

/**********获取驱动器类型部分**********/
CString strDriveType;

/**********获取驱动器总大小和剩余大小部分**********/
LPCTSTR lpDirectoryName = new char[2];
lpDirectoryName = (LPCTSTR)strRootPathName.Mid(0,2);
_ULARGE_INTEGER FreeBytesAvailable,TotalNumberOfBytes,TotalNumberOfFreeBytes ;

__int64 iVolumeSize = 0,iVolumeFreeSize = 0;

GetDiskFreeSpaceEx(strRootPathName.Mid(0,2),&FreeBytesAvailable,&TotalNumberOfBytes,&TotalNumberOfFreeBytes );
iVolumeSize = TotalNumberOfBytes.QuadPart / 1024 / 1024;
iVolumeFreeSize = FreeBytesAvailable.QuadPart / 1024 / 1024;

/**********根据参数得出响应的驱动器信息**********/
switch(iGTI_TYPE)
{
case GDI_VOLUMENAME:
if (lpVolumeNameBuffer != NULL)
strResult.Format(_T("驱动器 %c 的名字为:%s."),bAlpha,lpVolumeNameBuffer);
else
strResult.Format(_T("驱动器 %c 的名字为:%s."),bAlpha,lpVolumeNameBuffer);
//strResult.Format(_T("获取驱动器名字失败!"));
break;
case GDI_VOLUMESERIALNUMBER:
if (nVolumeSerialNumber != 0)
strResult.Format(_T("%X"),nVolumeSerialNumber);
else
strResult.Format(_T(""));
break;
case GDI_VOLUMEFILESYSTEM:
if (lpFileSystemNameBuffer != NULL)
strResult.Format(_T("驱动器 %c 的文件系统为:%s."),bAlpha,lpFileSystemNameBuffer);
else
strResult.Format(_T("获取驱动器文件系统失败!"));
break;
case GDI_VOLUMESIZE:
if (iVolumeSize != 0)
strResult.Format(_T("驱动器 %c 的总大小为:%.2fGB."),bAlpha,(float)iVolumeSize/1024);
else
strResult.Format(_T("获取驱动器总大小失败!"));
break;
case GDI_VOLUMEFREESIZE:
if (iVolumeFreeSize != 0)
strResult.Format(_T("驱动器 %c 的剩余大小为:%.2fGB."),bAlpha,(float)iVolumeFreeSize/1024);
else
strResult.Format(_T("获取驱动器剩余大小失败!"));
break;
case GDI_VOLUMETYPE:
switch(GetDriveType(lpRootPathName))
{
case DRIVE_UNKNOWN:
strDriveType = _T("未知类型!");
break;
case DRIVE_NO_ROOT_DIR:
strResult = _T("获取驱动器类型时参数设置错误!");
return strResult;
case DRIVE_REMOVABLE:
strDriveType = _T("可移动磁盘");
break;
case DRIVE_FIXED:
strDriveType = _T("硬盘");
break;
case DRIVE_REMOTE:
strDriveType = _T("远程设备");
break;
case DRIVE_CDROM:
strDriveType = _T("CD");
break;
case DRIVE_RAMDISK:
strDriveType = _T("RAM");
break;
default:
strResult = _T("未知错误!");
return strResult;
}
strResult.Format(_T("驱动器 %c 的类型为:%s."),bAlpha,strDriveType);
break;
default:
strResult = _T("获取驱动器信息时参数设置错误!");
break;
}

}
return strResult;
}