using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsRetValue。
	/// </summary>
	public class SmsRetValue
	{
		public SmsRetValue()
		{}
		#region  成员方法

		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string strSpgate,string strRet)
		{
            return false;
		}

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public int Add(SmsServer.Model.SmsRetValue model)
		{
            return 0;
		}
		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsRetValue model)
		{
			
		}

		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(int ID)
		{
			
		}

		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsRetValue GetModel(int ID)
		{

            return null;
		}

		/// <summary>
		/// 获得数据列表
		/// </summary>
		public DataSet GetList(string strWhere)
		{
            return null;
		}

		/// <summary>
		/// 获得前几行数据
		/// </summary>
		public DataSet GetList(int Top,string strWhere,string filedOrder)
		{
            return null;
		}
		#endregion  成员方法
	}
}

