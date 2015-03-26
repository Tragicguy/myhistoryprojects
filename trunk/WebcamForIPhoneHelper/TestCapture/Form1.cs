using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestCapture
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        //此路径一定要与WebCamera的默认保存路径一致，且最好是独立的文件夹
        private void button1_Click(object sender, EventArgs e)
        {
            int iRet = WebcamWrapper.WebcamWrapper.DoCapturePictrue(textBox1.Text, textBox2.Text);
            if (iRet != 1)
            {
                MessageBox.Show("失败");
            }
            else {
                System.Diagnostics.Process process = new System.Diagnostics.Process();
                //设置文件名，此处为图片的真实路径+文件名    
                process.StartInfo.FileName = textBox2.Text;
                //此为关键部分。设置进程运行参数，此时为最大化窗口显示图片。    
                process.StartInfo.Arguments = "rundll32.exe C://WINDOWS//system32//shimgvw.dll,ImageView_Fullscreen";
                //此项为是否使用Shell执行程序，因系统默认为true，此项也可不设，但若设置必须为true    
                process.StartInfo.UseShellExecute = true;
                //此处可以更改进程所打开窗体的显示样式，可以不设    
                process.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                process.Start();
                process.Close(); 
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = @"C:\Users\Public\webcamera";
            textBox2.Text = @"E:\Test.jpg";
        }
    }
}
