object Form1: TForm1
  Left = 239
  Top = 197
  Width = 340
  Height = 209
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 16
    Top = 144
    Width = 36
    Height = 14
    Caption = #32534#31227#37327
  end
  object btn3: TButton
    Left = 48
    Top = 48
    Width = 75
    Height = 25
    Caption = #21551#21160
    TabOrder = 0
    OnClick = btn3Click
  end
  object Button1: TButton
    Left = 168
    Top = 48
    Width = 75
    Height = 25
    Caption = #20572#27490
    Enabled = False
    TabOrder = 1
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 72
    Top = 136
    Width = 57
    Height = 22
    TabOrder = 2
    Text = '0'
  end
  object Edit2: TEdit
    Left = 144
    Top = 136
    Width = 65
    Height = 22
    TabOrder = 3
    Text = '0'
    Visible = False
  end
  object tmr1: TTimer
    Left = 536
    Top = 64
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=data.mdb;Persist Se' +
      'curity Info=False'
    LoginPrompt = False
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 16
    Top = 32
  end
  object ADOQuery1: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 56
    Top = 32
  end
  object ADOQuery2: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 248
    Top = 48
  end
  object Timer2: TTimer
    OnTimer = Timer2Timer
    Left = 104
    Top = 16
  end
  object Timer3: TTimer
    Enabled = False
    OnTimer = Timer3Timer
    Left = 248
    Top = 88
  end
end
