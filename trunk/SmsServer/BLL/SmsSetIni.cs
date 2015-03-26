using System;
using System.Collections.Generic;
using System.Text;

namespace SmsServer.BLL
{
    public class SmsSetIni
    {
        private static int _isautorun;
        private static int _sendthreadnum;
        private static int _sendthreadinterval;
        private static int _readqueuetimeout;
        private static string _taskqueuepath;
        private static int _queuemaxlen;

        private static int _queryinterval;
        private static string _keepqueryurl;
        private static string _keeponlineurl;
        private static int _historythreadnum;
        private static int _readmothreadnum;
        /// <summary>
        /// 是否在网站启动的时候自动启动发送线程
        /// </summary>
        public static int IsautoRun
        {
            set
            {
                _isautorun = value;
                IniUtility.WriteIniKey("SendThread", "IsautoRun", value.ToString());//写入配置文件
            }
            get { return _isautorun; }
        }
        /// <summary>
        /// 发送线程的数量
        /// </summary>
        public static int SendThreadNum
        {
            set
            {
                _sendthreadnum = value;
                IniUtility.WriteIniKey("SendThread", "Num", value.ToString());//写入配置文件
            }
            get { return _sendthreadnum; }
        }
        /// <summary>
        /// 发送线程的执行间隔[MS]
        /// </summary>
        public static int SendThreadInterval
        {
            set
            {
                _sendthreadinterval = value;
                IniUtility.WriteIniKey("SendThread", "SendThreadInterval", value.ToString());//写入配置文件
            }
            get { return _sendthreadinterval; }
        }
        /// <summary>
        /// 从消息队列中读取消息时的超时设置[S]
        /// </summary>
        public static int ReadQueueTimeout
        {
            set
            {
                _readqueuetimeout = value;
                IniUtility.WriteIniKey("SendThread", "ReadQueueTimeout", value.ToString());
            }
            get { return _readqueuetimeout; }
        }
        /// <summary>
        /// 等待发送消息队列的路径
        /// </summary>
        public static string TaskQueuePath
        {
            set
            {
                _taskqueuepath = value;
                IniUtility.WriteIniKey("MsgQueue", "TaskQueuePath", value.ToString());
            }
            get 
            { 
                if (_taskqueuepath == "")
                {
                    _taskqueuepath = IniUtility.GetIniKeyValue("MsgQueue", "TaskQueuePath");
                }
                return _taskqueuepath; 
            }
        }
        /// <summary>
        /// 维持连接的URL
        /// </summary>
        public static string KeepOnlineUrl
        {
            set
            {
                _keeponlineurl = value;
                IniUtility.WriteIniKey("SendThread", "KeepOnlineUrl", value.ToString());
            }
            get
            {
                if (_keeponlineurl == "")
                {
                    _keeponlineurl = IniUtility.GetIniKeyValue("SendThread", "KeepOnlineUrl");
                }
                return _keeponlineurl;
            }
        }
        /// <summary>
        /// 请求激活的链接
        /// </summary>
        public static string KeepQueryUrl
        {
            set
            {
                _keepqueryurl = value;
                IniUtility.WriteIniKey("SendThread", "KeepQueryUrl", value.ToString());
            }
            get
            {
                if (_keepqueryurl == "")
                {
                    _keepqueryurl = IniUtility.GetIniKeyValue("SendThread", "KeepQueryUrl");
                }
                return _keepqueryurl;
            }
        }
        /// <summary>
        /// 请求激活间隔
        /// </summary>
        public static int QueryInterval
        {
            set
            {
                _queryinterval = value;
                IniUtility.WriteIniKey("SendThread", "QueryInterval", value.ToString());
            }
            get
            {
                if (_queryinterval < 10)
                {
                    _queryinterval = 10;
                    IniUtility.WriteIniKey("SendThread", "QueryInterval", _queryinterval.ToString());
                }
                return _queryinterval;
            }
        }
        
        /// <summary>
        /// 存放进缓冲消息队列的阈值，超过这个值则将信息存入数据库
        /// </summary>
        public static int QueueMaxlen
        {
            set
            {
                _queuemaxlen = value;
                IniUtility.WriteIniKey("MsgQueue", "QueueMaxlen", value.ToString());
            }
            get { return _queuemaxlen; }
        }
        /// <summary>
        /// 初始化运行参数信息
        /// </summary>
        public static void InitParams(string strPath)
        {
            IniUtility.FilePath = strPath;
            _sendthreadnum = IniUtility.GetIniKeyIntValue("SendThread", "Num");
            _isautorun = IniUtility.GetIniKeyIntValue("SendThread", "IsautoRun");
            _readqueuetimeout = IniUtility.GetIniKeyIntValue("SendThread", "ReadQueueTimeout");
            _taskqueuepath = IniUtility.GetIniKeyValue("MsgQueue", "TaskQueuePath");
            _queuemaxlen = IniUtility.GetIniKeyIntValue("MsgQueue", "QueueMaxlen");
            _sendthreadinterval = IniUtility.GetIniKeyIntValue("SendThread", "SendThreadInterval");
            _keeponlineurl = IniUtility.GetIniKeyValue("SendThread", "KeepOnlineUrl");
            _keepqueryurl = IniUtility.GetIniKeyValue("SendThread", "KeepQueryUrl");
            _queryinterval = IniUtility.GetIniKeyIntValue("SendThread", "QueryInterval");
            if (_sendthreadnum < 10 )
            {
                new SmsLog().WriteDataBaseLog(2, "读取配置文件失败" + strPath, "", "");
            }
        }
    }
}
