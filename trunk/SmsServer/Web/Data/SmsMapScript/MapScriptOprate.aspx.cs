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
    public partial class MapScriptOprate : System.Web.UI.Page
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
                    AddMapScript();
                    return;
                }
                else if (strType == "1")    //编辑
                {
                    ModifyMapScript();
                    return;
                }
                else if (strType == "2")    //删除
                {
                    DeleteMapScript();
                    return;
                }
                else if (strType == "3")    //获取指定映射的信息
                {
                    GetMapScriptData();
                    return;
                }
                else if (strType == "4")    //获取映射列表
                {
                    GetMapScriptList();
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
        private void AddMapScript()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strout = "";
            if (new SmsServer.BLL.MapScript().Add(strJson, out strout))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
            }
        }
        private void ModifyMapScript()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strOut = string.Empty;
            if (new SmsServer.BLL.MapScript().Update(strJson, out strOut))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'" + strOut + "'}");
            }
        }
        private void DeleteMapScript()
        {
            if (string.IsNullOrEmpty(Request.Form["mapid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strMapIDList = Request.Form["mapid"].ToString();
            int iIndex = strMapIDList.LastIndexOf(',');
            if (iIndex > 0)
            {
                strMapIDList = strMapIDList.Substring(0, iIndex);
            }
            else
            {
                Response.Write("{success:false,errorInfo:'未选定删除项'}");
                return;
            }
            string[] strArray = strMapIDList.Split(',');
            //变更数据库
            if (new SmsServer.BLL.MapScript().Delete(strArray))
            {
                Response.Write("{success:true}");
            }
            else //失败
            {
                Response.Write("{success:false,errorInfo:'删除失败'}");
            }     
        }
        private void GetMapScriptData()
        {
            if (string.IsNullOrEmpty(Request.Form["mapid"]))
            {
                Response.Write("{success:false,errorInfo:'非法操作'}");
                return;
            }
            string strMapID = Request.Form["mapid"].ToString();
            string strJson = new SmsServer.BLL.MapScript().GetMapScriptFields(strMapID);
            Response.Write(strJson);
        }
        //获取映射列表
        private void GetMapScriptList()
        {
            Response.Write(new BLL.MapScript().GetMapScriptList());
        }
    }
}
