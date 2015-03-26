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
            this.SuspendLayout();
            // 
            // btnImportAcc
            // 
            this.btnImportAcc.Location = new System.Drawing.Point(457, 41);
            this.btnImportAcc.Name = "btnImportAcc";
            this.btnImportAcc.Size = new System.Drawing.Size(71, 23);
            this.btnImportAcc.TabIndex = 0;
            this.btnImportAcc.Text = "导入账号";
            this.btnImportAcc.UseVisualStyleBackColor = true;
            this.btnImportAcc.Click += new System.EventHandler(this.btnImportAcc_Click);
            // 
            // radioFreshEx
            // 
            this.radioFreshEx.AutoSize = true;
            this.radioFreshEx.Location = new System.Drawing.Point(86, 19);
            this.radioFreshEx.Name = "radioFreshEx";
            this.radioFreshEx.Size = new System.Drawing.Size(83, 16);
            this.radioFreshEx.TabIndex = 1;
            this.radioFreshEx.TabStop = true;
            this.radioFreshEx.Text = "自定义刷新";
            this.radioFreshEx.UseVisualStyleBackColor = true;
            // 
            // textRreshInterval
            // 
            this.textRreshInterval.Location = new System.Drawing.Point(415, 14);
            this.textRreshInterval.Name = "textRreshInterval";
            this.textRreshInterval.Size = new System.Drawing.Size(43, 21);
            this.textRreshInterval.TabIndex = 2;
            this.textRreshInterval.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(317, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "刷新间隔[小时]:";
            // 
            // radioFresh
            // 
            this.radioFresh.AutoSize = true;
            this.radioFresh.Location = new System.Drawing.Point(12, 19);
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
            this.btnImportGoodFile.Location = new System.Drawing.Point(534, 41);
            this.btnImportGoodFile.Name = "btnImportGoodFile";
            this.btnImportGoodFile.Size = new System.Drawing.Size(85, 23);
            this.btnImportGoodFile.TabIndex = 5;
            this.btnImportGoodFile.Text = "导入商品信息";
            this.btnImportGoodFile.UseVisualStyleBackColor = true;
            this.btnImportGoodFile.Click += new System.EventHandler(this.btnImportGoodFile_Click);
            // 
            // SynchroGoodInfo
            // 
            this.SynchroGoodInfo.Location = new System.Drawing.Point(625, 41);
            this.SynchroGoodInfo.Name = "SynchroGoodInfo";
            this.SynchroGoodInfo.Size = new System.Drawing.Size(86, 23);
            this.SynchroGoodInfo.TabIndex = 6;
            this.SynchroGoodInfo.Text = "同步商品信息";
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
            this.btnStartFresh.Location = new System.Drawing.Point(264, 41);
            this.btnStartFresh.Name = "btnStartFresh";
            this.btnStartFresh.Size = new System.Drawing.Size(65, 23);
            this.btnStartFresh.TabIndex = 8;
            this.btnStartFresh.Text = "启动刷新";
            this.btnStartFresh.UseVisualStyleBackColor = true;
            this.btnStartFresh.Click += new System.EventHandler(this.btnStartFresh_Click);
            // 
            // btnStopFresh
            // 
            this.btnStopFresh.Location = new System.Drawing.Point(335, 41);
            this.btnStopFresh.Name = "btnStopFresh";
            this.btnStopFresh.Size = new System.Drawing.Size(41, 23);
            this.btnStopFresh.TabIndex = 9;
            this.btnStopFresh.Text = "停止";
            this.btnStopFresh.UseVisualStyleBackColor = true;
            this.btnStopFresh.Click += new System.EventHandler(this.btnStopFresh_Click);
            // 
            // btnModifyPrice
            // 
            this.btnModifyPrice.Location = new System.Drawing.Point(382, 41);
            this.btnModifyPrice.Name = "btnModifyPrice";
            this.btnModifyPrice.Size = new System.Drawing.Size(69, 23);
            this.btnModifyPrice.TabIndex = 10;
            this.btnModifyPrice.Text = "修改价格";
            this.btnModifyPrice.UseVisualStyleBackColor = true;
            this.btnModifyPrice.Click += new System.EventHandler(this.btnModifyPrice_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(472, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "刷新时间段:";
            // 
            // textFreshStart
            // 
            this.textFreshStart.Location = new System.Drawing.Point(549, 14);
            this.textFreshStart.Name = "textFreshStart";
            this.textFreshStart.Size = new System.Drawing.Size(64, 21);
            this.textFreshStart.TabIndex = 11;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(619, 19);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 12);
            this.label3.TabIndex = 13;
            this.label3.Text = "至:";
            // 
            // textFreshEnd
            // 
            this.textFreshEnd.Location = new System.Drawing.Point(643, 14);
            this.textFreshEnd.Name = "textFreshEnd";
            this.textFreshEnd.Size = new System.Drawing.Size(64, 21);
            this.textFreshEnd.TabIndex = 14;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(178, 22);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(83, 12);
            this.label4.TabIndex = 16;
            this.label4.Text = "单日刷新次数:";
            // 
            // textFreshTime
            // 
            this.textFreshTime.Location = new System.Drawing.Point(266, 15);
            this.textFreshTime.Name = "textFreshTime";
            this.textFreshTime.Size = new System.Drawing.Size(43, 21);
            this.textFreshTime.TabIndex = 15;
            this.textFreshTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(152, -5);
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
            this.checkShowLog.Location = new System.Drawing.Point(111, 45);
            this.checkShowLog.Name = "checkShowLog";
            this.checkShowLog.Size = new System.Drawing.Size(72, 16);
            this.checkShowLog.TabIndex = 20;
            this.checkShowLog.Text = "显示日志";
            this.checkShowLog.UseVisualStyleBackColor = true;
            this.checkShowLog.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 49);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(47, 12);
            this.label5.TabIndex = 22;
            this.label5.Text = "线程数:";
            // 
            // textThreadNum
            // 
            this.textThreadNum.Location = new System.Drawing.Point(60, 41);
            this.textThreadNum.Name = "textThreadNum";
            this.textThreadNum.Size = new System.Drawing.Size(43, 21);
            this.textThreadNum.TabIndex = 21;
            this.textThreadNum.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // checkFresh
            // 
            this.checkFresh.AutoSize = true;
            this.checkFresh.Location = new System.Drawing.Point(187, 45);
            this.checkFresh.Name = "checkFresh";
            this.checkFresh.Size = new System.Drawing.Size(72, 16);
            this.checkFresh.TabIndex = 23;
            this.checkFresh.Text = "暂停刷新";
            this.checkFresh.UseVisualStyleBackColor = true;
            this.checkFresh.CheckedChanged += new System.EventHandler(this.checkFresh_CheckedChanged);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(774, 81);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 24;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Visible = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(840, 569);
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
    }
}

