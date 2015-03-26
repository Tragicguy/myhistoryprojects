using System;
using System.Data;
using System.Collections.Generic;
using System.Text;

namespace SmsServer.BLL
{
    public class DataCache
    {
        public static Dictionary<string, SmsServer.Model.SmsUser> pUserList = new Dictionary<string, SmsServer.Model.SmsUser>();
        public static Dictionary<string, SmsServer.Model.SmsSpgate> pSpgateList = new Dictionary<string, SmsServer.Model.SmsSpgate>();
        //非法关键词
        public static Dictionary<string, int> pIllegalList = new Dictionary<string, int>();
        //号段表
        public static Dictionary<string, int> pPhoneSection = new Dictionary<string, int>();
        //发送返回值匹配脚本表
        public static Dictionary<string, SmsServer.Model.MapScript> pMapScriptList = new Dictionary<string, SmsServer.Model.MapScript>();
        /****************************************************************/
        /// <summary>
        /// 从缓存中取用户信息
        /// </summary>
        /// <param name="strUserKey"></param>
        /// <param name="objUser"></param>
        public static bool GetCacheUser(string strUserID,string strPwd,ref SmsServer.Model.SmsUser objUser)
        {
            bool bExist = false;
            string strUserKey = strUserID + strPwd;
            lock (pUserList)
            {
                bExist = pUserList.ContainsKey(strUserKey);
                if(bExist)
                {
                    objUser = pUserList[strUserKey];
                }
                
            }
            if (!bExist)
            {
                objUser = new SmsUser().GetSelectSmsUser(strUserID, strPwd);
                if (objUser == null)
                {
                    return false;
                }
                AddCacheUser(objUser);
            }
            return true;
        }
        /// <summary>
        /// 添加用户信息到缓存
        /// </summary>
        /// <param name="objUser"></param>
        public static void AddCacheUser(SmsServer.Model.SmsUser objUser)
        {
            lock (pUserList)
            {
                pUserList[objUser.UserID+objUser.UserPwd] = objUser;
            }
        }
        /// <summary>
        /// 删除缓存中的用户信息
        /// </summary>
        /// <param name="strUserKey"></param>
        public static void DeleteCacheUser(string strUserKey)
        {
            lock (pUserList)
            {
                if (pUserList.ContainsKey(strUserKey))
                {
                    pUserList.Remove(strUserKey);
                }                
            }
        }
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        /// <summary>
        /// 从缓存中获取通道信息
        /// </summary>
        /// <param name="strSpgateKey"></param>
        /// <param name="objSpgate"></param>
        /// <returns></returns>
        public static bool GetCacheSpgate(string strSpgateKey, ref Model.SmsSpgate objSpgate)
        {
            bool bExist = false;
            lock (pSpgateList)
            {
                bExist = pSpgateList.ContainsKey(strSpgateKey);
                if (bExist)
                {
                    objSpgate = pSpgateList[strSpgateKey];
                }
            }
            if (!bExist)
            {
                objSpgate = new SmsSpgate().GetModel(strSpgateKey);
                if (objSpgate==null)
                {
                    return false;
                }
                AddCacheSpgate(objSpgate);
            }
            return true;
        }
        /// <summary>
        /// 添加通道信息到缓存
        /// </summary>
        /// <param name="objUser"></param>
        public static void AddCacheSpgate(Model.SmsSpgate objSpgate)
        {
            lock (pSpgateList)
            {
                pSpgateList[objSpgate.GateNumber] = objSpgate;
            }
        }
        /// <summary>
        /// 删除缓存中的通道信息
        /// </summary>
        /// <param name="strUserKey"></param>
        public static void DeleteCacheSpgate(string strSpgateKey)
        {
            lock (pSpgateList)
            {
                if (pSpgateList.ContainsKey(strSpgateKey))
                {
                    pSpgateList.Remove(strSpgateKey);
                }                
            }
        }
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        public static void InitCacheIllegal()
        {
            pIllegalList.Clear();
            DataSet ds = new SmsIllegal().GetAllIllegalWords();
            foreach (DataRow row in ds.Tables[0].Rows)
            {
                pIllegalList[row["KeyWord"].ToString( )] = 1;
            }    
        }
        /// <summary>
        /// 获取非法关键词缓存列表
        /// </summary>
        /// <param name="IllegalList"></param>
        public static void GetCacheIllegal(ref Dictionary<string, int> IllegalList)
        {
            lock(pIllegalList)
            {
                IllegalList = pIllegalList;
            }
        }
        /// <summary>
        /// 添加非法关键词缓存
        /// </summary>
        /// <param name="strKey"></param>
        public static void AddCacheIllegal(string strKey)
        {
            lock (pIllegalList)
            {
                pIllegalList[strKey] = 1;
            }
        }
        /// <summary>
        /// 删除非法关键词缓存信息
        /// </summary>
        /// <param name="strKey"></param>
        public static void DeleteCacheIllegal(string strKey)
        {
            lock (pIllegalList)
            {
                pIllegalList.Remove(strKey);
            }
        }
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        public static void InitCacheSection()
        {
            //从数据库中读取出来
            pPhoneSection.Clear();
            DataSet ds = new PhoneSection().GetPhoneSection();
            if (ds.Tables[0].Rows.Count > 0)
            {
                for (int i = 1; i < 4;i++ )
                {
                    string[] strMobileArray = ds.Tables[0].Rows[0][i].ToString().Split(',');
                    foreach (string strHeader in strMobileArray)
                    {
                        pPhoneSection[strHeader] = i+2;
                    }
                }                
            }                       
        }
        //获取号段缓存列表
//         public static void GetCacheSection(ref Dictionary<string, int> PhoneSection)
//         {
//             lock(pPhoneSection)
//             {
//                 PhoneSection = pPhoneSection;
//             }
//         }
        //添加号段缓存
        public static void AddCacheSection(string strPhoneKey, int iFlag)
        {
            lock(pPhoneSection)
            {
                pPhoneSection[strPhoneKey] = iFlag;
            }
        }
        //删除号段缓存
        public static void DeleteCacheSection(string strPhoneKey)
        {
            lock(pPhoneSection)
            {
                pPhoneSection.Remove(strPhoneKey);
            }
        }
        /// <summary>
        /// 从缓存中取映射脚本
        /// </summary>
        /// <param name="strMapID"></param>
        /// <param name="objScript"></param>
        /// <returns></returns>
        public static bool GetCacheMapScript(string strMapID, ref SmsServer.Model.MapScript objScript)
        {
            bool bExist = false;
            lock (pMapScriptList)
            {
                bExist = pMapScriptList.ContainsKey(strMapID);
                if (bExist)
                {
                    objScript = pMapScriptList[strMapID];
                }

            }
            if (!bExist)
            {
                objScript = new MapScript().GetSelectMapScript(strMapID);
                if (objScript == null)
                {
                    return false;
                }
                AddCacheMapScript(objScript);
            }
            return true;
        }
        /// <summary>
        /// 添加映射脚本到缓存
        /// </summary>
        /// <param name="objScript"></param>
        public static void AddCacheMapScript(SmsServer.Model.MapScript objScript)
        {
            lock (pMapScriptList)
            {
                pMapScriptList[objScript.MapID] = objScript;
            }
        }
        /// <summary>
        /// 从缓存中删除映射脚本
        /// </summary>
        /// <param name="strKey"></param>
        public static void DeleteCacheMapScript(string strKey)
        {
            lock (pMapScriptList)
            {
                if (pMapScriptList.ContainsKey(strKey))
                {
                    pMapScriptList.Remove(strKey);
                }
            }
        }
    }    
}
