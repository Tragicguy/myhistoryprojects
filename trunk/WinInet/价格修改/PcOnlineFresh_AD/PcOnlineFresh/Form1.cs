//#define __PAOPAO
//#define TRY_AD
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

        bool m_bSuspendFresh;

        string m_strCheckCode = "";
        string m_strMsgSend = "";
        Hashtable m_hLockTable = new Hashtable();
        void SaveParams()
        {
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\set.ini";
            IniUtility.WriteIniKey("set", "freshtime", m_iRefreshTime.ToString());
            IniUtility.WriteIniKey("set", "freshinterval", m_iRefreshInterval.ToString());
            IniUtility.WriteIniKey("set", "freshstart", m_strStartTime);
            IniUtility.WriteIniKey("set", "freshend", m_strEndTime);
            IniUtility.WriteIniKey("set", "freshtype", m_iFreshType.ToString());
            IniUtility.WriteIniKey("set", "showlog", checkShowLog.Checked==true?"1":"0");
            IniUtility.WriteIniKey("set", "threadnum", m_iThreadCount.ToString());
        }

        void LoadParams()
        {
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\set.ini";

            textFreshTime.Text = IniUtility.GetIniKeyValue("set", "freshtime", "5");
            textRreshInterval.Text = IniUtility.GetIniKeyValue("set", "freshinterval", "2");
            textFreshStart.Text = IniUtility.GetIniKeyValue("set", "freshstart", "");
            textFreshEnd.Text = IniUtility.GetIniKeyValue("set", "freshend", "");
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
            this.Text = "太平洋留言机";
        }

        bool IsSetIllegal()
        {
            m_strStartTime = textFreshStart.Text;
            m_strEndTime = textFreshEnd.Text;

            try
            {
                m_iRefreshTime = int.Parse(textFreshTime.Text);
                m_iRefreshInterval = int.Parse(textRreshInterval.Text);
                m_iThreadCount = int.Parse(textThreadNum.Text);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("刷新间隔或者刷新次数或线程数设置非法");
                return true;
            }
            
            if (m_iRefreshTime < 1)
            {
                MessageBox.Show("刷新次数不能小于1");
                return true;
            }
            if (m_iRefreshInterval < 1)
            {
                MessageBox.Show("刷新间隔不能小于1小时");
                return true;
            }

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

            DateTime dtStart;
            DateTime dtEnd;
            try
            {
                string strTime = string.Format("2001-01-01 {0}:0", m_strStartTime);
                dtStart = DateTime.Parse(strTime);
            }
            catch 
            {
                MessageBox.Show("开始时间设置非法");
                return true;
            }
            try
            {
                dtEnd = DateTime.Parse(string.Format("2001-01-01 {0}:0", m_strEndTime));
            }
            catch
            {
                MessageBox.Show("结束时间设置非法");
                return true;
            }
            TimeSpan tSpan = dtEnd - dtStart;
            if (tSpan.TotalHours < 2)
            {
                MessageBox.Show("开始时间与结束时间的时间差不应该小于2小时");
                return true;
            }
            m_iFreshType = radioFresh.Checked == true ? 0 : 1;
            this.Text = "太平洋留言机";
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
                        ShowInfo(goodInfo);
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
            listView.Width = 815;
            listView.Height = this.ClientRectangle.Height - 80;
            listBoxInfo.Left = 10;
            listBoxInfo.Width = 815;
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
            listView.Columns.Add("店址", 400);
            listView.Columns.Add("状态", 200);
            ImageList imgList = new ImageList();
            imgList.ImageSize = new Size(1, 20);//分别是宽和高
            listView.SmallImageList = imgList;

        }

        private void AddViewItem(string strAcc,string strPwd)
        {
            ListViewItem li = new ListViewItem(); 
            li.SubItems.Clear();
            li.SubItems[0].Text = listView.Items.Count.ToString();
            li.SubItems.Add(strAcc);
            li.SubItems.Add("");
            listView.Items.Add(li);
        }

        private void ModifyViewItem(_GoodsFreshStr goodInfo)
        {
            for (int i = 0; i < listView.Items.Count;i++ )
            {
                if (listView.Items[i].SubItems[1].Text.ToString() == goodInfo.strAcc)
                {
                   

                    if (goodInfo.iSynchronizeCount > 0)
                    {
                        listView.Items[i].BackColor = Color.FromArgb(100, 255, 255, 255);
                    }
                    listView.Items[i].SubItems[2].Text = goodInfo.strFreshStatus;                   
                    break;
                }                
            }
        }

        private void btnStartFresh_Click(object sender, EventArgs e)
        {            
            if (IsSetIllegal())
            {
                return;
            }
            SaveParams();
            
            m_iThreadStopCount = 0;
           // TimerFunction();
            //设置TIMER
//             m_Timer = new System.Timers.Timer(60 * 1000);
//             m_Timer.Elapsed += new System.Timers.ElapsedEventHandler(timer_Tick);
//             m_Timer.AutoReset = true;  //设置一直执行(true)
//             m_Timer.Enabled = true;    //是否执行System.Timers.Timer.Elapsed事件
//             m_Timer.Start();
         
            //启动10个线程
            m_bFresh = true;
            m_bIsStartFresh = true;
            for (int i = 0; i < m_iThreadCount;i++ )
            {
                ThreadStart myThreadStart = new ThreadStart(FreshProc);
                triggerThread[i] = new Thread(myThreadStart);
                triggerThread[i].Start();
            }
            btnStartFresh.Enabled = false;
            btnStopFresh.Enabled = true;
            ShowLogString("刷新线程启动");
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
                //同步列表
                strSynAcc = DataContrl.GetFirstSynAccount();
                if (strSynAcc != "")
                {
                    ShowLogString(string.Format("{0} 开始同步信息!", strSynAcc));
                    bRet = DataContrl.GetDataCache(strSynAcc, ref goodInfo);
                    if (!bRet)
                    {
                        Thread.Sleep(100);
                        continue;
                    }
                    objFresh.SetWebPost(goodInfo.webPost);
                    bRet = Fresh(ref goodInfo, objFresh,true);
                    if (bRet)
                    {
                        goodInfo.strSynStatus = "同步成功,需要重启";
                        ShowLogString(string.Format("{0} 同步成功,需要重启", goodInfo.strAcc));
                    }
                    else
                    {
                        //goodInfo.strSynStatus = "同步失败";
                        ShowLogString(string.Format("{0} 同步失败:{1}", goodInfo.strAcc, goodInfo.strSynStatus));
                    }
                    ShowInfo(goodInfo);
                    Thread.Sleep(1000);
                    continue;
                }
                //从修改价格列表获取,如果有,则修改
                strModifyAcc = DataContrl.GetFirstModifyAccount();
                if (strModifyAcc != "")
                {
                  //  ShowLogString(string.Format("{0} 开始修改价格!", strModifyAcc));
                    bRet = DataContrl.GetDataCache(strModifyAcc, ref goodInfo);
                    if (!bRet)
                    {
                        Thread.Sleep(100);
                        continue;
                    }
                    objFresh.SetWebPost(goodInfo.webPost);
                    bRet = Fresh(ref goodInfo, objFresh,false);
//                     if (bRet)
//                     {                        
//                         ShowLogString(string.Format("{0} 修改成功", goodInfo.strAcc));
//                     }
//                     else
//                     {
//                         goodInfo.iFreshFailCount += 1;
//                         ShowLogString(string.Format("{0} 修改失败:{1}", goodInfo.strAcc, goodInfo.strFreshStatus));
//                     }
                    Thread.Sleep(1000);
                    continue;                   
                }

#if __PAOPAO
                Thread.Sleep(500);
                continue;
#endif
                m_bSuspendFresh = true;
                if (m_bSuspendFresh)
                {
                    TimeSpan tSpan = DateTime.Now - tOldShowTime;
                    if (tSpan.TotalMinutes > 2)
                    {
                        ShowLogString("刷新暂停中...");
                        tOldShowTime = DateTime.Now;
                    }
                    Thread.Sleep(500);
                    continue; 
                }
                //从刷新列表获取,有则刷新,无则继续循环
                strFreshAcc = DataContrl.GetFirstAccount();
                if (strFreshAcc == "")
                {
                    Thread.Sleep(100);
                    continue;
                }
                bRet = DataContrl.GetDataCache(strFreshAcc, ref goodInfo);
                if (!bRet)
                {
                    Thread.Sleep(100);
                    continue;
                }
                objFresh.SetWebPost(goodInfo.webPost);
                if (m_iFreshType == 0) //一键刷新
                {
                    goodInfo.bModify = false;
                    bRet = FreshQuick(ref goodInfo, objFresh);
                }
                else
                {
                    bRet = Fresh(ref goodInfo, objFresh,false);
                }

                if (bRet)
                {
                    //goodInfo.strFreshStatus = "刷新成功";
                    goodInfo.iFreshSucCount += 1;
                    ShowLogString(string.Format("{0} {1}", goodInfo.strAcc, m_strCheckCode));
                }
                else
                {
                    goodInfo.iFreshFailCount += 1;
                    ShowLogString(string.Format("{0} 刷新失败:{1}", goodInfo.strAcc, goodInfo.strFreshStatus));
                }
                ShowInfo(goodInfo);
                Thread.Sleep(1000);
            }	
        }

        private void ShowLog(string strInfo)
        {
            //添加至日志显示列表
            listBoxInfo.Items.Add(strInfo);
            //写日志
            WriteLog(strInfo);
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

        private void AddToList(_GoodsFreshStr goodInfo)
        {
            ModifyViewItem(goodInfo);
        }        

        public void ShowInfo(_GoodsFreshStr goodInfo)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    ShowEventHandler _myInvoke = new ShowEventHandler(AddToList);
                    this.Invoke(_myInvoke, new object[] { goodInfo }); 
                }
                else
                {
                    AddToList(goodInfo);
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

        void ImportAcc(string strPath)
        {
            string strContent = ReadFileLine(strPath);
            string[] strArray = strContent.Split('\n');
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\setEx.ini";

            foreach (string strLine in strArray)
            {
                //解析帐号密码
                string[] strInfo = strLine.Split(',');
                if (strInfo.Length < 2)
                {
                    continue;
                }
                if (DataContrl.IsInCache(strInfo[0]))
                {
                    continue;
                }
                if (strInfo[0].Length < 1)
                {
                    continue;
                }
                _GoodsFreshStr GoodInfo = new _GoodsFreshStr();
                GoodInfo.strAcc = strInfo[0];
                GoodInfo.strPwd = strInfo[1];
                GoodInfo.webPost = new ClassHttpPost();
                GoodInfo.iFreshCount = 0;
                GoodInfo.iFreshSucCount = 0;
                GoodInfo.iFreshFailCount = 0;
                ShowLogString(string.Format("正在加载 {0} 的商品同步信息", GoodInfo.strAcc));
                //此处需要添加已经同步好的文本文档，以便添加修改文件时对比，得出需要添加的商品
                string strSynPath = System.AppDomain.CurrentDomain.BaseDirectory + "synfile\\" + GoodInfo.strAcc + ".txt";
                if (!File.Exists(strSynPath))
                {
                    GoodInfo.iSynchronizeCount = 0;
                    GoodInfo.strSynStatus = "未找到同步信息文件";
                    ShowLogString(GoodInfo.strSynStatus);
                }
                else
                {
                    //加载同步信息
                    string strInfoEx = ReadFileLine(strSynPath);
                    string[] strArrayEx = strInfoEx.Split('\n');
                    Hashtable hSynTable = new Hashtable();
                    Hashtable hNoList = new Hashtable();
                    foreach (string strGoodInfo in strArrayEx)
                    {
                        string[] strSynInfo = strGoodInfo.Split(',');
                        if (strSynInfo.Length < 7)
                        {
                            continue;
                        }
                        _GoodsModifyStr goodModify = new _GoodsModifyStr();
                        goodModify.strNo = strSynInfo[0];
                        goodModify.strNoEx = strSynInfo[1];
                        goodModify.strName = "";
                        float.TryParse(strSynInfo[2], out goodModify.fPrice);
                        float.TryParse(strSynInfo[3], out goodModify.fLowPrice);
                        float.TryParse(strSynInfo[4], out goodModify.fHighPrice);
                        if (strSynInfo.Length > 7)
                        {
                            for (int i = 5; i < strSynInfo.Length - 2; i++)
                            {
                                goodModify.strName += strSynInfo[i];
                            }
                        }
                        else
                        {
                            goodModify.strName = strSynInfo[5];
                        }
                        if (hNoList.ContainsKey(goodModify.strNoEx))
                        {
                            hNoList[goodModify.strNoEx] = (string)hNoList[goodModify.strNoEx] + "," + goodModify.strNo;
                        }
                        else
                        {
                            hNoList[goodModify.strNoEx] = goodModify.strNo;
                        }
                        hSynTable.Add(goodModify.strNo, goodModify);
                    }

                    foreach (DictionaryEntry dEntry in hNoList)
                    {
                        DataContrl.AddSynNumberList(GoodInfo.strAcc + (string)dEntry.Key, (string)dEntry.Value);
                    }
                    GoodInfo.iSynchronizeCount = hSynTable.Count;
                    DataContrl.AddSynList(GoodInfo.strAcc, hSynTable);
                    ShowLogString(string.Format("导入同步信息 {0} 条", hSynTable.Count));
                }
                string strFlag = IniUtility.GetIniKeyValue("setex", GoodInfo.strAcc, "0");
                
                DataContrl.AddDataCache(GoodInfo);
                AddViewItem(GoodInfo.strAcc, GoodInfo.strPwd);
                GoodInfo.bModify = true;
                //导入上次一键刷新的时间
                GoodInfo.strQuickFreshTimeLeft = "未知";
                GoodInfo.strLastFreshQuickTime = IniUtility.GetIniKeyValue("set", GoodInfo.strAcc + "_lastFreshTime", "");
                if (GoodInfo.strLastFreshQuickTime == "")
                {
                    GoodInfo.strLastFreshQuickTime = "未刷新过";
                }
                if (GoodInfo.iSynchronizeCount < 1)
                {
                    GoodInfo.strSynStatus = "尚未同步";
                }
                ShowInfo(GoodInfo);
#if TRY_AD
               break;
#endif
            }
        }
        private void btnImportAcc_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog find = new FolderBrowserDialog();
            if (find.ShowDialog() != DialogResult.OK)
            {
                return;
            }

           DataContrl.ClearModifyData();
           DirectoryInfo dInfo = new DirectoryInfo(find.SelectedPath);

           foreach (FileInfo fileInfo in dInfo.GetFiles())
           {
              
               string strName = fileInfo.Name;
               ImportAcc(find.SelectedPath+"\\"+strName);
#if TRY_AD
               break;
#endif
           }
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
            string strPath = "";
            OpenFileDialog find = new OpenFileDialog();
            find.Filter = "TXT文件(*.txt)|*.txt";

            if (find.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            strPath = find.FileName;
            string strContent = ReadFileLine(strPath);
            strContent = strContent.Replace("\n", "        ");
            m_strMsgSend = strContent;
            textAdString.Text = m_strMsgSend;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < 1000;i++ )
            {
                builder.Append(i.ToString() + "," + "1\r\n");
            }
            WriteInfo("C:\\TEST",builder,"test");
            return;
           // _GoodsFreshStr objInfoEx = new _GoodsFreshStr();
            //objInfoEx.strAcc = "aaa";
            TestStr1 bulder = new TestStr1();
            bulder.strAcc = "AAA";
            Hashtable tblHash = new Hashtable();
            bulder.strAcc = "AAABBB";

            _GoodsFreshStr objInfoExEx = new _GoodsFreshStr();

            string strDirectoryXls = System.AppDomain.CurrentDomain.BaseDirectory + "xlsfile";
            SaveToExcel(strDirectoryXls, null, "aaaaa");
            return;
            GoodsFresh objFreshEx = new GoodsFresh();
            ClassHttpPost webPost = new ClassHttpPost();
            objFreshEx.SetWebPost(webPost);

            string strDirect = System.AppDomain.CurrentDomain.BaseDirectory + "pic";
            if (!Directory.Exists(strDirect))
            {
                Directory.CreateDirectory(strDirect);
            }
            
            return;
            //string str = System.Web.HttpUtility.UrlEncode("你好", Encoding.GetEncoding("gb2312"));
            string str = string.Format("{0}    {1}", 519.0, 535.01);
            MessageBox.Show(str);
            return;
            TimerFunction();
            return;
            string strAcc = "heilongjiangjt";
            _GoodsFreshStr objInfo = new _GoodsFreshStr();
            DataContrl.GetDataCache(strAcc, ref objInfo);
            MessageBox.Show(objInfo.iFreshCount.ToString());
            objInfo.iFreshCount = 2;
            //DataContrl.InitFreshTime();
            DataContrl.GetDataCache(strAcc, ref objInfo);
            MessageBox.Show(objInfo.iFreshCount.ToString());
            return;

            IsSetIllegal(); return;
            _GoodsFreshStr goodInfo = new _GoodsFreshStr();
            bool bRet = DataContrl.GetDataCache("heilongjiangjt", ref goodInfo);
            if (!bRet)
            {
                return;
            }
            GoodsFresh objFresh = new GoodsFresh();
            objFresh.SetWebPost(goodInfo.webPost);
            bRet = objFresh.Login(goodInfo.strAcc, goodInfo.strPwd,true);
            string strError = "";
            objFresh.FreshQuick(out strError,out strError);
        }

        private bool FreshQuick(ref _GoodsFreshStr goodInfo, GoodsFresh objFresh)
        {
            string strError = "";
            if (string.IsNullOrEmpty(goodInfo.strLastFreshQuickTime))
            {
                goodInfo.strLastFreshQuickTime = "未知";
            }
            
            //得到还有的次数显示
            goodInfo.strQuickFreshTimeLeft = "未知";
            goodInfo.strFreshStatus = "正在登录...";
            ShowInfo(goodInfo);
            bool bRet = objFresh.Login(goodInfo.strAcc, goodInfo.strPwd,true);
            if (!bRet)
            {
                strError = "登录失败";
                goodInfo.strFreshStatus = strError;
                return false;
            }
            goodInfo.strFreshStatus = "正在刷新...";
            ShowInfo(goodInfo);
            string strLeft = "";
            int iRet = objFresh.FreshQuick(out strError, out strLeft);
            if (iRet != 0)
            {
                goodInfo.strFreshStatus = strError;
                if (strError == "您今天的刷新次数已用完" || strError == "没有一键刷新权限")
                {
                    goodInfo.strQuickFreshTimeLeft = "0";
                }
                ShowInfo(goodInfo);
                return false;
            }
            //写入配置文件
            goodInfo.strLastFreshQuickTime = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
            //得到还有的次数显示
            goodInfo.strQuickFreshTimeLeft = strLeft;
            ShowInfo(goodInfo);
            IniUtility.WriteIniKey("set", goodInfo.strAcc + "_lastFreshTime", goodInfo.strLastFreshQuickTime);
            return true;
        }

        
        private bool Modify(ref _GoodsFreshStr goodInfo, GoodsFresh objFresh)
        {
            goodInfo.strFreshStatus = "";
            goodInfo.bModify = true;
            string strError = "";

            Hashtable hWaitTable = new Hashtable();
            Hashtable hFailTable = new Hashtable(); //修改失败的价格列表
            bool bRet = DataContrl.GetModifyData(goodInfo.strAcc, ref hWaitTable);   //得到等待刷新的商品列表
            if (!bRet)
            {
                goodInfo.strModifyStatus = "无价格刷新列表";
                ShowInfo(goodInfo);
                ShowLogString(strError);
                return true;
            }
            int iTotalCount = hWaitTable.Count;
            goodInfo.iModifyCount = iTotalCount;
            goodInfo.strModifyStatus = "登录中...";
            ShowInfo(goodInfo);

#if __PAOPAO
            string strDirect = System.AppDomain.CurrentDomain.BaseDirectory + "pic";
            if (!Directory.Exists(strDirect))
            {
                Directory.CreateDirectory(strDirect);
            }
            string strFile = string.Format("{0}\\{1}.bmp", strDirect, goodInfo.strAcc);
            bRet = objFresh.GetImage(strFile,out strError);
            string strCheckCode = "";
            if (bRet)
            {
                lock (m_hLockTable)
                {
                    GetCodeHandler _myInvoke = new GetCodeHandler(GetCode);
                    strCheckCode = (string)this.Invoke(_myInvoke, new object[] { strFile });
                    if (strCheckCode.Length != 4)
                    {
                        goodInfo.strFreshStatus = "验证码错误";
                        ShowInfo(goodInfo);
                        return false;
                    }
                }
            }
            else
            {
                goodInfo.strFreshStatus = strError;
                ShowInfo(goodInfo);
                ShowLogString(strError);
                return false;
            }
            bRet = objFresh.PaoPaoLogin(goodInfo.strAcc, goodInfo.strPwd, strCheckCode, out strError);
#else
            bRet = objFresh.Login(goodInfo.strAcc, goodInfo.strPwd, false);
#endif
            if (!bRet)
            {
                strError = "登录失败";
                goodInfo.strModifyStatus = strError;
                ShowInfo(goodInfo);
                ShowLogString(strError);
                return false;
            }
            goodInfo.strModifyStatus = "修改中...";
            ShowInfo(goodInfo);
            int iCurPage = 1;
            int iRet = -1;
            int iModifyCount = 0;
            int iGoodCount = 0;
            bool bBreak = false;
            
            
            int iNetErrorCount = 0;
            while (m_bFresh && !bBreak)
            {

#if __PAOPAO
                //FreshPaoPao
                iRet = objFresh.FreshPaoPao(out strError, out iModifyCount, out iGoodCount, out bBreak, iCurPage + 1, true, ref hWaitTable, ref hFailTable, ref iNetErrorCount);
#else
                iRet = objFresh.Fresh(out strError, out iModifyCount, out iGoodCount, out bBreak, iCurPage + 1, true, ref hWaitTable, ref hFailTable, ref iNetErrorCount);
#endif

                goodInfo.strModifyStatus = strError;
                //成功
                if (iRet == 0)
                {
                    goodInfo.iModifyCount = iTotalCount;
                    goodInfo.iModifySucCount += iModifyCount;
                    goodInfo.strModifyStatus = "修改中...";
                    if (bBreak)
                    {
                        goodInfo.strModifyStatus = "修改完成";
                    }
                }
                else if (iRet == -3)//网络错误
                {
                    goodInfo.strModifyStatus = strError;
                    ShowLogString(string.Format("{0} {1}", iCurPage, strError));
                }
                else if (iRet == -1)//数据错误，直接退出
                {
                    ShowLogString("发生数据错误，退出");
                    ShowLogString(string.Format("{0} {1}", iCurPage, strError));
                }
                else
                {
                    goodInfo.strModifyStatus = strError;
                    ShowLogString(string.Format("{0} {1}", iCurPage, strError));
                }
                ShowInfo(goodInfo);
                if ( iRet != -3 )
                {
                    iCurPage++;
                }                
                if (bBreak)
                {                    
                    bRet = true;
                    break;
                }
                if (iRet == -1)
                {
                    bRet = false;
                    break;
                }
                Thread.Sleep(500);
            }
            //DataContrl.AddNeedAddList(goodInfo.strAcc, hFailTable);
            return bRet;
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

        private bool Fresh(ref _GoodsFreshStr goodInfo, GoodsFresh objFresh,bool bSynFlag)
        {
            //http://m.pconline.com.cn/member/product/products_onsell.htm?&pageSize=60&pageNo=1
            goodInfo.strModifyStatus = "";
            goodInfo.bModify = false;
            string strError = "";
            
            bool bRet = false;
            
            int iCurPage = 0;
            int iRet = -1;
            int iFreshCount;
            int iGoodCount;
            bool bBreak = false;

            if (bSynFlag)
            {
                goodInfo.strFreshStatus = "正在同步...";
            }
            else
            {
                goodInfo.strFreshStatus = "正在刷新...";
            }
            ShowInfo(goodInfo);

            Hashtable hWaitTable = new Hashtable();
            Hashtable hFailTable = new Hashtable(); //修改失败的价格列表
            Hashtable hSynTable = new Hashtable(); //同步列表
            Hashtable hSynDuplicateTable = new Hashtable(); //同步列表
            int iNetErrorCount = 0;
            string strContent = "";// "为了方便在太平洋商城的店主们  批量  更新商品信息,      \r\n我们特别为店主们量身定做了一款批量刷新、    \r\n同步商品信息,       \r\n批量修改商品价格信息的软件.        \r\n 以达到商家的商品一直排前, 提高客户关注度.            \r\n在商品价格需要修改时, 只需要导入一个Excel 文件,  即可实现批量修改价格.       让商家从繁复的操作中解脱出来, 从战略的角度考虑商品销售.        \r\n另可定制各种商城的同类刷新修改软件:             \r\n如: 中关村在线,  pcpop,  天极等商城.";
            //strContent = strContent + "          \r\n 详情请联系QQ: 1194410385 请注明[太平洋刷新] ";


            strContent = m_strMsgSend;
            //strContent = "test info111";
            goodInfo.iFreshNumCount = 0;
            while (m_bFresh && !bBreak)
            {
                if (bSynFlag)
                {                    
                    iRet = objFresh.SynchronizeSeller(out strError, out iFreshCount, out iGoodCount, out bBreak, iCurPage + 1, ref iNetErrorCount, ref hSynTable, ref hSynDuplicateTable,goodInfo.strAcc);
                }
                else{
                    bBreak = true;
                    goodInfo.strFreshStatus = "正在获取验证码...";
                    ShowInfo(goodInfo);
                    //获取验证码
                    string strDirect = System.AppDomain.CurrentDomain.BaseDirectory + "pic";
                    if (!Directory.Exists(strDirect))
                    {
                        Directory.CreateDirectory(strDirect);
                    }
                    m_strCheckCode = "SDFKD";
                    string strName = goodInfo.strAcc;
                    strName = strName.Replace(":", "");
                    strName = strName.Replace(".", "");
                    strName = strName.Replace("\\", "");
                    strName = strName.Replace("/", "");
                    string strFile = string.Format("{0}\\{1}.bmp", strDirect, strName);
                    string strMemo = "";
                    string strTarget = "";
                    if (m_strCheckCode == "")
                    {
                        bRet = objFresh.GetImage(goodInfo.strAcc, strFile, out strError, out strMemo, out strTarget);
                        if (!bRet)
                        {
                            goodInfo.strFreshStatus = "获取验证码失败";
                            ShowInfo(goodInfo);
                            return false;
                        }
                        lock (m_hLockTable)
                        {
                            GetCodeHandler _myInvoke = new GetCodeHandler(GetCode);
                            m_strCheckCode = (string)this.Invoke(_myInvoke, new object[] { strFile });
                            if (m_strCheckCode.Length < 1)
                            {
                                goodInfo.strFreshStatus = "验证码错误";
                                ShowInfo(goodInfo);
                                return false;
                            }
                        }
                    }
                    
                    iRet = objFresh.AddNotie(goodInfo.strAcc, strContent, m_strCheckCode, strMemo, strTarget,out strError);
                    iGoodCount = 0;
                    iFreshCount = 0;
                }
                
                //成功
                if (iRet == 0)
                {
                    goodInfo.iFreshNumCount += iGoodCount;
                    if (bSynFlag)
                    {
                        goodInfo.iFreshSucNumCount = hSynTable.Count;
                        goodInfo.iSynchronizeCount = hSynTable.Count;
                    }
                    else
                        goodInfo.iFreshSucNumCount += iFreshCount;

                    if (bSynFlag)
                    {
                        goodInfo.strFreshStatus = string.Format("正在同步:{0}", goodInfo.iFreshSucNumCount);
                    }
                    else
                    {
                        goodInfo.strFreshStatus = "发送成功";
                    }
                    if (bBreak)
                    {
                        if (bSynFlag)
                        {
                          //  goodInfo.strFreshStatus = "同步完成";
                            goodInfo.strFreshStatus = string.Format("同步完成:{0},需要重启", goodInfo.iFreshSucNumCount);
                        }
                        else
                        {
                            goodInfo.strFreshStatus = "发广告成功";
                            //将发送成功的URL地址写入新的配置文件 
                            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\setEx.ini";
                            IniUtility.WriteIniKey("setex", goodInfo.strAcc, "1");
                            goodInfo.iSynchronizeCount = 1;
                        }
                    }
                }
                else if (iRet == -1)
                {
                    goodInfo.strFreshStatus = strError;
                    goodInfo.strSynStatus = strError;
                    goodInfo.strModifyStatus = strError;
                    bRet = false;
                    ShowLogString(string.Format("{0} {1}", iCurPage, strError));
                    break;
                }
                else
                {                    
                    goodInfo.strFreshStatus = strError;
                    ShowLogString(string.Format("{0} {1}",iCurPage,strError));
                }
                ShowInfo(goodInfo);
                if (iRet != -3)
                {
                    iCurPage++;
                }                
                if (bBreak)
                {
                    if (bSynFlag)
                    {
                        //保存到文件
                        StringBuilder strBuilder = new StringBuilder();
                        _GoodsModifyStr objStru = new _GoodsModifyStr();

                        string[] keyArray = new string[hSynTable.Count];
                        string[] valueArray = new string[hSynTable.Count];
                        hSynTable.Keys.CopyTo(keyArray, 0);
                        int i=0;
                        foreach (string skey in keyArray)
                        {
                            objStru = (_GoodsModifyStr)hSynTable[skey];
                            valueArray[i] = objStru.strNoEx;
                            i++;
                        }
                        Array.Sort(valueArray, keyArray);
                        foreach (string skey in keyArray)
                        {                            
                            objStru = (_GoodsModifyStr)hSynTable[skey];
                            strBuilder.Append(string.Format("{0},{1},{2},{3},{4},{5},\r\n", objStru.strNo, objStru.strNoEx, objStru.fPrice, objStru.fLowPrice, objStru.fHighPrice, objStru.strName));
                        }
                        string strDirectory = System.AppDomain.CurrentDomain.BaseDirectory + "synfile";
                        string strDirectoryXls = System.AppDomain.CurrentDomain.BaseDirectory + "xlsfile";

                        WriteInfo(strDirectory, strBuilder, goodInfo.strAcc);
                        SaveToExcel(strDirectoryXls, hSynTable, goodInfo.strAcc);

                        if (hSynDuplicateTable.Count > 0)
                        {
                            StringBuilder strBuilderEx = new StringBuilder();

                            string[] keyArrayDup = new string[hSynDuplicateTable.Count];
                            string[] valueArrayDup = new string[hSynDuplicateTable.Count];
                            hSynTable.Keys.CopyTo(keyArrayDup, 0);
                            int ii = 0;
                            foreach (string skey in keyArrayDup)
                            {
                                objStru = (_GoodsModifyStr)hSynDuplicateTable[skey];
                                valueArrayDup[ii] = objStru.strNoEx;
                                ii++;
                            }
                            Array.Sort(valueArrayDup, keyArrayDup);
                            foreach (string skey in keyArrayDup)
                            {
                                objStru = (_GoodsModifyStr)hSynDuplicateTable[skey];
                                strBuilder.Append(string.Format("{0},{1},{2},{3},{4},{5},\r\n", objStru.strNo, objStru.strNoEx, objStru.fPrice, objStru.fLowPrice, objStru.fHighPrice, objStru.strName));
                            }
                            
                            WriteInfo(strDirectory, strBuilderEx, goodInfo.strAcc + "_duplicate");
                            SaveToExcel(strDirectoryXls, hSynDuplicateTable, goodInfo.strAcc + "_duplicate");
                        }                        
                    }
                    break;
                }
                Thread.Sleep(500);
            }            
            return true;
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
            ShowLogString("停止刷新");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            int iii = 666 / 16;
            LoadParams();
            ShowLogString("程序启动");
            btnStopFresh.Enabled = false;
#if TRY_AD
            SynchroGoodInfo.Visible = false;
#endif
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
                MessageBox.Show("请先导入店铺地址!");
                return;
            }
            if (m_strMsgSend == "")
            {
                MessageBox.Show("请先导入广告语!");
                return;
            }
            int iCount = DataContrl.GetModifyCount();
            if (iCount > 0)
            {
                MessageBox.Show("正在发送中,请稍后再执行此操作!");
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
            string strShow = string.Format("此操作将会抓取店铺信息,并导入正在进行的发送过程中断，要执行吗？");
            if (DialogResult.OK != MessageBox.Show(strShow,"询问",MessageBoxButtons.OKCancel, MessageBoxIcon.Question))
            {
                return;
            }
            DataContrl.DeleteDataCache();
            listView.Items.Clear();
            for (int i = 0; i < 27;i++ )
            {
                _GoodsFreshStr goodInfo = new _GoodsFreshStr();
                goodInfo.strAcc = i.ToString();
                goodInfo.webPost = new ClassHttpPost();
                DataContrl.ClearModifyAccount();                
                DataContrl.AddDataCache(goodInfo);               
                AddViewItem(goodInfo.strAcc, "");
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
                cmd.CommandText = string.Format("INSERT INTO [sheet1] VALUES('{0}',{1})", objStru.strNoEx, objStru.fPrice);
                cmd.ExecuteNonQuery();
            }
            cn.Close();
            cn.Dispose();
         }

        private void listView_SelectedIndexChanged(object sender, EventArgs e)
        {

        }


        //sohu reg
        private void button2_Click(object sender, EventArgs e)
        {
            //GetImageEx
            GoodsFresh objFreshEx = new GoodsFresh();
            ClassHttpPost webPost = new ClassHttpPost();
            objFreshEx.SetWebPost(webPost);
            string strRet = webPost.GetHttpPage("http://passport.sohu.com/web/dispatchAction.action?registerType=Passport&showAllType=1",
                    "gb2312", "get","", "",
                    "",
                    "*/*", "");
            string strError = "";
            objFreshEx.GetImageEx("c:\\ddddddd.bmp", out strError);
            //
            string strCode = "";
            string strNumber = "";
            strCode = "hfabe3a8";
            strNumber = "SCYLSLALELJLAPCLSLELSLAJYJPLYPJPNPULYJPLELYPNJSLJLAJSLYLAPUJUPCLAPLPJPNLYLAJSLYLAPUJUPCLSLAPLPNLALEPJLYLAPCJUPJLYLSLAJYJPLELYPCLEPNJPLEJYLEPNPJPCPJLALSPCPJLALYLEJSPULEJSPNPCPJLAPJLEJUPJLEJSPULEJPJY";
            string strData = string.Format("checkRegType=on&shortname=testregidnew2&user.password=wawawo20&password2=wawawo20&user.question=%CE%D2%BE%CD%B6%C1%B5%C4%D3%D7%B6%F9%D4%B0%C3%FB%B3%C6&questionbak=&answer=%B3%A4%BD%AD%D0%A1%D1%A7&validate={1}&privacy=1&uuidCode={0}&app_para=registerType%3DPassport%26showAllType%3D1&ot_registerid=&registerid=&appid=9998&autologin=1&ru=&domainName=sohu.com&registerType=Passport&showAllType=1",
                strNumber,
                strCode
                );
            strRet = webPost.GetHttpPage("https://passport.sohu.com/web/Passportregister.action",
                    "gb2312", "POST","", strData,
                    "https://passport.sohu.com/web/dispatchAction.action?registerType=Passport&showAllType=1",
                    "*/*", "");

        }

        private void button3_Click(object sender, EventArgs e)
        {
            //删除文件
            File.Delete(System.AppDomain.CurrentDomain.BaseDirectory + "\\setEx.ini");

        }
    }
}