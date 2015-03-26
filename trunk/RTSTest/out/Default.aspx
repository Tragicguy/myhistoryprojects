<%@ Page Language="C#" AutoEventWireup="true"  CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
    <script language="javascript">
    function changeImg()
    {
	alert('test message');
   /*window.open("http://www.baidu.com","_blank", "height=400, width=600, top=0, left=255, toolbar=no,menubar=no, scrollbars=no, resizable=no,location=no, status=no");
*/
       // var obj = document.GetElementById('aa');
      //  obj.src = '1.jpg';
      //  i = i==1?2:1;
    }
    
    </script>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    testing.....
    </div>
    <div>
    <a href="svn.rar">RAR文件下载</a>.....
    </div>
    <div>
    testing.....
    </div>
<a target="_blank" href="http://www.baidu.com">BAIDU</a>.....

<a href="2.jpg">BAIDU</a>.....
    <img id='aa' src="1.jpg">
    <br />
    <button onclick='changeImg();'>change</button>
    </form>
</body>
</html>
