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
using SmsServer.BLL;
namespace SmsServer.Web.Data
{
    public partial class TreeShow : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string strout = string.Empty;
            if (SmsServer.BLL.SmsManager.HasPower(GetType().Name, Session["admin"], out strout) != 0)
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
                return;
            }
            int iID = 0;
            if (object.Equals(null, Request.Form["id"]))
            {
                Response.Write("success:false");
                return;
            }
            else
            {
                try
                {
                    string strID = Request.Form["id"].ToString();
                    iID = Int32.Parse(strID);
                }
                catch
                {
                    Response.Write("success:false");
                    return;
                }
            }
            SmsServer.BLL.TreeNode node = new SmsServer.BLL.TreeNode();
            string strJson = node.GetTreeNode(Session["admin"].ToString(), iID);
            strJson = strJson.Replace("\"leaf\":1", "\"leaf\":true");
            strJson = strJson.Replace("\"leaf\":0", "\"leaf\":false");
            Response.Write(strJson);
        }
    }
}
