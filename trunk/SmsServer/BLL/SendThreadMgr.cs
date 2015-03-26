using System;
using System.Web;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;
using SmsTaskEx = SmsServer.Model.SmsTaskA;
using SmsSpgateEx = SmsServer.Model.SmsSpgate;
using MapScriptEx=SmsServer.Model.MapScript;
using SmsServer.DataAdapter;
using System.IO;
namespace SmsServer.BLL
{
    public class SendThreadMgr
    {
        public static Thread[] SendThread = new Thread[100];                
        public static bool m_bExit = true;
        public static DateTime StartDate = DateTime.Now.AddYears(-100);
        SendThreadMgr(){}
        public static void AutoStartSendThreads()
        {
            if (SmsSetIni.IsautoRun == 1)
            {
                new SmsLog().WriteDataBaseLog(2, "发送线程自动启动","","");
                StartSendThreads();
            }
        }
        /// <summary>
        /// 启动发送线程
        /// </summary>
        /// <returns></returns>
        public static bool StartSendThreads()
        {
            new SmsLog().WriteDataBaseLog(2, string.Format("发送线程启动,线程数[{0}]", SmsSetIni.SendThreadNum), "", "");
            m_bExit = false;
            StartDate = DateTime.Now;
            for (int i = 0; i < SmsSetIni.SendThreadNum; i++)
            {
                SendThread[i] = new Thread(new ParameterizedThreadStart(SendProc));
                SendThread[i].Start(i);
            }            
            return true;
        }
        /// <summary>
        /// 停止线程
        /// </summary>
        public static void StopSendThreads()
        {
            m_bExit = true;
            new SmsLog().WriteDataBaseLog(2, "发送线程停止", "", "");
        }
        private static void KeepOnlineProc()
        {
            string strShow = "维持在线连接线程启动";
            ShowInfo(strShow);
            while (!m_bExit)
            {

            }
        }
        /// <summary>
        /// 发送线程执行函数
        /// </summary>
        /// <param name="iThreadNumber"></param>
        private static void SendProc(object iThreadNumber)
        {
            string strShow = string.Format("线程：{0}启动", iThreadNumber.ToString( ));
            ShowInfo(strShow);
            Thread.Sleep(500);
            SmsTaskEx objSmsTask = new SmsTaskEx();
            SmsSpgateEx objSpgate = new SmsSpgateEx();
            string strContent = string.Empty;
            string strParams = string.Empty;
            string strPortUrl = string.Empty;
            string strRet = string.Empty;
            MapScriptEx objMap = new MapScriptEx();
            DataAdapterClass objAdapter = new DataAdapterClass();
            Model.SendRetValue objRetValue = new Model.SendRetValue();
            SmsHistory objHist = new SmsHistory();
            DateTime oldLogTime = DateTime.Now;
            while (!m_bExit)
            {
                if ((int)iThreadNumber == 0 )
                {
                    TimeSpan tSpan = DateTime.Now - oldLogTime ;
                    int iInterval = SmsSetIni.QueryInterval;
                    if (tSpan.TotalSeconds > iInterval)
                    {
                        strPortUrl = SmsSetIni.KeepQueryUrl  + "OnlineQuery.aspx?";
                        oldLogTime = DateTime.Now;
                        strParams = "url="+HttpUtility.UrlEncode(SmsSetIni.KeepOnlineUrl) + "&" + DateTime.Now.ToString("yyyyMMddhhmmss");
                        strRet = SmsHttpSend.HttpSendMessge(ref strPortUrl, ref strParams, 30000, 0, 2);
                    }
                }
                Thread.Sleep(SmsSetIni.SendThreadInterval);
                try
                {
                    objSmsTask = QueueManage<SmsTaskEx>.GetSingleMessage(SmsSetIni.TaskQueuePath);
                    if (objSmsTask == null)
                    {
                        //Thread.Sleep(100);
                        continue;
                    }
                    //strShow = string.Format("线程：{0}从消息队列中读取到数据。 。 。", iThreadNumber.ToString());
                    //ShowInfo(strShow);
                    //拿到后，从缓存中获取通道信息，获取通道各参数后发送
                    if (!DataCache.GetCacheSpgate(objSmsTask.SpgateNo, ref objSpgate))
                    {
                        //通道不存在,保存失败历史
                        continue;
                    }
                    strContent = ConvertString(objSmsTask.MsgContent, objSpgate.EncodeType);
                    strParams = string.Format(objSpgate.SendParams, objSmsTask.PhoneList.Substring(0, objSmsTask.PhoneList.Length - 1),
                        strContent, objSmsTask.PhoneNum, objSmsTask.SubPort + objSmsTask.SubPortEx, objSmsTask.SpgateNo);
                    strPortUrl = objSpgate.PortUrl;
                    //发送
                    strRet = SmsHttpSend.HttpSendMessge(ref strPortUrl, ref strParams, objSpgate.TimeOut, objSpgate.EncodeType, objSpgate.SendType);
                    //先处理PostError[-1111]返回值
                    if (strRet.IndexOf("PostError[-1111]") == 0)
                    {
                        //POST异常，重发
                        objHist.Add(objSmsTask, 1, "", strRet);
                        continue;
                    }
                    string strPtMsgID = string.Empty;                    
                    //拿到结果后，匹配返回值                    
                    if (!DataCache.GetCacheMapScript(objSpgate.MtScriptID, ref objMap))   //如果没有匹配到映射脚本
                    {
                        //无映射脚本，默认成功
                        objHist.Add(objSmsTask, 0, strPtMsgID, "无映射脚本,默认成功");
                        continue;
                    }
                    //找到了脚本
                    if (!objAdapter.AdapterSendResult(objMap.ScriptCode, strRet, ref objRetValue))
                    {
                        //映射异常，算成功
                        objHist.Add(objSmsTask, 0, strPtMsgID, objRetValue.RetInfo);
                        continue;
                    }
                    if (objRetValue.RetValue == 0) //发送成功了
                    {
                        objHist.Add(objSmsTask, objRetValue.RetValue, strPtMsgID, objRetValue.RetInfo);
                    }
                    else
                    {
                        if (objRetValue.IsNeedResend != 1)  //需要重发
                        {
                            //处理重发
                            //objHist.Add(objSmsTask, objRetValue.RetValue, strPtMsgID, objRetValue.RetInfo);
                        }
                        else
                        {
                            //保存失败历史
                            //objHist.Add(objSmsTask, objRetValue.RetValue, strPtMsgID, objRetValue.RetInfo);
                        }
                        objHist.Add(objSmsTask, objRetValue.RetValue, strPtMsgID, objRetValue.RetInfo);
                    }
                    //根据匹配到的返回值决定是否需要重发此数据,如果不重发,则保存至历史表中
                    //保存发送历史
                    ShowInfo(strRet);
                }
                catch (Exception e)
                {
                    ShowInfo(e.Message);
                    Thread.Sleep(2000);
                }
            }
            strShow = string.Format("线程：{0}退出", iThreadNumber.ToString());
            ShowInfo(strShow);
        }
        /// <summary>
        /// 显示调试信息
        /// </summary>
        /// <param name="strInfo"></param>
        public static void ShowInfo(string strInfo)
        {
            Debug.Listeners.Add(new TextWriterTraceListener(Console.Out));
            Debug.AutoFlush = true;
            Debug.WriteLine(strInfo);            
        }
        private static string ConvertString(string strSrc,int iType)
        {
            return HttpUtility.UrlEncode(strSrc, iType == 1 ? Encoding.GetEncoding("gb2312") : Encoding.UTF8);
        }
        public static void WriteLog(string str)
        {
            StreamWriter sw = null;
            string LogingPath = "";
            string filename = null;
            try
            {
                LogingPath = @"E:\ASP.NET\SmsServer\Web\Log";//System.Configuration.ConfigurationSettings.AppSettings["LogingPath"].ToString();
                filename = LogingPath + "\\" + System.DateTime.Now.ToShortDateString() + ".log";
                sw = new StreamWriter(filename, true);
                sw.Write(DateTime.Now.ToString() + "\t");
                sw.WriteLine(str);
                sw.Close();
            }
            catch(Exception e)
            {
                //throw new Exception(e.Message);
                return;
            }
        }
    }
}