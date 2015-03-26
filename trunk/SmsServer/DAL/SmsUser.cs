using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsUser。
	/// </summary>
	public class SmsUser
	{
		public SmsUser()
		{}
		#region  成员方法

		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string strUserID)
		{
			int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsUser).Where(SmsPlatForm.SmsUser.UserID == strUserID).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0?true:false;
		}
		/// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(SmsServer.Model.SmsUser model)
		{
            MyDataBase.db.Insert(SmsPlatForm.SmsUser)
            .AddColumn(SmsPlatForm.SmsUser.UserID, model.UserID)
            .AddColumn(SmsPlatForm.SmsUser.UserPwd, model.UserPwd)
            .AddColumn(SmsPlatForm.SmsUser.ParentID, model.ParentID)
            .AddColumn(SmsPlatForm.SmsUser.UserClass, model.UserClass)
            .AddColumn(SmsPlatForm.SmsUser.Status, model.Status)
            .AddColumn(SmsPlatForm.SmsUser.DisabledBy, model.DisabledBy)
            .AddColumn(SmsPlatForm.SmsUser.SendedNum, model.SendedNum)
            .AddColumn(SmsPlatForm.SmsUser.Balance, model.Balance)
            .AddColumn(SmsPlatForm.SmsUser.MaxCount, model.MaxCount)
            .AddColumn(SmsPlatForm.SmsUser.MinCount, model.MinCount)
            .AddColumn(SmsPlatForm.SmsUser.AuditNum, model.AuditNum)
            .AddColumn(SmsPlatForm.SmsUser.IsAudit, model.IsAudit)
            .AddColumn(SmsPlatForm.SmsUser.MaxLen, model.MaxLen)
            .AddColumn(SmsPlatForm.SmsUser.MobileLen, model.MobileLen)
            .AddColumn(SmsPlatForm.SmsUser.UnicomLen, model.UnicomLen)
            .AddColumn(SmsPlatForm.SmsUser.TelecomLen, model.TelecomLen)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneLen, model.ChinaPhoneLen)
            .AddColumn(SmsPlatForm.SmsUser.SignStr, model.SignStr)
            .AddColumn(SmsPlatForm.SmsUser.HasIndex, model.HasIndex)
            .AddColumn(SmsPlatForm.SmsUser.MobileSpgate, model.MobileSpgate)
            .AddColumn(SmsPlatForm.SmsUser.UnicomSpgate, model.UnicomSpgate)
            .AddColumn(SmsPlatForm.SmsUser.TelecomSpgate, model.TelecomSpgate)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneSpgate, model.ChinaPhoneSpgate)
            .AddColumn(SmsPlatForm.SmsUser.UnicomMap, model.UnicomMap)
            .AddColumn(SmsPlatForm.SmsUser.TelecomMap, model.TelecomMap)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneMap, model.ChinaPhoneMap)
            .AddColumn(SmsPlatForm.SmsUser.HistoryType, model.HistoryType)
            .AddColumn(SmsPlatForm.SmsUser.CreatTime, model.CreatTime)
            .AddColumn(SmsPlatForm.SmsUser.UserName, model.UserName)
            .AddColumn(SmsPlatForm.SmsUser.CompanyName, model.CompanyName)
            .AddColumn(SmsPlatForm.SmsUser.Phone, model.Phone)
            .AddColumn(SmsPlatForm.SmsUser.ExtraInfo, model.ExtraInfo).Execute();
            return true;
		}
		/// <summary>
		/// 更新一条数据
		/// </summary>
		public bool Update(SmsServer.Model.SmsUser model)
		{
            try
            {
             MyDataBase.db.Update(SmsPlatForm.SmsUser)
            .AddColumn(SmsPlatForm.SmsUser.Status, model.Status)
            .AddColumn(SmsPlatForm.SmsUser.MaxCount, model.MaxCount)
            .AddColumn(SmsPlatForm.SmsUser.MinCount, model.MinCount)
            .AddColumn(SmsPlatForm.SmsUser.AuditNum, model.AuditNum)
            .AddColumn(SmsPlatForm.SmsUser.IsAudit, model.IsAudit)
            .AddColumn(SmsPlatForm.SmsUser.MaxLen, model.MaxLen)
            .AddColumn(SmsPlatForm.SmsUser.MobileLen, model.MobileLen)
            .AddColumn(SmsPlatForm.SmsUser.UnicomLen, model.UnicomLen)
            .AddColumn(SmsPlatForm.SmsUser.TelecomLen, model.TelecomLen)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneLen, model.ChinaPhoneLen)
            .AddColumn(SmsPlatForm.SmsUser.SignStr, model.SignStr)
            .AddColumn(SmsPlatForm.SmsUser.HasIndex, model.HasIndex)
            .AddColumn(SmsPlatForm.SmsUser.MobileSpgate, model.MobileSpgate)
            .AddColumn(SmsPlatForm.SmsUser.UnicomSpgate, model.UnicomSpgate)
            .AddColumn(SmsPlatForm.SmsUser.TelecomSpgate, model.TelecomSpgate)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneSpgate, model.ChinaPhoneSpgate)
            .AddColumn(SmsPlatForm.SmsUser.MobileMap, model.MobileMap)
            .AddColumn(SmsPlatForm.SmsUser.UnicomMap, model.UnicomMap)
            .AddColumn(SmsPlatForm.SmsUser.TelecomMap, model.TelecomMap)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneMap, model.ChinaPhoneMap)
            .AddColumn(SmsPlatForm.SmsUser.MobileSubport, model.MobileSubport)
            .AddColumn(SmsPlatForm.SmsUser.UnicomSubport, model.UnicomSubport)
            .AddColumn(SmsPlatForm.SmsUser.TelecomSubport, model.TelecomSubport)
            .AddColumn(SmsPlatForm.SmsUser.ChinaPhoneSubport, model.ChinaPhoneSubport)
            .AddColumn(SmsPlatForm.SmsUser.HistoryType, model.HistoryType)
            .AddColumn(SmsPlatForm.SmsUser.UserName, model.UserName)
            .AddColumn(SmsPlatForm.SmsUser.CompanyName, model.CompanyName)
            .AddColumn(SmsPlatForm.SmsUser.Phone, model.Phone)
            .AddColumn(SmsPlatForm.SmsUser.IsSpgateFee, model.IsSpgateFee)            
            .AddColumn(SmsPlatForm.SmsUser.ExtraInfo, model.ExtraInfo)
            .Where(SmsPlatForm.SmsUser.UserID == model.UserID).Execute();
            return true;
            }
            catch (Exception ee)
            {
                return false;
            }
            
		}
		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{
            MyDataBase.db.Delete(SmsPlatForm.SmsUser).Where(SmsPlatForm.SmsUser.ID == ID).Execute();
        }

        /// <summary>
        /// 删除一条数据
        /// </summary>
        public void Delete(string strUserID)
        {
            MyDataBase.db.Delete(SmsPlatForm.SmsUser).Where(SmsPlatForm.SmsUser.UserID == strUserID).Execute();
        }
        /// <summary>
        /// 删除多条数据
        /// </summary>
        public bool Delete(string[] UserIDArray)
        {
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.SmsUser)
                .Where(SmsPlatForm.SmsUser.UserID.In(UserIDArray)).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
        //修改余额信息[发送信息]
        public bool UpdateBalance(string strUserID, int iFee)
        {
            try
            {
                int iRetVal = -1;
                int iRefRetVal = -1;

                SmsPlatForm.DeductUserFee(MyDataBase.db, out iRetVal, strUserID, iFee, ref iRefRetVal);
//                 MyDataBase.db.Update(SmsPlatForm.SmsUser)
//                     .AddColumn(SmsPlatForm.SmsUser.SendedNum, SmsPlatForm.SmsUser.SendedNum + iFee)
//                 .AddColumn(SmsPlatForm.SmsUser.Balance, SmsPlatForm.SmsUser.Balance-iFee)
//                 .Where(SmsPlatForm.SmsUser.UserID == strUserID).Execute();
                if (iRefRetVal == -1)
                {
                    return false;
                }
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
        //扣除余额
        public int DeductUserFee(string strUserID, int iFee)
        {
            return 0;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public DataSet GetSelectSmsUser(string strUserID)
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsUser).Where(SmsPlatForm.SmsUser.UserID == strUserID).ToDataSet();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public DataSet GetSelectSmsUser(string strUserID, string strPwd)
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsUser).Where(SmsPlatForm.SmsUser.UserID == strUserID
                && SmsPlatForm.SmsUser.UserPwd == strPwd).ToDataSet();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="iStart"></param>
        /// <param name="iLimit"></param>
        /// <param name="strUserID"></param>
        /// <param name="strParentID"></param>
        /// <param name="iStatus"></param>
        /// <param name="iTotalCount"></param>
        /// <returns></returns>
        public DataSet GetUserList(int iStart, int iLimit, string strUserID, string strParentID, int iStatus, ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            if (strUserID != "")
            {
                strWhere.AppendFormat(" and UserID='{0}'", strUserID);
            }
            if (iStatus != -1)
            {
                strWhere.AppendFormat(" and Status={0}", iStatus);
            }
            if (strParentID != "")
            {
                strWhere.AppendFormat(" and ParentID='{0}'", strParentID);
            }
            string strNumSQL = string.Format("select count(ID) from SmsUser {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from SmsUser {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} UserID,ParentID,Status,SendedNum,Balance,SignStr,CreatTime,UserName,CompanyName,Phone,ExtraInfo from SmsUser {1} {2} order by id asc",
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

