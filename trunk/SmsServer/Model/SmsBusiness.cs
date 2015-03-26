using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsBusiness 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsBusiness
	{
		public SmsBusiness()
		{}
		#region Model
		private int _id;
		private int _businessid;
		private string _businessname;
		/// <summary>
		/// 
		/// </summary>
		public int ID
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int BusinessID
		{
			set{ _businessid=value;}
			get{return _businessid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string BusinessName
		{
			set{ _businessname=value;}
			get{return _businessname;}
		}
		#endregion Model

	}
}

