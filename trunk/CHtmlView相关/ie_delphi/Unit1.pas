unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs,uWebIntface, ExtCtrls, StdCtrls, DB, ADODB;

type
  TForm1 = class(TForm)
    btn3: TButton;
    tmr1: TTimer;
    ADOConnection1: TADOConnection;
    ADOQuery1: TADOQuery;
    ADOQuery2: TADOQuery;
    Timer2: TTimer;
    Button1: TButton;
    Edit1: TEdit;
    Label1: TLabel;
    Timer3: TTimer;
    Edit2: TEdit;
    procedure FormCreate(Sender: TObject);
    procedure btn3Click(Sender: TObject);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Timer2Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Timer3Timer(Sender: TObject);
  private
    { Private declarations }
    WebInt: TWebIntface;
    FCount: Integer;
    FCPU: Extended;
    FExCPU: Extended;
    FJMStr: string;
    FStopCount: Integer;
    function GetCpuSpeed: Extended;
//    function JM(): Boolean;
    procedure MyShortCut(var message: TMessage); message WM_HOTKEY;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;


implementation

uses DateUtils;
{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
begin
//  JM;
  WebInt := TWebIntface.Create(Self);
  FCount := 0;
  RegisterHotKey(Handle, 1, MOD_CONTROL,Byte('E'));

end;

procedure TForm1.btn3Click(Sender: TObject);
var
  i: Integer;
begin
  WebInt.FOffSetLeft := StrToInt(Edit1.Text);
  WebInt.FOffSetTop := StrToInt(Edit2.Text);
  WebInt.FStart := True;
  Button1.Enabled := True;
  btn3.Enabled := False;
  //while WebInt.FStart do
  begin
    with ADOQuery1 do
    begin
      Close;
      SQL.Text := 'select * from Keywords';
      Open;
      while not eof do
      begin
        TImer3.Enabled := True;
        WebInt.FStop := False;
        FStopCount := 0;
        WebInt.FClick := (Fields[3].AsInteger = 1);
        if WebInt.DoBegin(Fields[1].AsString, Fields[2].AsString) then
        begin
        WebInt.WSleep(1000);
        if WebInt.FClicked then
        begin
          ADOQuery2.Close;
          ADOQuery2.SQL.Text := 'insert into ClickInfo (keyWord_ID, KeyWord, KeyWord_URL, Click_Time)'+
                                'values('+Fields[0].AsString+','''+Fields[1].AsString+''','''+Fields[2].AsString+''',now())';
          ADOQuery2.ExecSQL;
        end;
        end;
        WebInt.WSleep(2000);
        Application.ProcessMessages;
        if not WebInt.FStart then Break;
        Next;
      end;
    end;
    for i := 0 to 300 do
    begin
      sleep(10);
      Application.ProcessMessages;
      if not WebInt.FStart then Break;
    end;
  end;
  Button1.Enabled := False;
  btn3.Enabled := True;
end;

procedure TForm1.FormMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  ps: TPoint;
begin
  GetCursorPos(ps);
  Showmessage('x: ' + IntToStr(ps.X) + ', y: ' + IntToStr(ps.Y));
end;
function   TForm1.GetCpuSpeed:   Extended;
  var   
                  t,   mhi,   mlo,   nhi,   nlo:   dword;
                  shr32   :   comp;
  begin   
      shr32   :=   65536;   
      shr32   :=   shr32   *   65536;   
      t:=   GetTickCount;   
      while   t   =   GetTickCount   do   ;   
        asm   
          DB   0FH,031H   //   rdtsc   
          mov   mhi,edx   
          mov   mlo,eax   
        end;   
      while   GetTickCount   <   (t   +   100)   do   ;   
        asm   
          DB   0FH,031H   //   rdtsc   
          mov   nhi,edx   
          mov   nlo,eax   
        end;   
        Result   :=   ((nhi   *   shr32   +   nlo)   -   (mhi   *   shr32   +   mlo))   /   1E6;   
  end;

procedure TForm1.Timer2Timer(Sender: TObject);
begin
  if not WebInt.FStart then
  begin
    btn3.Enabled := True;
    Button1.Enabled := False;
  end;

end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  WebInt.FStart := False;
  Button1.Enabled := False;
end;

{function TForm1.JM: Boolean;
var
  y, m, d, h, n, s, ms: Word;
  str: string;
  i, j: Integer;
  dt: TDateTime;
begin
  dt := Now;
  DecodeDateTime(dt, y, m, d, h, n ,s, ms);
  str := INtToStr(Round(GetCpuSpeed * 3.7) + y * 74 + m * 584+ d * 584 + h * 12 + n * 452 + s * 54 + ms +653);
  j := 1;
  setLength(FJMStr, Length(str));
  for i := 1 to Length(str) do
  begin
    if i mod 2 = 0 then
    begin
      FJMStr[j] := str[i];
      inc(j);
    end;

  end;
  for i := 1 to Length(str) do
  begin
    if i mod 2 <> 0 then
    begin
      FJMStr[j] := str[i];
      inc(j);
    end;

  end;
  if ProJM(IntToStr(h)+'-'+IntToStr(n)+'-'+IntToStr(s)+'-'+IntToStr(ms) + '-' + FloatToStr(GetCpuSpeed)) <> FJMStr then
  begin
    MessageBox(0, 'Ê§°Ü', '', MB_ICONWARNING or MB_OK);
    Application.Terminate;
  end; 
end;    }

procedure TForm1.FormDestroy(Sender: TObject);
begin
  UnregisterHotKey(handle,1);
  WebInt.Free;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  WebInt.FStart := False;
end;

procedure TForm1.MyShortCut(var message: TMessage);
begin
  Button1.Click;
end;

procedure TForm1.Timer3Timer(Sender: TObject);
begin
  inc(FStopCount);
  if FStopCount > 60 then
  begin
    WebInt.FStop := True;
    Timer3.Enabled := False;
  end;
end;

end.
