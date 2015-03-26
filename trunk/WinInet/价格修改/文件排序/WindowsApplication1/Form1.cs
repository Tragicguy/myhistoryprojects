using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private string ReadFileLine(string strFilePath)
        {
            FileStream   fs=new   FileStream(strFilePath,FileMode.Open,FileAccess.Read,FileShare.None);
            StreamReader sr = new StreamReader(fs, System.Text.Encoding.UTF8);            
            sr.BaseStream.Seek(0,SeekOrigin.Begin); 
            StringBuilder builder = new StringBuilder();
            string     strLine     =sr.ReadLine();
            while (strLine != null) 
            { 
                builder.Append(strLine+"\n");
                strLine=sr.ReadLine(); 
            }
            sr.Close();
            fs.Close();
            return builder.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StringBuilder strBuilderEx = new StringBuilder();
            OpenFileDialog find = new OpenFileDialog();
            find.Filter = "TXT文件(*.txt)|*.txt";

            if (find.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            string strPath = find.FileName;
            string strContent = ReadFileLine(strPath);
            string[] strArray = strContent.Split('\n');

            Array.Sort(strArray);
            foreach (string strLine in strArray)
            {
                if (strLine != "")
                {
                    strBuilderEx.Append(strLine + "\r\n");
                }                
            }
            WriteInfo(strPath, strBuilderEx);
            MessageBox.Show("排序已完成，请查看文件");
        }

        public void WriteInfo(string strPath, StringBuilder strInfoBuilder)
        {
            try
            {
                if (File.Exists(strPath))
                {
                    File.Delete(strPath);
                }
                StreamWriter fs = File.AppendText(strPath);
                fs.Write(strInfoBuilder.ToString());
                fs.Close();
            }
            catch
            {

            }
        }
    }
}