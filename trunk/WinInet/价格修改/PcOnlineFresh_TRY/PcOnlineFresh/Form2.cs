using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PcOnlineFresh
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }
        public string m_strCode = "";
        public string m_strPath = "";
        private void button1_Click(object sender, EventArgs e)
        {
            m_strCode = textBox1.Text;
            this.Close();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            try
            {
                pictureBox1.Load(m_strPath);
            }
            catch (System.Exception ex)
            {
            	
            }            
        }
    }
}