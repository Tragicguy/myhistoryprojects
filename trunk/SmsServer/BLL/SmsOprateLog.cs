using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsOprateLog 的摘要说明。
	/// </summary>
	public class SmsOprateLog
	{
		private readonly SmsServer.DAL.SmsOprateLog dal=new SmsServer.DAL.SmsOprateLog();
		public SmsOprateLog()
		{}
		#region  成员方法
		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(long ID)
		{
            return false;
		}

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public int  Add(SmsServer.Model.SmsOprateLog model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsOprateLog model)
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
		public SmsServer.Model.SmsOprateLog GetModel(long ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// 得到一个对象实体，从缓存中。
		/// </summary>
		public SmsServer.Model.SmsOprateLog GetModelByCache(long ID)
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
		public List<SmsServer.Model.SmsOprateLog> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsOprateLog> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsOprateLog> modelList = new List<SmsServer.Model.SmsOprateLog>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsOprateLog model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsOprateLog();
					//model.ID=dt.Rows[n]["ID"].ToString();
					model.OpraterType=dt.Rows[n]["OpraterType"].ToString();
					model.OpraterID=dt.Rows[n]["OpraterID"].ToString();
					model.OprationInfo=dt.Rows[n]["OprationInfo"].ToString();
					if(dt.Rows[n]["OprateTime"].ToString()!="")
					{
						model.OprateTime=DateTime.Parse(dt.Rows[n]["OprateTime"].ToString());
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

