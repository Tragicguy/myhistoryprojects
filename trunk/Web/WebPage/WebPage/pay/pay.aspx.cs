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
using System.IO;
using System.Xml.Serialization;
using ClassLibrary;
namespace WebPage.pay
{
    public partial class pay : System.Web.UI.Page
    {
        protected static string m_strUser = ConfigurationSettings.AppSettings["username"];
        protected static string m_strPwd = ConfigurationSettings.AppSettings["userpwd"];
        protected static string m_strName = ConfigurationSettings.AppSettings["nickname"];
        protected static string m_strMail = ConfigurationSettings.AppSettings["Email"];
        protected static double m_dPrice = Double.Parse(ConfigurationSettings.AppSettings["CardPrice"]);
        protected static int    m_iIsVip = int.Parse(ConfigurationSettings.AppSettings["IsVip"].ToString());

        protected void Page_Load(object sender, EventArgs e)
        {
            if (tableID.Visible == false)   //下订单
            {
                Order();
            }
            else
            {
               // Response.Write("bb");
                //return;
                if (Request["checkcode"]==null)
                {
                    return;
                }
                string strCheckCode = Request["checkcode"].ToString();
                bool bRet = Login(m_strUser, m_strPwd, strCheckCode);
                if (!bRet)
                {
                    Response.Write("系统维护中，暂停充值，请联系本站客服手动为您充值。");
                    Response.End();
                    return;
                }
                Order();
            }
        }

        public void Order()
        {
            tableID.Visible = false;
            if (Session["price"] == null || Session["price"].ToString().Trim() == "")
            {
                Response.Write("系统维护中，暂停充值，请联系本站客服手动为您充值。");
                Response.End();
                return;
            }
            string strPrice = Session["price"].ToString();
            double dPrice = 0.00;
            try
            {
                dPrice = Double.Parse(strPrice);
            }
            catch (System.Exception ex)
            {
                Response.Write("系统维护中，暂停充值，请联系本站客服手动为您充值。");
                Response.End();
                return;
            }
//             if (dPrice < 60)
//             {
//                 Response.Write("50元充值卡暂缺，请联系本站客服。");
//                 Response.End();
//                 return;
//             }
            //获取各个在后台配置的信息
            int iNum = (int)(dPrice / m_dPrice);
            int iMemintegral = (int)((int)dPrice % (int)m_dPrice);
            bool bRet = Order(m_iIsVip, iNum, dPrice, iMemintegral);
            if (bRet)
            {
                return;
            }
            if (m_iIsVip == 0)
            {
                Response.Write("系统维护中，暂停充值，请联系本站客服手动为您充值。");
                Response.End();
                return;
            }
            //说明未登录,进入系统登录流程
            tableID.Visible = true;
            //Response.Redirect("pay.aspx");
           // Response.Write("cccccc");
            return;
        }
        public bool Order(int iIsVip, int iNum, double iTotalPrice, int iMemintegral)
        {
            string strCardID = "210207";
            if (iTotalPrice < 60.00)
            {
                strCardID = "210208";
                iMemintegral = 5;
                iNum = 1;
            }
            string strObjData = "";
            if (iIsVip == 1)
            {
                strObjData = string.Format("userid=&Email2=&cardid={6}&hidcardid=2102&paytype=null&quantity={3}&memprice={4}&memintegral={5}&sms_text=&select_paygate=radiobutton&sh_mob={0}&paygate={1}&paygate1=&bankname={2}",
                                                "%C7%EB%CA%E4%D2%C6%B6%AF%CA%D6%BB%FA%BA%C5",
                                                "%D6%A7%B8%B6%B1%A6",
                                                "%D6%A7%B8%B6%B1%A6",
                                                iNum,
                                                iTotalPrice,
                                                iMemintegral,
                                                strCardID
                                                );
            }
            else
            {
                //非会员模式订单
                strObjData = string.Format("userid={6}&Email2={7}&cardid={8}&hidcardid=2102&paytype=null&quantity={3}&memprice={4}&memintegral={5}&sms_text=&select_paygate=radiobutton&sh_mob={0}&paygate={1}&paygate1=&bankname={2}",
                                                "%C7%EB%CA%E4%D2%C6%B6%AF%CA%D6%BB%FA%BA%C5",
                                                "%D6%A7%B8%B6%B1%A6",
                                                "%D6%A7%B8%B6%B1%A6",
                                                iNum,
                                                iTotalPrice,
                                                iMemintegral,
                                                HttpUtility.UrlEncode(m_strName, System.Text.Encoding.GetEncoding("gb2312")),
                                                HttpUtility.UrlEncode(m_strMail, System.Text.Encoding.GetEncoding("gb2312")),
                                                strCardID
                                                );
            }
            string strReffer = string.Format("http://www.ofcard.com/card/{0}.htm", strCardID);
            string strRet = ClassHttpPost.GetHttpPage("http://www.ofcard.com/order.do", "gb2312", "post", "", strObjData, strReffer, "*/*", "");
            if (strRet.IndexOf("第二步：订单确认") < 0)
            {
                return false;
            }
            string strCardNo = "";
            string strAmount = "";
            string strBefore = "<input name=\"OrderNo\" type=\"hidden\" value=\"";
            string strAfter = "\"";
            strCardNo = GetKeyString(strRet, strBefore, strAfter);
            strBefore = "<input name=\"amount\" type=\"hidden\" value=\"";
            //<input name="amount" type="hidden" value="
            strAfter = "\"";
            strAmount = GetKeyString(strRet, strBefore, strAfter);
            strObjData = string.Format("paygate={0}&OrderNo={1}&paygate1=&amount={2}&cardid={3}",
                "%D6%A7%B8%B6%B1%A6",
                strCardNo,
                strAmount,
                strCardID
                );

            strRet = ClassHttpPost.GetHttpPage("http://www.ofcard.com/updatebankstate.do", "gb2312", "post", "", strObjData, "http://www.ofcard.com/order.do", "*/*", "");
            if (strRet.IndexOf("正在转向支付页面") < 0)
            {
                return false;
            }
            
            strBefore = "<form name=\"qqbank\" action=\"";
            strAfter = "\"";
            string strUrl = GetKeyString(strRet, strBefore, strAfter);
            string strJscript = string.Format("<script>top.location='{0}'</script>",strUrl);
            Response.Write(strJscript);
            return true;
        }

