using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsReceived。
	/// </summary>
	public class SmsReceived
	{
		public SmsReceived()
		{}
		#region  成员方法


		/// <summary>
		/// 增加一条数据
		/// </summary>
		public int Add(SmsServer.Model.SmsReceived model)
		{
            return 0;
		}
		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsReceived model)
		{
			
		}

		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{
		
		}


		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsReceived GetModel(long ID)
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

