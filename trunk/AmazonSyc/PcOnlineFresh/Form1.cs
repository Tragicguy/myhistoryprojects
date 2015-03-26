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


namespace PcOnlineFresh
{
    public partial class Form1 : Form
    {
        public delegate string GetCodeHandler(string strPath);
        public delegate void ShowEventHandler(_GoldPrice goodInfo);
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
        int m_iGetPage;
        int m_iRefreshInterval;
        int m_iFreshType;
        int m_iThreadCount;
        DateTime m_dLastFreshTime;
        int m_iHaveFreshedTime;//当天已经刷新的次数
        List<_GoldPrice> m_goldPriceList = new List<_GoldPrice>();
        List<string> m_list_Good = new List<string>();
        List<string> m_list_GoodEx = new List<string>();
        bool m_bSuspendFresh;
        Hashtable m_hLockTable = new Hashtable();
        void SaveParams()
        {
            return;
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\set.ini";
            IniUtility.WriteIniKey("set", "freshtime", m_iGetPage.ToString());
            IniUtility.WriteIniKey("set", "freshinterval", m_iRefreshInterval.ToString());
            IniUtility.WriteIniKey("set", "freshstart", m_strStartTime);
            IniUtility.WriteIniKey("set", "freshend", m_strEndTime);
            IniUtility.WriteIniKey("set", "freshtype", m_iFreshType.ToString());
            IniUtility.WriteIniKey("set", "showlog", checkShowLog.Checked==true?"1":"0");
            IniUtility.WriteIniKey("set", "threadnum", m_iThreadCount.ToString());
        }

        
        string m_strRef = "";
        string m_strdocId = "";
        string m_strgpid = "";
        string m_strTempUrl = "";
        void LoadParams()
        {
            IniUtility.FilePath = System.AppDomain.CurrentDomain.BaseDirectory + "\\set.ini";

            m_strRef = IniUtility.GetIniKeyValue("set", "ref", "amb_link_31233252_2");
            m_strdocId = IniUtility.GetIniKeyValue("set", "docid", "146308");
            m_strgpid = IniUtility.GetIniKeyValue("set", "gpid", "1");
            m_strTempUrl = IniUtility.GetIniKeyValue("set", "url", "");
            string strCheck = "1";
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
            m_strStartTime = textFreshStart.Text;
            m_strEndTime = textFreshEnd.Text;

            try
            {
                m_iGetPage = int.Parse(textFreshTime.Text);
                m_iThreadCount = int.Parse(textThreadNum.Text);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("刷新间隔或者刷新次数或线程数设置非法");
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
                DataContrl.InitFreshTime(m_iGetPage);
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
                }
            }
            m_bIsInFreshduration = true;
            if (m_iHaveFreshedTime >= m_iGetPage)
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
            //listView.Columns.Add("序号",40);
            listView.Columns.Add("单价", 50);
            listView.Columns.Add("克重", 60);
            listView.Columns.Add("总价", 90);
            listView.Columns.Add("标题", 250);
            listView.Columns.Add("URL", 300);
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
            li.SubItems.Add(strPwd);
            li.SubItems.Add("0/0/0");
            li.SubItems.Add("0/0");
            li.SubItems.Add("0/0");
            li.SubItems.Add("等待刷新");
            li.SubItems.Add("等待修改");
            li.SubItems.Add("未同步");
            li.SubItems.Add("未知/未知");
            listView.Items.Add(li);
        }

        private void ModifyViewItem(_GoldPrice goodInfo)
        {
            ListViewItem li = new ListViewItem();
            li.SubItems.Clear();
          //  li.SubItems[0].Text = listView.Items.Count.ToString();
            li.SubItems[0].Text = goodInfo.perPrice.ToString();
           // li.SubItems.Add(goodInfo.perPrice.ToString());
            li.SubItems.Add(goodInfo.weight.ToString());
            li.SubItems.Add(goodInfo.totalPrice.ToString());
            li.SubItems.Add(goodInfo.strName);
            li.SubItems.Add(goodInfo.strUrl);
            if (goodInfo.perPrice < 340)
                li.BackColor = Color.Green;
            else if (goodInfo.perPrice < 350)
                li.BackColor = Color.Pink;
            else if (goodInfo.perPrice > 360)
                li.BackColor = Color.Red;
            else
                li.BackColor = Color.Yellow;
            listView.Items.Add(li);
        }

