using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类uploadFile 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class uploadFile
	{
		public uploadFile()
		{}
		#region Model
		private string _dir_name;
		private string _dir_alias;
		private DateTime _upload_date;
		private string _uploader;
		private string _mobile_file;
		private int _mobile_num;
		private string _unicom_file;
		private int _unicom_num;
		private string _telecom_file;
		private int _telecom_num;
		private string _china_file;
		private int _china_num;
		/// <summary>
		/// 实际存放于服务器上的文件夹名称
		/// </summary>
		public string dir_name
		{
			set{ _dir_name=value;}
			get{return _dir_name;}
		}
		/// <summary>
		/// 客户命名的文件夹别名
		/// </summary>
		public string dir_alias
		{
			set{ _dir_alias=value;}
			get{return _dir_alias;}
		}
		/// <summary>
		/// 上传时间
		/// </summary>
		public DateTime upload_date
		{
			set{ _upload_date=value;}
			get{return _upload_date;}
		}
		/// <summary>
		/// 上传者
		/// </summary>
		public string uploader
		{
			set{ _uploader=value;}
			get{return _uploader;}
		}
		/// <summary>
		/// 移动存储路径
		/// </summary>
		public string mobile_file
		{
			set{ _mobile_file=value;}
			get{return _mobile_file;}
		}
		/// <summary>
		/// 移动号码数量
		/// </summary>
		public int mobile_num
		{
			set{ _mobile_num=value;}
			get{return _mobile_num;}
		}
		/// <summary>
		/// 联通存储路径
		/// </summary>
		public string unicom_file
		{
			set{ _unicom_file=value;}
			get{return _unicom_file;}
		}
		/// <summary>
		/// 联通号码数量
		/// </summary>
		public int unicom_num
		{
			set{ _unicom_num=value;}
			get{return _unicom_num;}
		}
		/// <summary>
		/// 电信存储路径
		/// </summary>
		public string telecom_file
		{
			set{ _telecom_file=value;}
			get{return _telecom_file;}
		}
		/// <summary>
		/// 电信号码数量
		/// </summary>
		public int telecom_num
		{
			set{ _telecom_num=value;}
			get{return _telecom_num;}
		}
		/// <summary>
		/// 小灵通存储路径
		/// </summary>
		public string china_file
		{
			set{ _china_file=value;}
			get{return _china_file;}
		}
		/// <summary>
		/// 小灵通号码数量
		/// </summary>
		public int china_num
		{
			set{ _china_num=value;}
			get{return _china_num;}
		}
		#endregion Model

	}
}

