using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类sysMessage 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class sysMessage
	{
		public sysMessage()
		{}
		#region Model
		private int _msg_id;
		private int _msg_type;
		private string _msg_title;
		private string _msg_content;
		private DateTime _pub_date;
		private string _publisher;
		private DateTime? _avi_date;
		private string _recivers;
		/// <summary>
		/// 消息ID
		/// </summary>
		public int msg_id
		{
			set{ _msg_id=value;}
			get{return _msg_id;}
		}
		/// <summary>
		/// 消息类型(0:公告、1:通知)
		/// </summary>
		public int msg_type
		{
			set{ _msg_type=value;}
			get{return _msg_type;}
		}
		/// <summary>
		/// 消息标题
		/// </summary>
		public string msg_title
		{
			set{ _msg_title=value;}
			get{return _msg_title;}
		}
		/// <summary>
		/// 消息内容
		/// </summary>
		public string msg_content
		{
			set{ _msg_content=value;}
			get{return _msg_content;}
		}
		/// <summary>
		/// 发布日期
		/// </summary>
		public DateTime pub_date
		{
			set{ _pub_date=value;}
			get{return _pub_date;}
		}
		/// <summary>
		/// 发布人
		/// </summary>
		public string publisher
		{
			set{ _publisher=value;}
			get{return _publisher;}
		}
		/// <summary>
		/// 有效日期
		/// </summary>
		public DateTime? avi_date
		{
			set{ _avi_date=value;}
			get{return _avi_date;}
		}
		/// <summary>
		/// 接收人
		/// </summary>
		public string recivers
		{
			set{ _recivers=value;}
			get{return _recivers;}
		}
		#endregion Model

	}
}

