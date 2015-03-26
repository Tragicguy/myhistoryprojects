
<%@ Application Language="C#" %>

<script runat="server">

void Application_Start(object sender, EventArgs e) 
    {
        new SmsServer.BLL.SmsLog().WriteDataBaseLog(2,"网站启动","","");
        //初始化系统配置信息
        SmsServer.BLL.SmsSetIni.InitParams(HttpContext.Current.Server.MapPath("dataset.ini"));  
        //初始化消息队列
        SmsServer.BLL.QueueManage<SmsServer.Model.SmsTaskA>.EnsureQueueExists(SmsServer.BLL.SmsSetIni.TaskQueuePath);   
        //初始化号段信息
        SmsServer.BLL.DataCache.InitCacheSection();
        //初始化非法关键词
        SmsServer.BLL.DataCache.InitCacheIllegal();
        //自动启动发送线程
        SmsServer.BLL.SendThreadMgr.AutoStartSendThreads();
        //test 
        //SmsServer.Model.MapScript objMap = new SmsServer.Model.MapScript();
        //SmsServer.BLL.DataCache.GetCacheMapScript("2222", ref objMap);
    }
    
    void Application_End(object sender, EventArgs e) 
    {
        //  在应用程序关闭时运行的代码;
        new SmsServer.BLL.SmsLog().WriteDataBaseLog(2, "Application_End函数被触发", "", "" );
        try
        {
            string strPortUrl = SmsServer.BLL.SmsSetIni.KeepQueryUrl + "OnlineQuery.aspx?";
            string strParams = "url=" + HttpUtility.UrlEncode(SmsServer.BLL.SmsSetIni.KeepOnlineUrl) + "&" + DateTime.Now.ToString();
            string strRet = SmsServer.BLL.SmsHttpSend.HttpSendMessge(ref strPortUrl, ref strParams, 40000, 0, 2);
            strRet = string.Format("结果[{1},Application_End时访问指定页面：{0}]", strPortUrl, strRet);
            new SmsServer.BLL.SmsLog().WriteDataBaseLog(2, strRet, "", "");
        }
        catch
        {
            new SmsServer.BLL.SmsLog().WriteDataBaseLog(2, "请求激活网站时出现异常", "", "");
        }        
    }
        
    void Application_Error(object sender, EventArgs e) 
    { 
        // 在出现未处理的错误时运行的代码

    }

    void Session_Start(object sender, EventArgs e) 
    {
        // 在新会话启动时运行的代码
        
    }

    void Session_End(object sender, EventArgs e) 
    {
        // 在会话结束时运行的代码。 
        // 注意: 只有在 Web.config 文件中的 sessionstate 模式设置为
        // InProc 时，才会引发 Session_End 事件。如果会话模式设置为 StateServer 
        // 或 SQLServer，则不会引发该事件。
    }
       
</script>
