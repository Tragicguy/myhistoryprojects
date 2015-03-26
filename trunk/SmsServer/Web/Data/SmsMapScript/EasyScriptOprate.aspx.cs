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
    public partial class EasyScriptOprate : System.Web.UI.Page
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
                    AddEasyScript();
                    return;
                }
                else if (strType == "1")    //编辑
                {
                    ModifyEasyScript();
                    return;
                }
                else if (strType == "2")    //删除
                {
                    DeleteEasyScript();
                    return;
                }
                else if (strType == "3")    //获取指定映射的信息
                {
                    Response.Write("{success:false,errorInfo:'非法操作'}");
                    return;
                }
                else if (strType == "4")    //获取所有的规则映射的信息
                {
                    GetEasyScript();
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
        private void AddEasyScript()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            if (string.IsNullOrEmpty(Request.Form["mapid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strMapID = Request.Form["mapid"].ToString();
            string strJson = Request.Form["jdata"].ToString();
            string strout = "";
            if (new SmsServer.BLL.MapScriptDetail().Add(strJson, strMapID,out strout))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
            }
        }
        private void ModifyEasyScript()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            if (string.IsNullOrEmpty(Request.Form["mapid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strMapID = Request.Form["mapid"].ToString();
            string strJson = Request.Form["jdata"].ToString();
            string strOut = string.Empty;
            if (new SmsServer.BLL.MapScriptDetail().Update(strJson,strMapID, out strOut))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strOut + "'}");
            }
        }
        private void DeleteEasyScript()
        {
            if (string.IsNullOrEmpty(Request.Form["id"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            if (string.IsNullOrEmpty(Request.Form["mapid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strIDList = Request.Form["id"].ToString();
            string strMapID = Request.Form["mapid"].ToString();
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
            if (new SmsServer.BLL.MapScriptDetail().Delete(strArray, strMapID))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'删除失败'}");
            }
        }
        /// <summary>
        /// 获取所有的规则映射的信息
        /// </summary>
        private void GetEasyScript()
        {
            int start = 0;
            int limit = 0;
            int iType = -1;
            string strMapID = string.Empty;
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
                if (string.IsNullOrEmpty(Request.Form["mapid"]))
                {
                    Response.Write("success:false");
                    return;
                }
                strMapID = Request.Form["mapid"].ToString();
                string strout = new SmsServer.BLL.MapScriptDetail().GetEasyScriptList(start, limit, strMapID);
                Response.Write(strout);
            }
            catch (Exception ee)
            {
                Response.Write("success:false");
            }
        }
    }
}
