<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="index.aspx.cs" Inherits="WebPage.pay.index" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>正在为您跳转到银行......</title>
    <script>
self.moveTo(0,0);
self.resizeTo(screen.availWidth,screen.availHeight);
</script>
</head>

<body style="text-align:center;" onload="document.wjrbiaodan.submit();">
<img src="wjrloading.gif" border=0>正在为您跳转到银行......
<iframe src="about:blank" id="kaixin" name="kaixin" align="center" width="1000"  height="501" marginwidth="1" marginheight="1" frameborder="0" scrolling="no"> </iframe>
<form method="get" name="wjrbiaodan" id="wjrbiaodan" action="pay.aspx" target="kaixin">
</form>
</body>
</html>
