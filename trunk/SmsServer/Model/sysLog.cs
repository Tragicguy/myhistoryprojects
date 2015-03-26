using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类sysLog 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class sysLog
	{
		public sysLog()
		{}
		#region Model
		private int _log_id;
		private int _log_type;
		private string _log_detail;
		private DateTime _log_time;
		private string _operatorid;
		private string _op_module;
		/// <summary>
		/// 日志ID
		/// </summary>
		public int log_id
		{
			set{ _log_id=value;}
			get{return _log_id;}
		}
		/// <summary>
		/// 日志类型(0:异常日志、1:操作日志)
		/// </summary>
		public int log_type
		{
			set{ _log_type=value;}
			get{return _log_type;}
		}
		/// <summary>
		/// 日志详情
		/// </summary>
		public string log_detail
		{
			set{ _log_detail=value;}
			get{return _log_detail;}
		}
		/// <summary>
		/// 记录时间
		/// </summary>
		public DateTime log_time
		{
			set{ _log_time=value;}
			get{return _log_time;}
		}
		/// <summary>
		/// 操作人
		/// </summary>
		public string operatorID
		{
			set{ _operatorid=value;}
			get{return _operatorid;}
		}
		/// <summary>
		/// 操作模块
		/// </summary>
		public string op_module
		{
			set{ _op_module=value;}
			get{return _op_module;}
		}
		#endregion Model

	}
}

