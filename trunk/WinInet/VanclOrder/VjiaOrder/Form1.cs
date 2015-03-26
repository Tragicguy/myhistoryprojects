//#define __PAOPAO

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Threading;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Security.Cryptography;
using System.IO;
using ClassLibrary;
using APIMethod;
using System.Collections;
using System.Data.OleDb;
using System.Web;

namespace PcOnlineFresh
{
    
    public partial class Form1 : Form
    {
        public delegate string GetCodeHandler(string strPath);
        public delegate void ShowEventHandler(_GoodsFreshStr goodInfo);
        public delegate void ShowEventLog(string strInfo);
        public delegate void OnThreadStopEvent(string strInfo);
        public Thread[] triggerThread = new Thread[10];
        string m_strLogPath;        //日志路径
        bool m_bIsStartFresh;   //是否在刷新中[是否点击开始刷新按钮]
        bool m_bFresh;          //控制线程是否运行的标志
        bool m_bIsInFreshduration;//是否已经在刷新时间段内
        System.Timers.Timer m_Timer;    //用TIMER来控制刷新的项

        int m_iThreadStopCount; //线程停止个数
        string m_strStartTime;  
        string m_strEndTime;
        int m_iRefreshTime;
        int m_iRefreshInterval;
        int m_iFreshType;
        int m_iThreadCount;
        DateTime m_dLastFreshTime;
        int m_iHaveFreshedTime;//当天已经刷新的次数
        int m_iCheckGoodInterval;
        bool m_bHaveGood = false;
        bool m_bSuspendFresh;
        Hashtable m_hLockTable = new Hashtable();
        Hashtable m_hLockTableEx = new Hashtable();
        void SaveParams()
        {
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\set.ini";
            IniUtility.WriteIniKey("set", "freshtime", m_iRefreshTime.ToString());
            IniUtility.WriteIniKey("set", "freshinterval", m_iRefreshInterval.ToString());
            //IniUtility.WriteIniKey("set", "freshstart", m_strStartTime);
            //IniUtility.WriteIniKey("set", "freshend", m_strEndTime);
            //IniUtility.WriteIniKey("set", "freshtype", m_iFreshType.ToString());
           // IniUtility.WriteIniKey("set", "showlog", checkShowLog.Checked==true?"1":"0");
            IniUtility.WriteIniKey("set", "threadnum", m_iThreadCount.ToString());
        }

        void LoadParams()
        {
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\set.ini";

            textFreshTime.Text = IniUtility.GetIniKeyValue("set", "freshtime", "5");
            textRreshInterval.Text = IniUtility.GetIniKeyValue("set", "freshinterval", "2");
            textFreshStart.Text = IniUtility.GetIniKeyValue("set", "freshstart", "");
            textFreshEnd.Text = IniUtility.GetIniKeyValue("set", "freshend", "");


            textBoxGoodId.Text = IniUtility.GetIniKeyValue("set", "goodid", "");
            textBoxSizeNo.Text = IniUtility.GetIniKeyValue("set", "skuid", ""); 

            string strInterval = IniUtility.GetIniKeyValue("set", "checkgoodInterval", "120");
            m_iCheckGoodInterval = Int32.Parse(strInterval);
                
            string strType = IniUtility.GetIniKeyValue("set", "freshtype", "0");
            string strCheck = IniUtility.GetIniKeyValue("set", "showlog", "0");
            textThreadNum.Text = IniUtility.GetIniKeyValue("set", "threadnum", "3");
            if (strType == "0")
            {
                radioFresh.Checked = true;
                radioFreshEx.Checked = false;
            }
            else{
                radioFresh.Checked = false;
                radioFreshEx.Checked = true;
            }
            m_bSuspendFresh = false;
            //checkFresh.Checked = true;
            if (strCheck == "1")
            {
                listView.Height = this.ClientRectangle.Height - 80 - listBoxInfo.Height - 10;
                listBoxInfo.Visible = true;
                checkShowLog.Checked = true;
            }
#if __PAOPAO
            this.Text = "泡泡网刷新程序";
#endif
#if DEBUG
#else
            button1.Visible = false;
#endif
        }

        bool IsSetIllegal()
        {
            m_iThreadCount = Int32.Parse(textThreadNum.Text);
            if (m_iThreadCount < 1)
            {
                MessageBox.Show("线程数不能小于1");
                return true;
            }
            if (m_iThreadCount > 10)
            {
                MessageBox.Show("线程数不能大于10");
                return true;
            }
            return false;
        }

        public Form1()
        {
            InitializeComponent();

            this.Height = 700;
            m_strLogPath = System.AppDomain.CurrentDomain.BaseDirectory + "log";
            createHeader();

            m_iHaveFreshedTime = 0;
            m_bIsStartFresh = false;
            m_bFresh = false;
            m_dLastFreshTime = DateTime.Now.AddHours(-4);
            m_iThreadStopCount = 0;          
        }

        private bool IsInFreshduration()
        {
            DateTime dtStart;
            DateTime dtEnd;
            DateTime dtNow;
            dtNow = DateTime.Parse(string.Format("2011-02-12 {0}:{1}:0",DateTime.Now.Hour,DateTime.Now.Minute));
            string strTime = string.Format("2011-02-12 {0}:0", m_strStartTime);
            dtStart = DateTime.Parse(strTime);
            dtEnd = DateTime.Parse(string.Format("2011-02-12 {0}:0", m_strEndTime));
            TimeSpan tSpan = dtNow - dtStart;
            if (tSpan.TotalMinutes < 0)
            {
                return false;
            }
            tSpan = dtEnd - dtNow;
            if (tSpan.TotalMinutes < 0)
            {
                return false;
            }
            return true;
        }

