using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类MapScriptDetail 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class MapScriptDetail
	{
		public MapScriptDetail()
		{}
		#region Model
		private long _id;
		private string _mapid;
		private int _maptype;
		private string _srcvalue;
		private int _mapretvalue;
		private int _retusevalue;
		private string _retstring;
		private int? _charnum;
		private string _splitchar;
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
		public string MapID
		{
			set{ _mapid=value;}
			get{return _mapid;}
		}
		/// <summary>
		/// 匹配类别[1:等于;2:小于;3:大于;4:包含;5:左边几位等于;6右边几位等于;7:左边几位不等于;8:分隔符左边的数等于;9:分隔符左边的数不等于]
		/// </summary>
		public int MapType
		{
			set{ _maptype=value;}
			get{return _maptype;}
		}
		/// <summary>
		/// 用的判断的比较值
		/// </summary>
		public string SrcValue
		{
			set{ _srcvalue=value;}
			get{return _srcvalue;}
		}
		/// <summary>
		/// 映射后反馈到上层的返回值
		/// </summary>
		public int MapRetValue
		{
			set{ _mapretvalue=value;}
			get{return _mapretvalue;}
		}
		/// <summary>
		/// 判断是否需要重发的返回值
		/// </summary>
		public int RetUseValue
		{
			set{ _retusevalue=value;}
			get{return _retusevalue;}
		}
		/// <summary>
		/// 反馈给上层的详细错误信息
		/// </summary>
		public string RetString
		{
			set{ _retstring=value;}
			get{return _retstring;}
		}
		/// <summary>
		/// 左边或者右边取的位数
		/// </summary>
		public int? CharNum
		{
			set{ _charnum=value;}
			get{return _charnum;}
		}
		/// <summary>
		/// 分隔字符串
		/// </summary>
		public string SplitChar
		{
			set{ _splitchar=value;}
			get{return _splitchar;}
		}
		#endregion Model

	}
}