        public bool Login(string strUser, string strPwd, string strCheckCode)
        {
            string strUrl = string.Format("http://www.ofcard.com/getrand.jsp?userpsw={0}&name={1}", strPwd, strUser);
            string strRet = ClassHttpPost.GetHttpPage(strUrl, "gb2312", "get", "", "", "http://www.ofcard.com/include/top1/login_iframe.jsp", "*/*", "");

            strPwd = strRet;
            strPwd = strPwd.Trim();
            strPwd = strPwd.Replace("\n", "");
            strPwd = strPwd.Replace("\r", "");
            strPwd = strPwd.Replace("\r\n", "");

            string strObjData = string.Format("{0}\r\n{1}\r\n{2}\r\n{3}\r\n{4}\r\n{5}\r\n{6}\r\n{7}\r\n{8}\r\n{9}",
                                "--A",
                                "Content-Disposition: form-data; name=\"nickName\"\r\n",
                                strUser,
                                "--A",
                                "Content-Disposition: form-data; name=\"logPsw\"\r\n",
                                strPwd,
                                "--A",
                                "Content-Disposition: form-data; name=\"code\"\r\n",
                                strCheckCode,
                                "--A--\r\n");
            strRet = ClassHttpPost.GetHttpPage("http://www.ofcard.com/loginperson.do", "gb2312", "post", "", strObjData, "http://www.ofcard.com/include/top1/login_iframe.jsp", "*/*", "multipart/form-data; boundary=A");
            if (strRet.IndexOf("欢迎您来到") > 0)//登录成功
            {
                return true;
            }
            return false;
        }
        
        public string GetKeyString(string strSrc, string strBefore, string strAfter)
        {
            int iPosBefore = 0;
            int iPosAfter = 0;
            iPosBefore = strSrc.IndexOf(strBefore) + strBefore.Length;
            iPosAfter = strSrc.IndexOf(strAfter, iPosBefore);
            if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0)
            {
                return "";
            }
            else
            {
                return strSrc.Substring(iPosBefore, iPosAfter - iPosBefore);
            }
        }
    }
}
