using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsSpgate。
	/// </summary>
	public class SmsSpgate
	{
		public SmsSpgate()
		{}
		#region  成员方法

		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string GateNumber)
		{
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsSpgate).Where(SmsPlatForm.SmsSpgate.GateNumber == GateNumber).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
		}

        public int RetCount(string strSQL)
        {
            return (int)DbHelperSQL.GetSingle(strSQL);
        }

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(SmsServer.Model.SmsSpgate model)
		{
            try
            {
                 MyDataBase.db.Insert(SmsPlatForm.SmsSpgate)
                 .AddColumn(SmsPlatForm.SmsSpgate.GateNumber, model.GateNumber)
                .AddColumn(SmsPlatForm.SmsSpgate.GateName, model.GateName)
                .AddColumn(SmsPlatForm.SmsSpgate.GateType, model.GateType)
                .AddColumn(SmsPlatForm.SmsSpgate.BusinessID, model.BusinessID)
                .AddColumn(SmsPlatForm.SmsSpgate.IsLong, model.IsLong)
                .AddColumn(SmsPlatForm.SmsSpgate.IsSubport, model.IsSubport)
                .AddColumn(SmsPlatForm.SmsSpgate.IsSurportMo, model.IsSurportMo)
                .AddColumn(SmsPlatForm.SmsSpgate.IsReport, model.IsReport)
                .AddColumn(SmsPlatForm.SmsSpgate.MsgLen, model.MsgLen)
                .AddColumn(SmsPlatForm.SmsSpgate.MaxPhoneNum, model.MaxPhoneNum)
                .AddColumn(SmsPlatForm.SmsSpgate.MobileLen, model.MobileLen)
                .AddColumn(SmsPlatForm.SmsSpgate.UnicomLen, model.UnicomLen)
                .AddColumn(SmsPlatForm.SmsSpgate.TeleComLen, model.TeleComLen)
                .AddColumn(SmsPlatForm.SmsSpgate.ChinaPhoneLen, model.ChinaPhoneLen)
                .AddColumn(SmsPlatForm.SmsSpgate.GateInfo, model.GateInfo)
                .AddColumn(SmsPlatForm.SmsSpgate.PortUrl, model.PortUrl)
                .AddColumn(SmsPlatForm.SmsSpgate.MoUrl, model.MoUrl)
                .AddColumn(SmsPlatForm.SmsSpgate.ReportUrl, model.ReportUrl)
                .AddColumn(SmsPlatForm.SmsSpgate.UseridParamString, model.UseridParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.PwdParamString, model.PwdParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.MobileParamString, model.MobileParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.MsgParamString, model.MsgParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.SubportParamString, model.SubportParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.GateNoParamString, model.GateNoParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.PhoneNumParamString, model.PhoneNumParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.UserID, model.UserID)
                .AddColumn(SmsPlatForm.SmsSpgate.UserPwd, model.UserPwd)
                .AddColumn(SmsPlatForm.SmsSpgate.SendParams, model.SendParams)
                .AddColumn(SmsPlatForm.SmsSpgate.SendType, model.SendType)
                .AddColumn(SmsPlatForm.SmsSpgate.EncodeType, model.EncodeType)
                .AddColumn(SmsPlatForm.SmsSpgate.TimeOut, model.TimeOut)
                .AddColumn(SmsPlatForm.SmsSpgate.MtScriptID, model.MtScriptID)
                .AddColumn(SmsPlatForm.SmsSpgate.MoScriptID, model.MoScriptID)
                .AddColumn(SmsPlatForm.SmsSpgate.ReportScriptID, model.ReportScriptID)
                .AddColumn(SmsPlatForm.SmsSpgate.IsWebService, model.IsWebService).Execute();
                return true;
            }
            catch (System.Exception e)
            {
            	
            }
            return false;
		}
        /// <summary>
        /// 判断通道名称是否已经存在
        /// </summary>
        public bool ExistsEx(SmsServer.Model.SmsSpgate model)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsSpgate)
                .Where(SmsPlatForm.SmsSpgate.GateNumber != model.GateNumber 
                && SmsPlatForm.SmsSpgate.GateName == model.GateName).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
        /// <summary>
        /// 判断通道名称或通道号是否已经存在
        /// </summary>
        public bool ExistsExEx(SmsServer.Model.SmsSpgate model)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsSpgate)
                .Where(SmsPlatForm.SmsSpgate.GateNumber == model.GateNumber
                || SmsPlatForm.SmsSpgate.GateName == model.GateName).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
		/// <summary>
		/// 更新一条数据
		/// </summary>
        public bool Update(SmsServer.Model.SmsSpgate model)
		{
            try
            {
                MyDataBase.db.Update(SmsPlatForm.SmsSpgate)
                .AddColumn(SmsPlatForm.SmsSpgate.GateName, model.GateName)
                .AddColumn(SmsPlatForm.SmsSpgate.GateType, model.GateType)
                .AddColumn(SmsPlatForm.SmsSpgate.BusinessID, model.BusinessID)
                .AddColumn(SmsPlatForm.SmsSpgate.IsLong, model.IsLong)
                .AddColumn(SmsPlatForm.SmsSpgate.IsSubport, model.IsSubport)
                .AddColumn(SmsPlatForm.SmsSpgate.IsSurportMo, model.IsSurportMo)
                .AddColumn(SmsPlatForm.SmsSpgate.IsReport, model.IsReport)
                .AddColumn(SmsPlatForm.SmsSpgate.MsgLen, model.MsgLen)
                .AddColumn(SmsPlatForm.SmsSpgate.MaxPhoneNum, model.MaxPhoneNum)
                .AddColumn(SmsPlatForm.SmsSpgate.MobileLen, model.MobileLen)
                .AddColumn(SmsPlatForm.SmsSpgate.UnicomLen, model.UnicomLen)
                .AddColumn(SmsPlatForm.SmsSpgate.TeleComLen, model.TeleComLen)
                .AddColumn(SmsPlatForm.SmsSpgate.ChinaPhoneLen, model.ChinaPhoneLen)
                .AddColumn(SmsPlatForm.SmsSpgate.GateInfo, model.GateInfo)
                .AddColumn(SmsPlatForm.SmsSpgate.PortUrl, model.PortUrl)
                .AddColumn(SmsPlatForm.SmsSpgate.MoUrl, model.MoUrl)
                .AddColumn(SmsPlatForm.SmsSpgate.ReportUrl, model.ReportUrl)
                .AddColumn(SmsPlatForm.SmsSpgate.UseridParamString, model.UseridParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.PwdParamString, model.PwdParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.MobileParamString, model.MobileParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.MsgParamString, model.MsgParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.SubportParamString, model.SubportParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.GateNoParamString, model.GateNoParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.PhoneNumParamString, model.PhoneNumParamString)
                .AddColumn(SmsPlatForm.SmsSpgate.UserID, model.UserID)
                .AddColumn(SmsPlatForm.SmsSpgate.UserPwd, model.UserPwd)
                .AddColumn(SmsPlatForm.SmsSpgate.SendParams, model.SendParams)
                .AddColumn(SmsPlatForm.SmsSpgate.SendType, model.SendType)
                .AddColumn(SmsPlatForm.SmsSpgate.IsWebService, model.IsWebService)
                .AddColumn(SmsPlatForm.SmsSpgate.EncodeType, model.EncodeType)
                .AddColumn(SmsPlatForm.SmsSpgate.TimeOut, model.TimeOut)
                .AddColumn(SmsPlatForm.SmsSpgate.MtScriptID, model.MtScriptID)
                .AddColumn(SmsPlatForm.SmsSpgate.MoScriptID, model.MoScriptID)
                .AddColumn(SmsPlatForm.SmsSpgate.ReportScriptID, model.ReportScriptID)
                .Where(SmsPlatForm.SmsSpgate.GateNumber == model.GateNumber).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
		}
        /// <summary>
        /// 删除一条数据
        /// </summary>
        public bool Delete(string GateNumber)
        {
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.SmsSpgate)
                .Where(SmsPlatForm.SmsSpgate.GateNumber == GateNumber).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
		/// <summary>
		/// 删除多条数据
		/// </summary>
        public bool Delete(string[] GateNumberArray)
		{
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.SmsSpgate)
                .Where(SmsPlatForm.SmsSpgate.GateNumber.In(GateNumberArray)).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public DataSet GetSelectSpgate(string strSpgate)
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsSpgate).Where(SmsPlatForm.SmsSpgate.GateNumber == strSpgate).ToDataSet();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="iStart"></param>
        /// <param name="iLimit"></param>
        /// <param name="iType"></param>
        /// <param name="iBusiness"></param>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public DataSet GetList(int iStart, int iLimit, int iType, int iBusiness, string strSpgate, ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            if (strSpgate != "")
            {
                strWhere.AppendFormat(" and GateNumber='{0}'", strSpgate);
            }
            if (iType != -1)
            {
                strWhere.AppendFormat(" and GateType={0}", iType);
            }
            if (iBusiness != -1)
            {
                strWhere.AppendFormat(" and BusinessID={0}", iBusiness);
            }
            string strNumSQL = string.Format("select count(ID) from SmsSpgate {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from SmsSpgate {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} * from SmsSpgate {1} {2} order by id asc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount);            
        }

        public DataSet GetSpgate(string strUserID)
        {
            DataSet ds = MyDataBase.db.Select(SmsPlatForm.SmsManager, SmsPlatForm.SmsManager.PortSet, SmsPlatForm.SmsManager.ManagerType).Where(SmsPlatForm.SmsManager.ManagerID == strUserID).ToDataSet();
            string strSQL  = string.Empty;
            string strType = ds.Tables[0].Rows[0]["ManagerType"].ToString();
            string strSpgates = ds.Tables[0].Rows[0]["PortSet"].ToString();
            if (strType == "1")
            {
                strSQL = "select a.gatenumber as value,a.gatetype,b.BusinessName +'---'+a.gatename +'---' as [name]  from smsspgate a right join SmsBusiness b on a.businessid = b.businessid where a.gatenumber in(" + strSpgates + ") order by gatetype asc";
            }
            else
            {
                strSQL = "select a.gatenumber as value,a.gatetype,b.BusinessName +'---'+a.gatename +'---' as [name]  from smsspgate a right join SmsBusiness b on a.businessid = b.businessid  order by gatetype asc";
            }
            return DbHelperSQL.Query(strSQL);
        }

        public DataSet GetSpgateFields(string strSpgate)
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsSpgate,
                SmsPlatForm.SmsSpgate.GateType,
                SmsPlatForm.SmsSpgate.MobileLen,
                SmsPlatForm.SmsSpgate.UnicomLen,
                SmsPlatForm.SmsSpgate.TeleComLen,
                SmsPlatForm.SmsSpgate.ChinaPhoneLen).Where(SmsPlatForm.SmsSpgate.GateNumber == strSpgate).ToDataSet();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="strSpgates"></param>
        /// <returns></returns>
        public DataSet GetSpgateEx(string strUserID, out string strSpgates)
        {
            strSpgates = "";
            DataSet ds = MyDataBase.db.Select(SmsPlatForm.SmsManager, SmsPlatForm.SmsManager.PortSet).Where(SmsPlatForm.SmsManager.ManagerID == strUserID).ToDataSet();
            strSpgates = ds.Tables[0].Rows[0]["PortSet"].ToString();
            string strSQL = "select a.gatenumber as id,'setport' as iconCls,'1' as leaf,a.gatetype,b.BusinessName +'---'+a.gatename +'---' as [text]  from smsspgate a left join SmsBusiness b on a.businessid = b.businessid";
            return DbHelperSQL.Query(strSQL);
        }
		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public Model.SmsSpgate GetModel(string GateNumber)
		{
            DataSet ds = GetSelectSpgate(GateNumber);
            int iCount = ds.Tables[0].Rows.Count;
            if (iCount <= 0)
            {
                return null;
            }
            //转换成实体
            return ObjectConvertor.ToObject<Model.SmsSpgate>(ds.Tables[0].Rows[0]);
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

