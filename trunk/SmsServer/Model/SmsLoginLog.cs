using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsLoginLog 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsLoginLog
	{
		public SmsLoginLog()
		{}
		#region Model
		private long _id;
		private DateTime _logintime;
		private string _loginip;
		private string _loginuser;
		private string _loginpwd;
		private string _loginresult;
		private int _logintype;
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
		public DateTime LoginTime
		{
			set{ _logintime=value;}
			get{return _logintime;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string LoginIp
		{
			set{ _loginip=value;}
			get{return _loginip;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string LoginUser
		{
			set{ _loginuser=value;}
			get{return _loginuser;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string LoginPwd
		{
			set{ _loginpwd=value;}
			get{return _loginpwd;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string LoginResult
		{
			set{ _loginresult=value;}
			get{return _loginresult;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int LoginType
		{
			set{ _logintype=value;}
			get{return _logintype;}
		}
		#endregion Model

	}
}

