using System;
using System.Data;
using System.Web;
using System.Collections;
using System.Web.Services;
using System.Web.Services.Protocols;
using System.ComponentModel;
using SmsServer.BLL;
namespace SmsServer.Web
{
    /// <summary>
    /// MsgGate 的摘要说明
    /// </summary>
    [WebService(Namespace = "http://tempuri.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [ToolboxItem(false)]
    public class MsgGate : System.Web.Services.WebService
    {

        [WebMethod]
        public string SendMessage(string strUserID,string strPwd,string strPhone,string strMsg,string strSubport)
        {
            return new SmsMessageSend().SendMessage(strUserID,strPwd,strPhone,strMsg,strSubport);
        }
    }
}
