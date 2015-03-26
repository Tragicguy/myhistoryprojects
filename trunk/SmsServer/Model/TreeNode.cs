using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类TreeNode 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class TreeNode
	{
		public TreeNode()
		{}
		#region Model
		private int _id;
		private int _parentid;
		private string _text;
		private int _leaf;
		private string _iconcls;
		private int _number;
		private string _url;
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
		public int ParentID
		{
			set{ _parentid=value;}
			get{return _parentid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string text
		{
			set{ _text=value;}
			get{return _text;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int leaf
		{
			set{ _leaf=value;}
			get{return _leaf;}
		}
		/// <summary>
		/// 
		/// </summary>
        public string iconCls
		{
			set{ _iconcls=value;}
			get{return _iconcls;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int Number
		{
			set{ _number=value;}
			get{return _number;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string Url
		{
			set{ _url=value;}
			get{return _url;}
		}
		#endregion Model

	}
}

