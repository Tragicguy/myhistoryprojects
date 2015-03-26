using System;
using System.Collections.Generic;
using System.Text;
namespace GmailClass
{
    public class GmailControl
    {
        public static bool ChangePassWord(string strUser,string strOldPwd,string strNewPwd)
        {
            ClassHttpPostEx objHttp = new ClassHttpPostEx();
            bool bRet = Login(objHttp, strUser, strOldPwd);
            if (!bRet)
            {
                return false;
            }
            //获取密码编辑页面
            string strRet = objHttp.GetHttpPage("https://www.google.com/accounts/EditPasswd?hl=zh-cn", "utf-8", "get", "", "", "http://www.google.com.hk", "*/*", "");
            if (strRet.IndexOf("请提供您目前的密码")<0)
            {
                return false;
            }
            string strData = "";
            string strtimeStmp = "";
            string strsecTok = "";  //URLENCODE

            string strTempRet = "";
            strTempRet = strRet.Substring(strRet.IndexOf("id=\"timeStmp\""));
            strtimeStmp = GetKeyString(strTempRet, "value='", "'");

            strTempRet = strRet.Substring(strRet.IndexOf("id=\"secTok\""));
            strsecTok = GetKeyString(strTempRet, "value='", "'");
            
            strsecTok = System.Web.HttpUtility.UrlEncode(strsecTok, System.Text.Encoding.GetEncoding("gb2312"));
            strData = string.Format("hl=zh-cn&timeStmp={0}&secTok={1}&group1=OldPasswd&OldPasswd={2}" +
                "&Passwd={3}&PasswdAgain={4}&p=&save=%E4%BF%9D%E5%AD%98",
                strtimeStmp,
                strsecTok,
                strOldPwd,
                strNewPwd,
                strNewPwd
                );
            strRet = objHttp.GetHttpPage("https://www.google.com/accounts/UpdatePasswd", "utf-8", "post", "", strData, "https://www.google.com/accounts/EditPasswd?hl=zh-cn", "*/*", "");
            if (strRet.IndexOf("新密码已保存")<0)
            {
                return false;
            }
            return true;
        }
        //登录
        private static bool Login(ClassHttpPostEx objHttp,string strUser, string strPwd)
        {
            string strLoginUrl = "http://www.google.com/accounts/Logout?continue=https%3A%2F%2Fwww.google.com%2Faccounts%2FServiceLoginAuth%3Fcontinue%3Dhttp%253A%252F%252Fwww.google.com.hk%252Flanguage_tools%253Fhl%253Dzh-CN%26hl%3Dzh-cn&il=true&zx=8pife4w3cezu";
            string strRet = objHttp.GetHttpPage(strLoginUrl, "utf-8", "get", "", "", "http://www.google.com.hk", "*/*", "");

            //找到标识,证明此页面获取无误
            if (strRet.IndexOf("保持登录状态")<0)
            {
                return false;
            }
            string strData = "";
            string strDsh = "";
            string strGALX = "";

            string strTempRet = "";
            strTempRet = strRet.Substring(strRet.IndexOf("id=\"dsh\""));
            strDsh = GetKeyString(strTempRet, "value=\"", "\"");

            strTempRet = strRet.Substring(strRet.IndexOf("name=\"GALX\""));
            strGALX = GetKeyString(strTempRet, "value=\"", "\"");
            strData = string.Format("continue=http%3A%2F%2Fwww.google.com.hk%2Flanguage_tools%3Fhl%3Dzh-CN&dsh={0}&hl=zh-cn&timeStmp=&secTok=&GALX={1}" +
                "&Email={2}&Passwd={3}&rmShown=1&signIn=%E7%99%BB%E5%BD%95&asts=",
            strDsh,
            strGALX,
            strUser,
            strPwd
            );

            strRet = objHttp.GetHttpPage("https://www.google.com/accounts/ServiceLoginAuth", "utf-8", "post", "", strData, "https://www.google.com/accounts/ServiceLoginAuth?continue=http%3A%2F%2Fwww.google.com.hk%2F&hl=zh-cn", "*/*", "");
            if (strRet.IndexOf("转接")<0)
            {
                return false;
            }
            //得到跳转的链接
            string strUrl = GetKeyString(strRet, "&#39;", "&#39;");
            strUrl = strUrl.Replace("&amp;", "&");
            strRet = objHttp.GetHttpPage(strUrl, "utf-8", "get", "", "", "http://www.google.com.hk", "*/*", "");
            if (strRet.IndexOf("退出</a>")<0)
            {
                return false;
            }
            return true;
        }

        private static string GetKeyString(string strSrc, string strBefore, string strAfter)
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
