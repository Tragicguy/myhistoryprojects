<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="WebForm2.aspx.cs" Inherits="SmsServer.Web.WebForm2" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
     <script language=javascript>
       var varJson = "{'Results':[{'CustList':'dfe'},{'PageTotal':'24'},{'PageNow':'2'},{'PageValue':'6'},{'PageName':'451'}]}";       
      
       var objd = JsonDecode(varJson);
       alert(objd.Results[0].CustList);
       
       function JsonDecode(varJson)
       {
       	    varJson = "var obj = " + varJson;
       	     eval(varJson);
       	    return obj;
       }
     </script>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        URL： &nbsp;&nbsp;
        <asp:TextBox ID="TextBox1" runat="server" Width="931px"></asp:TextBox>
        <br />
        Params: &nbsp;
        <asp:TextBox ID="TextBox2" runat="server" Width="936px"></asp:TextBox>
        <br />
        内容： &nbsp;
        <asp:TextBox ID="TextBox5" runat="server"></asp:TextBox><br />
        TimeOut:
        <asp:TextBox ID="TextBox3" runat="server"></asp:TextBox><br />
        Type: &nbsp; &nbsp;&nbsp;
        <asp:TextBox ID="TextBox4" runat="server"></asp:TextBox>
        <br />
        <br />
        <asp:Button ID="Button3" runat="server" OnClick="Button3_Click" Text="编码测试" /><br />
        <br />
        <asp:Button ID="Button5" runat="server" OnClick="Button5_Click" Text="HtmlTable" /><br />
        <br />
        <asp:Button ID="Button4" runat="server" OnClick="Button4_Click" Text="正则测试" /><br />
        <br />
        <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="测试HTTP GET发送" />
        <br />
        <br />
        <asp:Button ID="Button2" runat="server" Text="测试HTTP POST发送" OnClick="Button2_Click" /><br />
        &nbsp;</div>
    </form>
</body>
</html>
