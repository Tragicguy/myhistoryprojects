using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsTaskB 的摘要说明。
	/// </summary>
	public class SmsTaskB
	{
		private readonly SmsServer.DAL.SmsTaskB dal=new SmsServer.DAL.SmsTaskB();
		public SmsTaskB()
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
		public int  Add(SmsServer.Model.SmsTaskB model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsTaskB model)
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
		public SmsServer.Model.SmsTaskB GetModel(long ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// 得到一个对象实体，从缓存中。
		/// </summary>
		public SmsServer.Model.SmsTaskB GetModelByCache(long ID)
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
		public List<SmsServer.Model.SmsTaskB> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsTaskB> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsTaskB> modelList = new List<SmsServer.Model.SmsTaskB>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsTaskB model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsTaskB();
					//model.ID=dt.Rows[n]["ID"].ToString();
					model.PhoneList=dt.Rows[n]["PhoneList"].ToString();
					model.MsgContent=dt.Rows[n]["MsgContent"].ToString();
					if(dt.Rows[n]["SubTime"].ToString()!="")
					{
						model.SubTime=DateTime.Parse(dt.Rows[n]["SubTime"].ToString());
					}
					if(dt.Rows[n]["SendTime"].ToString()!="")
					{
						model.SendTime=DateTime.Parse(dt.Rows[n]["SendTime"].ToString());
					}
					model.UserID=dt.Rows[n]["UserID"].ToString();
					model.ParentID=dt.Rows[n]["ParentID"].ToString();
					model.SpgateNo=dt.Rows[n]["SpgateNo"].ToString();
					model.SubPort=dt.Rows[n]["SubPort"].ToString();
					model.SubPortEx=dt.Rows[n]["SubPortEx"].ToString();
					if(dt.Rows[n]["FeeNum"].ToString()!="")
					{
						model.FeeNum=int.Parse(dt.Rows[n]["FeeNum"].ToString());
					}
					if(dt.Rows[n]["PhoneNum"].ToString()!="")
					{
						model.PhoneNum=int.Parse(dt.Rows[n]["PhoneNum"].ToString());
					}
					if(dt.Rows[n]["MsgLen"].ToString()!="")
					{
						model.MsgLen=int.Parse(dt.Rows[n]["MsgLen"].ToString());
					}
					if(dt.Rows[n]["HasIndex"].ToString()!="")
					{
						model.HasIndex=int.Parse(dt.Rows[n]["HasIndex"].ToString());
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

