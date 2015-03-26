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

namespace SmsServer.Web.Data.SmsConfig
{
    public partial class PhoneSectionEdit : System.Web.UI.Page
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
            //加一个type 来判断是添加,编辑,删除
            try
            {
                if (object.Equals(null, Request.Form["type"]))  //没有类型控制参数
                {
                    Response.Write("{success:false,errorInfo:'非法操作'}");
                    return;
                }
                string strType = Request.Form["type"].ToString();               
                if (strType == "1")    //编辑
                {
                    ModifyPhoneSection();
                    return;
                }                
                else if (strType == "3")    //获取指定映射的信息
                {
                    LoadPhoneSection();
                    return;
                }               
                else
                {
                    Response.Write("{success:false,errorInfo:'非法操作'}");
                    return;
                }
            }
            catch (System.Exception ee)
            {
                Response.Write("{success:false,errorInfo:'服务器异常'}");
                return;
            }
        }
        //保存号段信息
        private void ModifyPhoneSection()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strOut = string.Empty;
            if (new SmsServer.BLL.PhoneSection().Update(strJson,  out strOut))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strOut + "'}");
            }
        }
        //加载号段信息
        private void LoadPhoneSection()
        {
            string strJson = new SmsServer.BLL.PhoneSection().GetPhoneSectionFields();
            Response.Write(strJson);
        }
    }
}
