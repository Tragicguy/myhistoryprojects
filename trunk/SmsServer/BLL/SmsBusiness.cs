using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsBusiness 的摘要说明。
	/// </summary>
	public class SmsBusiness
	{
		public SmsBusiness()
		{}
		#region  成员方法

		/// <summary>
		/// 得到最大ID
		/// </summary>
        public static string GetBusinessList()
        {
            DataSet ds = SmsServer.DAL.SmsBusiness.GetBusinessList();
            return JSonClass.DataTableToJsonEx(ds.Tables[0], ds.Tables[0].Rows.Count);
        }
		#endregion  成员方法
	}
}