        private void TimerFunction()
        {
            //判断是否到了刷新时间段
            if (!IsInFreshduration())
            {
                m_bIsInFreshduration = false;
                return;
            }
            if (m_bIsInFreshduration == false)
            {
                ShowLogString(string.Format("首次进入当日的刷新时间段，初始化数据"));
                //刚进入时间段,需要初始化次数
                m_iHaveFreshedTime = 0;
                DataContrl.InitFreshTime(m_iRefreshTime);
                //ShowInfo
                string[] strArray = DataContrl.GetAllAccount().Split(',');
                _GoodsFreshStr goodInfo = new _GoodsFreshStr();
                foreach (string strAcc in strArray)
                {
                    if (strAcc == "")
                    {
                        break;
                    }
                    bool bRet = DataContrl.GetDataCache(strAcc, ref goodInfo);
                    if (bRet)
                    {
                        ShowStatusInfo(goodInfo);
                    }
                }
            }
            m_bIsInFreshduration = true;
            if (m_iHaveFreshedTime >= m_iRefreshTime)
            {
                ShowLogString(string.Format("已经达到设定的当日刷新次数"));
                return;
            }
            //判断是否超过了刷新间隔
            TimeSpan tSpan = DateTime.Now - m_dLastFreshTime;
            int i = (int)tSpan.TotalMinutes;
            if (i < m_iRefreshInterval * 60)   //未到刷新间隔
            {
                return;
            }            
            m_dLastFreshTime = DateTime.Now;
            //将列表中的所有账号加入等待刷新队列
            DataContrl.InitFreshAccount();
            m_iHaveFreshedTime++;
            ShowLogString(string.Format("达到刷新间隔时间,初始化刷新账号队列,刷新类型：{0}",m_iFreshType==0?"一键刷新":"自定义刷新"));
        }
        private void timer_Tick(object sender, EventArgs e)
        {
            TimerFunction();
        }

        private void createHeader()//为listview添加列名
        {
            listBoxInfo.Height = 200;

            listView.Left = 10;
            listView.Top = 70;
            listView.Width = 1015;
            listView.Height = this.ClientRectangle.Height - 80;
            listBoxInfo.Left = 10;
            listBoxInfo.Width = 1015;
            listBoxInfo.Top = 460;
            listBoxInfo.Visible = false;
            listView.GridLines = true;
        //显示各个记录的分隔线
            listView.FullRowSelect = true;
            listView.View = View.Details;
            listView.Scrollable = true;
            
        //需要时候显示滚动条
            listView.MultiSelect = false; // 不可以多行选择
            listView.HeaderStyle = ColumnHeaderStyle.Nonclickable;
            listView.Columns.Clear();
            listView.Columns.Add("序号",40);
            listView.Columns.Add("帐号", 80);
            //listView.Columns.Add("密码", 80);
            listView.Columns.Add("商品ID", 120);
            listView.Columns.Add("商品名称", 90);
            listView.Columns.Add(@"状态\备注", 230);
            listView.Columns.Add("收货人", 70);
            listView.Columns.Add("电话", 65);
            listView.Columns.Add("邮编", 55);
            listView.Columns.Add("收货地址", 230);
            ImageList imgList = new ImageList();
            imgList.ImageSize = new Size(1, 20);//分别是宽和高
            listView.SmallImageList = imgList;

        }

        private void AddViewItem(_GoodsFreshStr goodInfo)
        {
            //userid|pwd|goodid|skuid|name|phone|postcode|province|city|town|addr
            ListViewItem li = new ListViewItem(); 
            li.SubItems.Clear();
            li.SubItems[0].Text = listView.Items.Count.ToString();
            li.SubItems.Add(goodInfo.strAcc);
            li.SubItems.Add( string.Format("{0}:{1}", goodInfo.goodId, goodInfo.skuId));
            li.SubItems.Add("");    //goodname
            li.SubItems.Add(goodInfo.strFreshStatus);
            li.SubItems.Add(goodInfo.strReceiver);
            li.SubItems.Add(goodInfo.strPhone);
            li.SubItems.Add(goodInfo.strPostCode);
            li.SubItems.Add(  string.Format("{0} {1} {2} {3}", goodInfo.strProvince, goodInfo.strCity, goodInfo.strTown, goodInfo.strAddr) );
            listView.Items.Add(li);
            goodInfo.listItem = li;
        }

        private void ModifyViewItem(_GoodsFreshStr goodInfo)
        {
            goodInfo.listItem.SubItems[4].Text = goodInfo.strFreshStatus;
            goodInfo.listItem.SubItems[3].Text = goodInfo.strGoodName;
            if (goodInfo.strFreshStatus.IndexOf("失败") >= 0)
            {
                string strInfo = string.Format("{0}     {1}     {2}:{3}     {4}{5}{6}{7}{8}{9}", goodInfo.strFreshStatus, goodInfo.strAcc, goodInfo.goodId, goodInfo.skuId, goodInfo.strProvince, goodInfo.strCity, goodInfo.strTown, goodInfo.strAddr, goodInfo.strReceiver, goodInfo.strPhone);
                WriteLogFail(strInfo);
                goodInfo.listItem.BackColor = Color.Red;
            }
            else if (goodInfo.strFreshStatus.IndexOf("下单成功") >= 0)
            {
                string strInfo = string.Format("{0}     {1}     {2}:{3}     {4}{5}{6}{7}{8}{9}", goodInfo.strFreshStatus, goodInfo.strAcc, goodInfo.goodId, goodInfo.skuId, goodInfo.strProvince, goodInfo.strCity, goodInfo.strTown, goodInfo.strAddr, goodInfo.strReceiver, goodInfo.strPhone);
                WriteLogSuc(strInfo);
                goodInfo.listItem.BackColor = Color.Green;
            }
            else
                goodInfo.listItem.BackColor = Color.White;

            return;
        }

        private void btnStartFresh_Click(object sender, EventArgs e)
        {
            if (IsSetIllegal())
                return;
            SaveParams();
            if (DataContrl.GetAccCount() < 1)
            {
                MessageBox.Show("请先导入帐号!");
                return;
            }
            m_bSuspendFresh = checkFresh.Checked;
            m_iThreadStopCount = 0;
            //启动10个线程
            m_bFresh = true;
            m_bIsStartFresh = true;
            m_bSuspendFresh = false;
            for (int i = 0; i < m_iThreadCount;i++ )
            {
                ThreadStart myThreadStart = new ThreadStart(FreshProc);
                triggerThread[i] = new Thread(myThreadStart);
                triggerThread[i].Start();
            }
            btnStartFresh.Enabled = false;
            btnStopFresh.Enabled = true;
            checkFresh.Enabled = false;
            btnImportAcc.Enabled = false;
            ShowLogString("下单线程启动");
        }
        private string MD5Create(string STR) //STR为待加密的string
        {
            string pwd = "";
            //pwd为加密结果
            MD5 md5 = MD5.Create();
            byte[] s = md5.ComputeHash(Encoding.GetEncoding("gb2312").GetBytes(STR));
            //这里的UTF8是编码方式，你可以采用你喜欢的方式进行，比如UNcode等等
            for (int i = 0; i < s.Length; i++)
            {
                pwd = pwd + s[i].ToString();
            }
            return pwd;
        }

       
        public void FreshProc()
        {
            Fresh();
            m_iThreadStopCount++;
            if (m_iThreadStopCount >= m_iThreadCount)
            {
                ShowLogString("所有线程均已经退出");
                ResetButton("");
            }
        }

