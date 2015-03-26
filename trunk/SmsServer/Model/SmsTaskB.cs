using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsTaskB 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsTaskB
	{
		public SmsTaskB()
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
		#endregion Model

	}
}

