unit uWebIntface;

interface
  uses
    sysUtils, windows,shellapi, OleCtrls, wininet,shdocvw, MSHTML, ActiveX,
    ComObj, uWebInc, Classes, ExtCtrls, Controls, Forms, Messages;

type
  TObjectFromLResult = function(LRESULT: lResult; const IID: TIID; WPARAM: wParam; out pObject): HRESULT; stdcall;
  TBrowserType = (ie6, ie7, ie8);
type
  TWebIntface = class
  private
    FDoc: IHTMLDocument2;
    FParentHandle: HWND;
    FDocHandle: HWND;
    FBrowserHandle: HWND;
    FTimer: TTimer;
    FBrowserType: TBrowserType;
    function GetIEFromHWND(WHandle: HWND; var IE: IWebbrowser2): HRESULT;
    function InvokeScript(Doc: IHTMLDocument2; aName, aParam: String): Variant;
    function OpenUrl(url: string; path: string = ''): boolean;
    function GetDocFromIE(): IHTMLDocument2;
    function GetElementPos(Doc: IHTMLDocument2;ElementId: string; offsetX: Integer): TPoint;overload;
    function GetElementPos(Doc: IHTMLDocument2;Element: IHTMLElement; offsetX: Integer): TPoint;overload;
    function MoveMouse(EndPos, OffsetPos: TPoint;stept: Integer = 1): Boolean;
    function SetInputKeyWords(Doc: IHTMLDocument2;ElementId, KeyWords: string): Boolean;
    function MouseDown(nPos: Tpoint): Boolean;overload;
    function MouseDown(): Boolean;overload;
    function GetElementPosFromWebUrl(Doc: IHTMLDocument2;WebUrl: string): TPoint;
    function ScrollVBrowser(): Boolean;
    function CloseBrowser: Boolean;
    function SysLog(msg: string): Boolean;
    function AutoDoIt(Doc: IHTMLDocument2;KeyWords: string): Boolean;
    function ScrollUpDown(var Doc: IHTMLDocument2): Boolean;
  public
    FOffSetLeft, FOffSetTop: Integer;
    FStart: Boolean;
    FStop: Boolean;
    FClick: Boolean;
    FClicked: Boolean;
    function WSleep(ms: Cardinal): Boolean;
    function DoBegin(KeyWord, KeyUrl: string): Boolean;

    constructor Create(AOwner: TComponent);
    destructor Destroy; override;
  end;
implementation

uses Types;

{ TWebIntface }

function TWebIntface.AutoDoIt(Doc: IHTMLDocument2;KeyWords: string): Boolean;
var
  w: HTMLDocument;
  a: HTMLElementCollection;
  f: HTMLFormElement;
begin
  SetInputKeyWords(Doc, 'kw', KeyWords);
  WSleep(2000);
  f := doc.forms.item(0, 0) as HTMLFormElement;
  f.submit;
{  if Doc.scripts.length > 0 then
      Script:=Doc.scripts.item(0, 0) as IHTMLScriptElement;
  script.text := Format(setKeyWords, [KeyWords]);
  InvokeScript(Doc, 'setKeyWords', ElementId);}
end;

function TWebIntface.CloseBrowser: Boolean;
var
  r, R1: TRect;
  i, j: Integer;
  Label lb;
begin
  r := Rect(0, 0, 0, 0);
  GetWindowRect(FBrowserHandle, r);
  if (r.Right = 0)and(r.Bottom = 0 ) then goto lb;
  MoveMouse(Point(R.Right, R.Top), Point(-15, 10), 50);
  WSleep(300);
  MouseDown();
  WSleep(200);
 lb: i := FindWindowEx(0,0,'IEFrame', nil);
  if i = 0 then Exit;
  if FBrowserType <> ie6 then
  begin
  WSleep(1000);
  r1 := Rect(0, 0, 0, 0);
  GetClientRect(FBrowserHandle, r1);
  if not ((r1.Bottom = 0)and(r1.Right = 0)) then
  begin
    WSleep(100);
    if FBrowserType = ie7 then
      MoveMouse(Point((R.Right - r1.Right) + r1.Right div 2, R.Bottom div 2), Point(-20, 35), 50)
    else
      MoveMouse(Point((R.Right - r1.Right) + r1.Right div 2, R.Bottom div 2), Point(-20, 17), 50);
    WSleep(300);
    MouseDown;
  end;
  end;
  for j := 0 to 5 do
  begin
    i:= FindWindowEx(0,0,'IEFrame', nil);

    if i = 0 then Break;

    if self.FBrowserType = ie6 then
    begin
      wSleep(1000);
      R := Rect(0, 0, 0, 0);
      GetWindowRect(i, R);
      if (R.Right = 0 ) and (R.Bottom = 0 ) then break;
      self.MoveMouse(Point(R.Right, R.Top), Point(-15, 10), 20);
      self.MouseDown;
    end
    else
      SendMessage(I,   WM_Close,   0,   0);
    wsleep(10);
  end;
