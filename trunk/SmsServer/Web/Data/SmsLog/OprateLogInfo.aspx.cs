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

namespace SmsServer.Web.Data.SmsLog
{
    public partial class OprateLogInfo : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string strout = string.Empty;
            if (SmsServer.BLL.SmsManager.HasPower(GetType().Name, Session["admin"], out strout) != 0)
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
                return;
            }
            int start = 0;
            int limit = 0;
            int iType = -1;
            string strOptID = string.Empty;
            string strDate = string.Empty;
            try
            {
                string starts = Request.Form["start"];
                string limits = Request.Form["limit"];
                if (starts != null)
                {
                    start = int.Parse(starts);
                }
                if (limits != null)
                {
                    limit = int.Parse(limits);
                }
                if (!string.IsNullOrEmpty(Request.Form["opratorID"] ))
                {
                    strOptID = Request.Form["opratorID"].ToString();
                }                
                if (!string.IsNullOrEmpty(Request.Form["type"]))
                {
                    iType = Int32.Parse(Request.Form["type"].ToString());
                }
                if (!string.IsNullOrEmpty(Request.Form["optdate"]))
                {
                    strDate = Request.Form["optdate"].ToString();
                }
                strout = new SmsServer.BLL.SmsLog().GetLogList(start, limit, iType,strOptID,strDate);
                Response.Write(strout);
            }
            catch (Exception ee)
            {
                Response.Write("success:false");
            }
        }
    }
}
