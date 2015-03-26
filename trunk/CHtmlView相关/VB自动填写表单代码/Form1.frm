VERSION 5.00
Object = "{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}#1.1#0"; "shdocvw.dll"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   8145
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   13470
   LinkTopic       =   "Form1"
   ScaleHeight     =   8145
   ScaleWidth      =   13470
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command4 
      Caption         =   "Command4"
      Height          =   375
      Left            =   12840
      TabIndex        =   9
      Top             =   7560
      Width           =   495
   End
   Begin VB.CommandButton Command3 
      Caption         =   "返回查看日志"
      Height          =   495
      Left            =   11520
      TabIndex        =   8
      Top             =   7440
      Width           =   1215
   End
   Begin VB.TextBox Text3 
      Height          =   495
      Left            =   3960
      TabIndex        =   5
      Top             =   7440
      Width           =   6135
   End
   Begin VB.TextBox Text2 
      Height          =   375
      Left            =   16920
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   3360
      Visible         =   0   'False
      Width           =   2295
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   840
      TabIndex        =   3
      Top             =   7440
      Width           =   2295
   End
   Begin VB.CommandButton Command2 
      Caption         =   "获取IFRAME"
      Height          =   615
      Left            =   17640
      TabIndex        =   2
      Top             =   1680
      Visible         =   0   'False
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "一键发表日志"
      Height          =   495
      Left            =   10200
      TabIndex        =   1
      Top             =   7440
      Width           =   1215
   End
   Begin SHDocVwCtl.WebBrowser WebBrowser1 
      Height          =   7215
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   13095
      ExtentX         =   23098
      ExtentY         =   12726
      ViewMode        =   0
      Offline         =   0
      Silent          =   0
      RegisterAsBrowser=   0
      RegisterAsDropTarget=   1
      AutoArrange     =   0   'False
      NoClientEdge    =   0   'False
      AlignLeft       =   0   'False
      NoWebView       =   0   'False
      HideFileNames   =   0   'False
      SingleClick     =   0   'False
      SingleSelection =   0   'False
      NoFolders       =   0   'False
      Transparent     =   0   'False
      ViewID          =   "{0057D0E0-3573-11CF-AE69-08002B2E1262}"
      Location        =   "http:///"
   End
   Begin VB.Label Label2 
      Caption         =   "内容"
      Height          =   255
      Left            =   3480
      TabIndex        =   7
      Top             =   7560
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "标题"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   7440
      Width           =   615
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()

Dim divElem As MSHTML.HTMLDivElement
Dim textElem As MSHTML.HTMLTextAreaElement
Dim buttonElem As MSHTML.HTMLButtonElement
Set divElem = WebBrowser1.Document.getElementById("twComposer")
For Each x In divElem.All
    If x.tagName = "TEXTAREA" Then
    Set textElem = x
    textElem.Value = Text3.text
    Else
    If x.tagName = "A" Then
    Set buttonElem = x
    Text1.text = buttonElem.innerText
    buttonElem.Click
    
    Exit For
    End If
    
    End If
Next x

End Sub
Private Sub Command3_Click()
Set frame1 = WebBrowser1.Document.getElementById("tblog")
Dim doc As MSHTML.HTMLDocument
Set doc = frame1.contentWindow.Document

Dim retBtn As MSHTML.HTMLButtonElement
For Each x In doc.All
    If x.tagName = "BUTTON" Then
    Set retBtn = x
    If retBtn.innerText = "返回查看日志" Then
    retBtn.Click
    Exit For
    End If
    End If
Next x
End Sub

Private Sub Command4_Click()
Dim fForm As MSHTML.HTMLFormElement
Set fForm = WebBrowser1.Document.getElementById("popFormSubmit")
fForm.submit

End Sub

Private Sub Form_Load()
Text1.text = "test title"
Text3.text = "test content"
WebBrowser1.Navigate "http://t.sohu.com/home"
End Sub
