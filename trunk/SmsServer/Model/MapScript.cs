using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类MapScript 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class MapScript
	{
		public MapScript()
		{}
		#region Model
		private int _id;
		private string _mapid;
		private string _mapname;
		private string _easyscriptcode;
		private string _scriptcode;
		private int _scripttype;
		private int _scriptflag;
		private string _mapinfo;
		private string _mospgate;
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
		public string MapID
		{
			set{ _mapid=value;}
			get{return _mapid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MapName
		{
			set{ _mapname=value;}
			get{return _mapname;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string EasyScriptCode
		{
			set{ _easyscriptcode=value;}
			get{return _easyscriptcode;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ScriptCode
		{
			set{ _scriptcode=value;}
			get{return _scriptcode;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int ScriptType
		{
			set{ _scripttype=value;}
			get{return _scripttype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int ScriptFlag
		{
			set{ _scriptflag=value;}
			get{return _scriptflag;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MapInfo
		{
			set{ _mapinfo=value;}
			get{return _mapinfo;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MoSpgate
		{
			set{ _mospgate=value;}
			get{return _mospgate;}
		}
		#endregion Model

	}
}

