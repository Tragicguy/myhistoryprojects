using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsHistory。
	/// </summary>
	public class SmsHistory
	{
		public SmsHistory()
		{}
		#region  成员方法

		
        /// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(Model.SmsTaskA model,int iSendResult,string strSpMsgID,string strStatusStr)
        {
            
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.SmsHistory)
                 .AddColumn(SmsPlatForm.SmsHistory.PhoneList, model.PhoneList)
                 .AddColumn(SmsPlatForm.SmsHistory.MsgContent, model.MsgContent)
                 .AddColumn(SmsPlatForm.SmsHistory.SubTime, model.SubTime)
                 .AddColumn(SmsPlatForm.SmsHistory.SendTime,DateTime.Now)
                 .AddColumn(SmsPlatForm.SmsHistory.UserID, model.UserID)
                 .AddColumn(SmsPlatForm.SmsHistory.ParentID, model.ParentID)
                 .AddColumn(SmsPlatForm.SmsHistory.SpgateNo, model.SpgateNo)
                 .AddColumn(SmsPlatForm.SmsHistory.SubPort, model.SubPort)
                 .AddColumn(SmsPlatForm.SmsHistory.SubPortEx, model.SubPortEx)
                 .AddColumn(SmsPlatForm.SmsHistory.FeeNum, model.FeeNum)
                 .AddColumn(SmsPlatForm.SmsHistory.PhoneNum, model.PhoneNum)
                 .AddColumn(SmsPlatForm.SmsHistory.MsgLen, model.MsgLen)
                 .AddColumn(SmsPlatForm.SmsHistory.HasIndex, model.HasIndex)
                 .AddColumn(SmsPlatForm.SmsHistory.SendType, model.SendType)
                 .AddColumn(SmsPlatForm.SmsHistory.SendFlag, model.SendFlag)
                 .AddColumn(SmsPlatForm.SmsHistory.SaveType, model.SaveType)
                 .AddColumn(SmsPlatForm.SmsHistory.SendResult, iSendResult)
                 .AddColumn(SmsPlatForm.SmsHistory.MobileNum, model.MobileNum)
                 .AddColumn(SmsPlatForm.SmsHistory.UnicomNum, model.UnicomNum)
                 .AddColumn(SmsPlatForm.SmsHistory.TelecomNum, model.TelecomNum)
                 .AddColumn(SmsPlatForm.SmsHistory.ChinaNum, model.ChinaNum)
                 .AddColumn(SmsPlatForm.SmsHistory.SingleMobileFee, model.SingleMobileFee)
                 .AddColumn(SmsPlatForm.SmsHistory.SingleUnicomFee, model.SingleUnicomFee)
                 .AddColumn(SmsPlatForm.SmsHistory.SingleTelecomFee, model.SingleTelecomFee)
                 .AddColumn(SmsPlatForm.SmsHistory.SingleChinaFee, model.SingleChinaFee)
                // .AddColumn(SmsPlatForm.SmsHistory.RecvFlag, model.RecvFlag)
                 //.AddColumn(SmsPlatForm.SmsHistory.RecvTime, model.RecvTime)
                 //.AddColumn(SmsPlatForm.SmsHistory.PtMessageID, strSpMsgID)
                 .AddColumn(SmsPlatForm.SmsHistory.SpMessageID, strSpMsgID)
                 .AddColumn(SmsPlatForm.SmsHistory.StatusString, strStatusStr).Execute();
                return true;
            }
            catch (System.Exception e)
            {
            	
            }
            return false;
		}
		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(Model.SmsHistory model)
		{
			
		}

		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{
        
        }
        public DataSet GetHistoryList(int iStart, int iLimit, ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            string strNumSQL = string.Format("select count(ID) from SmsHistory {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from SmsHistory {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} UserID,ParentID,PhoneNum,FeeNum,SendResult,SpgateNo,PhoneList,MsgContent,SendTime,StatusString from SmsHistory {1} {2} order by id asc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount); 
        }
		#endregion  成员方法
	}
}