end;

constructor TWebIntface.Create(AOwner: TComponent);
begin
  FTimer := TTimer.Create(nil);
  FParentHandle := TWinControl(AOwner).Handle;
end;

destructor TWebIntface.Destroy;
begin
  FTimer.Free;
  inherited;
end;

function TWebIntface.DoBegin(KeyWord, KeyUrl: string): Boolean;
var
  doc: IHTMLDocument2;
  pt: TPoint;
  s: string;
  
begin
  result := false;
  setLength(s, 256);
  GetSystemDirectory(Pchar(s), 256);
  OpenUrl('http://www.baidu.com', Pchar(s[1]+':\Program Files\Internet Explorer\iexplore.exe'));
  try
  SysLog('1 ');
  if not WSleep(5000) then Exit;
  doc := GetDocFromIE();
  SysLog('2 ');
  if doc = nil then Exit;
  if not WSleep(100) then Exit;

  pt := GetElementPos(Doc, 'kw', FOffSetLeft);
  SysLog('3 ');
  if (pt.X = 0)and(pt.Y = 0) then Exit;
  if not WSleep(100) then Exit;
  MoveMouse(pt, Point(20, 10), 50);
  SysLog('4 ');
  if not WSleep(500) then Exit;
  SetInputKeyWords(Doc, 'kw', KeyWord);
  SysLog('5 ');
  if not WSleep(100) then Exit;
  pt := GetElementPos(Doc, 'su', FOffSetLeft);
  if not WSleep(200) then Exit;
  MoveMouse(pt, Point(50, 10), 50);
  SysLog('7 ');
  if not WSleep(200) then Exit;
  try
  (doc.forms.item(0, 0) as HTMLFormElement).submit;
  except
  exit;
  end;
  SysLog('8 ');
  //AutoDoIt(Doc, KeyWord);
  if not WSleep(2000) then Exit;
  //doc := GetDocFromIE();

  ScrollUpDown(Doc);
  if not WSleep(200) then Exit;
  pt := GetElementPosFromWebUrl(doc, KeyUrl);
  SysLog('9 ');
  if (pt.X = 0)and(pt.Y = 0) then begin FClicked := false; Exit; end;
  FClicked := true;
  if not WSleep(1000) then Exit;
  MoveMouse(pt, Point(100, 5), 50);
  SysLog('10 ');
  if not WSleep(100) then Exit;
  MouseDown;
  result := true;
  SysLog('11 ');
  finally

  WSleep(10000);
  CloseBrowser;
  WSleep(2000);
  SysLog('12 ');
  end;
  //keybd_event(Byte(' '), 0, KEYEVENTF_EXTENDEDKEY or KEYEVENTF_KEYUP, GetMessageExtraInfo);
  //WSleep(100);
  //SendMessage(FBrowserHandle, WM_SYSCOMMAND, VK_SPACE, WM_KEYUP);

end;

function TWebIntface.GetDocFromIE: IHTMLDocument2;
var
  All: IHtmlElementCollection;
  HtmlElement: IHtmlElement;
  I: Integer;
  Document: IHtmlDocument2;
  IE: IWebBrowser2;
  Wnd: HWND;
  WndChild: HWND;
  script: IHTMLScriptElement;
  r: TRect;
  str: string;
