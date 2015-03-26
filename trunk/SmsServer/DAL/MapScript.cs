using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using NBearLite;//请先添加引用
using System.Collections;
namespace SmsServer.DAL
{
    public class MapScript
    {

        public bool ExistsEx(Model.MapScript model)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.MapScript)
                .Where(SmsPlatForm.MapScript.MapID != model.MapID
                && SmsPlatForm.MapScript.MapName == model.MapName).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
        /// <summary>
        /// 判断映射名称或映射编号是否已经存在
        /// </summary>
        public bool ExistsExEx(SmsServer.Model.MapScript model)
        {
            int iRowCount = MyDataBase.db.Select(SmsPlatForm.MapScript)
                .Where(SmsPlatForm.MapScript.MapID == model.MapID
                || SmsPlatForm.MapScript.MapName == model.MapName).ToDataSet().Tables[0].Rows.Count;
            return iRowCount > 0 ? true : false;
        }
        /// <summary>
        /// 增加一条数据
        /// </summary>
        public bool Add(SmsServer.Model.MapScript model)
        {
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.MapScript)
                .AddColumn(SmsPlatForm.MapScript.MapID, model.MapID)
                .AddColumn(SmsPlatForm.MapScript.MapName, model.MapName)
                .AddColumn(SmsPlatForm.MapScript.ScriptFlag, model.ScriptFlag)
                .AddColumn(SmsPlatForm.MapScript.ScriptType, model.ScriptType)
                .AddColumn(SmsPlatForm.MapScript.ScriptCode, model.ScriptCode)
                .AddColumn(SmsPlatForm.MapScript.EasyScriptCode, "")
                .AddColumn(SmsPlatForm.MapScript.MoSpgate, model.MoSpgate)
                .AddColumn(SmsPlatForm.MapScript.MapInfo, model.MapInfo).Execute();
                return true;
            }
            catch (System.Exception e)
            {

            }
            return false;
        }
        public bool Update(string strMapID,string strEasyCode)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.MapScript)
                .AddColumn(SmsPlatForm.MapScript.EasyScriptCode, strEasyCode)
                .Where(SmsPlatForm.MapScript.MapID == strMapID).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
        /// <summary>
        /// 更新一条数据
        /// </summary>
        public bool Update(SmsServer.Model.MapScript model)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.MapScript)
                .AddColumn(SmsPlatForm.MapScript.MapName, model.MapName)
                .AddColumn(SmsPlatForm.MapScript.ScriptFlag, model.ScriptFlag)
                .AddColumn(SmsPlatForm.MapScript.ScriptType, model.ScriptType)
                .AddColumn(SmsPlatForm.MapScript.ScriptCode, model.ScriptCode)
                .AddColumn(SmsPlatForm.MapScript.MoSpgate, model.MoSpgate)
                .AddColumn(SmsPlatForm.MapScript.MapInfo, model.MapInfo)
                .Where(SmsPlatForm.MapScript.MapID == model.MapID).Execute();
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
                MyDataBase.db.Delete(SmsPlatForm.MapScript)
                .Where(SmsPlatForm.MapScript.MapID.In(strArray)).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
        //获取所有映射脚本
        public DataSet GetAllMapScriptList()
        {
            DataSet ds = MyDataBase.db.Select(SmsPlatForm.MapScript,
                SmsPlatForm.MapScript.MapID,
                SmsPlatForm.MapScript.MapName,
                SmsPlatForm.MapScript.ScriptType).ToDataSet();
            return ds;
        }
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        public DataSet GetMapScriptList(int iStart, int iLimit, string strMapName, int iType,ref int iTotalCount)
        {
            StringBuilder strWhere = new StringBuilder();
            strWhere.Append(" where 1=1");
            if (strMapName != "")
            {
                strWhere.AppendFormat(" and MapName ='{0}'", strMapName);
            }
            if (iType != -1)
            {
                strWhere.AppendFormat(" and ScriptType={0}", iType);
            }
            string strNumSQL = string.Format("select count(ID) from MapScript {0}", strWhere.ToString());
            string strWhereEx = string.Format(" and ID not in (select top {0} id from MapScript {1} order by id asc)", iStart, strWhere.ToString());
            string strDataSQL = string.Format("select top {0} * from MapScript {1} {2} order by id asc",
                iLimit,
                strWhere.ToString(),
                strWhereEx
                );
            int iRet = -1;
            return SmsPlatForm.GetSelectPageData(MyDataBase.db, out iRet, strNumSQL, strDataSQL, ref iTotalCount);    
        }

        public DataSet GetSelectMapScript(string strMapID)
        {
            return MyDataBase.db.Select(SmsPlatForm.MapScript).Where(SmsPlatForm.MapScript.MapID == strMapID).ToDataSet();
        }
    }
}
