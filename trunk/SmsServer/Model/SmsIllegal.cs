using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsIllegal 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsIllegal
	{
		public SmsIllegal()
		{}
		#region Model
		private long _id;
		private string _belongspgate;
		private string _keyword;
		/// <summary>
		/// 
		/// </summary>
		public long id
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string BelongSpgate
		{
			set{ _belongspgate=value;}
			get{return _belongspgate;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string KeyWord
		{
			set{ _keyword=value;}
			get{return _keyword;}
		}
		#endregion Model

	}
}

