using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using NBearLite;//请先添加引用
using System.Collections;
namespace SmsServer.DAL
{
    public class MapScriptDetail
    {
        /// <summary>
        /// 增加一条数据
        /// </summary>
        public bool Add(Model.MapScriptDetail model)
        {
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.MapScriptDetail)
                .AddColumn(SmsPlatForm.MapScriptDetail.MapID, model.MapID)
                .AddColumn(SmsPlatForm.MapScriptDetail.MapType, model.MapType)
                .AddColumn(SmsPlatForm.MapScriptDetail.SrcValue, model.SrcValue)
                .AddColumn(SmsPlatForm.MapScriptDetail.MapRetValue, model.MapRetValue)
                .AddColumn(SmsPlatForm.MapScriptDetail.RetUseValue, model.RetUseValue)
                .AddColumn(SmsPlatForm.MapScriptDetail.RetString, model.RetString)
                .AddColumn(SmsPlatForm.MapScriptDetail.CharNum, model.CharNum)
                .AddColumn(SmsPlatForm.MapScriptDetail.SplitChar, model.SplitChar).Execute();
                return true;
            }
            catch (System.Exception e)
            {

            }
            return false;
        }
        /// <summary>
        /// 更新一条数据
        /// </summary>
        public bool Update(Model.MapScriptDetail model)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.MapScriptDetail)
                //.AddColumn(SmsPlatForm.MapScriptDetail.MapID, model.MapID)
                .AddColumn(SmsPlatForm.MapScriptDetail.MapType, model.MapType)
                .AddColumn(SmsPlatForm.MapScriptDetail.SrcValue, model.SrcValue)
                .AddColumn(SmsPlatForm.MapScriptDetail.MapRetValue, model.MapRetValue)
                .AddColumn(SmsPlatForm.MapScriptDetail.RetUseValue, model.RetUseValue)
                .AddColumn(SmsPlatForm.MapScriptDetail.RetString, model.RetString)
                .AddColumn(SmsPlatForm.MapScriptDetail.CharNum, model.CharNum)
                .AddColumn(SmsPlatForm.MapScriptDetail.SplitChar, model.SplitChar)
                .Where(SmsPlatForm.MapScriptDetail.id == model.id).Execute();
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
        /// <param name="strArray"></param>
        /// <returns></returns>
        public bool Delete(string[] strArray)
        {
            try
            {
                MyDataBase.db.Delete(SmsPlatForm.MapScriptDetail)
                .Where(SmsPlatForm.MapScriptDetail.id.In(strArray)).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
        public DataSet GetEasyScriptList(int iStart,int iLimit,string strMapID,ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.AppendFormat(" where MapID='{0}'", strMapID);
            string strNumSQL = string.Format("select count(id) from MapScriptDetail {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from MapScriptDetail {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} * from MapScriptDetail {1} {2} order by id asc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount);  
        }

    }
}
