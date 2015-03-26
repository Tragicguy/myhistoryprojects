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
    public partial class IllegalOperate : System.Web.UI.Page
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
                if (strType == "0")     //添加
                {
                    AddIllegal();
                    return;
                }
                else if (strType == "1")    //编辑
                {
                    ModifyIllegal();
                    return;
                }
                else if (strType == "2")    //删除
                {
                    DeleteIllegal();
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
        private void AddIllegal()
        {
            if (object.Equals(null, Request.Form["name"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strName = Request.Form["name"].ToString();
            string strout = string.Empty;
            if (new BLL.SmsIllegal().Add(strName, out strout))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
            }
        }
        private void ModifyIllegal()
        {
            if (object.Equals(null, Request.Form["name"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            if (object.Equals(null, Request.Form["id"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            int iId = Convert.ToInt32(Request.Form["id"].ToString());            
            string strName = Request.Form["name"].ToString();
            string strout = string.Empty;
            if (new BLL.SmsIllegal().Update(iId, strName, out strout))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
            }
        }
        private void DeleteIllegal()
        {
            string strIDList = Request.Form["id"].ToString();
            int iIndex = strIDList.LastIndexOf(',');
            if (iIndex > 0)
            {
                strIDList = strIDList.Substring(0, iIndex);
            }
            else
            {
                Response.Write("{success:false,errorInfo:'未选定删除项'}");
                return;
            }
            string[] strArray = strIDList.Split(',');
            //变更数据库
            if (new BLL.SmsIllegal().Delete(strArray))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'删除失败'}");
            }   
        }
    }
}
