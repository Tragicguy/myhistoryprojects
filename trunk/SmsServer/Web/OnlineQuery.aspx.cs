using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Threading;
namespace SmsServer.Web
{
    public partial class OnlineQuery : System.Web.UI.Page
    {
        public Thread KeepOnlineThread;
        protected void Page_Load(object sender, EventArgs e)
        {
            //访问来了的时候，读取参数
            string strKeepUrl = "";
            if (string.IsNullOrEmpty(Request.QueryString["url"]))
            {
                Response.Write("Illegal access ");
                return;
            }
            strKeepUrl = Request.QueryString["url"].ToString() + "keeponline.aspx?";
            KeepOnlineThread = new Thread(new ParameterizedThreadStart(KeepOnlineProc));
            KeepOnlineThread.Start(strKeepUrl);
            Response.Write("received");
        }
        private void KeepOnlineProc(object objUrl)
        {
            string strKeepUrl = objUrl.ToString();
            int iRunCount = 0;
            while(true)
            {
                if (iRunCount > 3)
                {
                    break;
                }
                Thread.Sleep(3000); //暂停两秒
                iRunCount++;
                string strParams = "&" + DateTime.Now.ToString();
                string strRet = SmsServer.BLL.SmsHttpSend.HttpSendMessge(ref strKeepUrl, ref strParams, 25000, 0, 2);               
                if (strRet.Length > 2)
                {
                    if (strRet.Substring(0,2)=="OK")
                    {
                        break;
                    }
                    else
                    {
                        Thread.Sleep(10000);                        
                    }
                }
            }
            
        }
    }
}
