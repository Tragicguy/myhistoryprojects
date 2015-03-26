using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsOprateLog 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsOprateLog
	{
		public SmsOprateLog()
		{}
		#region Model
		private long _id;
		private string _opratertype;
		private string _opraterid;
		private string _oprationinfo;
		private DateTime _opratetime;
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
		public string OpraterType
		{
			set{ _opratertype=value;}
			get{return _opratertype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string OpraterID
		{
			set{ _opraterid=value;}
			get{return _opraterid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string OprationInfo
		{
			set{ _oprationinfo=value;}
			get{return _oprationinfo;}
		}
		/// <summary>
		/// 
		/// </summary>
		public DateTime OprateTime
		{
			set{ _opratetime=value;}
			get{return _opratetime;}
		}
		#endregion Model

	}
}

