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
    public partial class SmsUserOprate : System.Web.UI.Page
    {
        SmsServer.BLL.SmsUser bll = new SmsServer.BLL.SmsUser();
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
                    AddSmsUser();
                    return;
                }
                else if (strType == "1")    //编辑
                {
                    ModifySmsUser();
                    return;
                }
                else if (strType == "2")    //删除
                {
                    DeleteSmsUser();
                    return;
                }
                else if (strType == "3")    //获取指定管理员的信息
                {
                    GetSmsUserData();
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
        /// <summary>
        /// 删除选中管理员
        /// </summary>
        public void DeleteSmsUser()
        {
            if (object.Equals(null, Request.Form["userid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strUserIDList = Request.Form["userid"].ToString();
            int iIndex = strUserIDList.LastIndexOf(',');
            if (iIndex > 0)
            {
                strUserIDList = strUserIDList.Substring(0, iIndex);
            }
            else
            {
                Response.Write("{success:false,errorInfo:'未选定删除项'}");
                return;
            }
            string[] strArray = strUserIDList.Split(',');
            //变更数据库
            if (bll.Delete(strArray))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'删除失败'}");
            }
        }
        /// <summary>
        /// 获取管理员信息
        /// </summary>
        public void GetSmsUserData()
        {
            if (object.Equals(null, Request.Form["userid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strUserID = Request.Form["userid"].ToString();
            if (strUserID == "")
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strTest = bll.GetSelectSmsUser(strUserID);
            Response.Write(strTest);
        }
        /// <summary>
        /// 修改账户信息
        /// </summary>
        public void ModifySmsUser()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strOut = string.Empty;
            if (bll.Update(strJson, out strOut))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strOut + "'}");
            }
        }
        /// <summary>
        /// 添加账户信息
        /// </summary>
        public void AddSmsUser()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strout = "";
            if (bll.Add(strJson, out strout))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
            }
        }
    }
}
