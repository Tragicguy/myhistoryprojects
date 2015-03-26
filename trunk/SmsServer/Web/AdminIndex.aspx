<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="AdminIndex.aspx.cs" Inherits="SmsServer.Web.AdminIndex" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
<title>短信管理页面</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<link href="./CSS/Loading.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div id="loading">
    <div  class="loading-indicator" >
        <img src="Images/extanim32.gif" alt="" width="32" height="32" style="margin-right:8px;" align="center"/>
            Loading......            
    </div>
     </div>
     <div id="loading-mask">     
     </div>
    <iframe width='0px' height = '0px' style="border:0px;" visible="false" id='downFileFrame'></iframe>
    
<link href="./CSS/css.css" rel="stylesheet" type="text/css" />
<link rel="stylesheet" type="text/css" href="./ext/resources/css/ext-all.css" />
<script type="text/javascript" src="./ext/adapter/ext/ext-base.js"></script>
<script type="text/javascript" src="./ext/ext-all.js"></script>
<script type="text/javascript" src="./ext/ext-lang-zh_CN.js"></script>
<script type="text/javascript" src="./js/extTab.js" charset="utf-8"></script> 
<script type="text/javascript" src="./js/extTree.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/All.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/SmsManager/SmsManagerAuth.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsManager/SmsManagerEdit.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsManager/SmsManagerAdd.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsManager/SmsManagerInfo.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsManager/ManagerPwdModify.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsManager/SmsManagerPortSet.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/SmsSpgate/SmsSpgateEdit.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsSpgate/SmsSpgateInfo.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/SmsUser/SmsUserEdit.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsUser/SmsUserInfo.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/SmsMapScript/MapScriptEasyEdit.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsMapScript/MapScriptEdit.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsMapScript/MapScriptInfo.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/SmsConfig/PhoneSection.js" charset="utf-8"></script>
<script type="text/javascript" src="./js/SmsConfig/SmsIllegalInfo.js" charset="utf-8"></script>
<!--log-->
<script type="text/javascript" src="./js/SmsLog/OprateLogInfo.js" charset="utf-8"></script>

<script type="text/javascript" src="./js/SmsHistory/SmsHistoryInfo.js" charset="utf-8"></script>
<script type="text/javascript">
Ext.onReady(function(){            //布局
Ext.get('loading').remove();
Ext.get('loading-mask').fadeOut({ remove: true }); 
new Ext.Viewport({
layout:"border",
items:[{region:"north",     //头部
height:60,
margins:'6 6 0 6',
title:"header"},
{region:"south",
height:50,
title:"底部面板",
margins:'0 6 6 6',
content:"aaaa"},
tabPanel,
tree
]
});
tree.expandAll();   
});
</script> 
</body>
</html>
