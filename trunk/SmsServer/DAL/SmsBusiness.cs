using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsBusiness。
	/// </summary>
	public class SmsBusiness
	{
		public SmsBusiness()
		{}
		#region  成员方法

        public static DataSet GetBusinessList()
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsBusiness, SmsPlatForm.SmsBusiness.BusinessID, SmsPlatForm.SmsBusiness.BusinessName).ToDataSet();
        }
		#endregion  成员方法
	}
}

