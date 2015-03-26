using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsHistory 的摘要说明。
	/// </summary>
	public class SmsHistory
	{
		private readonly SmsServer.DAL.SmsHistory dal=new SmsServer.DAL.SmsHistory();
		public SmsHistory()
		{}

		#region  成员方法

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(Model.SmsTaskA model,int iResult,string strSpMsgID,string strStatusStr)
		{
            model.PhoneList = model.PhoneList.Substring(0, model.PhoneList.Length - 1);
            model.FeeNum = model.MobileNum * model.SingleMobileFee +
                            model.UnicomNum * model.SingleUnicomFee +
                            model.TelecomNum * model.SingleTelecomFee +
                            model.ChinaNum * model.SingleChinaFee;
            return dal.Add(model, iResult,strSpMsgID, strStatusStr);
		}
		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{
			
			dal.Delete(ID);
		}
		public string GetHistoryList(int iStart,int iLimit)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetHistoryList(iStart, iLimit, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
		#endregion  成员方法
	}
}

