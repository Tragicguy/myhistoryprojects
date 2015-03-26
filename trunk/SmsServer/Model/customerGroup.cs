using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类customerGroup 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class customerGroup
	{
		public customerGroup()
		{}
		#region Model
		private int _group_id;
		private int _detail_id;
		private string _ref_user;
		private string _group_name;
		private string _group_desc;
		private string _remark;
		/// <summary>
		/// 群组ID
		/// </summary>
		public int group_id
		{
			set{ _group_id=value;}
			get{return _group_id;}
		}
		/// <summary>
		/// ID
		/// </summary>
		public int detail_id
		{
			set{ _detail_id=value;}
			get{return _detail_id;}
		}
		/// <summary>
		/// 引用的用户
		/// </summary>
		public string ref_user
		{
			set{ _ref_user=value;}
			get{return _ref_user;}
		}
		/// <summary>
		/// 群组名称
		/// </summary>
		public string group_name
		{
			set{ _group_name=value;}
			get{return _group_name;}
		}
		/// <summary>
		/// 群组描述
		/// </summary>
		public string group_desc
		{
			set{ _group_desc=value;}
			get{return _group_desc;}
		}
		/// <summary>
		/// 备注
		/// </summary>
		public string remark
		{
			set{ _remark=value;}
			get{return _remark;}
		}
		#endregion Model

	}
}

