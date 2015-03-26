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

namespace SmsServer.Web.Data.SmsSpgate
{
    public partial class SmsSpgateInfo : System.Web.UI.Page
    {
        SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
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
            int iBusiness = -1;
            string strSpgate = string.Empty;
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
                if (!object.Equals(null, Request.Form["spgate"]))
                {
                    strSpgate = Request.Form["spgate"].ToString();
                }
                if (!object.Equals(null, Request.Form["type"]))
                {
                    iType = Int32.Parse(Request.Form["type"].ToString());
                }
                if (!object.Equals(null, Request.Form["business"]))
                {
                    iBusiness = Int32.Parse(Request.Form["business"].ToString());
                }
                strout = bll.GetSpgateList(start, limit, iType,iBusiness, strSpgate);
                Response.Write(strout);
            }
            catch (Exception ee)
            {
                Response.Write("success:false");
            }
        }
    }
}
