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

namespace SmsServer.Web.Data.SmsUser
{
    public partial class SmsUserInfo : System.Web.UI.Page
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
            int iStatus = -1;
            string strUserID = string.Empty;
            string strParentID = string.Empty;
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
                if (!object.Equals(null, Request.Form["userid"]))
                {
                    strUserID = Request.Form["userid"].ToString();
                }
                if (!object.Equals(null, Request.Form["status"]))
                {
                    iStatus = Int32.Parse(Request.Form["status"].ToString());
                }
                if (!object.Equals(null, Request.Form["parentid"]))
                {
                    strParentID = Request.Form["parentid"].ToString();
                }
                strout = new SmsServer.BLL.SmsUser().GetUserList(start, limit,strUserID,strParentID,iStatus);
                Response.Write(strout);
            }
            catch (Exception ee)
            {
                Response.Write("success:false");
            }
        }
    }
}
