using System;
using System.Collections.Generic;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
using System.Data;
namespace SmsServer.DAL
{
    public class SmsLog
    {
        public void AddLog(SmsServer.Model.sysLog model)
        {
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.sysLog)
                .AddColumn(SmsPlatForm.sysLog.log_type, model.log_type)
                .AddColumn(SmsPlatForm.sysLog.log_detail, model.log_detail)
                .AddColumn(SmsPlatForm.sysLog.log_time, model.log_time)
                .AddColumn(SmsPlatForm.sysLog.operatorID, model.operatorID)
                .AddColumn(SmsPlatForm.sysLog.op_module, model.op_module).Execute();
            }
            catch
            {
            	
            }
        }
        public DataSet GetLogList(int iStart, int iLimit, int iType, string strOptID, string strDate, ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            if (strOptID != "")
            {
                strWhere.AppendFormat(" and operatorID ='{0}'", strOptID);
            }
            if (iType != -1)
            {
                strWhere.AppendFormat(" and log_type={0}", iType);
            }
            if (strDate == "")
            {
                strDate = DateTime.Now.ToString();                
            }
            strWhere.AppendFormat(" and DATEDIFF(dd,convert(datetime,'{0}',120),log_time) = 0", strDate);

            string strNumSQL = string.Format("select count(log_id) from sysLog {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and log_id not in (select top {0} log_id from sysLog {1} order by log_id desc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} * from sysLog {1} {2} order by log_id desc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount);    
        }
    }
}
