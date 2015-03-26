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
    public partial class SpgateType_Bus_List : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (object.Equals(null, Request.Form["type"]))  //没有类型控制参数
            {
               // Response.Write("{success:false,errorInfo:'非法操作'}");
                //return;
            }
            string strType = "1";
            //string strType = Request.Form["type"].ToString();
            if (strType == "1")     //类型
            {
                GetBusinessList();
                return;
            }
            else if (strType == "2")    //支持的号段
            {
               // ModifySpgate();
                return;
            }
            else if (strType == "2")    //符合条件的通道列表
            {
                //DeleteSpgate();
                return;
            }
        }
        private void GetBusinessList()
        {
            try
            {
                string strOut =SmsServer.BLL.SmsBusiness.GetBusinessList();
                Response.Write(strOut);
            }
            catch (System.Exception e)
            {
                Response.Write("{success:false,errorInfo:'服务器异常'}");
            }
            
        }
    }
}
