using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类PhoneSection 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class PhoneSection
	{
		public PhoneSection()
		{}
		#region Model
		private int _id;
		private string _mobileheader;
		private string _unicomheader;
		private string _telecomheader;
		/// <summary>
		/// 
		/// </summary>
		public int id
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MobileHeader
		{
			set{ _mobileheader=value;}
			get{return _mobileheader;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UnicomHeader
		{
			set{ _unicomheader=value;}
			get{return _unicomheader;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string TelecomHeader
		{
			set{ _telecomheader=value;}
			get{return _telecomheader;}
		}
		#endregion Model

	}
}

