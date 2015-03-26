#include "StdAfx.h"
#include "XlsCtrolClass.h"


//#define TEST_TEST
CXlsCtrolClass::CXlsCtrolClass(void)
{
}

CXlsCtrolClass::~CXlsCtrolClass(void)
{
}

//也是导入
bool CXlsCtrolClass::ImportEx(CString strFile,vector<ExpressStr> &vecStr)
{
	CDatabase database;
	CString sSql;
	CString sItem1, sItem2;
	CString sDriver;
	CString sDsn;
	CString sFile = strFile;             // 将被读取的Excel文件名
	vecStr.clear();
	// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{
		return false;
	}
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	TRY
	{
		// 打开数据库(既Excel文件)
		database.Open(NULL, false, false, sDsn);
		int iSheetCount =1;
		while (true)
		{
			CRecordset adoRst(&database);

			// 设置读取的查询语句.
			sSql = "SELECT * from ";
			sSql.Format("SELECT * from [Sheet%d$]",iSheetCount);
			// 执行查询语句
			try
			{
				adoRst.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
			}			
			catch (CException* e)
			{
				int ii = 0;
				char strMessage[MAX_PATH];
				e->GetErrorMessage(strMessage, sizeof(strMessage) - 1);
				e->Delete();
				CString strError = CString(strMessage);
				if (strError.Find("不是一个有效名称。请确认它不包含无效的字符或标点") >=0 )
				{
					//所有列已经遍历完
					break;
				}
			}

			if (adoRst.IsEOF())		//已经为空了
			{
				break;
			}
			ExpressStr ExpressInfo;
			BookStr bookStr;
			CString strTemp = "";
			//CString strColumnName = adoRst.GetFieldName(0);
			bool bColumnFlag = false;//是否已经读取了列名
			bool bNewStruInfo = false;
			bool bInfoEnd = false;
			bool bReadBookInfo = false;
			while (!adoRst.IsEOF())
			{
				if (bInfoEnd)
				{
					vecStr.push_back(ExpressInfo);
					bNewStruInfo = false;
					bInfoEnd = false;
					ExpressInfo.vecBookInfo.clear();
					bReadBookInfo = false;
				}
				adoRst.GetFieldValue((short)0,strTemp);
				if (strTemp != "")			//OrderNo
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strOrderNo);
					bNewStruInfo = true;
					adoRst.MoveNext();
					continue;
				}
				strTemp = "";
				adoRst.GetFieldValue((short)1,strTemp);		
				if (strTemp == "")		//One Record End
				{
					bInfoEnd = true;
					adoRst.MoveNext();
					continue;
				}
				else if (strTemp == "ISBN")	//Book Info
				{
					bReadBookInfo = true;
					adoRst.MoveNext();
					continue;
				}
				else if (bReadBookInfo)		// add book info 
				{
					bookStr.strISBN = strTemp;
					adoRst.GetFieldValue((short)2,bookStr.strName);
					adoRst.GetFieldValue((short)3,bookStr.strPress);
					adoRst.GetFieldValue((short)4,bookStr.strPrice);
					adoRst.GetFieldValue((short)5,bookStr.strNum);
					adoRst.GetFieldValue((short)6,bookStr.strSellPrice);
					ExpressInfo.vecBookInfo.push_back(bookStr);
				}
				else if (strTemp == "发货地址")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strAddr);
				}
				else if (strTemp == "联 系 人")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strRecver);
				}
				else if (strTemp == "联系方式")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strPhone);
				}
				else if (strTemp == "备    注")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strMemo);
				}
				else if (strTemp == "付款时间")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strPayTime);
				}
				else if (strTemp == "快递名称")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strExpressCompany);
				}
				else if (strTemp == "快递单号")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strExpressNo);
				}
				adoRst.MoveNext();
			}
			if (ExpressInfo.vecBookInfo.size()>0)
			{
				vecStr.push_back(ExpressInfo);
			}
			iSheetCount++;
		}		
		// 关闭数据库
		database.Close();
	}
	CATCH(CDBException, e)
	{
		// 数据库操作产生异常时...
		AfxMessageBox("数据库错误: " + e->m_strError);
	}
	END_CATCH;

	return true;
}

