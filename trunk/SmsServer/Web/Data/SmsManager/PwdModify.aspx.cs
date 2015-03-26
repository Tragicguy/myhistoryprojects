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

namespace SmsServer.Web.Data.SmsManager
{
    public partial class PwdModify : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            //权限检测
            string strout = string.Empty;
            if (SmsServer.BLL.SmsManager.HasPower(GetType().Name, Session["admin"], out strout) != 0)
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
                return;
            }

            try
            {
                if (object.Equals(null, Request.Form["jdata"]))  //没有类型控制参数
                {
                    Response.Write("{success:false,errorInfo:'非法操作'}");
                    return;
                }
                string strJson = Request.Form["jdata"].ToString();
                strJson = strJson.Replace("ReNewManagerPwd", "ExtraInfo");
                string strErrorInfo = string.Empty;
                SmsServer.BLL.SmsManager bll = new SmsServer.BLL.SmsManager();
                if (bll.ModifyPwd(strJson, Session["admin"].ToString(), out strErrorInfo))
                {
                    Response.Write("{success:true}");
                }
                else
                {
                    Response.Write("{success:false,errorInfo:'" + strErrorInfo + "'}");
                }
            }
            catch (System.Exception ee)
            {
                Response.Write("{success:false,errorInfo:'服务器异常'}");
                return;
            }
        }
    }
}