        private void btnStartFresh_Click(object sender, EventArgs e)
        {
            IsSetIllegal();
            SaveParams();
           
            m_iThreadStopCount = 0;
           
            //启动10个线程
            m_bFresh = true;
            m_bIsStartFresh = true;
            m_list_Good.Clear();
            m_goldPriceList.Clear();
            m_bMainPageFinished = false;
            m_nExitThreadCount = 0;
            m_nPage = 0;
            listView.Items.Clear();
            listBoxInfo.Items.Clear();
            m_iFreshType = 1;
            if (radioFreshEx.Checked)
            {
                m_iFreshType = 2;
            }
            ShowLeftPageInfo(string.Format("剩余大页面数：{0}", 0));
            ShowLeftPageInfoEx(string.Format("剩余子页面数：{0}", 0));
            for (int i = 0; i < m_iThreadCount;i++ )
            {
                ThreadStart myThreadStart = new ThreadStart(FreshProc);
                triggerThread[i] = new Thread(myThreadStart);
                triggerThread[i].Start();
            }
            btnStartFresh.Enabled = false;
            btnStopFresh.Enabled = true;
            ShowLogString("线程启动");
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
        ClassHttpPost gl_webPost = new ClassHttpPost();
        int m_nPage = 1;
        public int GetNextPage()
        {
            int nPage = 0;
            lock (gl_webPost)
            {
                nPage = m_nPage;
                if(m_nPage < 10000)
                    m_nPage++;                
            }
            return nPage;
        }
        void AddToListEx(string strUrl)
        {
            lock (m_list_GoodEx)
            {
                m_list_GoodEx.Add(strUrl);
                ShowLeftPageInfoEx(string.Format("剩余子页面数：{0}", m_list_GoodEx.Count));
            }
        }

        void AddToList(string strUrl)
        {
            lock (m_list_Good)
            {
                m_list_Good.Add(strUrl);
                ShowLeftPageInfo(string.Format("剩余大页面数：{0}", m_list_Good.Count));
            }
        }

        public string GetNextUrl()
        {
            string strRet = "";
            lock (m_list_Good)
            {
                if (m_list_Good.Count > 0)
                {
                    strRet = m_list_Good[0];
                    m_list_Good.RemoveAt(0);
                    ShowLeftPageInfo(string.Format("剩余大页面数：{0}", m_list_Good.Count));
                }
            }
            return strRet;
        }

        public string GetNextUrlEx()
        {
            string strRet = "";
            lock (m_list_GoodEx)
            {
                if (m_list_GoodEx.Count > 0)
                {
                    strRet = m_list_GoodEx[0];
                    m_list_GoodEx.RemoveAt(0);
                    ShowLeftPageInfoEx(string.Format("剩余子页面数：{0}", m_list_GoodEx.Count));
                }
            }
            return strRet;
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

        public bool IsNumber(char c)
        {
            string strFlag = "0123456789";
            return strFlag.IndexOf(c) >= 0;
        }
        public float GetWeight(string strName)
        {
            strName = strName.Replace("）", ")");
            strName = strName.Replace("克", "g");
            int nPos = strName.LastIndexOf('.');
            if (nPos < 0)
            {
                return -10.0f;
            }
            string strPrice = "";
            for (int n = nPos-1; n >= 0; n--)
            {
                if (!IsNumber(strName[n]))
                {
                    strPrice = strName.Substring(n+1, nPos - n -1);
                    break;
                }
            }
            for (int n = nPos+1; n < strName.Length; n++)
            {
                if (!IsNumber(strName[n]))
                {
                    strPrice += strName.Substring(nPos, n - nPos);
                }
            }
            float price = -10.0f;
            float.TryParse(strPrice, out price);
            return price;
        }
        public bool GetPageData(int nPage)
        {
            ClassHttpPost webPost = new ClassHttpPost();
            string strUrl = string.Format("http://www.amazon.cn/gp/feature.html/ref={0}?ie=UTF8&docId={1}&plgroup={2}&plpage={3}",
                                m_strRef, m_strdocId, m_strgpid, nPage);

            if (m_iFreshType == 1)
            {
                if (m_strTempUrl == "")
                    m_strTempUrl = "http://www.amazon.cn/s/ref=sr_nr_p_n_feature_eight_br_mrr_0?rh=n%3A816482051%2Cp_n_feature_eight_browse-bin%3A2051095051%2Cp_n_feature_eight_browse-bin%3A2051096051%2Cp_n_feature_eight_browse-bin%3A2051097051%2Cp_n_feature_eight_browse-bin%3A2051098051&sort=-pct-off&ie=UTF8&page={0}";
                strUrl = string.Format(m_strTempUrl,
                                nPage);
            }
            string strRet = webPost.GetHttpPage(strUrl,
                "utf-8", "get", "", "",
                "",
                "*/*", "");

            strRet = AnanyzePageData(strRet);

            
            return true;
        }

        private string AnanyzePageDataEx(string strRet)
        {
            string strInfo = "";
            string strFindFlag = "查看准确送达时间。";
            while (true)
            {
                //<a target="_blank" href="http://www.amazon.cn/%E6%82%A6%E4%BA%BA-%E5%A4%A9%E7%84%B6A%E8%B4%A7%E7%BF%A1%E7%BF%A0%E9%95%B6%E8%B6%B3%E9%87%91%E5%87%A4%E5%90%8A%E5%9D%A0-%E9%BE%99%E5%87%A4%E5%91%88%E7%A5%A5/dp/B002LVTJOO/ref=sr_1_25_ddm?s=jewelry&ie=UTF8&qid=1347086104&sr=1-25">
                int nIndex = strRet.IndexOf(strFindFlag);
                if (nIndex < 0)
                    break;
                string strBefore = strRet.Substring(0, nIndex);
                int nPos = strBefore.LastIndexOf("<a ");
                strBefore = strBefore.Substring(nPos);

                string strDetailUrl = GetKeyString(strBefore, "href=\"", "\"");
                AddToList(strDetailUrl);
                strRet = strRet.Substring(nIndex + strFindFlag.Length);
            }
            return strRet;
        }


        private string AnanyzePageData(string strRet)
        {
            if (m_iFreshType == 1)
            {
                return AnanyzePageDataEx(strRet);
            }
            string strInfo = "";
            string strFindFlag = "价格:";
            while (true)
            {
                int nIndex = strRet.IndexOf(strFindFlag);
                if (nIndex < 0)
                    break;
                string strBefore = strRet.Substring(0, nIndex);
                int nPos = strBefore.LastIndexOf("<a ");
                strBefore = strBefore.Substring(nPos);
                string strAfter = strRet.Substring(nIndex);

                string strDetailUrl = "http://www.amazon.cn" + GetKeyString(strBefore, "href=\"", "\"");
                string strName = GetKeyString(strBefore, ">", "</a>");

                string strPrice = GetKeyString(strAfter, "price\">", "</b>");
                if (strPrice.IndexOf("-") < 0)
                {
                    strPrice = strPrice.Replace("￥", "");
                    strPrice = strPrice.Replace(",", "");
                    strPrice = strPrice.Trim();
                    float price = -10.0f;
                    float.TryParse(strPrice, out price);
                    float weight = GetWeight(strName);
                    if (weight > 0 && price > 0)
                    {
                        _GoldPrice gold = new _GoldPrice();
                        gold.weight = weight;
                        gold.totalPrice = price;
                        gold.perPrice = price / weight;
                        gold.strName = strName;
                        gold.strUrl = strDetailUrl;
                        ShowInfo(gold);

                    }
                    else
                    {
                        AddToList(strDetailUrl);
                    }
                }
                else
                {
                    AddToList(strDetailUrl);
                }
                strRet = strRet.Substring(nIndex + strFindFlag.Length);
            }
            return strRet;
        }

        public void GetDetailData(string strUrl)
        {
            ShowLogString(strUrl);
            ClassHttpPost webPost = new ClassHttpPost();
            string strRet = webPost.GetHttpPage(strUrl,
                "utf-8", "get", "", "",
                "",
                "*/*", "");
            strRet = strRet.ToLower();
            strRet.Replace("id=asinredirect", "id=\"asinredirect\"");
            if (strRet.IndexOf("id=\"asinredirect\"") < 0)
            {
                AnanyzeData(strRet, strUrl);
                return;
            }
                
            string strSelect = GetKeyString(strRet, "id=\"asinredirect\"", "</select>");
            while (true)
            {
                int nPos = strSelect.IndexOf("<option");
                if (nPos < 0)
                    break;
                string strId = "";
                if(strSelect.IndexOf("=\"") > 0)
                    strId = GetKeyString(strSelect, "value=\"", "\"");
                else
                    strId = GetKeyString(strSelect, "value", ">");

                if (strId.IndexOf("-") > 0)
                {
                    int iii = 0;
                }

                if (strId != "" && strId.IndexOf("-") < 0)
                {
                    AddToListEx("http://www.amazon.cn/gp/product/" + strId);
                }
                else
                    break;
                nPos = strSelect.IndexOf(">", nPos);
                if (nPos < 0)
                    break;
                strSelect = strSelect.Substring(nPos);
            }
        }

        public void AnanyzeData(string strRet, string strUrl)
        {
            strRet = strRet.ToLower();
            int nPos = strRet.IndexOf("id=\"btasintitle\"");
            if(nPos < 0)
                return;
            strRet = strRet.Substring(nPos);
            string strName = GetKeyString(strRet, ">", "</span>");
            nPos = strRet.IndexOf("id=\"actualpricevalue\"");
            if (nPos < 0)
                return;
            strRet = strRet.Substring(nPos);
            string strPrice = GetKeyString(strRet, "￥", "<");
            strPrice = strPrice.Replace(",", "");
            strPrice = strPrice.Trim();
            float weight = GetWeight(strName);
            float price = 0f;
            float.TryParse(strPrice, out price);
            if (price > 0 && weight > 0)
            {
                _GoldPrice gold = new _GoldPrice();
                gold.perPrice = price / weight;
                gold.totalPrice = price;
                gold.weight = weight;
                gold.strName = strName;
                gold.strUrl = strUrl;
                m_goldPriceList.Add(gold);
                ShowInfo(gold);
            }
        }
        public void GetDetailDataEx(string strUrl)
        {
            ShowLogString(strUrl);
            ClassHttpPost webPost = new ClassHttpPost();
            string strRet = webPost.GetHttpPage(strUrl,
                "utf-8", "get", "", "",
                "",
                "*/*", "");
            AnanyzeData(strRet, strUrl);
            
        }

        bool m_bMainPageFinished = false;
        int m_nExitThreadCount = 0;
        int m_nGetMainPageCount = 0;
        public void Fresh()
        {
            _GoodsFreshStr goodInfo = new _GoodsFreshStr();
            GoodsFresh objFresh = new GoodsFresh();
            DateTime tOldShowTime = DateTime.Now.AddHours(-1);
            while (m_bFresh)
            {
                //get the page no
                int nPage = GetNextPage();
                if (nPage != m_iGetPage)
                {
                    if (m_bMainPageFinished)
                    {
                        string strUrl = GetNextUrl();
                        if (strUrl != "")
                        {
                            m_nGetMainPageCount++;
                            GetDetailData(strUrl);
                            m_nGetMainPageCount--;
                        }
                        else
                        {

                            string strUrlEx = GetNextUrlEx();
                            if (strUrlEx != "")
                            {
                                GetDetailDataEx(strUrlEx);
                            }
                            else
                            {
                                if (m_nGetMainPageCount == 0)
                                    break;
                            }
                        }
                    }
                }
                else
                {
                    m_bMainPageFinished = false;
                    ShowLogString(string.Format("开始获取第 {0} 页数据", m_iGetPage));
                    GetPageData(nPage);
                    m_bMainPageFinished = true;
                    ShowLogString(string.Format("第 {0} 页数据获取完成", m_iGetPage));
                }
                Thread.Sleep(1000);
            }
            ShowLogString(string.Format("线程退出"));
            m_nExitThreadCount++;
            if (m_nExitThreadCount == m_iThreadCount)
            {
                ShowLogString(string.Format("所有线程退出"));
                ResetButton("");
                
            }
        }

        public void ResetButton(string strInfo)
        {
            try
            {
                if (btnStartFresh.InvokeRequired)
                {
                    ShowEventLog _myInvoke = new ShowEventLog(ResetButtonEx);
                    this.Invoke(_myInvoke, new object[] { strInfo });
                }
                else
                {
                    ResetButtonEx(strInfo);
                }
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }

        public void ResetButtonEx(string strInfo)
        {
            try
            {
                btnStartFresh.Enabled = true;
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }

        private void WriteLogEx(string strInfo)
        {
            lock (m_list_Good)
            {
                ShowLogString(strInfo);
                StreamWriter fs = File.AppendText("c:\\test1.html");
                fs.Write(strInfo + "\r\n");
                fs.Close();
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

        private void AddToList(_GoldPrice gold)
        {
            WriteLogEx(string.Format("{0} {1} {2} {3} {4}", gold.perPrice, gold.weight, gold.totalPrice, gold.strName, gold.strUrl));
            ModifyViewItem(gold);
        }

        public void ShowLeftPageInfoEx(string strInfo)
        {
            try
            {
                if (label_left_page_ex.InvokeRequired)
                {
                    ShowEventLog _myInvoke = new ShowEventLog(AddPageNoticeEx);
                    this.Invoke(_myInvoke, new object[] { strInfo });
                }
                else
                {
                    AddPageNoticeEx(strInfo);
                }
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }

        public void AddPageNoticeEx(string strInfo)
        {
            label_left_page_ex.Text = strInfo;
        }

        public void ShowLeftPageInfo(string strInfo)
        {
            try
            {
                if (label_left_page.InvokeRequired)
                {
                    ShowEventLog _myInvoke = new ShowEventLog(AddPageNotice);
                    this.Invoke(_myInvoke, new object[] { strInfo });
                }
                else
                {
                    AddPageNotice(strInfo);
                }
            }
            catch (Exception e)
            {
                string strError = e.Message;
            }
        }

        public void AddPageNotice(string strInfo)
        {
            label_left_page.Text = strInfo;
        }

        public void ShowInfo(_GoldPrice goodInfo)
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

        private void btnImportAcc_Click(object sender, EventArgs e)
        {
            
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
            StreamReader sr = new StreamReader(fs, System.Text.Encoding.UTF8);            
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
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
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
            ShowLogString("停止刷新");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            LoadParams();
            ShowLogString("程序启动");
            ShowLeftPageInfo("");
            ShowLeftPageInfoEx("");
            radioFresh.Checked = true;
            radioFreshEx.Checked = false;
            btnStopFresh.Enabled = false;
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

        //sohu reg
        private void button2_Click(object sender, EventArgs e)
        {
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

        private void listView_DoubleClick(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("iexplore.exe", listView.SelectedItems[0].SubItems[4].Text);

        }

        private void button3_Click(object sender, EventArgs e)
        {
            
            listView.Sort();
        }
        private int CurrentColumn = -1; //记录上次点击的列号
        private bool b_Convert = false; //记录正逆序信息

        private void listView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            if (e.Column == CurrentColumn)
            {
                b_Convert = !b_Convert;
                if (e.Column == 3 || e.Column == 1)
                    this.listView.ListViewItemSorter = new SortBySubItem(e.Column, b_Convert, 1);//数值排序
                else
                    this.listView.ListViewItemSorter = new SortBySubItem(e.Column, b_Convert, 1);//数值排序
            }
            else
            {
                CurrentColumn = e.Column;
                b_Convert = false;
                if (e.Column == 3 || e.Column == 1)
                    this.listView.ListViewItemSorter = new SortBySubItem(e.Column, b_Convert, 1);
                else
                    this.listView.ListViewItemSorter = new SortBySubItem(e.Column, b_Convert, 1);//数值排序

            }
            listView.Sort();
        }

        private void radioFresh_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioFresh_MouseClick(object sender, MouseEventArgs e)
        {

        }


        /*
         private void button2_Click(object sender, EventArgs e)
        {
            //GetImageEx
            GoodsFresh objFreshEx = new GoodsFresh();
            ClassHttpPost webPost = new ClassHttpPost();
            objFreshEx.SetWebPost(webPost);
            string strRet = webPost.GetHttpPage("http://passport.sohu.com/web/dispatchAction.action?registerType=Passport&showAllType=1",
                    "gb2312", "get","", "",
                    "",
                    "*//*", "");
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
                    "*//*", "");

        }
         */
    }
}