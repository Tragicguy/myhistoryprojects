using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Top.Api.Domain;
using System.Threading;
namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        public delegate void ShowEventLog(string strInfo);
        public Thread triggerThread;
        long m_lOrderID = 0;
        public Form1()
        {
            InitializeComponent();
            textBox1.Text = "75675828156188";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            while (listBox1.Items.Count > 0)
            {
                listBox1.Items.RemoveAt(0);
            }
            m_lOrderID = long.Parse(textBox1.Text.Trim());
            ThreadStart myThreadStart = new ThreadStart(FunProc);
            triggerThread = new Thread(myThreadStart);
            triggerThread.Start();
            ShowStringEx("开始查询");
        }
        public void FunProc()
        {
            Trade objTrade = TaoApiClass.GetOrderInfo("http://gw.api.taobao.com/router/rest", "12141919", "7c2fa1cde41102ac21a540a07255fd9a", m_lOrderID, "");
            if (objTrade == null)
            {
                MessageBox.Show("调用失败");
                return;
            }
            //显示数据
            ShowStringEx("SellerNick: " + objTrade.SellerNick);
            ShowStringEx("BuyerNick: " + objTrade.BuyerNick);
            ShowStringEx("订单总数: " + objTrade.Orders.Count.ToString());
            ShowStringEx("/*************************************************************/");
            foreach (Order objOrder in objTrade.Orders)
            {
                ShowStringEx("Title: " + objOrder.Title);
                ShowStringEx("Price: " + objOrder.Price);                
                ShowStringEx("/*************************************************************/");
            }

            ShowStringEx("查询结束");
        }

        private void ShowString(string strInfo)
        {
            //添加至日志显示列表
            listBox1.Items.Add(strInfo);
        }

        public void ShowStringEx(string strInfo)
        {
            try
            {
                if (listBox1.InvokeRequired)
                {
                    ShowEventLog _myInvoke = new ShowEventLog(ShowString);
                    this.Invoke(_myInvoke, new object[] { strInfo });
                }
                else
                {
                    ShowString(strInfo);
                }
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }
        
    }
}