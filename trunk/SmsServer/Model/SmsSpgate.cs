using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsSpgate 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsSpgate
	{
		public SmsSpgate()
		{}
		#region Model
		private long _id;
		private string _gatenumber;
		private string _gatename;
		private int _businessid;
		private int _gatetype;
		private int _islong;
		private int _issubport;
		private int _issurportmo;
		private int _isreport;
		private int _msglen;
		private int _maxphonenum;
		private int _mobilelen;
		private int _unicomlen;
		private int _telecomlen;
		private int _chinaphonelen;
		private string _gateinfo;
		private string _porturl;
		private string _mourl;
		private string _reporturl;
		private string _useridparamstring;
		private string _pwdparamstring;
		private string _mobileparamstring;
		private string _msgparamstring;
		private string _subportparamstring;
		private string _gatenoparamstring;
		private string _phonenumparamstring;
		private string _userid;
		private string _userpwd;
		private string _sendparams;
		private int _iswebservice;
		private int _sendtype;
		private int _timeout;
		private int _encodetype;
		private string _mtscriptid;
		private string _moscriptid;
		private string _reportscriptid;
		/// <summary>
		/// 
		/// </summary>
		public long ID
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string GateNumber
		{
			set{ _gatenumber=value;}
			get{return _gatenumber;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string GateName
		{
			set{ _gatename=value;}
			get{return _gatename;}
		}
		/// <summary>
		/// 区分是HTTP接口类型还是数据库转存型
		/// </summary>
		public int BusinessID
		{
			set{ _businessid=value;}
			get{return _businessid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int GateType
		{
			set{ _gatetype=value;}
			get{return _gatetype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsLong
		{
			set{ _islong=value;}
			get{return _islong;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsSubport
		{
			set{ _issubport=value;}
			get{return _issubport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsSurportMo
		{
			set{ _issurportmo=value;}
			get{return _issurportmo;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsReport
		{
			set{ _isreport=value;}
			get{return _isreport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MsgLen
		{
			set{ _msglen=value;}
			get{return _msglen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MaxPhoneNum
		{
			set{ _maxphonenum=value;}
			get{return _maxphonenum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MobileLen
		{
			set{ _mobilelen=value;}
			get{return _mobilelen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int UnicomLen
		{
			set{ _unicomlen=value;}
			get{return _unicomlen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int TeleComLen
		{
			set{ _telecomlen=value;}
			get{return _telecomlen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int ChinaPhoneLen
		{
			set{ _chinaphonelen=value;}
			get{return _chinaphonelen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string GateInfo
		{
			set{ _gateinfo=value;}
			get{return _gateinfo;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string PortUrl
		{
			set{ _porturl=value;}
			get{return _porturl;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MoUrl
		{
			set{ _mourl=value;}
			get{return _mourl;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ReportUrl
		{
			set{ _reporturl=value;}
			get{return _reporturl;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UseridParamString
		{
			set{ _useridparamstring=value;}
			get{return _useridparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string PwdParamString
		{
			set{ _pwdparamstring=value;}
			get{return _pwdparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MobileParamString
		{
			set{ _mobileparamstring=value;}
			get{return _mobileparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MsgParamString
		{
			set{ _msgparamstring=value;}
			get{return _msgparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SubportParamString
		{
			set{ _subportparamstring=value;}
			get{return _subportparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string GateNoParamString
		{
			set{ _gatenoparamstring=value;}
			get{return _gatenoparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string PhoneNumParamString
		{
			set{ _phonenumparamstring=value;}
			get{return _phonenumparamstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UserID
		{
			set{ _userid=value;}
			get{return _userid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UserPwd
		{
			set{ _userpwd=value;}
			get{return _userpwd;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SendParams
		{
			set{ _sendparams=value;}
			get{return _sendparams;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsWebService
		{
			set{ _iswebservice=value;}
			get{return _iswebservice;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SendType
		{
			set{ _sendtype=value;}
			get{return _sendtype;}
		}
		/// <summary>
		/// 超时时限
		/// </summary>
		public int TimeOut
		{
			set{ _timeout=value;}
			get{return _timeout;}
		}
		/// <summary>
		/// 信息内容编码的格式
		/// </summary>
		public int EncodeType
		{
			set{ _encodetype=value;}
			get{return _encodetype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MtScriptID
		{
			set{ _mtscriptid=value;}
			get{return _mtscriptid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MoScriptID
		{
			set{ _moscriptid=value;}
			get{return _moscriptid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ReportScriptID
		{
			set{ _reportscriptid=value;}
			get{return _reportscriptid;}
		}
		#endregion Model

	}
}

