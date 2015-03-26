using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using Newtonsoft.Json;
namespace SmsServer.BLL
{
    public class MapScriptDetail
    {
        private readonly SmsServer.DAL.MapScriptDetail dal = new SmsServer.DAL.MapScriptDetail();
        public string GetEasyScriptList(int iStart, int iLimit, string strMapID)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetEasyScriptList(iStart, iLimit,strMapID, ref iTotalCount);            
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
        /// <summary>
        /// 增加一条数据
        /// </summary>
        public bool Add(string strJson,string strMapID, out string strout)
        {
            SmsServer.Model.MapScriptDetail model = (SmsServer.Model.MapScriptDetail)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.MapScriptDetail));
            strout = "添加映射失败";
            model.MapID = strMapID;
            bool bRet = dal.Add(model);
            if (bRet)
            {
                //重新计算脚本结果
                string strEasyCode = GenMapScriptEasy(model.MapID);
                if (new MapScript().Update(model.MapID, strEasyCode))
                {
                    DataCache.DeleteCacheMapScript(model.MapID);//清除缓存
                }
            }
            return bRet;
        }
        /// <summary>
        /// 更新一条数据
        /// </summary>
        public bool Update(string strJson,string strMapID, out string strout)
        {
            SmsServer.Model.MapScriptDetail model = (SmsServer.Model.MapScriptDetail)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.MapScriptDetail));
            strout = "更新映射规则失败";
            model.MapID = strMapID;
            bool bRet = dal.Update(model);
            if (bRet)
            {
                //重新计算脚本结果
                string strEasyCode = GenMapScriptEasy(model.MapID);
                if (new MapScript().Update(model.MapID, strEasyCode))
                {
                    DataCache.DeleteCacheMapScript(model.MapID);//清除缓存
                }
            }
            return bRet;
        }
        /// <summary>
        /// 删除多条数据
        /// </summary>
        public bool Delete(string[] strArray,string strMapID)
        {
            bool bRet = dal.Delete(strArray);            
            if (bRet)
            {
                //重新计算脚本结果
                string strEasyCode = GenMapScriptEasy(strMapID);
                if(new MapScript().Update(strMapID, strEasyCode))
                {
                    DataCache.DeleteCacheMapScript(strMapID);//清除缓存
                }
            }
            return bRet;
        }
        /// <summary>
        /// 生成新的映射脚本
        /// </summary>
        /// <param name="strMapID"></param>
        /// <returns></returns>
        private string GenMapScriptEasy(string strMapID)
        {
            return "test script";
        }
    }
}
