using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsManager。
	/// </summary>
	public class SmsManager
	{
		public SmsManager()
		{}
		#region  成员方法

		/// <summary>
		/// 是否存在该记录
		/// </summary>
		public bool Exists(string ManagerID)
		{
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsManager).Where(SmsPlatForm.SmsManager.ManagerID == ManagerID).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
		}
        /// <summary>
        /// 是否存在该记录,判断密码是否正确
        /// </summary>
        public bool Exists(SmsServer.Model.SmsManager model)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsManager)
                .Where(SmsPlatForm.SmsManager.ManagerID == model.ManagerID
                && SmsPlatForm.SmsManager.ManagerPwd == model.ManagerPwd).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
        /// <summary>
        /// 判断管理员是否有对应的权限
        /// </summary>
        /// <param name="strCodeFile"></param>
        /// <param name="strManagerID"></param>
        /// <returns></returns>
        public static bool HasPower(string strCodeFile,string strManagerID)
        {
            return true;
        }
		/// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(SmsServer.Model.SmsManager model)
		{
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.SmsManager)
                .AddColumn(SmsPlatForm.SmsManager.ManagerID, model.ManagerID)
                .AddColumn(SmsPlatForm.SmsManager.ManagerPwd, model.ManagerPwd)
                .AddColumn(SmsPlatForm.SmsManager.UserName, model.UserName)
                .AddColumn(SmsPlatForm.SmsManager.CreatTime, DateTime.Now)
                .AddColumn(SmsPlatForm.SmsManager.ManagerType, model.ManagerType)
                .AddColumn(SmsPlatForm.SmsManager.CompanyName, model.CompanyName)
                .AddColumn(SmsPlatForm.SmsManager.Phone, model.Phone)
                .AddColumn(SmsPlatForm.SmsManager.Address, model.Address)
                .AddColumn(SmsPlatForm.SmsManager.ExtraInfo, model.ExtraInfo).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
            }
            return false;
		}
		/// <summary>
		/// 更新一条数据
		/// </summary>
		public bool Update(SmsServer.Model.SmsManager model)
		{
            try
            {
                //清空权限
                MyDataBase.db.Update(SmsPlatForm.SmsManager)
                .AddColumn(SmsPlatForm.SmsManager.Power, "")
                .Where(SmsPlatForm.SmsManager.ManagerID == model.ManagerID && SmsPlatForm.SmsManager.ManagerType != model.ManagerType).Execute();
                //修改其他数据
                MyDataBase.db.Update(SmsPlatForm.SmsManager)
                .AddColumn(SmsPlatForm.SmsManager.UserName, model.UserName)
                .AddColumn(SmsPlatForm.SmsManager.ManagerType, model.ManagerType)
                .AddColumn(SmsPlatForm.SmsManager.CompanyName, model.CompanyName)
                .AddColumn(SmsPlatForm.SmsManager.Phone, model.Phone)
                .AddColumn(SmsPlatForm.SmsManager.Address, model.Address)
                .AddColumn(SmsPlatForm.SmsManager.ExtraInfo, model.ExtraInfo)
                .Where(SmsPlatForm.SmsManager.ManagerID == model.ManagerID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
		}
        /// <summary>
        /// 删除多条数据
        /// </summary>
        public bool Delete(string[] ManagerIDArray)
        {
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.SmsManager)
                .Where(SmsPlatForm.SmsManager.ManagerID.In(ManagerIDArray)).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
		/// <summary>
		/// 删除一条数据
		/// </summary>
		public bool Delete(string ManagerID)
		{
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.SmsManager)
                .Where(SmsPlatForm.SmsManager.ManagerID == ManagerID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            } 
		}
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        public bool SetPower(string strUserID, string strPower)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.SmsManager).AddColumn(SmsPlatForm.SmsManager.Power, strPower)
                .Where(SmsPlatForm.SmsManager.ManagerID == strUserID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }           
        }
        /// <summary>
        /// 修改密码数据库操作
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        public bool ModifyPwd(SmsServer.Model.SmsManager model)
        {
            try
            {
                //修改密码
                MyDataBase.db.Update(SmsPlatForm.SmsManager)
                .AddColumn(SmsPlatForm.SmsManager.ManagerPwd, model.NewManagerPwd)
                .Where(SmsPlatForm.SmsManager.ManagerID == model.ManagerID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
        /// <summary>
        /// 返回指定管理员的数据
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public DataSet GetSelectManager(string strUserID)
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsManager).Where(SmsPlatForm.SmsManager.ManagerID == strUserID).ToDataSet();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="iStart"></param>
        /// <param name="iLimit"></param>
        /// <param name="iType"></param>
        /// <param name="strUserID"></param>
        /// <param name="iTotalCount"></param>
        /// <returns></returns>
        public DataSet GetList(int iStart, int iLimit, int iType, string strUserID,ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            if (strUserID != "")
            {
                strWhere.AppendFormat(" and ManagerID='{0}'", strUserID);
            }
            if (iType != -1)
            {
                strWhere.AppendFormat(" and ManagerType={0}", iType);
            }
            string strNumSQL = string.Format("select count(ID) from SmsManager {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from SmsManager {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} ManagerID,ManagerType,UserName,CreatTime,CompanyName,Address,Phone,ExtraInfo,Balance,Used,str(ManagerType)+','+ManagerID as PortPower from SmsManager {1} {2} order by id asc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount);
            /*WhereClip where = SmsPlatForm.SmsManager.ID != -1;
            if (iType != -1)
            {
                where = SmsPlatForm.SmsManager.ManagerType == iType;
            }
            if (strUserID != "")
            {
                where = where && SmsPlatForm.SmsManager.ManagerID == strUserID;
            }
            return MyDataBase.db.Select(SmsPlatForm.SmsManager, SmsPlatForm.SmsManager.ManagerID,
                SmsPlatForm.SmsManager.ManagerType,
                SmsPlatForm.SmsManager.UserName,
                SmsPlatForm.SmsManager.CompanyName, 
                SmsPlatForm.SmsManager.Address,
                SmsPlatForm.SmsManager.Phone,
                SmsPlatForm.SmsManager.ExtraInfo).SetSelectRange(iLimit, iStart, SmsPlatForm.SmsManager.ID).Where(where).ToDataSet();*/
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="strSpgates"></param>
        /// <returns></returns>
        public bool SetSpgates(string strUserID, string strSpgates)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.SmsManager).AddColumn(SmsPlatForm.SmsManager.PortSet, strSpgates)
                .Where(SmsPlatForm.SmsManager.ManagerID == strUserID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }    
        }
		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsManager GetModel(string ManagerID,int ID)
		{
            return null;
		}

		/// <summary>
		/// 获得数据列表
		/// </summary>
		public DataSet GetList(string strWhere)
		{
            return null;
		}

		/// <summary>
		/// 获得前几行数据
		/// </summary>
		public DataSet GetList(int Top,string strWhere,string filedOrder)
		{
            return null;
		}
		#endregion  成员方法
	}
}

