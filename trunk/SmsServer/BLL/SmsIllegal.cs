using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using SmsServer.Model;
using Newtonsoft.Json;
using System.Collections;
namespace SmsServer.BLL
{
    public class SmsIllegal
    {
        private readonly SmsServer.DAL.SmsIllegal dal = new SmsServer.DAL.SmsIllegal();
        public DataSet GetAllIllegalWords()
        {
            return dal.GetAllIllegalWords( );
        }
        /// <summary>
        /// 返回JSON字符串
        /// </summary>
        /// <param name="iStart"></param>
        /// <param name="iEnd"></param>
        /// <returns></returns>
        public string GetIllegalList(int iStart, int iLimit)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetIllegalList(iStart, iLimit, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
        public bool Delete(string[] strArray)
        {
            bool bRet = dal.Delete(strArray);
            if (bRet)
            {
                //初始化缓存信息
                DataCache.InitCacheIllegal();
            }
            return bRet;
        }
        //修改
        public bool Update(int iID,string strKeyWord,out string strout)
        {
            if (dal.Exists(iID,strKeyWord))
            {
                strout = "已经存在此关键词";
                return false;
            }
            strout = "更新关键词失败";
            bool bRet = dal.Update(iID,strKeyWord);
            if (bRet)
            {
                //初始化缓存信息
                DataCache.InitCacheIllegal();
            }
            return bRet;
        }
        //添加
        public bool Add(string strKeyWord, out string strout)
        {
            if (dal.Exists(strKeyWord))
            {
                strout = "已经存在此关键词";
                return false;
            }
            strout = "添加关键词失败";
            bool bRet = dal.Add(strKeyWord);
            if (bRet)
            {
                //添加进缓存
                DataCache.AddCacheIllegal(strKeyWord);
            }
            return bRet;
        }
    }
}
