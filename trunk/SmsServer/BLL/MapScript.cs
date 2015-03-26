using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using NBear.Mapping;
using Newtonsoft.Json;
namespace SmsServer.BLL
{
    public class MapScript
    {
        private readonly DAL.MapScript dal = new SmsServer.DAL.MapScript();
        public string GetMapScriptList(int iStart, int iLimit, string strMapName,int iType)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetMapScriptList(iStart, iLimit, strMapName, iType, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
        public string GetMapScriptList()
        {
            DataSet ds = dal.GetAllMapScriptList();
            if (ds.Tables[0].Rows.Count > 0)
            {
                string strRet =  JSonClass.DataTableToJsonEx(ds.Tables[0], ds.Tables[0].Rows.Count);
                return strRet;
            }
            else
            {
                return "{success:false,errorInfo:'无数据'}";
            }
        }
        /// <summary>
        /// 从数据库加载指定的映射脚本对象
        /// </summary>
        /// <param name="strMapID"></param>
        /// <returns></returns>
        public Model.MapScript GetSelectMapScript(string strMapID)
        {
            DataSet ds = GetSelectMapScriptDS(strMapID );
            int iCount = ds.Tables[0].Rows.Count;
            if (iCount <= 0)
            {
                return null;
            }
            return ObjectConvertor.ToObject<Model.MapScript>(ds.Tables[0].Rows[0]);
        }
        /// <summary>
        /// 返回DS对象
        /// </summary>
        /// <param name="strMapID"></param>
        /// <returns></returns>
        public DataSet GetSelectMapScriptDS(string strMapID)
        {
            return dal.GetSelectMapScript(strMapID);
        }
        public string GetMapScriptFields(string strMapID)
        {
            DataSet ds = dal.GetSelectMapScript(strMapID);
            string strJson = "{success:true,data:";
            strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
            return strJson;
        }
        /// <summary>
        /// 增加一条数据
        /// </summary>
        public bool Add(string strJson, out string strout)
        {
            SmsServer.Model.MapScript model = (SmsServer.Model.MapScript)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.MapScript));
            if (dal.ExistsExEx(model))
            {
                strout = "映射编号或映射名称已存在";
                return false;
            }
            strout = "添加映射失败";
            if (string.IsNullOrEmpty(model.MoSpgate))
            {
                model.MoSpgate = "";
            }
            return dal.Add(model);
        }
        /// <summary>
        /// 更新匹配脚本项
        /// </summary>
        /// <param name="strMapID"></param>
        /// <param name="strEasyCode"></param>
        /// <returns></returns>
        public bool Update(string strMapID,string strEasyCode)
        {
            return dal.Update(strMapID, strEasyCode);
        }
        /// <summary>
        /// 更新一条数据
        /// </summary>
        public bool Update(string strJson, out string strout)
        {
            SmsServer.Model.MapScript model = (SmsServer.Model.MapScript)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.MapScript));
            if (dal.ExistsEx(model))
            {
                strout = "映射名称已经存在";
                return false;
            }
            strout = "更新映射失败";
            //把参数字段匹配好
            if (string.IsNullOrEmpty(model.MoSpgate))
            {
                model.MoSpgate = "";
            }
            bool bRet = dal.Update(model);
            if (bRet)
            {
                DataCache.DeleteCacheMapScript(model.MapID);
            }
            //清除缓存
            return bRet;
        }
        /// <summary>
        /// 删除多条数据
        /// </summary>
        public bool Delete(string[] strArray)
        {
            bool bRet = dal.Delete(strArray);
            //清除缓存
            if (bRet)
            {
                foreach (string strNumber in strArray)
                {
                    DataCache.DeleteCacheMapScript(strNumber);
                }
            }
            return bRet;
        }
    }
}
