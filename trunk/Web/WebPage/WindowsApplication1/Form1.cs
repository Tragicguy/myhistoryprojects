using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Serialization;
using ClassLibrary;
namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //一进来就直接去充值

            string strCheckCode = textBox1.Text.Trim();
            Login("juntest2", "wawawo20", strCheckCode);
            bool bRet = Order(true, 1, 93.00, 9);
            if (bRet)
            {
                return;
            }
            Login("juntest2", "wawawo20", strCheckCode);
        }

        public bool IsLogin()
        {
            string strRet = ClassHttpPost.GetHttpPage("http://www.ofcard.com/include/top1/login_iframe.jsp", "gb2312", "get", "", "", "", "*/*", "");
            if (strRet.IndexOf("")>0)
            {
                return true;
            }
            return false;
        }

        public bool Order(bool bIsVip, int iNum, double iTotalPrice, int iMemintegral)
        {
            string strObjData = "";
            if (bIsVip)
            {
                strObjData = string.Format("userid=&Email2=&cardid=210207&hidcardid=2102&paytype=null&quantity={3}&memprice{4}&memintegral={5}&sms_text=&select_paygate=radiobutton&sh_mob={0}&paygate={1}&paygate1=&bankname={2}",
                                                "%C7%EB%CA%E4%D2%C6%B6%AF%CA%D6%BB%FA%BA%C5",
                                                "%D6%A7%B8%B6%B1%A6",
                                                "%D6%A7%B8%B6%B1%A6",
                                                iNum,
                                                iTotalPrice,
                                                iMemintegral
                                                );
            }
            else
            {
                //非会员模式订单
            }

            string strRet = ClassHttpPost.GetHttpPage("http://www.ofcard.com/order.do", "gb2312", "post", "", strObjData, "http://www.ofcard.com/card/210207.htm", "*/*", "");
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
            strObjData = string.Format("paygate={0}&OrderNo={1}&paygate1=&amount={2}&cardid=210207",
                "%D6%A7%B8%B6%B1%A6",
                strCardNo,
                strAmount
                );

            strRet = ClassHttpPost.GetHttpPage("http://www.ofcard.com/updatebankstate.do", "gb2312", "post", "", strObjData, "http://www.ofcard.com/order.do", "*/*", "");
            if (strRet.IndexOf("正在转向支付页面") < 0)
            {
                return false;
            }
            
            //	postData="paygate=&OrderNo=" & cardNo & "&paygate1=&amount=" & cardAmount & "&cardid=210207"
           
            strBefore = "<form name=\"qqbank\" action=\"";
            strAfter = "\"";
            string strUrl = GetKeyString(strRet, strBefore, strAfter);
            return true;
        }

        public bool Login(string strUser,string strPwd,string strCheckCode)
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
        public bool GetCheckCode()
        {
            try
            {
                Stream stream = ClassHttpPost.GetHttpPageSteam("http://www.ofcard.com/image.jsp?", "gb2312", "get", "", "", "", "*/*", "");

                List<byte> list = new List<byte>();
                while (true)
                {
                    int data = stream.ReadByte();
                    if (data == -1)
                        break;
                    list.Add((byte)data);
                }
                File.WriteAllBytes("c:\\2.bmp", list.ToArray());
                return true;
            }
            catch (System.Exception ex)
            {
            	
            }
            return false;
        }


        public string GetKeyString(string strSrc,string strBefore,string strAfter)
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
        private void button2_Click(object sender, EventArgs e)
        {
            Stream stream = ClassHttpPost.GetHttpPageSteam("http://www.ofcard.com/image.jsp?", "gb2312", "get", "", "", "", "*/*", "");

            List<byte> list = new List<byte>();
            while (true)
            {
                int data = stream.ReadByte();
                if (data == -1)
                    break;
                list.Add((byte)data);
            }
            File.WriteAllBytes("c:\\2.bmp", list.ToArray());

        }
    }
}