begin
  Result := nil;
  Wnd := FindWindow('IEFrame', nil);
  FBrowserHandle := Wnd;
  if Wnd = 0 then Exit;
  SendMessage(Wnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  WndChild := FindWindowEX(Wnd , 0, 'Frame Tab', nil);
  if WndChild <> 0 then
  begin
    WndChild := FindWindowEX(WndChild, 0, 'TabWindowClass', nil);
    WndChild := FindWindowEX(WndChild, 0, 'Shell DocObject View', nil);
    FBrowserType := ie8;
  end;
  if WndChild = 0 then
  begin
    WndChild := FindWindowEX(Wnd, 0, 'Shell DocObject View', nil);
    FBrowserType := ie6;
  end;
  if WndChild = 0 then
  begin
    WndChild := FindWindowEX(Wnd, 0, 'TabWindowClass', nil);
    WndChild := FindWindowEX(WndChild, 0, 'Shell DocObject View', nil);
    FBrowserType := ie7;
  end;

  if WndChild <> 0 then
  begin
    WndChild := FindWindowEX(WndChild, 0, 'Internet Explorer_Server', nil);
    if WndChild <> 0 then
    begin
      FDocHandle := WndChild;
      GetIEFromHWnd(WndChild, IE); //Get Iwebbrowser2 from Handle
      Result := IE.Document as IHtmlDocument2;
      str := Result.readyState;
      while Pos('COM', UpperCase(str))<= 0 do
      begin
        WSleep(1000);
        str := Result.readyState;
      end;
    end;
  end;
end;

function TWebIntface.GetElementPos(Doc: IHTMLDocument2;
  ElementId: string; offsetX: Integer): TPoint;
var
  script: iHTMLScriptElement;
  r: TRect;
  psStr: string;
  el: IHTMLElement;
  w: HTMLScriptElement;
  cns: IHTMLElementCollection;
  i, t, l: Integer;
  e: IHTMLElement;
begin
  if Assigned(Doc) then
  begin
  try
    Result := Point(0, 0);
    cns := Doc.all ;
    for i := 0 to  cns.length - 1 do
    begin
      el := cns.item(i, i) as IHTMLElement;
      if el.id = ElementId then
      begin
        result := GetElementPos(Doc, el, offsetX);
        Break;
      end;
    end;
  except
    exit;
  end;
  end;


    //'function Offset(id){var e=document.getElementById(id);var t = e.offsetTop;var l = e.offsetLeft;'+
   //             'while(e = e.offsetParent){'+
   //             't += e.offsetTop;l += e.offsetLeft;}return l+","+t}';
    {if Doc.scripts.length > 0 then
      Script:=Doc.scripts.item(0, 0) as iHTMLScriptElement;
    script.Text := script.text + ';' + offsetJsStr;
    {script.text := offsetJsStr;
    psStr := InvokeScript(Doc, 'Offset', ElementId);  }
    {webbody:=doc.body as htmlbody;
    doc.scripts.
    webbody.insertAdjacentHTML('beforeend','<script language="javascript">');
    webbody.insertAdjacentHTML('beforeend',offsetJsStr);
    webbody.insertAdjacentHTML('beforeend','</script>');  }
   // psStr := InvokeScript(Doc, 'Offset', ElementId);
end;

function TWebIntface.GetElementPos(Doc: IHTMLDocument2;
  Element: IHTMLElement; offsetX: Integer): TPoint;
var
  script: iHTMLScriptElement;
  r: TRect;
  psStr: string;
  el: IHTMLElement;
  w: HTMLScriptElement;
  cns: IHTMLElementCollection;
  i, t, l: Integer;
  e: IHTMLElement;
begin
  if Assigned(Doc) then
  begin
    try
    Result := Point(0, 0);
    el := Element;
    t := el.offsetTop;
    l := el.offsetLeft;
    e := el.offsetParent;
    while e <> nil do
    begin
      t := t + e.offsetTop;
      l := l+ e.offsetLeft;
      e := e.offsetParent
    end;
    //if FBrowserType <> ie8 then
      l := l + offsetX;
    //l := l + FOffSetLeft;
    //t := t + FOffSetTop;
    psStr := IntToStr(l) + ',' + IntToStr(t);

    if pos(',', psStr) > 0 then
    begin
      windows.GetWindowRect(FDocHandle, r);
      Result.X := r.Left + StrToInt(Copy(psStr, 1, Pos(',', psStr) - 1));
      Result.Y := r.Top + StrToInt(Copy(psStr, Pos(',', psStr) + 1, MaxInt));
    end;
    except
      exit;
    end;
  end;
  
end;

function TWebIntface.GetElementPosFromWebUrl(Doc: IHTMLDocument2;
  WebUrl: string): TPoint;
var
  script: IHTMLScriptElement;
  r: TRect;
  psStr, str: string;
  sy, i, j, l, t, esl, est: Integer;
  a: HTMLAnchorElement;
  es: HTMLElementCollection;
  e: IHTMLElement;
  pt : TPoint;
  f, f1: Boolean;
  fontTop: Integer;
  Doc3: IHTMLDocument3;
  Label lb;
begin
  Doc3 := (doc as IHTMLDocument3);
  f := False;
  f1 := False;
  Result := Point(0, 0);
  if Assigned(Doc) then
  begin
  try
   pt := Point(0, 0);
    while not (Pos('COM', UpperCase(Doc.readyState))>0) do
      WSleep(1000);

    for i := Doc.all.length  - 1 downto 0 do
    begin
      str := (Doc.all.item(i, i) as IHTMLElement).tagName;
      //MessageBox(0, Pchar(str), '', 64);
      if f1 then
      begin
        str := (Doc.all.item(i, i) as IHTMLElement).tagName;
          if UpperCase(str) = 'A' then
          begin
            f1 := False;
            e := Doc.all.item(i, i) as IHTMLElement;
            if pos('e.baidu.com', (e as HTMLAnchorElement).href) >0 then
              continue;
            if pos('www.baidu.com/baidu.php?url', (e as HTMLAnchorElement).href) > 0 then
              if not FClick then
              begin
                pt := Point(0, 0);
                pt := GetElementPos(Doc, e, 0);
                //pt.Y := pt.Y - 15;
                break;
              end;
            pt := Point(0, 0);
            //pt := GetElementPos(Doc, e, 0);
          end;
      end;
      if f then
      begin
        str := (Doc.all.item(i, i) as IHTMLElement).tagName;
          if UpperCase(str) = 'A' then
          begin
            f := False;
            e := Doc.all.item(i, i) as IHTMLElement;
            pt := Point(0, 0);
            pt := GetElementPos(Doc, e, 0);
            //q'http://www.baidu.com/baidu.php?url';
            //if pt.X > 100 then  //右边推广
            //if pos('http://www.baidu.com/baidu.php?url', e.innerHTML) > 0 then

            if pos('www.baidu.com/baidu.php?url', (e as HTMLAnchorElement).href)>0 then
            begin
              f1 := True
            end
            else if pos('e.baidu.com', (e as HTMLAnchorElement).href) >0 then
              pt := Point(0, 0)
            else// if (pt.Y > fontTop - 5) and (pt.Y < fontTop + 5) then
            begin
              if FClick then
                break
              else
                pt := Point(0, 0);
            end;
          end;
      end;
      if UpperCase(str) = 'FONT' then
      begin
        str := (Doc.all.item(i, i) as IHTMLElement).innerHTML;
        if Pos(WebUrl, str) > 0 then
        begin
          pt := Point(0, 0);
          pt := GetElementPos(Doc, (Doc.all.item(i, i) as IHTMLElement), 0);
          fontTop := pt.Y;
          f := true;
        end;
      end;
    end;
    if (pt.X = 0)and(pt.Y = 0) then Exit;
    //pt := GetElementPos(Doc, e, 0);
    psStr := IntToStr(pt.X) + ',' + IntToStr(pt.Y );
lb: if pos(',', psStr) > 0 then
    begin
      windows.GetWindowRect(FDocHandle, r);
      Result.X :={ r.Left + }StrToInt(Copy(psStr, 1, Pos(',', psStr) - 1));
      Result.Y :={ r.Top + }StrToInt(Copy(psStr, Pos(',', psStr) + 1, MaxInt));
      GetClientRect(FBrowserHandle, r);
      if (Result.Y > r.Bottom)or(Result.Y < 0) then
      begin
        sy := -120;
        sendmessage(self.FDocHandle, WM_VSCROLL, SB_LINEDOWN, 0);
        //mouse_event(MOUSEEVENTF_WHEEL, 0, 20, sy, GetMessageExtraInfo);
        WSleep(100);
        esl := doc3.documentElement.getAttribute('scrollLeft', 0);
        est := doc3.documentElement.getAttribute('scrollTop', 0);
        //esl := (doc.body as HTMLBODY).scrollLeft;
        //est := (doc.body as HTMLBODY).scrollTop;
        psStr := IntToStr(pt.X - esl) + ',' + IntToStr(pt.Y - est);
        goto lb;
      end;
    end;
    except

    end;
  end;
end;

function TWebIntface.GetIEFromHWND(WHandle: HWND;
  var IE: IWebbrowser2): HRESULT;
var
  hInst: HWND;
  lRes: Cardinal;
  MSG: Integer;
  pDoc: IHTMLDocument2;
  ObjectFromLresult: TObjectFromLresult;
begin
  hInst := LoadLibrary('Oleacc.dll');
  @ObjectFromLresult := GetProcAddress(hInst, 'ObjectFromLresult');
  if @ObjectFromLresult <> nil then
  begin
    try
      MSG := RegisterWindowMessage('WM_HTML_GETOBJECT');
      SendMessageTimeOut(WHandle, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, lRes);
      Result := ObjectFromLresult(lRes, IHTMLDocument2, 0, pDoc);
      if Result = S_OK then
        (pDoc.parentWindow as IServiceprovider).QueryService(IWebbrowserApp, IWebbrowser2, IE);
    finally
      FreeLibrary(hInst);
    end;
  end;

end;

function TWebIntface.InvokeScript(Doc: IHTMLDocument2; aName,
  aParam: String): Variant;
var
    Disp: IDispatch;
    Member:   WideString;
    TheDispId:   Integer;
    DispParams:   TDispParams;
    VariantArg:   TVariantArg;
    Args:   WideString;
begin
  Disp:=(Doc as IHTMLDocument2).Script;
  if   Assigned(Disp)
  then
  begin
    Member := aName;
    if Disp.GetIDsOfNames(GUID_NULL, @Member, 1, LOCALE_SYSTEM_DEFAULT, @TheDispId) = S_OK then
    begin
      FillChar(DispParams,   SizeOf(DispParams),   0);
      FillChar(VariantArg,   SizeOf(VariantArg),   0);
      DispParams.cArgs   :=   1;   //   1   argument
      DispParams.rgvarg   :=   @VariantArg;
      VariantArg.vt   :=   VT_BSTR;
      Args   :=   aParam;
      VariantArg.bstrVal   :=   pointer(Args);
      OleCheck(Disp.Invoke(TheDispId, GUID_NULL, 0, DISPATCH_METHOD,
          DispParams, @Result, nil, nil));
    end;
  end;
end;

function TWebIntface.MouseDown(nPos: Tpoint): Boolean;
var
  ipos: TPoint;
begin
  if nPos.X = 0 then
    GetCursorPos(ipos)
  else
    ipos := nPos;
  mouse_event(MOUSEEVENTF_LEFTDOWN, nPos.X, nPos.Y, 0, 0);
  WSleep(100);
  mouse_event(MOUSEEVENTF_LEFTUP, nPos.X, nPos.Y, 0, 0);
end;

function TWebIntface.MouseDown: Boolean;
begin
  MouseDown(Point(0, 0));
end;

function TWebIntface.MoveMouse(EndPos, OffsetPos: TPoint;stept: Integer = 1): Boolean;
var
  nStartPos, nEndPos: TPoint;
  stX, stY, ptX, ptY: Integer;
  bl: Double;
  nStept: Integer;
  isTrue: Boolean;
begin
  nEndPos.X := EndPos.X + OffsetPos.X;
  nEndPos.Y := EndPos.Y + OffsetPos.Y;
  nStept := 0;
  Repeat
    GetCursorPos(nStartPos);
    if (nStartPos.X = nEndPos.X) and (nStartPos.Y = nEndPos.Y) then break; 
    if nStartPos.X > nEndPos.X then stX := -1 else stX := 1;
    if nStartPos.Y > nEndPos.Y then stY := -1 else stY := 1;
    if Abs(nStartPos.X - nEndPos.X) > Abs(nStartPos.Y - nEndPos.Y) then
    begin
      if Abs(nStartPos.Y - nEndPos.Y) <> Abs(nStartPos.X - nEndPos.X)then
        bl := Abs(nStartPos.Y - nEndPos.Y) / Abs(nStartPos.X - nEndPos.X);
      if nStept = 0 then
        nStept := Abs(nStartPos.X - nEndPos.X) div stept;
      if nstept = 0 then nstept := 1;
      ptX := stX * nStept;
      ptY := Round(stY * nStept * bl);
      isTrue := (nStartPos.X < nEndPos.X - nStept) or ((nStartPos.X > nEndPos.X + nStept));
    end
    else
    begin
      if Abs(nStartPos.Y - nEndPos.Y) <> Abs(nStartPos.X - nEndPos.X) then
        bl := Abs(nStartPos.X - nEndPos.X) / Abs(nStartPos.Y - nEndPos.Y);
      if nStept = 0 then
        nStept := Abs(nStartPos.Y - nEndPos.Y) div stept;
      if nstept = 0 then nstept := 1;
      ptY := stY * nStept;
      ptX := Round(stX * nStept * bl);
      isTrue := (nStartPos.Y < nEndPos.Y - nStept) or ((nStartPos.Y > nEndPos.Y + nStept));
    end;
    if not WSleep(20) then Break;
    mouse_event(MOUSEEVENTF_MOVE, ptX, ptY, 0, GetMessageExtraInfo());

  until not isTrue;
  WSleep(20);
  Application.ProcessMessages;
  SetCursorPos(nEndPos.X, nEndPos.Y);
end;

function TWebIntface.OpenUrl(url, path: string): boolean;
begin
  if path = '' then
    ShellExecute(FParentHandle, 'C:\Program Files\Internet Explorer\iexplore.exe', Pchar(url), nil, nil, SW_SHOWMAXIMIZED)
  else
    ShellExecute(FParentHandle, 'open', Pchar(path), 'http://www.baidu.com', nil, SW_SHOWNORMAL);
end;

function TWebIntface.ScrollUpDown(var Doc: IHTMLDocument2): Boolean;
var
  r: TRect;
  i, sY, ct: Integer;
  body: HTMLBODY;
  Script : HTMLScriptElement;
  a: IHTMLElement;
  Doc3 : IHTMLDocument3;
begin
  Doc3 := (doc as IHTMLDocument3);
  body := ((doc as HTMLDocument).body as HTMLBODY);
  Result := false;
  r := Rect(0, 0, 0, 0);
  GetClientRect(FDocHandle, R);
  if (r.Bottom = 0) and (r.Right = 0) then exit;
  i := R.Top;
  if self.FBrowserType = ie8 then
  begin
    ct := body.scrollHeight; //R.Top + 1800 div (R.Bottom - R.Top)
 {   if Doc.scripts.length > 0 then
      Script:=Doc.scripts.item(0, 0) as HTMLScriptElement;
  script.text := script.text + ';' + #$A#9 + bodyScrollTop;  }

  end
  else ct := r.Bottom * 2;
  while i <= ct do
  begin
  //SendMessage(self.FDocHandle,   WM_VSCROLL,   SB_LINEDown,   0);//           '向上一行
  if self.FBrowserType = ie8 then
  begin
  //InvokeScript(Doc, 'bodyScrollTop', IntToStr(i));
  //body.scrollTop := i;
    Doc3.documentElement.setAttribute('scrollTop', i, 0);
  if not wsleep(10) then break;
  inc(i, 3);
  end
  else
  begin
    SendMessage(FDocHandle,   WM_VSCROLL,   MAKELONG(SB_THUMBPOSITION		,i),  0);
    if not wsleep(10) then break;
    inc(i, 2);
  end;


  end;
   SendMessage(FDocHandle,   WM_VSCROLL,   SB_BOTTOM	, 0);
  WSleep(1000);


  while i >= r.Top do
  begin

    if self.FBrowserType = ie8 then
    begin
        if not wsleep(10) then break;
        //InvokeScript(Doc, 'bodyScrollTop', IntToStr(i));
        Doc3.documentElement.setAttribute('scrollTop', i, 0);
        dec(i, 20);
    end
    else
    begin
      SendMessage(FDocHandle,   WM_VSCROLL,   MAKELONG(SB_THUMBPOSITION		,i),  0);
      if not wsleep(20) then break;
      dec(i, 20);
    end;    
  end;
  SendMessage(FDocHandle,   WM_VSCROLL,   SB_TOP		, 0 );
  exit;
  GetWindowRect(FDocHandle, R);
  MoveMouse(Point(R.Right,R.Top), Point(-10, 50), 50);
  WSleep(1000);
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo);
  i := R.Top - 50;
  sY := 0;
  while i <= R.Bottom do
  begin
    mouse_event( MOUSEEVENTF_MOVE, 0, sY, 0, GetMessageExtraInfo);
    if not WSleep(1) then exit;
    sY := 1;
    inc(i, 1);
  end;
  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo);
  WSleep(3000);
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo);
  i := R.Bottom + 50;
  sy := 0;
  while i > R.Top do
  begin
    mouse_event(MOUSEEVENTF_MOVE, 0, sY, 0, GetMessageExtraInfo);
    if not WSleep(1) then exit;
    sY := 0 - 5;
    dec(i, 5);
  end;
  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo);
  WSleep(100);
  MoveMouse(Point(R.Right -50,R.Bottom div 2), Point(0, 0), 20);
  Result := false;
