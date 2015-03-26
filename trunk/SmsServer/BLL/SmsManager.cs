using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
using Newtonsoft.Json;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsManager 的摘要说明。
	/// </summary>
	public class SmsManager
	{
		private readonly SmsServer.DAL.SmsManager dal=new SmsServer.DAL.SmsManager();
		public SmsManager()
		{}
		#region  成员方法
		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string ManagerID)
		{
			return dal.Exists(ManagerID);
		}
        public bool ManagerLogin(string strID,string strPwd)
        {
            Model.SmsManager objMgr = new Model.SmsManager();
            objMgr.ManagerID = strID;
            objMgr.ManagerPwd = strPwd;
            return dal.Exists(objMgr);
        }
        /// <summary>
        /// 判断是否有相应的权限
        /// </summary>
        /// <param name="strPower"></param>
        /// <param name="iPower"></param>
        /// <returns></returns>
        public static int HasPower(string strFile,object objSession,out string strout)
        {
            try
            {
                if (object.Equals(objSession, null))
                {
                    strout = "未登录或登录超时";
                    return -1;
                }
                string strManagerID = objSession.ToString();
                if (SmsServer.DAL.SmsManager.HasPower(strFile, strManagerID))
                {
                    strout = "";
                    return 0;
                }
                strout = "非法操作";
                return -2;      //无权限
            }
            catch (System.Exception e)
            {
                strout = "服务器异常";
                return -2;
            }
            
        }
		/// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(string strJson,out string strout)
		{
            SmsServer.Model.SmsManager model = (SmsServer.Model.SmsManager)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsManager));
            if (dal.Exists(model.ManagerID))
            {
                strout = "此账号已存在,请输入其他ID号";
                return false;
            }
            strout = "添加失败";
            return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public bool Update(string strJson)
		{
            SmsServer.Model.SmsManager model = (SmsServer.Model.SmsManager)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsManager));            
			return dal.Update(model);
		}
        /// <summary>
        /// 删除多条数据
        /// </summary>
        public bool Delete(string[] ManagerIDArray)
        {
            return dal.Delete(ManagerIDArray);
        }
		/// <summary>
		/// 删除一条数据
		/// </summary>
		public bool Delete(string ManagerID)
		{
			return dal.Delete(ManagerID);
		}
        /// <summary>
        /// 设置权限
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="strPower"></param>
        /// <returns></returns>
        public bool SetPower(string strUserID, string strPower)
        {
            return dal.SetPower(strUserID, strPower);
        }
        /// <summary>
        /// 修改管理员密码
        /// </summary>
        /// <param name="strJson"></param>
        /// <returns></returns>
        public bool ModifyPwd(string strJson,string strUserID,out string strErrorInfo)
        {
            SmsServer.Model.SmsManager model = (SmsServer.Model.SmsManager)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsManager));
            model.ManagerID = strUserID;
            //判断密码是否正确
            if (!dal.Exists(model))
            {
                strErrorInfo = "原密码错误,修改失败";
                return false;
            }
            strErrorInfo = "修改密码失败";
            return dal.ModifyPwd(model);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string GetManagerList(int iStart,int iLimit,int iType,string strUserID)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetList(iStart, iLimit, iType, strUserID, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsManager GetModel(string ManagerID,int ID)
		{
			
			return dal.GetModel(ManagerID,ID);
		}
        /// <summary>
        /// 返回管理员JSON数据
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSelectManager(string strUserID)
        {
            DataSet ds = dal.GetSelectManager(strUserID);
            ds.Tables[0].Columns.Remove("ManagerPwd");
            string strJson = "{success:true,data:";
            strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
            return strJson;
        }
        public bool SetSpgates(string strUserID, string strPorts)
        {
            return dal.SetSpgates(strUserID, strPorts);
        }
		/// <summary>
		/// 得到一个对象实体，从缓存中。
		/// </summary>
		public SmsServer.Model.SmsManager GetModelByCache(string ManagerID,int ID)
		{
            return null;
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
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsManager> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsManager> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsManager> modelList = new List<SmsServer.Model.SmsManager>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsManager model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsManager();
					if(dt.Rows[n]["ID"].ToString()!="")
					{
						model.ID=int.Parse(dt.Rows[n]["ID"].ToString());
					}
					model.ManagerID=dt.Rows[n]["ManagerID"].ToString();
					model.ManagerPwd=dt.Rows[n]["ManagerPwd"].ToString();
					if(dt.Rows[n]["ManagerType"].ToString()!="")
					{
						model.ManagerType=int.Parse(dt.Rows[n]["ManagerType"].ToString());
					}
					model.Power=dt.Rows[n]["Power"].ToString();
					model.UserName=dt.Rows[n]["UserName"].ToString();
					model.CompanyName=dt.Rows[n]["CompanyName"].ToString();
					model.Phone=dt.Rows[n]["Phone"].ToString();
					model.Address=dt.Rows[n]["Address"].ToString();
					model.ExtraInfo=dt.Rows[n]["ExtraInfo"].ToString();
					modelList.Add(model);
				}
			}
			return modelList;
		}

		/// <summary>
		/// 获得数据列表
		/// </summary>
		public DataSet GetAllList()
		{
			return GetList("");
		}

		/// <summary>
		/// 获得数据列表
		/// </summary>
		//public DataSet GetList(int PageSize,int PageIndex,string strWhere)
		//{
			//return dal.GetList(PageSize,PageIndex,strWhere);
		//}

		#endregion  成员方法
	}
}

