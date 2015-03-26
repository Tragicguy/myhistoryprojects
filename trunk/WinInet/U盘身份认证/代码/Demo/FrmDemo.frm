VERSION 5.00
Begin VB.Form FrmDemo 
   Caption         =   "Demo"
   ClientHeight    =   6855
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10605
   LinkTopic       =   "Form1"
   ScaleHeight     =   6855
   ScaleWidth      =   10605
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command3 
      Caption         =   "备份认证文件"
      Height          =   375
      Left            =   2865
      TabIndex        =   23
      Top             =   120
      Width           =   2550
   End
   Begin VB.ListBox List1 
      Height          =   5100
      Left            =   5160
      TabIndex        =   20
      Top             =   1200
      Width           =   5295
   End
   Begin VB.CommandButton Command2 
      Caption         =   "制作uKey"
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Top             =   120
      Width           =   2535
   End
   Begin VB.CommandButton Command1 
      Caption         =   "读uKey信息"
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   600
      Width           =   2535
   End
   Begin VB.Frame Frame1 
      Caption         =   "设备参数"
      Height          =   5295
      Left            =   240
      TabIndex        =   2
      Top             =   1080
      Width           =   4695
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         ForeColor       =   &H000000C0&
         Height          =   180
         Index           =   19
         Left            =   240
         TabIndex        =   24
         Top             =   4800
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   18
         Left            =   240
         TabIndex        =   22
         Top             =   4560
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   17
         Left            =   240
         TabIndex        =   21
         Top             =   4320
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   16
         Left            =   240
         TabIndex        =   19
         Top             =   4080
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   15
         Left            =   240
         TabIndex        =   18
         Top             =   3840
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   14
         Left            =   240
         TabIndex        =   17
         Top             =   3600
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   13
         Left            =   240
         TabIndex        =   16
         Top             =   3360
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   12
         Left            =   240
         TabIndex        =   15
         Top             =   3120
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   11
         Left            =   240
         TabIndex        =   14
         Top             =   2880
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   10
         Left            =   240
         TabIndex        =   13
         Top             =   2640
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   9
         Left            =   240
         TabIndex        =   12
         Top             =   2400
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   8
         Left            =   240
         TabIndex        =   11
         Top             =   2160
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   7
         Left            =   240
         TabIndex        =   10
         Top             =   1920
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   6
         Left            =   240
         TabIndex        =   9
         Top             =   1680
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   5
         Left            =   240
         TabIndex        =   8
         Top             =   1440
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   4
         Left            =   240
         TabIndex        =   7
         Top             =   1200
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   3
         Left            =   240
         TabIndex        =   6
         Top             =   960
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   2
         Left            =   240
         TabIndex        =   5
         Top             =   720
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   1
         Left            =   240
         TabIndex        =   4
         Top             =   480
         Width           =   90
      End
      Begin VB.Label Labelinfo 
         AutoSize        =   -1  'True
         Caption         =   "*"
         Height          =   180
         Index           =   0
         Left            =   240
         TabIndex        =   3
         Top             =   240
         Width           =   90
      End
   End
End
Attribute VB_Name = "FrmDemo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private oePass As New LEFUDePass.epassMain

Private Sub Command1_Click()
    's_SystemName 最大长度为128个字符
    's_InputPass  最大长度为512个字符

    '在初始化成功后返回（系统中可用U盘分区字符）
    Me.Labelinfo(18) = "系统可用分区  ：" & oePass.sBack_SystemuDiskWord
    Dim t As String
    If oePass.ReadKey(t, True) = False Then
        MsgBox oePass.sErr
    Else
        
        '在读取数据成功后返回信息
        With oePass
            Me.Labelinfo(0) = "认证文件编号  ：" & .sBack_DrvKeyNumber
            Me.Labelinfo(1) = "制造商识别名称：" & .sBack_DrvVenInfo
            Me.Labelinfo(2) = "产品识别名称  ：" & .sBack_DrvProdIDInfo
            Me.Labelinfo(3) = "产品版本及编号：" & .sBack_DrvRevSNInfo
            Me.Labelinfo(4) = "认证加密版本  ：" & .sBack_EncryptVer
            Me.Labelinfo(5) = "认证加密级别  ：" & .sBack_Encryptlevel
            Me.Labelinfo(6) = "认证制作日期  ：" & .sBack_MakeDate
            Me.Labelinfo(7) = "认证制作时间  ：" & .sBack_MakeTime
            Me.Labelinfo(8) = "认证制作计算机：" & .sBack_MakeComputerName
            Me.Labelinfo(9) = "限制使用次数  ：" & .sBack_CheckUseTimes
            Me.Labelinfo(10) = "最大可用次数  ：" & .sBack_SetedMaxUseTimes
            Me.Labelinfo(11) = "限制使用天数  ：" & .sBack_CheckUseDays
            Me.Labelinfo(12) = "最大可用天数  ：" & .sBack_SetedMaxUseDays
            Me.Labelinfo(13) = "任意条件触发  ：" & .sBack_CheckAnyWhereStarEvent

            Me.Labelinfo(14) = "剩余可用次数  ：" & .sBack_CanUseTimes
            Me.Labelinfo(15) = "剩余可用天数  ：" & .sBack_CanUseDays
            Me.Labelinfo(16) = "记录登录日志  ：" & .sBack_SaveLogInfo
            Me.Labelinfo(17) = "当前正使用分区：" & .sBack_CurUseDiskWord
            Me.Labelinfo(19) = "设备设备内信息：" & t
            
            Dim sLogin() As String
            sLogin = Split(.sBack_LogInfo, "|")
            Dim i As Long
            Me.List1.Clear
            Dim sT() As String
            For i = 0 To UBound(sLogin) - 1
                sT = Split(sLogin(i), "-")

                Me.List1.AddItem CDate(Mid(sT(0), 1, 4) & "-" & Mid(sT(0), 5, 2) & " -" & Mid(sT(0), 7, 2)) & " " & CDate(Mid(sT(0), 9, 2) & ":" & Mid(sT(0), 11, 2) & " :" & Mid(sT(0), 13, 2)) & " " & sT(1)
            Next
        End With
    End If
End Sub

Private Sub Command2_Click()
    Dim t As String
    If oePass.WriteKey("返回卡内信息") = False Then
        MsgBox oePass.sErr
    End If
End Sub

Private Sub Command3_Click()
    If oePass.BakKeyFile = False Then
        MsgBox oePass.sErr
    End If
End Sub

Private Sub Form_Load()
    If oePass.InitUDrv("1", "1234567", Date$) = False Then
        MsgBox oePass.sErr
        Exit Sub
    End If
End Sub
