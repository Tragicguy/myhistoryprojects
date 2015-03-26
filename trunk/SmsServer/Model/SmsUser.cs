using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsUser 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsUser
	{
		public SmsUser()
		{}
		#region Model
		private long _id;
		private string _userid;
		private string _userpwd;
		private string _parentid;
		private int _userclass;
		private int _status;
		private int _disabledby;
		private int _sendednum;
		private int _balance;
		private int _maxcount;
		private int _mincount;
		private int? _auditnum;
		private int _isaudit;
		private int _ischeckillegal;
		private int _maxlen;
		private int? _mobilelen;
		private int? _unicomlen;
		private int? _telecomlen;
		private int? _chinaphonelen;
		private string _signstr;
		private int _hasindex;
		private string _mobilespgate;
		private string _unicomspgate;
		private string _telecomspgate;
		private string _chinaphonespgate;
		private string _mobilesubport;
		private string _unicomsubport;
		private string _telecomsubport;
		private string _chinaphonesubport;
		private int? _mobilemap;
		private int? _unicommap;
		private int? _telecommap;
		private int? _chinaphonemap;
		private int _historytype;
		private DateTime _creattime;
		private string _username;
		private string _companyname;
		private string _phone;
		private string _extrainfo;
		private int? _isspgatefee;
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
		public string ParentID
		{
			set{ _parentid=value;}
			get{return _parentid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int UserClass
		{
			set{ _userclass=value;}
			get{return _userclass;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int Status
		{
			set{ _status=value;}
			get{return _status;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int DisabledBy
		{
			set{ _disabledby=value;}
			get{return _disabledby;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SendedNum
		{
			set{ _sendednum=value;}
			get{return _sendednum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int Balance
		{
			set{ _balance=value;}
			get{return _balance;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MaxCount
		{
			set{ _maxcount=value;}
			get{return _maxcount;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MinCount
		{
			set{ _mincount=value;}
			get{return _mincount;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? AuditNum
		{
			set{ _auditnum=value;}
			get{return _auditnum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsAudit
		{
			set{ _isaudit=value;}
			get{return _isaudit;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int IsCheckIllegal
		{
			set{ _ischeckillegal=value;}
			get{return _ischeckillegal;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MaxLen
		{
			set{ _maxlen=value;}
			get{return _maxlen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? MobileLen
		{
			set{ _mobilelen=value;}
			get{return _mobilelen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? UnicomLen
		{
			set{ _unicomlen=value;}
			get{return _unicomlen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? TelecomLen
		{
			set{ _telecomlen=value;}
			get{return _telecomlen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? ChinaPhoneLen
		{
			set{ _chinaphonelen=value;}
			get{return _chinaphonelen;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SignStr
		{
			set{ _signstr=value;}
			get{return _signstr;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int HasIndex
		{
			set{ _hasindex=value;}
			get{return _hasindex;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MobileSpgate
		{
			set{ _mobilespgate=value;}
			get{return _mobilespgate;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UnicomSpgate
		{
			set{ _unicomspgate=value;}
			get{return _unicomspgate;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string TelecomSpgate
		{
			set{ _telecomspgate=value;}
			get{return _telecomspgate;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ChinaPhoneSpgate
		{
			set{ _chinaphonespgate=value;}
			get{return _chinaphonespgate;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MobileSubport
		{
			set{ _mobilesubport=value;}
			get{return _mobilesubport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UnicomSubport
		{
			set{ _unicomsubport=value;}
			get{return _unicomsubport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string TelecomSubport
		{
			set{ _telecomsubport=value;}
			get{return _telecomsubport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ChinaPhoneSubport
		{
			set{ _chinaphonesubport=value;}
			get{return _chinaphonesubport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? MobileMap
		{
			set{ _mobilemap=value;}
			get{return _mobilemap;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? UnicomMap
		{
			set{ _unicommap=value;}
			get{return _unicommap;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? TelecomMap
		{
			set{ _telecommap=value;}
			get{return _telecommap;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? ChinaPhoneMap
		{
			set{ _chinaphonemap=value;}
			get{return _chinaphonemap;}
		}
		/// <summary>
		/// 历史保存方式,是拆分号码保存,还是不拆分号码保存
		/// </summary>
		public int HistoryType
		{
			set{ _historytype=value;}
			get{return _historytype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public DateTime CreatTime
		{
			set{ _creattime=value;}
			get{return _creattime;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UserName
		{
			set{ _username=value;}
			get{return _username;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string CompanyName
		{
			set{ _companyname=value;}
			get{return _companyname;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string Phone
		{
			set{ _phone=value;}
			get{return _phone;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ExtraInfo
		{
			set{ _extrainfo=value;}
			get{return _extrainfo;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? IsSpgateFee
		{
			set{ _isspgatefee=value;}
			get{return _isspgatefee;}
		}
		#endregion Model

	}
}

