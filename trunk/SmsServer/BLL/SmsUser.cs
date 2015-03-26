using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
using Newtonsoft.Json;
using NBear.Mapping;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsUser 的摘要说明。
	/// </summary>
	public class SmsUser
	{
		private readonly SmsServer.DAL.SmsUser dal=new SmsServer.DAL.SmsUser();
		public SmsUser()
		{}
		#region  成员方法
		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string strUserID)
		{
            return dal.Exists(strUserID);
		}

		/// <summary>
		/// 添加用户
		/// </summary>
        public bool Add(string strJson, out string strOut)
		{
            strOut = "添加账户失败";
            strJson = strJson.Replace("\"on\"","\"1\"");
            strJson = strJson.Replace("\"off\"", "\"0\"");
            SmsServer.Model.SmsUser model = (SmsServer.Model.SmsUser)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsUser));
			return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public bool Update(string strJson, out string strOut)
		{
            strOut = "更改账户信息失败";
            strJson = strJson.Replace("\"on\"", "\"1\"");
            strJson = strJson.Replace("\"off\"", "\"0\"");
            SmsServer.Model.SmsUser model = (SmsServer.Model.SmsUser)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsUser));
            return dal.Update(model);
		}
        //修改余额信息
        public bool UpdateBalance(string strUserID, int iFee)
        {
            return dal.UpdateBalance(strUserID, iFee);
        }
        //扣除余额
        public int DeductUserFee(string strUserID, int iFee)
        {
            return dal.DeductUserFee(strUserID, iFee);
        }
		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{			
			dal.Delete(ID);
		}
        /// <summary>
        /// 删除一条数据
        /// </summary>
        public void Delete(string strUserID)
        {
            dal.Delete(strUserID);
        }
        /// <summary>
        /// 删除多条数据
        /// </summary>
        public bool Delete(string[] strArray)
        {
            return dal.Delete(strArray);
        }
        /// <summary>
        /// 获取指定账户，转成JSON
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSelectSmsUser(string strUserID)
        {
            DataSet ds = dal.GetSelectSmsUser(strUserID);
            ds.Tables[0].Columns.Remove("UserPwd");
            string strJson = "{success:true,data:";
            strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
            return strJson;
        }
        public Model.SmsUser GetSelectSmsUser(string strUserID, string strPwd)
        {
            DataSet ds = dal.GetSelectSmsUser(strUserID, strPwd);
            if (ds.Tables[0].Rows.Count > 0)
            {
                return ObjectConvertor.ToObject<DataRow, SmsServer.Model.SmsUser>(ds.Tables[0].Rows[0]);
            }
            else
            {
                return null;
            }
        }
        /// <summary>
        /// 获取用户列表
        /// </summary>
        /// <param name="iStart"></param>
        /// <param name="iLimit"></param>
        /// <param name="strUserID"></param>
        /// <param name="strParentID"></param>
        /// <param name="iStatus"></param>
        /// <returns></returns>
        public string GetUserList(int iStart, int iLimit, string strUserID, string strParentID, int iStatus)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetUserList(iStart, iLimit, strUserID, strParentID, iStatus, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }

		#endregion  成员方法
	}
}

