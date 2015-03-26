using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsReceived 的摘要说明。
	/// </summary>
	public class SmsReceived
	{
		private readonly SmsServer.DAL.SmsReceived dal=new SmsServer.DAL.SmsReceived();
		public SmsReceived()
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
		public int  Add(SmsServer.Model.SmsReceived model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsReceived model)
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
		public SmsServer.Model.SmsReceived GetModel(long ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// 得到一个对象实体，从缓存中。
		/// </summary>
		public SmsServer.Model.SmsReceived GetModelByCache(long ID)
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
		public List<SmsServer.Model.SmsReceived> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsReceived> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsReceived> modelList = new List<SmsServer.Model.SmsReceived>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsReceived model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsReceived();
					//model.ID=dt.Rows[n]["ID"].ToString();
					model.Phone=dt.Rows[n]["Phone"].ToString();
					model.Content=dt.Rows[n]["Content"].ToString();
					model.SpGateNo=dt.Rows[n]["SpGateNo"].ToString();
					if(dt.Rows[n]["RecvTime"].ToString()!="")
					{
						model.RecvTime=DateTime.Parse(dt.Rows[n]["RecvTime"].ToString());
					}
					model.RecvUserID=dt.Rows[n]["RecvUserID"].ToString();
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

