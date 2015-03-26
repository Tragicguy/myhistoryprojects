using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsHistory 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsHistory
	{
		public SmsHistory()
		{}
		#region Model
		private long _id;
		private string _phonelist;
		private string _msgcontent;
		private DateTime _subtime;
		private DateTime _sendtime;
		private string _userid;
		private string _ParentID;
		private string _spgateno;
		private string _subport;
		private string _subportex;
		private int _feenum;
		private int _phonenum;
		private int _msglen;
		private int _hasindex;
		private int _sendtype;
		private int _sendflag;
		private int _savetype;
		private int _mobilenum;
		private int _unicomnum;
		private int _telecomnum;
		private int _chinanum;
		private int _singlemobilefee;
		private int _singleunicomfee;
		private int _singletelecomfee;
		private int _singlechinafee;
		private int? _recvflag;
		private int? _recvtime;
		private string _ptmessageid;
		private string _spmessageid;
		private string _statusstring;
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
		public string PhoneList
		{
			set{ _phonelist=value;}
			get{return _phonelist;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MsgContent
		{
			set{ _msgcontent=value;}
			get{return _msgcontent;}
		}
		/// <summary>
		/// 
		/// </summary>
		public DateTime SubTime
		{
			set{ _subtime=value;}
			get{return _subtime;}
		}
		/// <summary>
		/// 
		/// </summary>
		public DateTime SendTime
		{
			set{ _sendtime=value;}
			get{return _sendtime;}
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
		public string ParentID
		{
			set{ _ParentID=value;}
			get{return _ParentID;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SpgateNo
		{
			set{ _spgateno=value;}
			get{return _spgateno;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SubPort
		{
			set{ _subport=value;}
			get{return _subport;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SubPortEx
		{
			set{ _subportex=value;}
			get{return _subportex;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int FeeNum
		{
			set{ _feenum=value;}
			get{return _feenum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int PhoneNum
		{
			set{ _phonenum=value;}
			get{return _phonenum;}
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
		/// 拆分时是否加上编号
		/// </summary>
		public int HasIndex
		{
			set{ _hasindex=value;}
			get{return _hasindex;}
		}
		/// <summary>
		/// 表示是否是HTTP发送
		/// </summary>
		public int SendType
		{
			set{ _sendtype=value;}
			get{return _sendtype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SendFlag
		{
			set{ _sendflag=value;}
			get{return _sendflag;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SaveType
		{
			set{ _savetype=value;}
			get{return _savetype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MobileNum
		{
			set{ _mobilenum=value;}
			get{return _mobilenum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int UnicomNum
		{
			set{ _unicomnum=value;}
			get{return _unicomnum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int TelecomNum
		{
			set{ _telecomnum=value;}
			get{return _telecomnum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int ChinaNum
		{
			set{ _chinanum=value;}
			get{return _chinanum;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SingleMobileFee
		{
			set{ _singlemobilefee=value;}
			get{return _singlemobilefee;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SingleUnicomFee
		{
			set{ _singleunicomfee=value;}
			get{return _singleunicomfee;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SingleTelecomFee
		{
			set{ _singletelecomfee=value;}
			get{return _singletelecomfee;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int SingleChinaFee
		{
			set{ _singlechinafee=value;}
			get{return _singlechinafee;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? RecvFlag
		{
			set{ _recvflag=value;}
			get{return _recvflag;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? RecvTime
		{
			set{ _recvtime=value;}
			get{return _recvtime;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string PtMessageID
		{
			set{ _ptmessageid=value;}
			get{return _ptmessageid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SpMessageID
		{
			set{ _spmessageid=value;}
			get{return _spmessageid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string StatusString
		{
			set{ _statusstring=value;}
			get{return _statusstring;}
		}
		#endregion Model

	}
}