        public void Fresh()
        {
            _GoodsFreshStr goodInfo = new _GoodsFreshStr();
            GoodsFresh objFresh = new GoodsFresh();            
            bool bRet = false;
            string strFreshAcc = "";
            string strModifyAcc = "";
            string strSynAcc = "";
            DateTime tOldShowTime = DateTime.Now.AddHours(-1);
            while (m_bFresh)
            {                
                //从刷新列表获取,有则刷新,无则继续循环
                strFreshAcc = DataContrl.GetFirstAccount();
                if (strFreshAcc == "")
                {
                    Thread.Sleep(100);
                    break;
                }
                bRet = DataContrl.GetDataCache(strFreshAcc, ref goodInfo);
                if (!bRet)
                {
                    Thread.Sleep(100);
                    break;
                }
                if (m_bSuspendFresh)
                {
                    MessageBox.Show("请确定在继续之前IP已经变化", "确认");
                }
                m_iFreshType = 0;
                objFresh.m_objForm = this;
                objFresh.SetWebPost(goodInfo.webPost);
                FreshQuick(goodInfo, objFresh);
                ShowStatusInfo(goodInfo);
                Thread.Sleep(1000);
            }	
        }

        private void ShowLog(string strInfo)
        {
            //添加至日志显示列表
            listBoxInfo.Items.Add(strInfo);
            if (strInfo.IndexOf("商品有货了") == 0)
            {
                clsMCI cm = new clsMCI();
                cm.FileName = "Notice.mp3";
                cm.play();
            }
            //写日志
           // WriteLog(strInfo);
        }

