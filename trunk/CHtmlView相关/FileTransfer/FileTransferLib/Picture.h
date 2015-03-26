#pragma once
#include <atlbase.h>

//////////////////
// Picture object--encapsulates IPicture
//
class CPicture {
public:
	CPicture();
	~CPicture();

	// Load frm various sosurces
	BOOL Load(UINT nIDRes, CString strType = _T("Image"));
	BOOL Load(LPCTSTR pszPathName);
	BOOL Load(CFile& file);
	BOOL Load(CArchive& ar);
	BOOL Load(IStream* pstm);

	// render to device context
	BOOL Render(CDC* pDC, CRect rc=CRect(0,0,0,0),
		LPCRECT prcMFBounds=NULL) const;

	CSize GetImageSize(CDC* pDC=NULL) const;

	operator IPicture*() {
		return m_spIPicture;
	}

	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const 
	{
		try
		{
			cx = cy = 0;
			const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Width(&cx);
			ASSERT(SUCCEEDED(m_hr));
			const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Height(&cy);
			ASSERT(SUCCEEDED(m_hr));
		}
		catch(...){}
	}

	void Free() {
		if (m_spIPicture) {
			m_spIPicture.Release();
		}
	}

protected:
	CComQIPtr<IPicture>m_spIPicture;		 // ATL smart pointer to IPicture
	HRESULT m_hr;								 // last error code
};

//Add by Pan peng 2009-06-05
#include <gdiplus.h>
#include "afxwin.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

class CMyPicture
{
private:
	BOOL	m_bInit;
	char    *m_pBuffer;

	UINT	m_nLength;
	CString m_strFileName;

	GdiplusStartupInput m_gdiplusStartupInput; 
	ULONG_PTR m_gdiplusToken;
public:
	CMyPicture()
	{
		m_bInit = FALSE;
		m_pBuffer = NULL;
		m_nLength = 0;

		GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL); 
		CoInitialize(NULL);
	}

	~CMyPicture()
	{
		if (m_pBuffer)
			delete []m_pBuffer;
		GdiplusShutdown(m_gdiplusToken);
		CoUninitialize();
	}
	
	void SetFileName(LPCTSTR strFileName)
	{
		m_strFileName = strFileName;
	}

	void Render(CDC* pDC, CRect rect = CRect(0,0,0,0))
	{
		if(m_pBuffer == NULL)
			return;

		IStream* pIStream = NULL;

		if(CreateStreamOnHGlobal(NULL, FALSE, (LPSTREAM*)&pIStream) != S_OK)
		{
			AfxMessageBox(_T("Failed to create stream on global memory!"));
			return;
		}

		ULONG nWrited = 0;
		if(pIStream->Write(m_pBuffer, m_nLength, &nWrited) != S_OK)
		{
			AfxMessageBox(_T("Failed to write stream on global memory!"));
			return;
		}

		LARGE_INTEGER   li;
		li.u.HighPart=0;   
		li.u.LowPart   =0;   
		pIStream->Seek(li, STREAM_SEEK_SET, 0);   

		Bitmap bitmap(pIStream);
		
		Graphics graphics(pDC->m_hDC);
		graphics.DrawImage(&bitmap,  Gdiplus::Rect(rect.left, rect.top, rect.Width(), rect.Height())); 
		
		pIStream->Release(); 
	}

	BOOL Load(LPCTSTR strFileName)
	{
		try
		{
			m_strFileName = strFileName;

			if (m_strFileName.IsEmpty())
				return FALSE;

			HANDLE hFile = CreateFile(m_strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
			if (INVALID_HANDLE_VALUE == hFile)
			{
				return FALSE;
			}

			DWORD dwFileSize = GetFileSize(hFile, NULL); 
			m_nLength = dwFileSize;
			CloseHandle(hFile);

			CFile file;
			if (!file.Open(m_strFileName, CFile::modeRead))
				return FALSE;
			
			if (m_pBuffer != NULL)
				delete []m_pBuffer;

			m_pBuffer = new char[dwFileSize];
			UINT nRead = file.Read(m_pBuffer, dwFileSize);
			if (nRead == -1 || nRead != dwFileSize)
			{
				file.Close();
				return FALSE;
			}
			
			file.Close();

			m_bInit = TRUE;
			return TRUE;
		}
		catch(...)
		{
		}
		return TRUE;
	}
	
	void Free()
	{

	}
};