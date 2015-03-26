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

namespace SmsServer.Web.Data.SmsMapScript
{
    public partial class MapScriptInfo : System.Web.UI.Page
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
            string strMapName = string.Empty;
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
                if (!object.Equals(null, Request.Form["mapname"]))
                {
                    strMapName = Request.Form["mapname"].ToString();
                }
                if (!object.Equals(null, Request.Form["type"]))
                {
                    iType = Int32.Parse(Request.Form["type"].ToString());
                }
                strout = new SmsServer.BLL.MapScript().GetMapScriptList(start, limit, strMapName, iType);
                Response.Write(strout);
            }
            catch (Exception ee)
            {
                Response.Write("success:false");
            }
        }
    }
}
