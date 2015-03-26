<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="WebForm1.aspx.cs" Inherits="SmsServer.Web.WebForm1" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
    <script >
    var GetRetValue = function(varSrcInfo){return {RetValue:0,NeedResend:0,RetInfo:varSrcInfo};}
    function test()
    {
        //eval("var GetRetValue = function(varSrcInfo){return {RetValue:0,NeedResend:0,RetInfo:varSrcInfo};} var objAA = GetRetValue(\"你好吗？\");" );
         //var objAA = GetRetValue("你好吗？");
         return escape("你好吗？");
    }
    
    </script>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <asp:TextBox ID="TextBox1" runat="server" TextMode="MultiLine"></asp:TextBox>
        <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text='Button' />
        <asp:TextBox ID="TextBox2" runat="server"></asp:TextBox>
        <asp:Button ID="Button2" runat="server" OnClick="Button2_Click" Text="Button" />
        <asp:Button ID="Button3" runat="server" OnClick="Button3_Click" Text="Button" />
        <asp:Button ID="Button6" runat="server" OnClick="Button6_Click" CommandArgument = "222" Text="Button" /><br />
        <br />
        <asp:TextBox ID="TextBox3" runat="server" Height="271px" TextMode="MultiLine" Width="927px"></asp:TextBox><br />
        <br />
        <asp:Button ID="Button4" runat="server" OnClick="Button4_Click" Text="Test JS" />
        <input id="Button5" type="button" onclick='alert(test());' value="button" />
        <asp:Button ID="Button7" runat="server" OnClick="Button7_Click" Text="启动" />
        <asp:Button ID="Button8" runat="server" OnClick="Button8_Click" Text="停止" />
        <input id="Button9" type="button" value="button" /></div>
    </form>
</body>
</html>