end;

function TWebIntface.ScrollVBrowser: Boolean;
var
  r: TRect;
begin
  GetWindowRect(FDocHandle, r);
  MoveMouse(Point(r.Right, r.Bottom), Point(-10, -30), 50);
  MouseDown;
end;

function TWebIntface.SetInputKeyWords(Doc: IHTMLDocument2; ElementId,
  KeyWords: string): Boolean;
var
  script: IHTMLScriptElement;
  i: Integer;
begin
Result := False;
  if Assigned(Doc) then
  begin
    try


    for i := 0 to Doc.all.length - 1 do
      if (Doc.all.item(i, i) as IHTMLElement).id = ElementId then
      begin
        (Doc.all.item(i, i) as IHTMLElement).setAttribute('Value', KeyWords, 0);
        Break;
        Result := true;
      end;
      Exit;
    {(Doc as MSHTML.HTMLDocument).getElementById(ElementId).setAttribute('Value', KeyWords, 0);
    if Doc.scripts.length > 0 then
      Script:=Doc.scripts.item(0, 0) as IHTMLScriptElement;
    script.text := Format(setKeyWords, [KeyWords]);
    InvokeScript(Doc, 'setKeyWords', ElementId);   }
    except
      exit;
    end;
  end;
end;

function TWebIntface.SysLog(msg: string): Boolean;
var
  fs: TFileStream;
  s: string;
begin

  Exit;
  if not FileExists(ExtractFilePath(ParamStr(0)) + 'Log.log') then
    fs := TFileStream.Create(ExtractFilePath(ParamStr(0)) + 'Log.log', fmCreate	or fmOpenWrite)
  else
    fs := TFileStream.Create(ExtractFilePath(ParamStr(0)) + 'Log.log', fmOpenWrite);
  try
    fs.Position := fs.Size;
    s := msg + #13#10;
    fs.WriteBuffer(s[1], Length(s));
  finally
  fs.Free;
  end;

end;

function TWebIntface.WSleep(ms: Cardinal): Boolean;
var
  i: Integer;
begin
  result := true;
  if ms > 300 then
  begin
    for i := 0 to ms div 100 do
    begin
      sleep(100);
      Application.ProcessMessages;
      if (not FStart)or(FStop) then
      begin
        result := false;
        break;
      end;
    end;
  end
  else
  begin
    Sleep(ms);
    Application.ProcessMessages;
  end;
  if (not FStart)or(FStop) then
  begin
    result := false;
  end;
end;

end.
