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
    public partial class SpgateOprate : System.Web.UI.Page
    {
        SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
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
                    AddSpgate();
                    return;
                }
                else if (strType == "1")    //编辑
                {
                    ModifySpgate();
                    return;
                }
                else if (strType == "2")    //删除
                {
                    DeleteSpgate();
                    return;
                }
                else if (strType == "3")    //获取指定通道的信息
                {
                    GetSpgateData();
                    return;
                }
                else if (strType == "4")    //获取指定管理员分配的通道
                {
                    GetManagerSpgates();
                    return;
                }
                else if (strType == "5")    //获取指定通道的属性
                {
                    GetSelectSpgateFields();
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

        /// <summary>
        /// 获取指定代理商的通道信息列表
        /// </summary>
        public void GetManagerSpgates()
        {
            SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
            string strJson = bll.GetSpgate(Session["admin"].ToString());
            Response.Write(strJson);
        }
        /// <summary>
        /// 获取指定通道的属性
        /// </summary>
        public void GetSelectSpgateFields()
        {
            if (object.Equals(null, Request.Form["spgateno"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strSpgate= Request.Form["spgateno"].ToString();
            SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
            string strJson = bll.GetSpgateFields(strSpgate);
            Response.Write(strJson);
        }
        /// <summary>
        /// 删除选中管理员
        /// </summary>
        public void DeleteSpgate()
        {
            if (object.Equals(null, Request.Form["spgateno"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strUserIDList = Request.Form["spgateno"].ToString();
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
        public void GetSpgateData()
        {
            if (object.Equals(null, Request.Form["spgate"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strSpgate = Request.Form["spgate"].ToString();
            if (strSpgate == "")
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strTest = bll.GetSelectSpgate(strSpgate);
            Response.Write(strTest);
        }
        /// <summary>
        /// 修改账户信息
        /// </summary>
        public void ModifySpgate()
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
        public void AddSpgate()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strout = "";
            if (bll.Add(strJson,out strout))
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
