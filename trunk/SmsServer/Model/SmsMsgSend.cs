using System;
using System.Collections.Generic;
using System.Text;

namespace SmsServer.Model
{
    public class SmsMsgSend
    {
        public SmsMsgSend()
        {
            for (int i=0;i<4;i++)
            {
                strMessageArray[i] = new string[10];
                iSingleMsgFeeNum[i] = new int[10];
            }
            for (int i = 0; i < 6;i++ )
            {
                strPhoneList[i] = new StringBuilder();
                strMsgIdOffset[i] = new StringBuilder();
            }
        }
        private StringBuilder[] strPhoneList = new StringBuilder[6];//各号段号码的存储数组[包括错误号码，包括只计费不发送号码]
        private StringBuilder[] strMsgIdOffset = new StringBuilder[6];//各号段号码的存储数组[包括错误号码，包括只计费不发送号码]
        private SmsSpgate[] _PhoneSpgate = new SmsSpgate[4];        //通道数组
        
        private string[][] strMessageArray = new string[4][];       //等待发送信息的拆分后于各通道上形成的二维数组
        private int[][] iSingleMsgFeeNum = new int[4][];            //拆分各条的扣费数[针对单个号码]
        private int[] iMsgArrayLen = new int[4];                    //信息拆分成的条数
        private int[] iPhoneCount = new int[6];                     //各号段号码的发送号码数
        private int[] iFeePhoneCount = new int[6];                  //各号段号码的计费号码数[包括错误号码，和只计费不发送号码],//存储每个通道号每个号段的计费号码数
        private int[] iFeeMap = new int[6];                         //扣费映射
#region 属性
        /// <summary>
        /// 各号段号码的存储数组[包括错误号码，包括只计费不发送号码]
        /// </summary>
        public StringBuilder[] PhoneList
        {
            set { strPhoneList = value; }
            get { return strPhoneList; }
        }
        /// <summary>
        /// 手机号码对应的信息编号偏移量,这种发送方式最大支持一次1000个
        /// </summary>
        public StringBuilder[] MsgIdOffset
        {
            set { strMsgIdOffset = value; }
            get { return strMsgIdOffset; }
        }
        /// <summary>
        /// 通道数组
        /// </summary>
        public SmsSpgate[] PhoneSpgate
        {
            set { _PhoneSpgate = value; }
            get { return _PhoneSpgate; }
        }
        /// <summary>
        /// 各号段号码的发送号码数
        /// </summary>
        public int[] PhoneCount
        {
            set { iPhoneCount = value; }
            get { return iPhoneCount; }
        }
        /// <summary>
        /// 各号段号码的计费号码数[包括错误号码，和只计费不发送号码]
        /// </summary>
        public int[] FeePhoneCount
        {
            set { iFeePhoneCount = value; }
            get { return iFeePhoneCount; }
        }
        /// <summary>
        /// 等待发送信息的拆分后于各通道上形成的二维数组
        /// </summary>
        public string[][] MessageArray
        {
            set { strMessageArray = value; }
            get { return strMessageArray; }
        }
        public int[] MsgArrayLen
        {
            set { iMsgArrayLen = value; }
            get { return iMsgArrayLen; }
        }
        public int[][] SingleMsgFeeNum
        {
            set { iSingleMsgFeeNum = value; }
            get { return iSingleMsgFeeNum; }
        }
        public int[] FeeMap
        {
            set { iFeeMap = value; }
            get { return iFeeMap; }
        }
#endregion        
    }
}
