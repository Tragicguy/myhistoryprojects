using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类groupDetail 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class groupDetail
	{
		public groupDetail()
		{}
		#region Model
		private int _detail_id;
		private int _group_id;
		private string _iphone;
		private string _iname;
		private string _idesc;
		private string _remark;
		/// <summary>
		/// ID
		/// </summary>
		public int detail_id
		{
			set{ _detail_id=value;}
			get{return _detail_id;}
		}
		/// <summary>
		/// 引用群组ID
		/// </summary>
		public int group_id
		{
			set{ _group_id=value;}
			get{return _group_id;}
		}
		/// <summary>
		/// 群组成员手机号码
		/// </summary>
		public string iphone
		{
			set{ _iphone=value;}
			get{return _iphone;}
		}
		/// <summary>
		/// 群组成员姓名
		/// </summary>
		public string iname
		{
			set{ _iname=value;}
			get{return _iname;}
		}
		/// <summary>
		/// 群组成员描述
		/// </summary>
		public string idesc
		{
			set{ _idesc=value;}
			get{return _idesc;}
		}
		/// <summary>
		/// 备用字段
		/// </summary>
		public string remark
		{
			set{ _remark=value;}
			get{return _remark;}
		}
		#endregion Model

	}
}

