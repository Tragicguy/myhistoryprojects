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
using SmsServer.BLL;
using System.Text.RegularExpressions;
using System.Text;
using System.IO;

namespace SmsServer.Web
{
    public partial class WebForm2 : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                TextBox1.Text = "http://webs.montnets.com/gbyds/wmgw.asmx/MongateCsSendSmsEx?";
                TextBox2.Text = "userId=13600000412&password=456123&pszMobis=15019212360&pszMsg={0}&iMobiCount=1";
                TextBox3.Text = "3000";
                TextBox4.Text = "1";
                TextBox5.Text = "你好啊，测试信息一条";
            }
            
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            string strRet = "";
            try
            {
                string strUrl = TextBox1.Text;
                string strParams = TextBox2.Text;
                int iType = Int32.Parse(TextBox4.Text);
                Encoding myEncoding = iType == 1 ? Encoding.GetEncoding("gb2312") : Encoding.UTF8;
                string strContent = TextBox5.Text;
                strParams = string.Format(strParams, HttpUtility.UrlEncode(strContent, myEncoding));
                strRet = SmsHttpSend.HttpSendMessge(ref strUrl, ref strParams, Int32.Parse(TextBox3.Text), Int32.Parse(TextBox4.Text),1);
            }
            catch (System.Exception eee)
            {
                strRet = eee.Message;
            }
            Response.Write(strRet);
        }

        protected void Button2_Click(object sender, EventArgs e)
        {
            string strRet = "";
            try
            {
                string strUrl = TextBox1.Text;
                string strParams = TextBox2.Text;
                int iType = Int32.Parse(TextBox4.Text);
                Encoding myEncoding = iType == 1 ? Encoding.GetEncoding("gb2312") : Encoding.UTF8;
                string strContent = TextBox5.Text;
                strParams = string.Format(strParams, HttpUtility.UrlEncode(strContent, myEncoding));
                strRet = SmsHttpSend.HttpSendMessge(ref strUrl, ref strParams, Int32.Parse(TextBox3.Text), iType, 0);
            }
            catch (System.Exception eee)
            {
                strRet = eee.Message;
            }
            Response.Write(strRet);
        }

        protected void Button3_Click(object sender, EventArgs e)
        {
            string strRet = "";
            try
            {
                int iType = Int32.Parse(TextBox4.Text);
                Encoding myEncoding = iType==1?Encoding.GetEncoding("gb2312"):Encoding.UTF8;

               // HttpUtility.UrlEncode()
                string strUrl = TextBox1.Text;
                string strParams = TextBox2.Text;
                string strContent = TextBox5.Text;//SmsHttpSend.ConvertString(TextBox5.Text, iType); ;
                strParams = string.Format(strParams, HttpUtility.UrlEncode(strContent,myEncoding));
                strRet = strParams;
                //strRet = 
                //strRet = Server.UrlEncode(strRet);
            }
            catch (System.Exception eee)
            {
                strRet = eee.Message;
            }
            Response.Write(strRet);
        }

        protected void Button4_Click(object sender, EventArgs e)
        {
            string retStr = "<<?xml version=\"1.0\" encoding=\"utf-8\"?><string xmlns=\"http://tempuri.org/\">0511204742050414</string></></></><111/>";
            Regex r = new Regex(@"<[^>]*>");
            if (r.IsMatch(retStr))
            {
                retStr = r.Replace(retStr, "");
            }
            Response.Write(retStr);
        }

        protected void Button5_Click(object sender, EventArgs e)
        {
            HtmlTable tb = new HtmlTable();
            HtmlTableRow tr = new HtmlTableRow();
            HtmlTableCell td = new HtmlTableCell();
            tb.Width = "100%";
            Label lable1 = new Label();
            lable1.Text = "asdfesfsadfse";
            lable1.BackColor = System.Drawing.Color.Red;
            td.Controls.Add(lable1);
            tr.Controls.Add(td);
            tb.Controls.Add(tr);
            Page testPage = new Page();
            StringWriter output = new StringWriter();
            testPage.Controls.Add(tb);
            Server.Execute(testPage, output, false);
            string strRet = output.ToString();
            Response.Write(strRet);
        }
    }
}
