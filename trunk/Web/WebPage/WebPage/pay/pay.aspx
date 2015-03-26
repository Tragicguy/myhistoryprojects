<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="pay.aspx.cs" Inherits="WebPage.pay.pay" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>正在为您跳转到银行......</title>
</head>
<body>
    <form id="form1" runat="server">
    <div align="center">
    <table runat="Server" align="center" id="tableID" visible="false">
<tr>
<td valign="bottom">
请输入验证码:&nbsp;&nbsp;
</td>
<td><input type='text' name='checkcode' value='' style="height:26px;font-size:29;">
</td>
<td valign="top"> <img src='../ShowCode.aspx' style="border: 1px solid #CCCCCC;"  width=80 height=30 onclick="this.src='../ShowCode.aspx?'+Math.random()">
<span id="yanzhengma">(看不清验证码可点击图片更换)</span></td>
</tr>
<tr>
<td>
</td>
<td align="center"><input type='submit' style="height:50;width:109;" value="点击这里提交">
</td>
<td>
</td>
</tr>
</table>
</div>
</form>
</body>
</html>