bool CXlsCtrolClass::GetTables(CString strFile,CStringArray &strTableArray)
{
	CAdoConnection adoConn;
	if (!adoConn.IsOpen())
	{
		if(adoConn.ConnectExcel(strFile))
		{
			adoConn.OpenSchema(strTableArray);
			adoConn.Close();
			return true;
		}
		else
		{
			return false;
		}
	}	
}
//添加
bool CXlsCtrolClass::Add(CString strFile,vector<ExpressStr> &vecStr)
{
	return Export(strFile,vecStr,true);
}
//导出
bool CXlsCtrolClass::Export(CString strFile,vector<ExpressStr> &vecStr,bool bAppend)
{
	CDatabase database;
	CString sDriver;
	CString sExcelFile = strFile; 
	CString sSql;	
	
	CString tableName = "Sheet1";

	if (!bAppend)
	{
		if (_access(strFile,0) ==0)
		{
			CFile::Remove(strFile);
		}
	}	
	// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{		
		return false;
	}

	CStringArray strTableArray;
	GetTables(strFile,strTableArray);

	// 创建进行存取的字符串
	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
	CString strTemp;
	int iTemp;
	// 创建数据库 (既Excel表格文件)
	if( !database.OpenEx(sSql,CDatabase::noOdbcDialog))	
	{		
		return false;
	}
	int iCount = 1;
	int iNodeCount = 0;
	int iSheetCount = 1;
	if (!bAppend)
	{
		iSheetCount = 1;
	}
	else //得到所有目前已经存在的表数目
	{
		iSheetCount = strTableArray.GetSize();
	}	
	vector<ExpressStr>::iterator it;
	bool bTableChecked = false;
	bool bExist = false;
	tableName.Format("Sheet%d$",iSheetCount);
	int iSize = vecStr.size();

	for (it=vecStr.begin();it != vecStr.end();)
	{
		if (!bTableChecked)
		{
			bExist = false;
			tableName.Format("Sheet%d",iSheetCount);
			for (int i=strTableArray.GetCount()-1;i>=0;i--)
			{
				if (strTableArray.GetAt(i) == tableName)
				{
					bExist = true;
					bTableChecked = true;
					break;
				}
			}			
		}
		if (!bExist)
		{
			strTemp.Format("a TEXT,b TEXT,c TEXT,d TEXT,e TEXT,f TEXT,g TEXT");
			// 创建表结构			
			sSql = "CREATE TABLE [" + tableName + "] ( " + strTemp +  " ) ";
			int i = 0;
			database.ExecuteSQL(sSql);
			strTableArray.Add(tableName);
			bTableChecked = true;
			bExist = true;
		}
#ifdef TEST_TEST
		if(iNodeCount++ >= 10000)
			break;
#endif
		if (iCount + it->vecBookInfo.size() + 12 >= 32767)
		{

			iCount = 0;
			bTableChecked = false;
			iSheetCount++;
			continue;
		}
		//memcpy(&expressStr,&it,sizeof(expressStr));
		try
		{
			database.BeginTrans();
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"订单编号",it->strOrderNo,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"发货地址",it->strAddr,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"联 系 人",it->strRecver,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"联系方式",it->strPhone,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"备    注",it->strMemo,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"付款时间",it->strPayTime,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"快递名称",it->strExpressCompany,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"快递单号",it->strExpressNo,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"ISBN","书名","出版社","原价","数量","售价");
			database.ExecuteSQL(sSql);
			vector<BookStr>::iterator itEx;
			for (itEx=it->vecBookInfo.begin();itEx!=it->vecBookInfo.end();itEx++)
			{
				sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,"",itEx->strISBN,itEx->strName,itEx->strPress,itEx->strPrice,itEx->strNum,itEx->strSellPrice);
				database.ExecuteSQL(sSql);
				iCount++;
			}
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,"","","","","","","");
			database.ExecuteSQL(sSql);
			database.CommitTrans();
			iCount+=10;			
		}		
		catch (CException* e)
		{
			database.Rollback();
			sSql.Format("delete from [%s] where [a] = '%s'",tableName,it->strOrderNo);
			database.ExecuteSQL(sSql);
			bTableChecked = false;
			iSheetCount++;			
		}
#ifndef TEST_TEST
		it++;
#endif
	}
	return true;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CXlsCtrolClass::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}

