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

namespace SmsServer.Web.Data
{
    public partial class Login : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string strUserID = string.Empty;
            string strUserPwd = string.Empty;
            if (string.IsNullOrEmpty(Request.Form["userId"]))
            {
                Response.Write("{success:false,errorInfo:'用户名不能为空'}");
                return;
            }
            if (string.IsNullOrEmpty(Request.Form["passWord"]))
            {
                Response.Write("{success:false,errorInfo:'密码不能为空'}");
                return;
            }
            strUserID = Request.Form["userId"].ToString();
            strUserPwd = Request.Form["passWord"].ToString();
            if (new SmsServer.BLL.SmsManager().ManagerLogin(strUserID,strUserPwd))
            {
                //登录成功
                Session["admin"] = strUserID;
                Response.Write("{success:true}");
                return;
            }
            Response.Write("{success:false,errorInfo:'用户名或密码错误'}");
        }
    }
}
