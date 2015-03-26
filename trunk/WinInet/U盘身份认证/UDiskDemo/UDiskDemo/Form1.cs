using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace UDiskDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            label1.Text = "";
            string strDisks = CUDiskClass.GetDisks();
            if (strDisks == "")
            {
                label1.Text = "未找到U盘";
                return;
            }
            string[] strArray = strDisks.Split(',');

            foreach (string strDisk in strArray)
            {
                strDisk.Trim();
                if (strDisk == "")
                {
                    return;
                }
                listBox1.Items.Add(strDisk);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            label1.Text = "";
            int iIndex = listBox1.SelectedIndex;
            if (iIndex < 0)
            {
               label1.Text = "请选择U盘";
                return;
            }
            string strDisk = listBox1.Items[iIndex].ToString();
            string strSerial = "";
            bool bRet = CUDiskClass.GetDiskSerials(strDisk.Substring(0,1), ref strSerial);
            label1.Text = strDisk + " 的序列号为: " + strSerial;
        }
    }
}