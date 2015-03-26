using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using FunctionCode;
using AutoRobot;
using System.Text.RegularExpressions;
using mshtml;
using System.Threading;
namespace QQRobotSharp
{
    public partial class Form1 : Form
    {
        public string m_strSptNo = "";
        string m_strCusName = "";
        bool m_iExit = true;
        public delegate void OnCalQuestionEvent();
        public Thread triggerThread;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_webBrowser.Navigate("http://b.qq.com");
            textBox1.Text = "1002";
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            
        }

        private void button_Start_Click(object sender, EventArgs e)
        {
            m_strSptNo = textBox1.Text;
            m_strCusName = textBox2.Text;
            m_iExit = false;
            ThreadStart myThreadStart = new ThreadStart(FreshProc);
            triggerThread = new Thread(myThreadStart);
            triggerThread.Start();
        }
        public void FreshProc()
        {
            while (true)
            {
                if (m_iExit)
                {
                    break;
                }
                try
                {
                    if (m_webBrowser.InvokeRequired)
                    {
                        OnCalQuestionEvent _myInvoke = new OnCalQuestionEvent(Fresh);
                        this.Invoke(_myInvoke, new object[] { });
                    }
                    else
                    {
                        Fresh();
                    }
                }
                catch (Exception e)
                {
                    string strError = e.Message;
                }
                int iCount = 0;
                while(!m_iExit && iCount++ < 10)
                {
                    Thread.Sleep(600);
                }
            }            
        }

        public void Fresh()
        {
            //IHTMLDocument2 doc2 = (IHTMLDocument2)m_webBrowser.Document.DomDocument;
            //doc2.parentWindow.execScript("alert('fuck you');", "javascript");

            m_strSptNo = textBox1.Text;
            IHTMLElement iElement = (IHTMLElement)m_webBrowser.Document.GetElementById("acl_user").DomElement;
            string strText = iElement.innerHTML;
            Match match = Regex.Match(strText, "id=acl_user_(.+?)class=");
            string strIds = "";
            string matchValue = "";
            while (match.Success)
            {
                matchValue = match.Groups[1].Value;		//获取匹配到的字符串
                strIds += matchValue.Trim() + ",";
                match = match.NextMatch();
            }
            string[] strIdArray = strIds.Split(',');
            string strContent = "";
            foreach (string strId in strIdArray)
            {
                if (strId == "")
                {
                    continue;
                }
                iElement = (IHTMLElement)m_webBrowser.Document.GetElementById(string.Format("mv_{0}", strId)).DomElement;
                if (iElement == null)
                {
                    continue;
                }
                strContent = iElement.innerHTML;
                //解析内容,获取到客户最后发送过来的内容集合[可能分开发送多条]
                //如果最后是客服自动回复的内容,则直接关闭
                //class=hmCap>1002
                string strFlag = "";
                strContent = strContent.ToLower();
                if (strContent.IndexOf("class=msgcap") > 0)
                {
                    strFlag = "class=msgcap";
                }
                else
                {
                    strFlag = "class=hmcap";
                }

                int iPos = strContent.LastIndexOf(strFlag);
                if (iPos < 0)
                {
                    continue;
                }
                //1002&nbsp;20
                string strTemp = strContent.Substring(iPos);
                strTemp = strTemp.Substring(0, strTemp.IndexOf("</div>"));
                if (strTemp.IndexOf(m_strSptNo) > 0)    //最后是客服回复的消息，客户未回复
                {
                    iElement = (IHTMLElement)m_webBrowser.Document.GetElementById(string.Format("btn_closeChat_{0}", strId)).DomElement;
                    if (iElement == null)
                    {
                        continue;
                    }
                    iElement.click();
                    continue;
                }
                //如果是客户的问题,则回复,发送,不关闭
                //提取所有的信息内容
                iPos = strContent.LastIndexOf(m_strSptNo + "nbsp;");
                if (iPos > 0)
                {
                    strContent = strContent.Substring(iPos);
                }
                iPos = strContent.IndexOf("class=hmcap");
                if (iPos > 0)
                {
                    strContent = strContent.Substring(iPos);
                }
                else
                {
                    iPos = strContent.IndexOf("class=msgcap");
                    if (iPos > 0)
                    {
                        strContent = strContent.Substring(iPos);
                    }
                    else
                    {
                        strContent = "";
                    }
                }

                string strQuestion = "";    //取最后一个问题
                Match matchEx = Regex.Match(strContent, "<div(.+?)</div>");
                string matchValueEx = "";
                while (matchEx.Success)
                {
                    matchValueEx = matchEx.Groups[1].Value;		//获取匹配到的字符串
                    if (matchValueEx.IndexOf("nbsp;") < 0)
                    {
                        strQuestion = matchValueEx.Substring(matchValueEx.IndexOf(">") + 1);
                    }
                    matchEx = matchEx.NextMatch();
                }
                iPos = strQuestion.IndexOf("(本消息由您的好友");
                if (iPos > 0)
                {
                    strQuestion = strQuestion.Substring(0, iPos);
                }
                string strAnswer = AutoRobotClass.MatchResult(strQuestion);
                HTMLTextAreaElementClass inputElement = (HTMLTextAreaElementClass)m_webBrowser.Document.GetElementById(string.Format("chat_area_{0}", strId)).DomElement;
                inputElement.value = strAnswer;
                if (iElement == null)
                {
                    continue;
                }
                //continue;
                iElement = (IHTMLElement)m_webBrowser.Document.GetElementById(string.Format("btn_sendMsg_{0}", strId)).DomElement;
                if (iElement == null)
                {
                    continue;
                }
                if (m_strCusName == "" || strContent.IndexOf(m_strCusName + "&nbsp;") != -1)
                {
                    iElement.click();
                    System.Threading.Thread.Sleep(100);
                }
                //关闭这个
            }
            //mv_30097799   //从里面解析得到客户发来的内容
            //chat_area_30097799 //从这里写入要发送的内容
            //btn_sendMsg_30097799 //发送
            //btn_closeChat_30097799 //关闭

            //得到ID号为acl_user的TABLE
            //得到其中的内容,解析出所有已接入会话的ID号
            //根据ID号去读取发送过来的内容,进行回复
        }

        private void button_Stop_Click(object sender, EventArgs e)
        {
            m_iExit = true;            
            if(triggerThread != null)
            {
                Thread.Sleep(500);
                triggerThread.Abort();
            }
        }
    }
}