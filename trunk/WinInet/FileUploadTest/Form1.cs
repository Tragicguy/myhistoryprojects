using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using FileUploadClass;
namespace FileUploadTest
{
    public partial class Form1 : Form, IBack
    {
        public delegate void ShowEvent(int iCurr);
        FileUpload m_FileUpload;
        public Form1()
        {
            InitializeComponent();
            textBox1.Text = "http://localhost:2323/Upload.aspx";
        }
       
        public void ShowProcess(int iCurr)
        {
            progressBar1.Value = iCurr;
        }

        public void  NoticeProcess(int iCurr,string strUrl)
        {
            try
            {
                if (progressBar1.InvokeRequired)
                {
                    ShowEvent _myInvoke = new ShowEvent(ShowProcess);
                    this.Invoke(_myInvoke, new object[] { iCurr });
                }
                else
                {
                    ShowProcess(iCurr);
                }
                if (iCurr == 100)
                {
                    MessageBox.Show(strUrl);
                }
                else if (iCurr == -1)
                {
                    MessageBox.Show("读取本地文件失败");
                }
                else if (iCurr == -2)
                {
                    MessageBox.Show("上传失败");
                }
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            string strUrl = textBox1.Text;
            string strPath = "";
            OpenFileDialog find = new OpenFileDialog();
            find.Filter = "所有文件(*.*)|*.*";

            if (find.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            strPath = find.FileName;
            m_FileUpload = new FileUpload(this);
            m_FileUpload.SetUrl(strUrl);
            m_FileUpload.Start(strPath);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_FileUpload != null)
            {
                m_FileUpload.Stop();
            }
        }
    }
}