using System;
using System.Data;
using System.Collections.Generic;

using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsLoginLog 的摘要说明。
	/// </summary>
	public class SmsLoginLog
	{
		private readonly SmsServer.DAL.SmsLoginLog dal=new SmsServer.DAL.SmsLoginLog();
		public SmsLoginLog()
		{}
		#region  成员方法
		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(long ID)
		{
		//	return dal.Exists(ID);
            return false;
        }

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public int  Add(SmsServer.Model.SmsLoginLog model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsLoginLog model)
		{
			dal.Update(model);
		}

		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{
			
			dal.Delete(ID);
		}

		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsLoginLog GetModel(long ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// 得到一个对象实体，从缓存中。
		/// </summary>
		public SmsServer.Model.SmsLoginLog GetModelByCache(long ID)
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
		public List<SmsServer.Model.SmsLoginLog> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsLoginLog> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsLoginLog> modelList = new List<SmsServer.Model.SmsLoginLog>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsLoginLog model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsLoginLog();
					//model.ID=dt.Rows[n]["ID"].ToString();
					if(dt.Rows[n]["LoginTime"].ToString()!="")
					{
						model.LoginTime=DateTime.Parse(dt.Rows[n]["LoginTime"].ToString());
					}
					model.LoginIp=dt.Rows[n]["LoginIp"].ToString();
					model.LoginUser=dt.Rows[n]["LoginUser"].ToString();
					model.LoginPwd=dt.Rows[n]["LoginPwd"].ToString();
					model.LoginResult=dt.Rows[n]["LoginResult"].ToString();
					if(dt.Rows[n]["LoginType"].ToString()!="")
					{
						model.LoginType=int.Parse(dt.Rows[n]["LoginType"].ToString());
					}
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