        public void ShowLogString(string strInfo)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    ShowEventLog _myInvoke = new ShowEventLog(ShowLog);
                    this.Invoke(_myInvoke, new object[] { strInfo });
                }
                else
                {
                    ShowLog(strInfo);
                }
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }

        private void ModifyStatus(_GoodsFreshStr goodInfo)
        {
            ModifyViewItem(goodInfo);
        }        

        public void ShowStatusInfo(_GoodsFreshStr goodInfo)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    ShowEventHandler _myInvoke = new ShowEventHandler(ModifyStatus);
                    this.Invoke(_myInvoke, new object[] { goodInfo }); 
                }
                else
                {
                    ModifyStatus(goodInfo);
                }                
            }
            catch(Exception e)
            {
                string strError = e.Message;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_Timer != null)
            {
                m_Timer.Stop();
                m_Timer.Dispose();
            }

            for (int i = 0; i < m_iThreadCount; i++)
            {
                if (triggerThread[i] != null)
                {
                    triggerThread[i].Abort();
                }
            }           
        }

        private void btnImportAcc_Click(object sender, EventArgs e)
        {
            string strPath = "";
            OpenFileDialog find = new OpenFileDialog();
            find.Filter = "TXT文件(*.txt)|*.txt";

            if (find.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            DataContrl.DeleteAllAccount();
            listView.Items.Clear();

            strPath = find.FileName;
            string strContent = ReadFileLine(strPath);
            string[] strArray = strContent.Split('\n');
            int nCount = 0;
            foreach (string strLine in strArray)
            {
                //userid|pwd|goodid|skuid|num|name|phone|postcode|province|city|town|addr
                string[] strInfo = strLine.Split('|');
                if (strInfo.Length < 12)
                {
                    continue;
                }
                if (DataContrl.IsInCache(strInfo[0]))
                {
                    continue;
                }
                if (strInfo[0].Length < 1 || strInfo[1].Length < 1)
                {
                    continue;
                }
                _GoodsFreshStr GoodInfo = new _GoodsFreshStr();
                GoodInfo.strAcc = strInfo[0];
                GoodInfo.strPwd = strInfo[1];
                GoodInfo.goodId = strInfo[2];
                GoodInfo.skuId = strInfo[3];
                GoodInfo.strNum = strInfo[4];
                GoodInfo.strReceiver = strInfo[5];
                GoodInfo.strPhone = strInfo[6];
                GoodInfo.strPostCode = strInfo[7];
                GoodInfo.strProvince = strInfo[8];
                GoodInfo.strCity = strInfo[9];
                GoodInfo.strTown = strInfo[10];
                GoodInfo.strAddr = strInfo[11];
                GoodInfo.strSynStatus = "初始状态";
                GoodInfo.webPost = new ClassHttpPost();
                string strFlagAcc = string.Format("{0}_{1}", GoodInfo.strAcc, nCount);
                nCount++;
                DataContrl.AddDataCache(strFlagAcc, GoodInfo);
                AddViewItem(GoodInfo);
            }

            DataContrl.InitFreshAccount();
        }

        private string ReadFileLineXls(string strFilePath)
        {
            DataSet ds = ExceltoDataSet(strFilePath);     
            if (ds == null)
            {
                return  "";
            }
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < ds.Tables[0].Rows.Count;i++ )
            {
                builder.Append(ds.Tables[0].Rows[i][0].ToString() + "," + ds.Tables[0].Rows[i][1].ToString() +"\n");
            }
            ds.Dispose();
            return builder.ToString();
        }

        private string ReadFileLine(string strFilePath)
        {
            FileStream   fs=new   FileStream(strFilePath,FileMode.Open,FileAccess.Read,FileShare.None);
            StreamReader sr = new StreamReader(fs, System.Text.Encoding.GetEncoding("gb2312"));            
            sr.BaseStream.Seek(0,SeekOrigin.Begin); 
            StringBuilder builder = new StringBuilder();
            string     strLine     =sr.ReadLine(); 
            while(strLine!=null) 
            { 
                builder.Append(strLine+"\n");
                strLine=sr.ReadLine(); 
            }
            sr.Close();
            fs.Close();
            return builder.ToString();
        }
        public void WriteLog(string strShow)
        {
            try
            {
                if (!Directory.Exists(m_strLogPath))
                {
                    Directory.CreateDirectory(m_strLogPath);
                }
                string strFileName = DateTime.Now.ToString("yyyyMMdd") + ".txt";
                strFileName = m_strLogPath + "\\" + strFileName;
                StreamWriter fs = File.AppendText(strFileName);
                fs.Write(string.Format("{0}\t{1}\r\n", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strShow));
                fs.Close();
            }
            catch
            {

            }
        }

        public void WriteLogSuc(string strShow)
        {
            try
            {
                if (!Directory.Exists(m_strLogPath))
                {
                    Directory.CreateDirectory(m_strLogPath);
                }
                string strFileName = DateTime.Now.ToString("yyyyMMdd") + "_success.txt";
                strFileName = m_strLogPath + "\\" + strFileName;
                StreamWriter fs = File.AppendText(strFileName);
                fs.Write(string.Format("{0}\t{1}\r\n", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strShow));
                fs.Close();
            }
            catch
            {

            }
        }

        public void WriteLogFail(string strShow)
        {
            try
            {
                if (!Directory.Exists(m_strLogPath))
                {
                    Directory.CreateDirectory(m_strLogPath);
                }
                string strFileName = DateTime.Now.ToString("yyyyMMdd") + "fail.txt";
                strFileName = m_strLogPath + "\\" + strFileName;
                StreamWriter fs = File.AppendText(strFileName);
                fs.Write(string.Format("{0}\t{1}\r\n", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strShow));
                fs.Close();
            }
            catch
            {

            }
        }

        public void WriteInfo(string strDirectry, StringBuilder strInfoBuilder, string strAcc)
        {
            try
            {
                
                if (!Directory.Exists(strDirectry))
                {
                    Directory.CreateDirectory(strDirectry);
                }
                string strFileName = strAcc.ToString() + ".txt";
                strFileName = strDirectry + "\\" + strFileName;
                if (File.Exists(strFileName))
                {
                    File.Delete(strFileName);
                }
                StreamWriter fs = File.AppendText(strFileName);
                fs.Write(strInfoBuilder.ToString());
                fs.Close();
            }
            catch
            {

            }
        }

        private void btnImportGoodFile_Click(object sender, EventArgs e)
        {
            int iCount = DataContrl.GetModifyCount();
            if (iCount > 0 && m_bIsStartFresh)
            {
                MessageBox.Show("正在修改中,请等待修改完后再执行此操作!");
                return;
            }
            //选择文件夹
            FolderBrowserDialog find = new FolderBrowserDialog();
            if (find.ShowDialog() != DialogResult.OK)
            {
                return;
            }

           DataContrl.ClearModifyData();
           DirectoryInfo dInfo = new DirectoryInfo(find.SelectedPath);
           
           foreach (FileInfo fileInfo in dInfo.GetFiles())
           {
               Hashtable hTable = new Hashtable();
               hTable.Clear();
               string strName = fileInfo.Name;
               string[] strNameArray = strName.Split('.');
               if (strNameArray.Length != 2)
               {
                   ShowLogString("文件名不合法:" + strName);
                   continue;
               }
//                if (strNameArray[1] != "txt")    //不是文本文档
//                {
//                    continue;
//                }

               if (strNameArray[1] != "xls")    //不是文本文档
               {
                   ShowLogString("文件名不合法:" + strName);
                   continue;
               }

               string strAcc = strNameArray[0];  //以账号名为商品对应的账号
               if (!DataContrl.IsInCache(strAcc))//如果此账号不存在
               {
                   ShowLogString("此账号不存在:" + strAcc);
                   continue;
               }

               //得到账号名后，判断是否已经对此账号进行过同步
               _GoodsFreshStr goodInfo = new _GoodsFreshStr();
               if (!DataContrl.GetDataCache(strAcc, ref goodInfo))
               {
                   ShowLogString("此账号不存在:" + strAcc);
                   continue;
               }
               if (goodInfo.iSynchronizeCount < 1)
               {
                   goodInfo.strSynStatus = "尚未同步,要修改价格请先同步";
                   goodInfo.strModifyStatus = "尚未同步,要修改价格请先同步";
                   ShowStatusInfo(goodInfo);
                   ShowLogString("尚未同步,要修改价格请先同步");
                   continue;
               }
               goodInfo.iNeedAddCount = 0;
               string strContent = ReadFileLineXls(find.SelectedPath + "\\" + strName);
               string[] strArray = strContent.Split('\n');
               
               float fPrice = 0;
               Hashtable hTableSyn = new Hashtable();
               Hashtable hTableSynNumber = new Hashtable();
               Hashtable hTableNeedAdd = new Hashtable();
               bool bRet = DataContrl.GetSynData(strAcc, ref hTableSyn);
               
               if (!bRet)
               {
                   goodInfo.strModifyStatus = "获取同步商品信息失败";
                   ShowStatusInfo(goodInfo);
                   ShowLogString("获取同步商品信息失败");
                   continue;
               }
               foreach (string strLine in strArray)
               {
                   //品牌+型号，商品价格
                  
                   string[] strInfo = strLine.Split(',');
                   if (strInfo.Length < 2)
                   {
                       ShowLogString("导入文件内容格式错误");
                       continue;
                   }
                   string strGoodNoEx = strInfo[0].Trim();
                   strGoodNoEx = strGoodNoEx.Trim();
                   if (strGoodNoEx.Length < 1)
                   {
                       ShowLogString("导入文件内容格式错误");
                       continue;
                   }
                   string strNoList = DataContrl.GetSynNumberData(goodInfo.strAcc + strGoodNoEx).Trim();
                   if (strNoList == "")
                   {
                       float.TryParse(strInfo[1], out fPrice);
                       _GoodsModifyStr goodModify = new _GoodsModifyStr();
                       goodModify.strNo = strGoodNoEx;
                       goodModify.strNoEx = strGoodNoEx;
                       goodModify.fPrice = fPrice;
                       //加入需要添加的列表
                       if (!hTableNeedAdd.ContainsKey(strGoodNoEx))
                       {
                           hTableNeedAdd.Add(strGoodNoEx, goodModify);
                       }
                       //ShowLogString("未找到匹配的同步信息");
                       continue;
                   }
                   string[] strNolistArray = strNoList.Split(',');
                   
                  
                   //当等待修改的商品名称在同步列表中时，才添加
                   foreach (string strNo in strNolistArray)
                   {                       
                       if (strNo == "")
                       {
                           continue;
                       }
                       if (hTable.Contains(strNo))
                       {
                           continue;
                       }
                       float.TryParse(strInfo[1], out fPrice);
                       _GoodsModifyStr goodModify = new _GoodsModifyStr();
                       _GoodsModifyStr goodModifySyn = new _GoodsModifyStr();
                       if (hTableSyn.ContainsKey(strNo))
                       {
                           goodModifySyn = (_GoodsModifyStr)hTableSyn[strNo];
                           goodModify.strNo = goodModifySyn.strNo;
                           goodModify.strNoEx = goodModifySyn.strNoEx;
                           goodModify.strName = goodModifySyn.strName;
                           goodModify.fLowPrice = goodModifySyn.fLowPrice;
                           goodModify.fHighPrice = goodModifySyn.fHighPrice;

                           goodModify.fPrice = fPrice;
                           if (goodModify.fPrice < goodModify.fLowPrice)
                           {
                               goodModify.fPrice = goodModify.fLowPrice;
                           }
                           else if (goodModify.fPrice > goodModify.fHighPrice)
                           {
                               goodModify.fPrice = goodModify.fHighPrice;
                           }
                           hTable.Add(strNo, goodModify);
                       }
                       else
                       {
                           //此处还少商品名
                           goodModify.strNo = strGoodNoEx;
                           goodModify.strNoEx = strGoodNoEx;
                           goodModify.fPrice = fPrice;
                           //加入需要添加的列表
                           if (!hTableNeedAdd.ContainsKey(strNo))
                           {
                               hTableNeedAdd.Add(strNo, goodModify);
                           }                          
                       }
                   }                   
               }
               DataContrl.AddNeedAddList(strAcc, hTableNeedAdd);
               goodInfo.iNeedAddCount = hTableNeedAdd.Count;
               goodInfo.iModifyCount = hTable.Count;
               goodInfo.strModifyStatus = "等待修改";
               goodInfo.bModify = true;
               ShowStatusInfo(goodInfo);
               DataContrl.AddModifyList(strAcc, hTable);
               ShowLogString(string.Format("{0} 添加了 {1} 条价格修改信息", strAcc, hTable.Count));               
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _GoodsFreshStr goodInfo = new _GoodsFreshStr();
            GoodsFresh objFresh = new GoodsFresh();

            goodInfo.strAcc = "shaojun.hnist@gmail.com";
            goodInfo.strPwd = "wawawo23";
            goodInfo.goodId = "2892042";
            goodInfo.skuId = "25063749";
            goodInfo.strReceiver = "陈兵";
            goodInfo.strProvince = "广东";
            goodInfo.strCity = "深圳市";
            goodInfo.strTown = "南山区";
            goodInfo.strAddr = "南油地区";
            goodInfo.strPostCode = "518052";
            goodInfo.strPhone = "13028272672";

            goodInfo.webPost = new ClassHttpPost();
            string strError = "";
            Login(goodInfo.webPost, goodInfo.strAcc, goodInfo.strPwd, ref strError, true);
            MessageBox.Show(strError);

            objFresh.m_objForm = this;
            objFresh.SetWebPost(goodInfo.webPost);
            objFresh.AddToCart(goodInfo);
            objFresh.SetTransTypeAndPayType(goodInfo);

            /*
            ClassHttpPost webpost = new ClassHttpPost();
            string strError = "";
            bool bRet = Login(webpost, "shaojun.hnist@gmail.com", "wawawo20", ref strError);
            if (!bRet)
            {
                MessageBox.Show(strError);
                return;
            }
            GoodsFresh fresh = new GoodsFresh();
            fresh.SetWebPost(webpost);
            fresh.AddToCart(textBoxGoodId.Text, textBoxSizeNo.Text, textBoxNum.Text);
            fresh.AddAddress(out strError);
            string strOrderNo = "";
            fresh.SetTransTypeAndPayType(out strOrderNo);
            if (strOrderNo != "")
                MessageBox.Show(strOrderNo);
            */
        }

        bool IsGoodExist(_GoodsFreshStr goodInfo)
        {
            try
            {
                string[] strArray = goodInfo.goodId.Split('-');
                string strGoodUrl = string.Format("http://item.vancl.com/{0}.html", strArray[0]);

                string strRet = goodInfo.webPost.GetHttpPage(strGoodUrl,
                       "utf-8", "get", "", "",
                       "",
                       "*/*", "");
                if (strRet.IndexOf("<h2>出错了</h2>") > 0)
                {
                    strRet = strRet.Substring(strRet.IndexOf("<h2>出错了</h2>"));
                    goodInfo.strFreshStatus = GetKeyString(strRet, "<p>", "</p>");
                    return false;
                }
                else if (strRet == "")
                {
                    goodInfo.strFreshStatus = "加载商品页面出错";
                    return false;
                }
                if (strRet.IndexOf("抱歉，该商品暂不能购买") > 0)
                    return false;
                return true;
            }
            catch
            { }
            return false;
        }

        DateTime m_dateLastCheckTime = DateTime.Now.AddHours(-1);
        private bool FreshQuick(_GoodsFreshStr goodInfo, GoodsFresh objFresh)
        {
            lock (m_hLockTableEx)
            {
                //如果无货，循环检测
                while (!m_bHaveGood)
                {
                    TimeSpan tSpan = DateTime.Now - m_dateLastCheckTime;
                    if (tSpan.TotalSeconds > m_iCheckGoodInterval)
                    {
                        m_bHaveGood = IsGoodExist(goodInfo);
                        m_dateLastCheckTime = DateTime.Now;
                        if (m_bHaveGood)
                        {
                            ShowLogString("商品有货了   " + DateTime.Now.ToString());
                            break;
                        }
                        else
                        {
                            ShowLogString("商品无货 " +DateTime.Now.ToString());
                        }
                    }
                    System.Threading.Thread.Sleep(1500);                    
                }
            }
            string strError = "";
            goodInfo.strFreshStatus = "正在登录...";
            ShowStatusInfo(goodInfo);
            bool bRet = Login(goodInfo.webPost, goodInfo.strAcc, goodInfo.strPwd, ref strError, true);
            if (!bRet)
            {
                if (strError.IndexOf("为保证账户安全，请填写验证码") < 0)
                {
                    ShowLogString("为保证账户安全，请填写验证码");
                    goodInfo.strFreshStatus = "登录失败：" + strError;
                    ShowStatusInfo(goodInfo);
                    return false;
                }
                bRet = Login(goodInfo.webPost, goodInfo.strAcc, goodInfo.strPwd, ref strError, true);
                if (!bRet)
                {
                    goodInfo.strFreshStatus = "登录失败：" + strError;
                    ShowStatusInfo(goodInfo);
                    return false;
                }
            }
            goodInfo.strFreshStatus = "正在添加商品至购物车...";
            ShowStatusInfo(goodInfo);

            bRet = objFresh.AddToCart(goodInfo);
            if (!bRet)
            {
                goodInfo.strFreshStatus = "加入购物车失败：" + goodInfo.strFreshStatus;
                ShowStatusInfo(goodInfo);
                return false;
            }

            goodInfo.strFreshStatus = "正在设置寄送地址...";
            ShowStatusInfo(goodInfo);
            bRet = objFresh.AddAddress(goodInfo);
            if (!bRet)
            {
                goodInfo.strFreshStatus = "设置寄送地址失败：" + goodInfo.strFreshStatus;
                ShowStatusInfo(goodInfo);
            }

            goodInfo.strFreshStatus = "正在设置寄送方式、支持方式、提交定单...";
            ShowStatusInfo(goodInfo);
            bRet = objFresh.SetTransTypeAndPayType(goodInfo);
            if (!bRet)
            {
                goodInfo.strFreshStatus = "设置寄送方式、支持方式、提交定单失败：" + goodInfo.strFreshStatus;
                ShowStatusInfo(goodInfo);
            }
            return true;
        }

        
        public string GetCode(string strPath)
        {
            string strRet = "";
            //显示,拿返回值
            Form2 form = new Form2();
            form.m_strPath = strPath;
            form.ShowDialog();
            return form.m_strCode;
                
        }

        private void btnStopFresh_Click(object sender, EventArgs e)
        {
            if (m_Timer != null)
            {
                m_Timer.Stop();
                m_Timer.Dispose();
            }
           
            m_bIsStartFresh = false;
            m_bFresh = false;
            Thread.Sleep(500);
            for (int i = 0; i < m_iThreadCount; i++)
            {
                if (triggerThread[i] != null)
                {
                    triggerThread[i].Abort();
                }
            }
            btnStopFresh.Enabled = false;
            btnStartFresh.Enabled = true;
            checkFresh.Enabled = true;
            btnImportAcc.Enabled = true;
            ShowLogString("停止刷新");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            LoadParams();
            ShowLogString("程序启动");
            btnStopFresh.Enabled = false;
            
        }

        public void ResetButton(string strInfo)
        {
            if (btnStartFresh.InvokeRequired)
            {
                ShowEventLog _myInvoke = new ShowEventLog(ResetButtonEx);
                this.Invoke(_myInvoke, new object[] { "start" });
            }
            else
            {
                ResetButtonEx(btnStartFresh);
            }

            if (btnStopFresh.InvokeRequired)
            {
                ShowEventLog _myInvoke = new ShowEventLog(ResetButtonEx);
                this.Invoke(_myInvoke, new object[] { "stop" });
            }
            else
            {
                ResetButtonEx(btnStopFresh);
            }

            if (checkFresh.InvokeRequired)
            {
                ShowEventLog _myInvoke = new ShowEventLog(ResetButtonEx);
                this.Invoke(_myInvoke, new object[] { "check" });
            }
            else
            {
                ResetButtonEx(checkFresh);
            }

            if (btnImportAcc.InvokeRequired)
            {
                ShowEventLog _myInvoke = new ShowEventLog(ResetButtonEx);
                this.Invoke(_myInvoke, new object[] { "import" });
            }
            else
            {
                ResetButtonEx(btnImportAcc);
            }
        }

        public void ResetButtonEx(string strFlag)
        {
            Control btn = null;
            if (strFlag == "start")
                btn = btnStartFresh;
            else if (strFlag == "stop")
                btn = btnStopFresh;
            else if (strFlag == "import")
                btn = btnImportAcc;
            else btn = checkFresh;

            ResetButtonEx(btn);
        }

        public void ResetButtonEx(Control btn)
        {
            try
            {
                if(btn == btnStopFresh)
                    btn.Enabled = false;
                else btn.Enabled = true;
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }

        private void btnModifyPrice_Click(object sender, EventArgs e)
        {
            if (!m_bIsStartFresh)
            {
                MessageBox.Show("请先启动线程!");
                return;
            }
            if (DataContrl.GetAccCount() < 1)
            {
                MessageBox.Show("请先导入帐号!");
                return;
            }
            int iCount = DataContrl.GetModifyCount();
            if (iCount > 0)
            {
                MessageBox.Show("正在修改中,请等待修改完后再执行此操作!");
                return;
            }
            //添加
            DataContrl.InitModifyAccount();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
           if (checkShowLog.Checked)
           {
               listView.Height = this.ClientRectangle.Height - 80 - listBoxInfo.Height - 10;
               listBoxInfo.Visible = true;
           }
           else
           {
               listView.Height = this.ClientRectangle.Height - 80;
               listBoxInfo.Visible = false;
           }
        }

        private void checkFresh_CheckedChanged(object sender, EventArgs e)
        {
            m_bSuspendFresh = checkFresh.Checked;
        }

        private void listView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            return;
            ListViewHitTestInfo info = listView.HitTest(e.X, e.Y);
            if (info.Item != null)
            {
                string strAcc = info.Item.SubItems[1].Text;
                Hashtable hTable = new Hashtable();
                bool bRet = DataContrl.GetNeedAddData(strAcc, ref hTable);
                if (bRet)
                {
                    if (hTable.Count > 0)
                    {
                        StringBuilder strBuilder = new StringBuilder();
                        _GoodsModifyStr objModify = new _GoodsModifyStr();
                        int iCount = 0;
                        foreach (DictionaryEntry dEntry in hTable)
                        {
                            objModify = (_GoodsModifyStr)dEntry.Value;
                            string strInfo = "";
                            if (objModify.iFailFlag != 3 && objModify.iFailFlag != 2)
                            {
                                strInfo = "";
                                strBuilder.Append(string.Format("{0}  {1} {2}\r\n", objModify.strNoEx, objModify.fPrice, strInfo));
                                iCount++;
                            }
                        }
                        string strDirectory = System.AppDomain.CurrentDomain.BaseDirectory + "needaddfile";
                        WriteInfo(strDirectory, strBuilder, strAcc);
                        string strFileName = strAcc + ".txt";
                        strFileName = strDirectory + "\\" + strFileName;
                        string strInfoShow = strBuilder.ToString();
                        if (strInfoShow.Length > 200)
                        {
                           strInfoShow = strInfoShow.Substring(0, 200) + ".....";
                        }
                        string strShow = string.Format("待添加信息{0}条\r\n请在 needaddfile\\{1}.txt 中查看\r\n{2}", iCount,strAcc, strInfoShow);
                        MessageBox.Show(strShow);
                    }
                }
            }
        }

        private void SynchroGoodInfo_Click(object sender, EventArgs e)
        {
            if (!m_bIsStartFresh)
            {
                MessageBox.Show("请先启动线程!");
                return;
            }
            if (DataContrl.GetAccCount() < 1)
            {
                MessageBox.Show("请先导入帐号!");
                return;
            }
            string strShow = string.Format("此操作将会对未同步过的账号进行商品同步，完成后需要重启程序，要执行吗？");

            if (DialogResult.OK != MessageBox.Show(strShow,"询问",MessageBoxButtons.OKCancel, MessageBoxIcon.Question))
            {
                return;
            }
            //同步商品信息
            //开始同步商品信息，只对未同步过的账号进行同步
            DataContrl.InitSynAccount();
        }

        /// <summary>
        /// 读取excel文档
        /// </summary>
        /// <param name="path">文件名称</param>
        /// <returns>返回一个数据集</returns>
        public DataSet ExceltoDataSet(string path)
        {
            try
            {
                string strconn = "provider=microsoft.jet.oledb.4.0;" + "data source=" + path + ";" + "extended properties=excel 8.0;";
                OleDbConnection conn = new OleDbConnection(strconn);
                conn.Open();
                string strexcel = "";
                OleDbDataAdapter mycommand = null;
                DataSet ds = null;
                strexcel = "select * from [sheet1$]";
                mycommand = new OleDbDataAdapter(strexcel, strconn);
                ds = new DataSet();
                mycommand.Fill(ds, "table1");
                conn.Close();
                return ds;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(string.Format("失败:\r\n{0}\r\n{1}", ex.Message, path));
                return null;
            }            
        }
        public void SaveToExcel(string strDirectry, Hashtable hTable, string strAcc)
        {
            _GoodsModifyStr objStru = new _GoodsModifyStr();
            if (!Directory.Exists(strDirectry))
            {
                Directory.CreateDirectory(strDirectry);
            }
            string strFileName = strAcc.ToString() + ".xls";
            strFileName = strDirectry + "\\" + strFileName;
            if (File.Exists(strFileName))
            {
                File.Delete(strFileName);
            }
            string strconn = "provider=microsoft.jet.oledb.4.0;" + "data source=" + strFileName + ";" + "extended properties=excel 8.0;";
            
            OleDbConnection cn = new OleDbConnection(strconn);
            string sqlCreate = "CREATE TABLE [sheet1] ([品牌+型号] VarChar,[价格] Number)";

            OleDbCommand cmd = new OleDbCommand(sqlCreate, cn);

            cn.Open();

            //创建TestSheet工作表

            cmd.ExecuteNonQuery();
            //添加数据
            string[] keyArray = new string[hTable.Count];
            string[] valueArray = new string[hTable.Count];
            hTable.Keys.CopyTo(keyArray, 0);
            int i = 0;
            foreach (string skey in keyArray)
            {
                objStru = (_GoodsModifyStr)hTable[skey];
                valueArray[i] = objStru.strNoEx;
                i++;
            }
            Array.Sort(valueArray, keyArray);
            foreach (string skey in keyArray)
            {
                objStru = (_GoodsModifyStr)hTable[skey];
                cmd.CommandText = string.Format("INSERT INTO [sheet1] VALUES('{0}',{1})", objStru.strNoEx.Trim(), objStru.fPrice);
                cmd.ExecuteNonQuery();
            }
            cn.Close();
            cn.Dispose();
         }

        private void listView_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
        private string GetKeyString(string strSrc, string strBefore, string strAfter)
        {
            int iPosBefore = 0;
            int iPosAfter = 0;
            iPosBefore = strSrc.IndexOf(strBefore) + strBefore.Length;
            iPosAfter = strSrc.IndexOf(strAfter, iPosBefore);
            if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0)
            {
                return "";
            }
            else
            {
                return strSrc.Substring(iPosBefore, iPosAfter - iPosBefore);
            }
        }

        //sohu reg
        private void button2_Click(object sender, EventArgs e)
        {
            clsMCI cm = new clsMCI();
            cm.FileName = "Notice.mp3";
            cm.play();
            return;
            _GoodsFreshStr goodInfo = null;
            DataContrl.GetDataCache("shaojun.hnist@gmail.com", ref goodInfo);
            goodInfo.strFreshStatus = "下单成功";
            ShowStatusInfo(goodInfo);

            DataContrl.GetDataCache("shaojun.hnist@gmail.com1", ref goodInfo);
            goodInfo.strFreshStatus = "失败";
            ShowStatusInfo(goodInfo);

            DataContrl.GetDataCache("shaojun.hnist@gmail.com2", ref goodInfo);
            goodInfo.strFreshStatus = "登录中...";
            ShowStatusInfo(goodInfo);

            return;
            string strShow = string.Format("{0}", DateTime.Now.ToFileTime());
            MessageBox.Show(strShow);
            return;
            //GetImageEx
            GoodsFresh objFreshEx = new GoodsFresh();
            ClassHttpPost webPost = new ClassHttpPost();
            objFreshEx.SetWebPost(webPost);
            string strRet = webPost.GetHttpPage("https://session.trionworlds.com/login?service=unspecified&locale=zh_CN",
                    "gb2312", "get","", "",
                    "",
                    "*/*", "");
            string strError = "";

      //  https://session.trionworlds.com/login;jsessionid=F355EBF3FCC45A052024921548FD6D59
            //<form id="fm1" class="clearfix" action="/login;jsessionid=46E8217B16B969395A786F70C63DB4F0" method="post">
            //
            string strUrl = GetKeyString(strRet, "id=\"fm1\" class=\"clearfix\" action=\"", "\"");
            strUrl = "https://session.trionworlds.com" + strUrl;
            string strCode = GetKeyString(strRet, "name=\"lt\" value=\"", "\""); ;
            string strData = string.Format("username=jblackhat@live.com&password=l19b351k&lt={0}&_eventId=submit",
                    strCode
                );
            strRet = webPost.GetHttpPage(strUrl,
                    "gb2312", "POST","", strData,
                    "https://session.trionworlds.com/login?service=unspecified&locale=zh_CN",
                    "*/*", "");

        }


        
        public bool Login(ClassHttpPost webPost, string strAcc,string strPwd, ref string strError, bool bForceValidate)
        {
            strError = "";
            string strUrl = "https://www.vjia.com/login/login.aspx?http://www.vjia.com/";
            string strRet = webPost.GetHttpPage(strUrl, "utf-8", "get", "", "", "", "*/*", "");
            if (strRet.IndexOf("登录 注册-VJIA.COM") < 0)
            {
                strError = "加载登录页面失败";
                return false;
            }
            string strCheckCode = "";
            strRet = strRet.ToLower();

            /*
             <p id='pValidate' style="display:none"><label>验证码：</label><input maxlength="6" style="width:60px;"  id="calculatevalidate" name="calcultatevalidate" type="text" class="inputtextcolor" value=""  />
                <img id='img_validate' oldsrc='/Controls/CalculateValidateCode.ashx?key=Login' style="vertical-align:middle" /><a href="javascript:void(0)">换一题</a>
                </p>
             */

  //          var errortimes = 'true' == 'true';
            //var usererrortimesshow = 'true' == 'true';

            string strFlag1 = "var errortimes = 'true' == 'true'";
            string strFlag2 = "usererrortimesshow = 'true' == 'true'";
            string strReffer = strUrl;
            if (bForceValidate || strRet.IndexOf(strFlag1) > 0 || strRet.IndexOf(strFlag2) > 0)
            {
                string strUrlCheckCode = string.Format("https://www.vjia.com/login/process/VerifyCode.ashx?t={0}", DateTime.Now.ToFileTime()); ;
                
                if (!GetCheckCode(webPost, strAcc, out strError, out strCheckCode, strUrlCheckCode, strReffer))
                {
                    strError = "获取验证码失败";
                    return false;
                }
            }

            string strData = string.Format("LoginVjia=true&UserName={0}&UserPwd={1}&validatecode={2}",
                strAcc,
                strPwd,
                strCheckCode);
            strUrl = "https://www.vjia.com/login/process/login.ashx";
            strRet = webPost.GetHttpPage(strUrl,
                "utf-8", "post", "", strData,
                strReffer,
                "*/*", "");
            if (strRet != "")
            {
                //{"ErrorType":1,"Error":"登录信息和密码不匹配","ShowValidate":0}
                if (strRet.IndexOf("Error\":\"") > 0)
                {
                    strError = GetKeyString(strRet, "Error\":\"", "\"");
                    return false;
                }
            }
            strUrl = string.Format("http://www.vjia.com/Usercenter/GetUserName.ashx?rand=0.{{{0}}}", DateTime.Now.ToFileTime());
            strRet = webPost.GetHttpPage(strUrl, "utf-8", "get", "", "", "", "*/*", "");
            string strFlag = "var data='" + strAcc.Substring(0, 5);
            if (strRet.IndexOf(strFlag) >= 0)
                return true;
            strError = "提交登录信息出错";
            //获取页面成功
            return false;
        }

        public bool GetCheckCode(ClassHttpPost webPost, string strAcc, out string strError, out string strCheckCode, string strUrl, string strReffer)
        {
            string strDirect = System.AppDomain.CurrentDomain.BaseDirectory + "pic";
            if (!Directory.Exists(strDirect))
            {
                Directory.CreateDirectory(strDirect);
            }
            string strFile = string.Format("{0}\\{1}_{2}.bmp", strDirect, strAcc, DateTime.Now.ToFileTime());
            bool bRet = GetImage(webPost, strFile, out strError, strUrl, strReffer);
            strCheckCode = "";
            if (bRet)
            {
                lock (m_hLockTable)
                {
                    GetCodeHandler _myInvoke = new GetCodeHandler(GetCode);
                    strCheckCode = (string)this.Invoke(_myInvoke, new object[] { strFile });                    
                }
            }
            else
            {
                strError = "获取验证码失败";
                return false;
            }
            return true;
        }

        public bool GetImage(ClassHttpPost webPost, string strFile, out string strError, string strUrl, string strReffer)
        {
            strError = "";
            try
            {
                Stream stream = webPost.GetHttpPageSteam(strUrl, "gb2312", "get", "", "", strReffer, "*/*", "");
                if (stream == null)
                {
                    strError = "获取验证码失败,网络错误";
                    return false;
                }

                List<byte> list = new List<byte>();
                while (true)
                {
                    int data = stream.ReadByte();
                    if (data == -1)
                        break;
                    list.Add((byte)data);
                }
                byte[] bytes = list.ToArray();
                if (File.Exists(strFile))
                {
                    File.Delete(strFile);
                }
                FileStream fs = new FileStream(strFile, FileMode.Create);
                BinaryWriter bw = new BinaryWriter(fs);
                bw.Write(bytes);
                bw.Close();
                fs.Close();
                return true;
            }
            catch
            {
                strError = "获取验证码失败,获取异常";
                return false;
            }
        }
    }
}
        

