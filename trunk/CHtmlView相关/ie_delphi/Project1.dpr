program Project1;

uses
  Forms,
  Unit1 in 'Unit1.pas' {Form1},
  uWebIntface in 'uWebIntface.pas',
  uWebInc in 'uWebInc.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.Icon := nil;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
