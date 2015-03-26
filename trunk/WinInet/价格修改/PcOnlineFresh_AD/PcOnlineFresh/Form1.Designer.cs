namespace PcOnlineFresh
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnImportAcc = new System.Windows.Forms.Button();
            this.radioFreshEx = new System.Windows.Forms.RadioButton();
            this.textRreshInterval = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.radioFresh = new System.Windows.Forms.RadioButton();
            this.btnImportGoodFile = new System.Windows.Forms.Button();
            this.SynchroGoodInfo = new System.Windows.Forms.Button();
            this.btnOutputGoodFile = new System.Windows.Forms.Button();
            this.btnStartFresh = new System.Windows.Forms.Button();
            this.btnStopFresh = new System.Windows.Forms.Button();
            this.btnModifyPrice = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textFreshStart = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textFreshEnd = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textFreshTime = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.listView = new System.Windows.Forms.ListView();
            this.columnAccIndex = new System.Windows.Forms.ColumnHeader();
            this.columnAcc = new System.Windows.Forms.ColumnHeader();
            this.listBoxInfo = new System.Windows.Forms.ListBox();
            this.checkShowLog = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textThreadNum = new System.Windows.Forms.TextBox();
            this.checkFresh = new System.Windows.Forms.CheckBox();
            this.button2 = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.textAdString = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnImportAcc
            // 
            this.btnImportAcc.Location = new System.Drawing.Point(335, 41);
            this.btnImportAcc.Name = "btnImportAcc";
            this.btnImportAcc.Size = new System.Drawing.Size(71, 23);
            this.btnImportAcc.TabIndex = 0;
            this.btnImportAcc.Text = "导入店址";
            this.btnImportAcc.UseVisualStyleBackColor = true;
            this.btnImportAcc.Click += new System.EventHandler(this.btnImportAcc_Click);
            // 
            // radioFreshEx
            // 
            this.radioFreshEx.AutoSize = true;
            this.radioFreshEx.Location = new System.Drawing.Point(93, 210);
            this.radioFreshEx.Name = "radioFreshEx";
            this.radioFreshEx.Size = new System.Drawing.Size(83, 16);
            this.radioFreshEx.TabIndex = 1;
            this.radioFreshEx.TabStop = true;
            this.radioFreshEx.Text = "自定义刷新";
            this.radioFreshEx.UseVisualStyleBackColor = true;
            this.radioFreshEx.Visible = false;
            // 
            // textRreshInterval
            // 
            this.textRreshInterval.Location = new System.Drawing.Point(422, 205);
            this.textRreshInterval.Name = "textRreshInterval";
            this.textRreshInterval.Size = new System.Drawing.Size(43, 21);
            this.textRreshInterval.TabIndex = 2;
            this.textRreshInterval.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textRreshInterval.Visible = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(324, 212);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "刷新间隔[小时]:";
            this.label1.Visible = false;
            // 
            // radioFresh
            // 
            this.radioFresh.AutoSize = true;
            this.radioFresh.Location = new System.Drawing.Point(19, 210);
            this.radioFresh.Name = "radioFresh";
            this.radioFresh.Size = new System.Drawing.Size(71, 16);
            this.radioFresh.TabIndex = 4;
            this.radioFresh.TabStop = true;
            this.radioFresh.Text = "一键刷新";
            this.radioFresh.UseVisualStyleBackColor = true;
            this.radioFresh.Visible = false;
            // 
            // btnImportGoodFile
            // 
            this.btnImportGoodFile.Location = new System.Drawing.Point(419, 41);
            this.btnImportGoodFile.Name = "btnImportGoodFile";
            this.btnImportGoodFile.Size = new System.Drawing.Size(85, 23);
            this.btnImportGoodFile.TabIndex = 5;
            this.btnImportGoodFile.Text = "导入广告语";
            this.btnImportGoodFile.UseVisualStyleBackColor = true;
            this.btnImportGoodFile.Click += new System.EventHandler(this.btnImportGoodFile_Click);
            // 
            // SynchroGoodInfo
            // 
            this.SynchroGoodInfo.Location = new System.Drawing.Point(518, 41);
            this.SynchroGoodInfo.Name = "SynchroGoodInfo";
            this.SynchroGoodInfo.Size = new System.Drawing.Size(86, 23);
            this.SynchroGoodInfo.TabIndex = 6;
            this.SynchroGoodInfo.Text = "同步商家信息";
            this.SynchroGoodInfo.UseVisualStyleBackColor = true;
            this.SynchroGoodInfo.Click += new System.EventHandler(this.SynchroGoodInfo_Click);
            // 
            // btnOutputGoodFile
            // 
            this.btnOutputGoodFile.Location = new System.Drawing.Point(527, 299);
            this.btnOutputGoodFile.Name = "btnOutputGoodFile";
            this.btnOutputGoodFile.Size = new System.Drawing.Size(101, 23);
            this.btnOutputGoodFile.TabIndex = 7;
            this.btnOutputGoodFile.Text = "导出商品信息";
            this.btnOutputGoodFile.UseVisualStyleBackColor = true;
            this.btnOutputGoodFile.Visible = false;
            // 
            // btnStartFresh
            // 
            this.btnStartFresh.Location = new System.Drawing.Point(115, 41);
            this.btnStartFresh.Name = "btnStartFresh";
            this.btnStartFresh.Size = new System.Drawing.Size(65, 23);
            this.btnStartFresh.TabIndex = 8;
            this.btnStartFresh.Text = "启动线程";
            this.btnStartFresh.UseVisualStyleBackColor = true;
            this.btnStartFresh.Click += new System.EventHandler(this.btnStartFresh_Click);
            // 
            // btnStopFresh
            // 
            this.btnStopFresh.Location = new System.Drawing.Point(195, 41);
            this.btnStopFresh.Name = "btnStopFresh";
            this.btnStopFresh.Size = new System.Drawing.Size(41, 23);
            this.btnStopFresh.TabIndex = 9;
            this.btnStopFresh.Text = "停止";
            this.btnStopFresh.UseVisualStyleBackColor = true;
            this.btnStopFresh.Click += new System.EventHandler(this.btnStopFresh_Click);
            // 
            // btnModifyPrice
            // 
            this.btnModifyPrice.Location = new System.Drawing.Point(251, 41);
            this.btnModifyPrice.Name = "btnModifyPrice";
            this.btnModifyPrice.Size = new System.Drawing.Size(69, 23);
            this.btnModifyPrice.TabIndex = 10;
            this.btnModifyPrice.Text = "发送广告";
            this.btnModifyPrice.UseVisualStyleBackColor = true;
            this.btnModifyPrice.Click += new System.EventHandler(this.btnModifyPrice_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(479, 212);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "刷新时间段:";
            this.label2.Visible = false;
            // 
            // textFreshStart
            // 
            this.textFreshStart.Location = new System.Drawing.Point(556, 205);
            this.textFreshStart.Name = "textFreshStart";
            this.textFreshStart.Size = new System.Drawing.Size(64, 21);
            this.textFreshStart.TabIndex = 11;
            this.textFreshStart.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(626, 210);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 12);
            this.label3.TabIndex = 13;
            this.label3.Text = "至:";
            this.label3.Visible = false;
            // 
            // textFreshEnd
            // 
            this.textFreshEnd.Location = new System.Drawing.Point(650, 205);
            this.textFreshEnd.Name = "textFreshEnd";
            this.textFreshEnd.Size = new System.Drawing.Size(64, 21);
            this.textFreshEnd.TabIndex = 14;
            this.textFreshEnd.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(185, 213);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(83, 12);
            this.label4.TabIndex = 16;
            this.label4.Text = "单日刷新次数:";
            this.label4.Visible = false;
            // 
            // textFreshTime
            // 
            this.textFreshTime.Location = new System.Drawing.Point(273, 206);
            this.textFreshTime.Name = "textFreshTime";
            this.textFreshTime.Size = new System.Drawing.Size(43, 21);
            this.textFreshTime.TabIndex = 15;
            this.textFreshTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textFreshTime.Visible = false;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(422, 271);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 17;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // listView
            // 
            this.listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnAccIndex,
            this.columnAcc});
            this.listView.Location = new System.Drawing.Point(8, 81);
            this.listView.Name = "listView";
            this.listView.Size = new System.Drawing.Size(121, 97);
            this.listView.TabIndex = 18;
            this.listView.UseCompatibleStateImageBehavior = false;
            this.listView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView_MouseDoubleClick);
            this.listView.SelectedIndexChanged += new System.EventHandler(this.listView_SelectedIndexChanged);
            // 
            // columnAccIndex
            // 
            this.columnAccIndex.Text = "序号";
            // 
            // columnAcc
            // 
            this.columnAcc.Text = "帐号名";
            // 
            // listBoxInfo
            // 
            this.listBoxInfo.FormattingEnabled = true;
            this.listBoxInfo.ItemHeight = 12;
            this.listBoxInfo.Location = new System.Drawing.Point(8, 262);
            this.listBoxInfo.Name = "listBoxInfo";
            this.listBoxInfo.Size = new System.Drawing.Size(120, 88);
            this.listBoxInfo.TabIndex = 19;
            // 
            // checkShowLog
            // 
            this.checkShowLog.AutoSize = true;
            this.checkShowLog.Location = new System.Drawing.Point(398, 249);
            this.checkShowLog.Name = "checkShowLog";
            this.checkShowLog.Size = new System.Drawing.Size(72, 16);
            this.checkShowLog.TabIndex = 20;
            this.checkShowLog.Text = "显示日志";
            this.checkShowLog.UseVisualStyleBackColor = true;
            this.checkShowLog.Visible = false;
            this.checkShowLog.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 49);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(47, 12);
            this.label5.TabIndex = 22;
            this.label5.Text = "线程数:";
            // 
            // textThreadNum
            // 
            this.textThreadNum.Location = new System.Drawing.Point(55, 41);
            this.textThreadNum.Name = "textThreadNum";
            this.textThreadNum.Size = new System.Drawing.Size(43, 21);
            this.textThreadNum.TabIndex = 21;
            this.textThreadNum.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // checkFresh
            // 
            this.checkFresh.AutoSize = true;
            this.checkFresh.Location = new System.Drawing.Point(178, 249);
            this.checkFresh.Name = "checkFresh";
            this.checkFresh.Size = new System.Drawing.Size(72, 16);
            this.checkFresh.TabIndex = 23;
            this.checkFresh.Text = "暂停刷新";
            this.checkFresh.UseVisualStyleBackColor = true;
            this.checkFresh.Visible = false;
            this.checkFresh.CheckedChanged += new System.EventHandler(this.checkFresh_CheckedChanged);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(765, 213);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 24;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Visible = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(46, 21);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(47, 12);
            this.label6.TabIndex = 26;
            this.label6.Text = "广告语:";
            // 
            // textAdString
            // 
            this.textAdString.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.textAdString.Location = new System.Drawing.Point(105, 2);
            this.textAdString.Multiline = true;
            this.textAdString.Name = "textAdString";
            this.textAdString.ReadOnly = true;
            this.textAdString.Size = new System.Drawing.Size(723, 37);
            this.textAdString.TabIndex = 25;
            this.textAdString.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(523, 270);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(105, 23);
            this.button3.TabIndex = 27;
            this.button3.Text = "初始化发送状态";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Visible = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(840, 569);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.textAdString);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.checkFresh);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textThreadNum);
            this.Controls.Add(this.checkShowLog);
            this.Controls.Add(this.listBoxInfo);
            this.Controls.Add(this.listView);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textFreshTime);
            this.Controls.Add(this.textFreshEnd);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textFreshStart);
            this.Controls.Add(this.btnModifyPrice);
            this.Controls.Add(this.btnStopFresh);
            this.Controls.Add(this.btnStartFresh);
            this.Controls.Add(this.btnOutputGoodFile);
            this.Controls.Add(this.SynchroGoodInfo);
            this.Controls.Add(this.btnImportGoodFile);
            this.Controls.Add(this.radioFresh);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textRreshInterval);
            this.Controls.Add(this.radioFreshEx);
            this.Controls.Add(this.btnImportAcc);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "太平洋商品刷新器";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnImportAcc;
        private System.Windows.Forms.RadioButton radioFreshEx;
        private System.Windows.Forms.TextBox textRreshInterval;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton radioFresh;
        private System.Windows.Forms.Button btnImportGoodFile;
        private System.Windows.Forms.Button SynchroGoodInfo;
        private System.Windows.Forms.Button btnOutputGoodFile;
        private System.Windows.Forms.Button btnStartFresh;
        private System.Windows.Forms.Button btnStopFresh;
        private System.Windows.Forms.Button btnModifyPrice;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textFreshStart;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textFreshEnd;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textFreshTime;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListView listView;
        private System.Windows.Forms.ColumnHeader columnAccIndex;
        private System.Windows.Forms.ColumnHeader columnAcc;
        private System.Windows.Forms.ListBox listBoxInfo;
        private System.Windows.Forms.CheckBox checkShowLog;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textThreadNum;
        private System.Windows.Forms.CheckBox checkFresh;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textAdString;
        private System.Windows.Forms.Button button3;
    }
}

