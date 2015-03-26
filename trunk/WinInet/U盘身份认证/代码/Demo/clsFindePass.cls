VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsFindePass"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Public oePass As New LEFUDePass.epassMain
Dim sSysName As String
Dim sSysPass As String
Dim sCurDate As Date
Public sBackErr As String



'*************************************************************************
'**函 数 名：InitDrv
'**输    入：s_SysName(String)                       -
'**        ：s_SysPass(String)                       -
'**        ：Optional d_CurDate(Date = "2006-06-01") -
'**输    出：(Boolean) -
'**功能描述：初始化设备
'**全局变量：
'**调用模块：
'**作    者：lienfeng
'**日    期：2006-06-01 17:09:20
'**修 改 人：
'**日    期：
'**版    本：V1.0.0
'*************************************************************************
Public Function InitDrv(s_SysName As String, s_SysPass As String, Optional d_CurDate As Date = "2006-06-01") As Boolean
    
    sSysName = s_SysName
    sSysPass = s_SysPass
    sCurDate = d_CurDate
    InitDrv = oePass.InitUDrv(sSysName, sSysPass, sCurDate)
    If InitDrv = False Then sBackErr = oePass.sErr
End Function


'*************************************************************************
'**函 数 名：ReadDrv
'**输    入：Optional sBackText(String = "")         -
'**        ：Optional sBackLoginRec(Boolean = False) -
'**输    出：(Boolean) -
'**功能描述：读取文本内容
'**全局变量：
'**调用模块：
'**作    者：lienfeng
'**日    期：2006-06-01 17:09:20
'**修 改 人：
'**日    期：
'**版    本：V1.0.0
'*************************************************************************
Public Function ReadDrv(Optional sBackText As String = "", Optional sBackLoginRec As Boolean = False) As Boolean
    If InitDrv(sSysName, sSysPass, sCurDate) = False Then Exit Function
    ReadDrv = oePass.ReadKey(sBackText, sBackLoginRec)
    If ReadDrv = False Then sBackErr = oePass.sErr
End Function


'*************************************************************************
'**函 数 名：WriteDrv
'**输    入：Optional sInputSaveText(String = "") -
'**输    出：(Boolean) -
'**功能描述：制作加密设备
'**全局变量：
'**调用模块：
'**作    者：lienfeng
'**日    期：2006-06-01 17:09:20
'**修 改 人：
'**日    期：
'**版    本：V1.0.0
'*************************************************************************
Public Function WriteDrv(Optional sInputSaveText As String = "") As Boolean
    If InitDrv(sSysName, sSysPass, sCurDate) = False Then Exit Function
    WriteDrv = oePass.WriteKey(sInputSaveText)
    If WriteDrv = False Then sBackErr = oePass.sErr
End Function
 
'*************************************************************************
'**函 数 名：BakKey
'**输    入：无
'**输    出：(Boolean) -
'**功能描述：备份加密文件
'**全局变量：
'**调用模块：
'**作    者：lienfeng
'**日    期：2006-06-01 17:09:20
'**修 改 人：
'**日    期：
'**版    本：V1.0.0
'*************************************************************************
Public Function BakKey() As Boolean
    If InitDrv(sSysName, sSysPass, sCurDate) = False Then Exit Function
    BakKey = oePass.BakKeyFile
    If BakKey = False Then sBackErr = oePass.sErr
End Function
