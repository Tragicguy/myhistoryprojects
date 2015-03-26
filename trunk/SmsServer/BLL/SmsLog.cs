using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
namespace SmsServer.BLL
{
    public class SmsLog
    {
        private readonly SmsServer.DAL.SmsLog dal = new SmsServer.DAL.SmsLog();
        public void WriteDataBaseLog(int iType, string strDetail, string strOperator, string strModule)
        {
            SmsServer.Model.sysLog objLog = new SmsServer.Model.sysLog();
            objLog.log_type = iType;
            objLog.log_detail = strDetail;
            objLog.operatorID = strOperator;
            objLog.op_module = strModule;
            objLog.log_time = DateTime.Now;
            dal.AddLog(objLog);
        }
        public string GetLogList(int iStart,int iLimit,int iType,string strOptID,string strDate)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetLogList(iStart, iLimit, iType, strOptID,strDate,ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
    }
    
}
