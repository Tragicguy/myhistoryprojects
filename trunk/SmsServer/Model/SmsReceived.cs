using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsReceived 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsReceived
	{
		public SmsReceived()
		{}
		#region Model
		private long _id;
		private string _phone;
		private string _content;
		private string _spgateno;
		private DateTime _recvtime;
		private string _recvuserid;
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
		public string Phone
		{
			set{ _phone=value;}
			get{return _phone;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string Content
		{
			set{ _content=value;}
			get{return _content;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SpGateNo
		{
			set{ _spgateno=value;}
			get{return _spgateno;}
		}
		/// <summary>
		/// 
		/// </summary>
		public DateTime RecvTime
		{
			set{ _recvtime=value;}
			get{return _recvtime;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string RecvUserID
		{
			set{ _recvuserid=value;}
			get{return _recvuserid;}
		}
		#endregion Model

	}
}

