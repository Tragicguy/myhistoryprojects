using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace GmailClass
{
    public class PayPalQuery
    {
        /// <summary>
        /// 查询失败返回空字符串
        /// </summary>
        /// <param name="strUser"></param>
        /// <param name="strPwd"></param>
        /// <returns></returns>
        public static string QueryBalance(string strUser,string strPwd)
        {
            //<form method="post" name="login_form" action="https://www.paypal.com/c2/cgi-bin/webscr?cmd=_login-submit&amp;dispatch=5885d80a13c0db1f8e263663d3faee8d61ec37c409b56235bed2ddf64505aee9">
            ClassHttpPostEx objHttp = new ClassHttpPostEx();
            string strRet = objHttp.GetHttpPage("https://www.paypal.com/c2", "gb2312", "get", "", "", "http://www.bai.com", "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            if (strRet.IndexOf("Welcome - PayPal") < 0 && strRet.IndexOf("欢迎 - PayPal") < 0)
            {
                return "";
            }
            string strData = string.Format("login_email={0}&login_password={1}&target_page=0&submit.x=%E7%99%BB%E5%BD%95"+
                "&form_charset=UTF-8&browser_name=Microsoft+Internet+Explorer&browser_version=8&operating_system=Windows",
                strUser,
                strPwd
                );
            ///c2/cgi-bin/webscr?cmd=_login-submit&dispatch=5885d80a13c0db1f8e263663d3faee8d61ec37c409b56235bed2ddf64505aee9
            string strLoginUrl = GetKeyString(strRet,"<form method=\"post\" name=\"login_form\" action=\"","\">");
            strLoginUrl = strLoginUrl.Replace("&amp;", "&");
            strRet = objHttp.GetHttpPage(strLoginUrl, "gb2312", "post", "", strData, "https://www.paypal.com/c2/cgi-bin/webscr?cmd=_logout", "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            
            //&#27491;&#22312;&#30331;&#24405;
            if (strRet.IndexOf("<title>正在登录 - PayPal</title>")<0)
            {
                return "";
            }
            //<p class="note">如果此页面的显示时间超过5秒钟，请<a href="https://www.paypal.com/c2/cgi-bin/webscr?cmd=_login-done&amp;login_access=1297172353">点击此处</a>重新加载。</p>
            strLoginUrl = GetKeyString(strRet,"如果此页面的显示时间超过5秒钟，请<a href=\"", "\">点击此处");
            strLoginUrl = strLoginUrl.Replace("&amp;", "&");
            strRet = objHttp.GetHttpPage(strLoginUrl, "utf-8", "get", "", "", "", "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            if (strRet.IndexOf("PayPal余额：")<0)
            {
                return "";
            }
            string strTempRet = strRet.Substring(strRet.IndexOf("PayPal余额："));
            string strBalance = GetKeyString(strTempRet, "<strong>", "</strong>");
            return strBalance;
        }

        public static bool PayMoney(string strAcc,string strPwd,string strRecver,float fMoney,out string strError)
        {
            strError = "";
            ClassHttpPostEx objHttp = new ClassHttpPostEx();
            string strRet = objHttp.GetHttpPage("https://www.paypal.com/c2", "gb2312", "get", "", "", "http://www.bai.com", "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            if (strRet.IndexOf("Welcome - PayPal") < 0 && strRet.IndexOf("欢迎 - PayPal") < 0)
            {
                strError = "获取登录页面失败";
                return false;
            }
            string strData = string.Format("login_email={0}&login_password={1}&target_page=0&submit.x=%E7%99%BB%E5%BD%95" +
                "&form_charset=UTF-8&browser_name=Microsoft+Internet+Explorer&browser_version=8&operating_system=Windows",
                strAcc,
                strPwd
                );
            string strLoginUrl = GetKeyString(strRet, "<form method=\"post\" name=\"login_form\" action=\"", "\">");
            strLoginUrl = strLoginUrl.Replace("&amp;", "&");
            strRet = objHttp.GetHttpPage(strLoginUrl, "gb2312", "post", "", strData, "https://www.paypal.com/c2/cgi-bin/webscr?cmd=_logout", "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);

            if (strRet.IndexOf("<title>正在登录 - PayPal</title>") < 0)
            {
                strError = "登录失败";
                return false;
            }
          
            string strLoginAccess = GetKeyString(strRet, "如果此页面的显示时间超过5秒钟，请<a href=\"https://www.paypal.com/c2/cgi-bin/webscr?cmd=_login-done&login_access=", "\"");
            string strUrl = "https://www.paypal.com/c2/cgi-bin/webscr?cmd=_send-money&nav=1";
            string strReffer = string.Format("https://www.paypal.com/c2/cgi-bin/webscr?cmd=_login-done&login_access={0}", strLoginAccess);
            strRet = objHttp.GetHttpPage(strUrl, "gb2312", "get", "", strData, strReffer, "*/*", "");   //1299341733
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            if (strRet.IndexOf("您可以为购买和服务付款") < 0 && strRet.IndexOf("您可以为购买的物品和服务付款") < 0 )
            {
                strError = "打开付款页面失败";
                return false;
            }
            string strContext = GetKeyString(strRet,"id=\"CONTEXT_CGI_VAR\" name=\"CONTEXT\" value=\"","\"");
            string strAuth = GetKeyString(strRet,"name=\"auth\" type=\"hidden\" value=\"","\"");
            strData = "";
            //
            string strPostUrl = GetKeyString(strRet, "<form method=\"post\" id=\"rosetta\" class=\"rosetta\" action=\"", "\"");
            //https://www.paypal.com/c2/cgi-bin/webscr?cmd=_flow&SESSION=lZS9WYNF9O_e9u3oSF8jeGCAvKxKfU-WtkSiAPPVH4SmnGVeY-so_Ju7sOC&dispatch=5885d80a13c0db1f8e263663d3faee8d61ec37c409b56235bed2ddf64505aee9
            strReffer = strPostUrl;// +"&cmd=_flow";
            strData = string.Format("CONTEXT={0}&cmd=_flow&sender_email={1}&currency_out=USD&email_acInput={2}&email={2}&emailPrevValue={2}&amount={3}&amount_ccode=USD&payment_type=G&submit.x=%E7%BB%A7%E7%BB%AD&auth={4}&form_charset=UTF-8",
                strContext,
                strAcc,
                strRecver,
                fMoney,
                strAuth);
            strRet = objHttp.GetHttpPage(strPostUrl, "gb2312", "post", "", strData, strReffer, "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            if (strRet.IndexOf("查看您的付款并发送")<0)
            {
                strError = "尝试付款失败";
                return false;
            }
            string strAddr = GetKeyString(strRet, "<input type=\"radio\" class=\"radio\" id=\"adr-", "\"");

            strData = string.Format("CONTEXT={0}&myAllTextSubmitID=&cmd=_flow&pie_code_manual=&no_adr_required=%E6%97%A0%E9%9C%80%E9%80%81%E8%B4%A7&address="+
                "{1}&shipping_state=&shipping_city=&shipping_address1=&shipping_address2=&shipping_zip=&shipping_country_name=C2&subject=%E6%82%A8%E6%94%B6%E5%88%B0%E4%BB%98%E6%AC%BE%E4%BA%86&enterPersonalMessage=%E8%BE%93%E5%85%A5%E7%BB%99%E6%8E%A5%E6%94%B6%E4%BA%BA%E7%9A%84%E4%B8%AA%E4%BA%BA%E6%B6%88%E6%81%AF%E3%80%82&perMsg=%E8%BE%93%E5%85%A5%E7%BB%99%E6%8E%A5%E6%94%B6%E4%BA%BA%E7%9A%84%E4%B8%AA%E4%BA%BA%E6%B6%88%E6%81%AF%E3%80%82&note=&DefaultSubject=%E6%82%A8%E6%94%B6%E5%88%B0%E4%BB%98%E6%AC%BE%E4%BA%86&FlagSubjectChanged=false&submit.x=%E4%BB%98%E6%AC%BE&auth="+
                "{2}&form_charset=UTF-8",
                strContext,
                strAddr,
                strAuth);
            strPostUrl = GetKeyString(strRet, "class=\"safeSubmit\" id=\"reviewForm\" name=\"main\" action=\"", "\"");
            strRet = objHttp.GetHttpPage(strPostUrl, "utf-8", "post", "", strData, strReffer, "*/*", "");
            strRet = System.Web.HttpUtility.HtmlDecode(strRet);
            if (strRet.IndexOf("钱款已发送") > 0)
            {
                strError = "付款成功";
                return true;
            }
            strError = "付款失败";
            return false;
            
        }
        private static string GetKeyString(string strSrc, string strBefore, string strAfter)
        {
            if (strSrc == "")
            {
                return "";
            }
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
