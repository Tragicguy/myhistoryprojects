using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
using Newtonsoft.Json;
using System.Collections;
using System.Text;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsSpgate 的摘要说明。
	/// </summary>
	public class SmsSpgate
	{
		private readonly SmsServer.DAL.SmsSpgate dal=new SmsServer.DAL.SmsSpgate();
		public SmsSpgate()
		{}
		#region  成员方法
		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string GateNumber)
		{
			return dal.Exists(GateNumber);
		}
        public bool ExistsEx(string GateNumber)
        {
            string strSql = string.Format("select count(*) from SmsSpgate where GateNumber = '{0}'",GateNumber);
            int iRet = dal.RetCount(strSql);
            return iRet > 0 ? true : false;
        }
		/// <summary>
		/// 增加一条数据
		/// </summary>
        public bool Add(string strJson, out string strout)
		{
            SmsServer.Model.SmsSpgate model = (SmsServer.Model.SmsSpgate)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsSpgate));
            if (dal.ExistsExEx(model))
            {
                strout = "通道号或通道名已存在";
                return false;
            }
            strout = "添加通道失败";
            //把参数字段匹配好            
            UpdateSendParams(model);
            return dal.Add(model);
		}

		/// <summary>
        /// 更新一条数据
		/// </summary>
		public bool Update(string strJson,out string strout)
		{
            SmsServer.Model.SmsSpgate model = (SmsServer.Model.SmsSpgate)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsSpgate));
            if (dal.ExistsEx(model))
            {
                strout = "通道名已经存在";
                return false;
            }
            strout = "更新通道失败";
            //把参数字段匹配好
            UpdateSendParams(model);            
            bool bRet = dal.Update(model);
            if (bRet)
            {
                DataCache.DeleteCacheSpgate(model.GateNumber);
            }
            //清除缓存
            return bRet;
		}

		/// <summary>
		/// 删除多条数据
		/// </summary>
		public bool Delete(string[] strArray)
		{
            bool bRet = dal.Delete(strArray);
            //清除缓存
            if (bRet)
            {
                foreach (string strNumber in strArray)
                {
                    DataCache.DeleteCacheSpgate(strNumber);
                }
            }
            return bRet;
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public string GetSelectSpgate(string strSpgate)
        {
            DataSet ds = dal.GetSelectSpgate(strSpgate);
            if (ds.Tables[0].Rows.Count > 0)
            {
                string strJson = "{success:true,data:";
                strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
                return strJson;
            }
            else
            {
                return "{success:true,errorInfo:'无此通道数据'}";
            }
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
        public string GetSpgateList(int iStart, int iLimit, int iType,int iBusiness, string strSpgate)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetList(iStart, iLimit, iType,iBusiness, strSpgate, ref iTotalCount);
            //移除多余字段
            int iCount =  ds.Tables[0].Columns.Count;
            for (int i=16;i<iCount;i++)
            {
                ds.Tables[0].Columns.RemoveAt(16);
            }            
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSpgate(string strUserID)
        {
            DataSet ds = dal.GetSpgate(strUserID);
            foreach (DataRow row in ds.Tables[0].Rows)
            {
                row["name"] = row["name"].ToString() + row["value"] + "[" + ReturnPortString(row["gatetype"].ToString()) + "]";
            }           
            return JSonClass.DataTableToJsonEx(ds.Tables[0], ds.Tables[0].Rows.Count);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSpgateEx(string strUserID)
        {
            string strSpgates = string.Empty;
            DataSet ds = dal.GetSpgateEx(strUserID, out strSpgates);
            string[] strArray = strSpgates.Split(',');
            Hashtable powerDt = new Hashtable();
            for (int i = 0; i < strArray.Length; i++)
            {
                powerDt.Add(strArray[i], 1);
            }
            int iCount = ds.Tables[0].Rows.Count;
            ds.Tables[0].Columns.Add("checked");
            foreach (DataRow row in ds.Tables[0].Rows)
            {
                if (powerDt.Contains(row["id"].ToString()))
                {
                    row["checked"] = 1;
                }
                else
                {
                    row["checked"] = 0;
                }
                row["text"] = row["text"].ToString() + ReturnPortString(row["gatetype"].ToString()) + "[" + row["id"] + "]";
            }
            ds.Tables[0].Columns.Remove("gatetype");
            Hashtable hashDt = new Hashtable();
            hashDt.Add(3, 1);
            return JSonClass.DataTableToJson(ds.Tables[0], hashDt);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public string GetSpgateFields(string strSpgate)
        {
             
            DataSet ds = dal.GetSpgateFields(strSpgate);
            if (ds.Tables[0].Rows.Count > 0)
            {                
                string strJson = "{success:true,data:";
                strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
                return strJson;
            }
            else
            {
                return "{success:true,errorInfo:'无此通道数据'}";
            }
        }
        
		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public Model.SmsSpgate GetModel(string GateNumber)
		{
            //先从缓存中取，取到则返回，取不到则从数据库中取
            return dal.GetModel(GateNumber);
		}	

		/// <summary>
		/// 获得数据列表
		/// </summary>
		public DataSet GetList(string strWhere)
		{
			return dal.GetList(strWhere);
		}
		/// <summary>
		/// 获得前几行数据
		/// </summary>
		public DataSet GetList(int Top,string strWhere,string filedOrder)
		{
			return dal.GetList(Top,strWhere,filedOrder);
		}
        private string ReturnPortString(string strType)
        {
            switch (strType)
            {
                case "1":
                    return "全网";
                case "2":
                    return "三网";
                case "3":
                    return "移动";
                case "4":
                    return "联通";
                case "5":
                    return "电信";
                case "6":
                    return "小灵通";
                default:
                    return "未知号段";
            }
        }
        /// <summary>
        /// 更新通道的参数字符串
        /// </summary>
        /// <param name="model"></param>
        private void UpdateSendParams(SmsServer.Model.SmsSpgate model)
        {
            StringBuilder objBuilder = new StringBuilder();
            objBuilder.Append(model.UseridParamString + "=" + model.UserID + "&");
            objBuilder.Append(model.PwdParamString + "=" + model.UserPwd + "&");
            objBuilder.Append(model.MobileParamString + "={0}&");
            objBuilder.Append(model.MsgParamString + "={1}&");
            objBuilder.Append(model.PhoneNumParamString + "={2}&");
            objBuilder.Append(model.SubportParamString + "={3}&");
            objBuilder.Append(model.GateNoParamString + "={4}");
            model.SendParams = objBuilder.ToString();
//             model.UseridParamString = "";
//             model.PwdParamString = "";
//             model.UserID = "";
//             model.UserPwd = "";
        }
		#endregion  成员方法
	}
}

