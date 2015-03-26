using System;
using System.Data;
using System.Messaging;
namespace SmsServer.BLL
{
    public class QueueManage<T>
    {
        /// 
        /// 发送对象到队列中 
        /// 
        /// 队列名称，因为队列名称在一个应用中应该不改变的，所以最好写在配置文件中 
        /// 要发出去的对象 
        public static void SendQueue(string strPath, T sq)
        {
            EnsureQueueExists(strPath);
            MessageQueue mqSend = new MessageQueue(strPath, false);            
            mqSend.Send(sq);
        }

        /// 
        /// 检查队列，如果队列不存在，则建立 
        /// 
        /// 队列名称 
        public static void EnsureQueueExists(string strPath)
        {
            if (!MessageQueue.Exists(strPath))
            {
                MessageQueue.Create(strPath);
                MessageQueue mqTemp = new MessageQueue(strPath);
                mqTemp.SetPermissions("Everyone", MessageQueueAccessRights.FullControl);
            }
        }

        /// 
        /// 从队列中取出对象列表 
        /// 
        /// 队列名称 
        public static System.Collections.ArrayList GetMessage(string strPath)
        {
            MessageQueue mq = new MessageQueue(strPath, false);
            mq.Formatter = new XmlMessageFormatter(new Type[] { typeof(T) });
            Message[] arrM = mq.GetAllMessages();
            mq.Close();            
            System.Collections.ArrayList al = new System.Collections.ArrayList();
            foreach (Message m in arrM)
            {
                al.Add(m.Body);
            }
            return al;
        }

        //获取指定消息队列的长度
        public static int GetQueueLen(string strPath)
        {
            MessageQueue mq = new MessageQueue(strPath, false);
            int iNum = 0;
            MessageEnumerator msgEnum = mq.GetMessageEnumerator2();
            mq.Close();
            while(msgEnum.MoveNext())
            {
                iNum++;
            }            
            return iNum;
        }
        //判断队列长度是否到达一定数量
        public static bool IsOverSelectLen(string strPath,int iLength)
        {
            try
            {
                MessageQueue mq = new MessageQueue(strPath, false);
                int iNum = 0;
                MessageEnumerator msgEnum = mq.GetMessageEnumerator2();
                mq.Close();
                while (msgEnum.MoveNext())
                {
                    iNum++;
                    if (iNum >= iLength)
                    {
                        return true;
                    }
                }
                return false;
            }
            catch (System.Exception e)
            {
                return false;
            }
           
        }
        //读取一条信息
        public static T GetSingleMessage(string strPath)
        {
            //SmsLogTemp.WriteDataBaseLog(strPath);
            MessageQueue mq = new MessageQueue(strPath, false);
            mq.Formatter = new XmlMessageFormatter(new Type[] { typeof(T) });
            try
            {
                Message messageObj = mq.Receive(new TimeSpan(0, 0,0,SmsSetIni.ReadQueueTimeout,0));  //10秒的读取超时设置
                mq.Close();
                return (T)messageObj.Body;
            }
            catch(Exception ee)
            {
                //SmsLogTemp.WriteDataBaseLog(ee.Message.ToString());
                mq.Close();
                return default(T);
            }
        }
    }
}