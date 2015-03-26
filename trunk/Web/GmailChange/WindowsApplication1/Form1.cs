using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Serialization;
using GmailClass;

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
            string strUser = textBox1.Text;
            string strPwd = textBox5.Text;
            string strRet = PayPalQuery.QueryBalance(strUser, strPwd);
            MessageBox.Show(strRet);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            

        }

        private void button3_Click(object sender, EventArgs e)
        {
            string strEmail = textBox2.Text;
            string strOldPwd = textBox3.Text;
            string strNewPwd = textBox4.Text;
            bool bRet = GmailControl.ChangePassWord(strEmail, strOldPwd, strNewPwd);
            MessageBox.Show(bRet == true?"成功":"失败");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string strUser = textBox1.Text;
            string strPwd = textBox5.Text;
            string strRecver = textBox6.Text;
            string strMoney = textBox7.Text;
            float fMoney = 0;
            float.TryParse(strMoney, out fMoney);
            string strError = "";
            bool bRet = PayPalQuery.PayMoney(strUser, strPwd, strRecver, fMoney, out strError);
            MessageBox.Show(strError);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = "554352808@qq.com";
            textBox5.Text = "123qweasd";
            textBox6.Text = "shaojun.hnist@gmail.com";
            textBox7.Text = "0.15";
        }
    }
}