using System;
using System.Collections.Generic;
using System.Text;

namespace SmsServer.Model
{
    public class SendRetValue
    {
        private string _oldValue;   
        private int _mapValue;
        private int _needresend;
        private string _retInfo;
        /// <summary>
        /// 运营商返回信息
        /// </summary>
        public string OldRetValue
        {
            set { _oldValue = value; }
            get { return _oldValue; }
        }
        /// <summary>
        /// 匹配返回值，标识是否成功,0成功，其他失败[-1]
        /// </summary>
        public int RetValue
        {
            set { _mapValue = value; }
            get { return _mapValue; }
        }
        /// <summary>
        /// 是否需要重新发送,0不重发，1重发
        /// </summary>
        public int IsNeedResend
        {
            set { _needresend = value; }
            get { return _needresend; }
        }
        /// <summary>
        /// 备注运营商返回的信息,成功时存放返回的信息编号[如果需要]
        /// </summary>
        public string RetInfo
        {
            set { _retInfo = value; }
            get { return _retInfo; }
        }
    }
}