//导入
bool CXlsCtrolClass::Import(CString strFile,vector<ExpressStr> &vecStr)
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	vecStr.clear();
	bool bRet = false;
	if (!adoConn.IsOpen())
	{
		if(adoConn.ConnectExcel(strFile))
		{
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		else
		{
			return false;
		}
	}
	int iSheetCount = 1;
	while (true)
	{
	
		CString strSQL = "SELECT * from [Sheet1$]";
		strSQL.Format("SELECT * from [Sheet%d$]",iSheetCount);

		
		if( !adoRst.Open(strSQL, adCmdText))	//找不到了
		{
			//if (strError.Find("不是一个有效名称。请确认它不包含无效的字符或标点") >=0 )
			adoRst.Close();
			break;
		}
		CString strFormat = "";
		CString strPhone = "";
		int nFields  = adoRst.GetFieldsCount();
		if (nFields != 7)
		{
			//AfxMessageBox("文件格式不正确");
			adoRst.Close();
			break;
		}
		ExpressStr ExpressInfo;
		BookStr bookStr;
		CString strTemp = "";
		CString strColumnName = adoRst.GetFieldName(0);
		bool bColumnFlag = false;//是否已经读取了列名
		bool bNewStruInfo = false;
		bool bInfoEnd = false;
		bool bReadBookInfo = false;
		while (!adoRst.IsEOF())
		{
			if (bInfoEnd)
			{
				vecStr.push_back(ExpressInfo);
				bNewStruInfo = false;
				bInfoEnd = false;
				ExpressInfo.vecBookInfo.clear();
				bReadBookInfo = false;
			}			
			strTemp = "";
			adoRst.GetCollect((long)1,strTemp);		
			if (strTemp == "")		//One Record End
			{
				bInfoEnd = true;
				adoRst.MoveNext();
				continue;
			}
			else if (strTemp == "ISBN")	//Book Info
			{
				bReadBookInfo = true;
				adoRst.MoveNext();
				continue;
			}
			else if (bReadBookInfo)		// add book info 
			{
				bookStr.strISBN = strTemp;
				adoRst.GetCollect((long)2,bookStr.strName);
				adoRst.GetCollect((long)3,bookStr.strPress);
				adoRst.GetCollect((long)4,bookStr.strPrice);
				adoRst.GetCollect((long)5,bookStr.strNum);
				adoRst.GetCollect((long)6,bookStr.strSellPrice);
				ExpressInfo.vecBookInfo.push_back(bookStr);
			}
			else if (strTemp == "订单编号")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strOrderNo);
			}
			else if (strTemp == "发货地址")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strAddr);
			}
			else if (strTemp == "联 系 人")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strRecver);
			}
			else if (strTemp == "联系方式")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strPhone);
			}
			else if (strTemp == "备    注")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strMemo);
			}
			else if (strTemp == "付款时间")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strPayTime);
			}
			else if (strTemp == "快递名称")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strExpressCompany);
			}
			else if (strTemp == "快递单号")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strExpressNo);
			}						
			adoRst.MoveNext();
		}
		if (ExpressInfo.vecBookInfo.size()>0)
		{
			vecStr.push_back(ExpressInfo);
		}
		adoRst.Close();
		bRet = true;
		iSheetCount++;
	}
	adoConn.Close();
	return bRet;
}

//修改
bool CXlsCtrolClass::Modify(CString strFile,ExpressStr &expressStr)
{
	//先获得所有的表
	CStringArray strArray;
	GetTables(strFile,strArray);
	if (strArray.GetCount() < 1)
	{
		return false;
	}
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	if (!adoConn.IsOpen())
	{
		if(adoConn.ConnectExcel(strFile))
		{
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		else
		{
			return false;
		}
	}
	//获取他所在的表
	CString strTable = "";
	CString strSQL = "update [Sheet1$] set [a]='1'";
	for (int i=0;i<strArray.GetCount();i++)
	{
		strSQL.Format("select count(*) from [%s$] where [a]='%s'",strArray.GetAt(i),expressStr.strOrderNo);
		if( !adoRst.Open(strSQL, adCmdText) )
		{
			//CString strError = adoConn.GetLastErrorText();
			adoRst.Close();
			break;
		}
		if (!adoRst.IsEOF())
		{
			CString strTemp ="";
			adoRst.GetCollect((long)0,strTemp);
			if (atol(strTemp) > 0)
			{
				strTable = strArray.GetAt(i);
				adoRst.Close();
				break;
			}			
		}
		adoRst.Close();
	}
	if (strTable == "")
	{
		adoRst.Close();
		adoConn.Close();
		return false;
	}
	//得到了其所在的表
	//开始修改
	strSQL.Format("select * from [%s$] where [a]='%s'",strTable,expressStr.strOrderNo);

	if( !adoRst.Open(strSQL, adCmdText) )
	{
		CString strError = adoConn.GetLastErrorText();
		adoRst.Close();
		return false;
	}
	CString strTemp ="";	
	while(!adoRst.IsEOF())
	{
		adoRst.GetCollect((long)1,strTemp);
		if (strTemp == "发货地址")
		{
			adoRst.PutCollect((long)2,expressStr.strAddr);
		}
		else if (strTemp == "联 系 人")
		{
			adoRst.PutCollect((long)2,expressStr.strRecver);
		}
		else if (strTemp == "联系方式")
		{
			adoRst.PutCollect((long)2,expressStr.strPhone);
		}
		else if (strTemp == "备    注")
		{
			adoRst.PutCollect((long)2,expressStr.strMemo);
		}
		else if (strTemp == "付款时间")
		{
			adoRst.PutCollect((long)2,expressStr.strPayTime);
		}
		else if (strTemp == "快递名称")
		{
			adoRst.PutCollect((long)2,expressStr.strExpressCompany);
		}
		else if (strTemp == "快递单号")
		{
			adoRst.PutCollect((long)2,expressStr.strExpressCompany);
		}
		adoRst.MoveNext();
	}
	adoRst.UpdateBatch();
	adoRst.Close();
	adoConn.Close();
	return true;
}
