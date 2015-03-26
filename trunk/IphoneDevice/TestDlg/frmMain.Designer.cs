namespace iTunesDLLDebug
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.lblCommonFiles = new System.Windows.Forms.Label();
            this.lblFoundPath = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(174, 104);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(106, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "CommonFiles:";
            this.label1.Visible = false;
            // 
            // lblCommonFiles
            // 
            this.lblCommonFiles.AutoSize = true;
            this.lblCommonFiles.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCommonFiles.Location = new System.Drawing.Point(286, 104);
            this.lblCommonFiles.Name = "lblCommonFiles";
            this.lblCommonFiles.Size = new System.Drawing.Size(45, 16);
            this.lblCommonFiles.TabIndex = 1;
            this.lblCommonFiles.Text = "label2";
            this.lblCommonFiles.Visible = false;
            // 
            // lblFoundPath
            // 
            this.lblFoundPath.AutoSize = true;
            this.lblFoundPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblFoundPath.Location = new System.Drawing.Point(174, 134);
            this.lblFoundPath.Name = "lblFoundPath";
            this.lblFoundPath.Size = new System.Drawing.Size(45, 16);
            this.lblFoundPath.TabIndex = 5;
            this.lblFoundPath.Text = "label2";
            this.lblFoundPath.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(174, 119);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(240, 16);
            this.label4.TabIndex = 4;
            this.label4.Text = "Found iTunesMobileDevice.dll at:";
            this.label4.Visible = false;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(97, 44);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "获取信息";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(264, 159);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.lblFoundPath);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.lblCommonFiles);
            this.Controls.Add(this.label1);
            this.Name = "frmMain";
            this.Text = "Iphone设备信息获取";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblCommonFiles;
        private System.Windows.Forms.Label lblFoundPath;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button1;
    }
}

