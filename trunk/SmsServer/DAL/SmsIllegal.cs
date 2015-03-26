using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using NBearLite;//请先添加引用
using NBear.Mapping;
namespace SmsServer.DAL
{
    public class SmsIllegal
    {
        /// <summary>
        /// 是否存在该记录
        /// </summary>
        public bool Exists(string strKeyWord)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsIllegal).Where(SmsPlatForm.SmsIllegal.KeyWord == strKeyWord).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
        /// <summary>
        /// 是否存在该记录
        /// </summary>
        public bool Exists(int iID,string strKeyWord)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.SmsIllegal).Where(SmsPlatForm.SmsIllegal.KeyWord == strKeyWord && SmsPlatForm.SmsIllegal.id != iID).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
        //添加
        public bool Add(string strKeyWord)
        {
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.SmsIllegal)
                .AddColumn(SmsPlatForm.SmsIllegal.BelongSpgate, "")
                .AddColumn(SmsPlatForm.SmsIllegal.KeyWord, strKeyWord).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                return false;
            }            
        }
        //修改
        public bool Update(int iID,string strKeyWord)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.SmsIllegal)
                .AddColumn(SmsPlatForm.SmsIllegal.KeyWord, strKeyWord)
                .Where(SmsPlatForm.SmsIllegal.id == iID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                return false;
            }
        }
        //返回所以非法关键词
        public DataSet GetAllIllegalWords()
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsIllegal).ToDataSet();
        }

        public DataSet GetIllegalList(int iStart, int iLimit, ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            string strNumSQL = string.Format("select count(ID) from SmsIllegal {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from SmsIllegal {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} * from SmsIllegal {1} {2} order by id asc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount); 
        }
        public bool Delete(string[] strArray)
        {
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.SmsIllegal)
                .Where(SmsPlatForm.SmsIllegal.id.In(strArray)).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
    }
}
