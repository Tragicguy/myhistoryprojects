using System;
namespace SmsServer.Model
{
	/// <summary>
	/// 实体类SmsManager 。(属性说明自动提取数据库字段的描述信息)
	/// </summary>
	[Serializable]
	public class SmsManager
	{
		public SmsManager()
		{}
		#region Model
		private int _id;
		private string _managerid;
		private string _managerpwd;
		private int _managertype;
		private DateTime _creattime;
		private string _power;
		private string _username;
		private string _companyname;
		private string _phone;
		private string _address;
		private string _extrainfo;
		private int? _balance;
		private int? _used;
        private string _portset;
     	private string _newmanagerpwd;
		/// <summary>
		/// 
		/// </summary>
		public int ID
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ManagerID
		{
			set{ _managerid=value;}
			get{return _managerid;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ManagerPwd
		{
			set{ _managerpwd=value;}
			get{return _managerpwd;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int ManagerType
		{
			set{ _managertype=value;}
			get{return _managertype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public DateTime CreatTime
		{
			set{ _creattime=value;}
			get{return _creattime;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string Power
		{
			set{ _power=value;}
			get{return _power;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string UserName
		{
			set{ _username=value;}
			get{return _username;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string CompanyName
		{
			set{ _companyname=value;}
			get{return _companyname;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string Phone
		{
			set{ _phone=value;}
			get{return _phone;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string Address
		{
			set{ _address=value;}
			get{return _address;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ExtraInfo
		{
			set{ _extrainfo=value;}
			get{return _extrainfo;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? Balance
		{
			set{ _balance=value;}
			get{return _balance;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int? Used
		{
			set{ _used=value;}
			get{return _used;}
		}        
		/// <summary>
		/// 
		/// </summary>
		public string PortSet
		{
			set{ _portset=value;}
			get{return _portset;}
		}
		/// <summary>
   		/// 
    	/// </summary>
    	public string NewManagerPwd
    	{
        		set { _newmanagerpwd = value; }
        		get { return _newmanagerpwd; }
    	}
		#endregion Model

	}
}

