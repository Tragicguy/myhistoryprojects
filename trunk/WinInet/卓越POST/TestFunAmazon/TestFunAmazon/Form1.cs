using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ClassLibrary;
namespace TestFunAmazon
{
    public partial class Form1 : Form
    {
        ClassHttpPost m_webPost;
        string strToken = "";
        public Form1()
        {
            InitializeComponent();
            m_webPost = new ClassHttpPost();
            textBox2.Text = "shaojun@qq.com";
            textBox3.Text = "123456";
        }
        private string GetKeyString(string strSrc, string strBefore, string strAfter)
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
        private void button1_Click(object sender, EventArgs e)
        {

            string strUrl = "https://www.amazon.cn/ap/signin?_encoding=UTF8&openid.assoc_handle=cnflex&openid.return_to=https%3A%2F%2Fwww.amazon.cn%2Fgp%2Fyourstore%3Fie%3DUTF8%26ref_%3Dpd_irl_gw&openid.mode=checkid_setup&openid.ns=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0&openid.claimed_id=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.pape.max_auth_age=0&openid.ns.pape=http%3A%2F%2Fspecs.openid.net%2Fextensions%2Fpape%2F1.0&openid.identity=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select";
            string strRet = m_webPost.GetHttpPage(strUrl,
                "gb2312", "get", "", "",
                "",
                "*/*", "");
            //webPost.GetHttpPage()
            //appActionToken={0}&appAction=SIGNIN&openid.pape.max_auth_age=ape:OTAw&openid.ns=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjA%3D&openid.ns.pape=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvZXh0ZW5zaW9ucy9wYXBlLzEuMA%3D%3D&pageId=ape:Y25mbGV4&openid.identity=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjAvaWRlbnRpZmllcl9zZWxlY3Q%3D&openid.claimed_id=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjAvaWRlbnRpZmllcl9zZWxlY3Q%3D&openid.mode=ape:Y2hlY2tpZF9zZXR1cA%3D%3D&openid.assoc_handle=ape:Y25mbGV4&openid.return_to=ape:aHR0cHM6Ly93d3cuYW1hem9uLmNuL2dwL2Nzcy9oaXN0b3J5L29yZGVycy92aWV3Lmh0bWw%2FaWU9VVRGOCZyZWZfPXlhX182Yg%3D%3D&email=1113930168@qq.com991&create=0&password=123456&metadata1={1}&x=27&y=10
            strRet = strRet.Substring(strRet.IndexOf("id=\"ap_signin_form\""));
            strRet = strRet.Substring(strRet.IndexOf("<input"));
            strToken = GetKeyString(strRet, "value=\"", "\"");

        }

        private void button2_Click(object sender, EventArgs e)
        {
            string strUser = textBox2.Text;
            string strPwd = textBox3.Text;
            string strUrl = "https://www.amazon.cn/ap/signin";
            string strMetadata = textBox1.Text;
            string strData = string.Format("appActionToken={0}&appAction=SIGNIN&openid.pape.max_auth_age=ape:OTAw&openid.ns=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjA%3D&openid.ns.pape=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvZXh0ZW5zaW9ucy9wYXBlLzEuMA%3D%3D&pageId=ape:Y25mbGV4&openid.identity=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjAvaWRlbnRpZmllcl9zZWxlY3Q%3D&openid.claimed_id=ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjAvaWRlbnRpZmllcl9zZWxlY3Q%3D&openid.mode=ape:Y2hlY2tpZF9zZXR1cA%3D%3D&openid.assoc_handle=ape:Y25mbGV4&openid.return_to=ape:aHR0cHM6Ly93d3cuYW1hem9uLmNuL2dwL2Nzcy9oaXN0b3J5L29yZGVycy92aWV3Lmh0bWw%2FaWU9VVRGOCZyZWZfPXlhX182Yg%3D%3D&email={1}&create=0&password={2}&metadata1={3}&x=27&y=10", 
                strToken, 
                strUser,
                strPwd,
                strMetadata);
            string strRet = m_webPost.GetHttpPage(strUrl,
                "utf-8", "post", "", strData,
                "https://www.amazon.cn/ap/signin",
                "*/*", "");
           
        }
    }
}