using System;
using System.Data;
using System.Collections.Generic;

using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类SmsRetValue 的摘要说明。
	/// </summary>
	public class SmsRetValue
	{
		private readonly SmsServer.DAL.SmsRetValue dal=new SmsServer.DAL.SmsRetValue();
		public SmsRetValue()
		{}
		#region  成员方法
		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(int ID)
		{
			//return dal.Exists(ID);
            return false;
		}

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public int  Add(SmsServer.Model.SmsRetValue model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsRetValue model)
		{
			dal.Update(model);
		}

		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(int ID)
		{
			
			dal.Delete(ID);
		}

		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsRetValue GetModel(int ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// 得到一个对象实体，从缓存中。
		/// </summary>
		public SmsServer.Model.SmsRetValue GetModelByCache(int ID)
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
		public List<SmsServer.Model.SmsRetValue> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// 获得数据列表
		/// </summary>
		public List<SmsServer.Model.SmsRetValue> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsRetValue> modelList = new List<SmsServer.Model.SmsRetValue>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsRetValue model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsRetValue();
					if(dt.Rows[n]["ID"].ToString()!="")
					{
						model.ID=int.Parse(dt.Rows[n]["ID"].ToString());
					}
					model.SpgateNo=dt.Rows[n]["SpgateNo"].ToString();
					model.RetString=dt.Rows[n]["RetString"].ToString();
					if(dt.Rows[n]["MappingStatus"].ToString()!="")
					{
						model.MappingStatus=int.Parse(dt.Rows[n]["MappingStatus"].ToString());
					}
					if(dt.Rows[n]["MatchType"].ToString()!="")
					{
						model.MatchType=int.Parse(dt.Rows[n]["MatchType"].ToString());
					}
					if(dt.Rows[n]["NeedReSend"].ToString()!="")
					{
						model.NeedReSend=int.Parse(dt.Rows[n]["NeedReSend"].ToString());
					}
					model.ErrorInfo=dt.Rows[n]["ErrorInfo"].ToString();
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

