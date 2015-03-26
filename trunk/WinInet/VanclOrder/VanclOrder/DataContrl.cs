using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace PcOnlineFresh
{
    public class DataContrl
    {
        public static Dictionary<string, _GoodsFreshStr> pInfoList = new Dictionary<string, _GoodsFreshStr>();
        public static List<string> m_strFreshArray = new List<string>();
        public static List<string> m_strModifyArray = new List<string>();
        public static List<string> m_strSynArray = new List<string>();


        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        //修改列表
        public static Dictionary<string, Hashtable> pModifyList = new Dictionary<string, Hashtable>();
        //需要添加的列表
        public static Dictionary<string, Hashtable> pNeedAddList = new Dictionary<string, Hashtable>();
        //同步列表
        public static Dictionary<string, Hashtable> pSynList = new Dictionary<string, Hashtable>();
        //以品牌型号为KEY，以NO列表为值的列表
        public static Dictionary<string, string> pSynListNumber = new Dictionary<string, string>();
        #region pModifyList

        public static void ClearModifyData()
        {
            lock (pModifyList)
            {
                pModifyList.Clear();
            }
        }
        public static bool AddModifyList(string strAcc,Hashtable hTable)
        {
            lock (pModifyList)
            {
                pModifyList[strAcc] = hTable;
            }
            return true;
        }
        public static bool GetModifyData(string strAcc, ref Hashtable objHash)
        {
            bool bExist = false;
            lock (pModifyList)
            {
                bExist = pModifyList.ContainsKey(strAcc);
                if (bExist)
                {
                    objHash = pModifyList[strAcc];
                }
            }
            return bExist;
        }
#endregion
        #region pNeedAddList

        public static bool AddNeedAddList(string strAcc, Hashtable hTable)
        {
            lock (pNeedAddList)
            {
                pNeedAddList[strAcc] = hTable;
            }
            return true;
        }
        public static bool GetNeedAddData(string strAcc, ref Hashtable objHash)
        {
            bool bExist = false;
            lock (pNeedAddList)
            {
                bExist = pNeedAddList.ContainsKey(strAcc);
                if (bExist)
                {
                    objHash = pNeedAddList[strAcc];
                }
            }
            return bExist;
        }
        #endregion


        #region pSynListNumber

        public static bool AddSynNumberList(string strAcc, string strNoList)
        {
            lock (pSynListNumber)
            {
                pSynListNumber[strAcc] = strNoList;
            }
            return true;
        }

        public static string GetSynNumberData(string strAcc)
        {
            string strRet = "";
            lock (pSynListNumber)
            {
                if (pSynListNumber.ContainsKey(strAcc))
                {
                    strRet = (string)pSynListNumber[strAcc];
                }
                else{
                    strRet = "";
                }
            }
            return strRet;
        }
        #endregion

        #region pSynList

        public static bool AddSynList(string strAcc, Hashtable hTable)
        {
            lock (pSynList)
            {
                pSynList[strAcc] = hTable;
            }
            return true;
        }
        public static bool GetSynData(string strAcc, ref Hashtable objHash)
        {
            bool bExist = false;
            lock (pSynList)
            {
                bExist = pSynList.ContainsKey(strAcc);
                if (bExist)
                {
                    objHash = pSynList[strAcc];
                }
            }
            return bExist;
        }
        #endregion
        public static bool IsInCache(string strAcc)
        {
            bool bExist = false;
            lock (pInfoList)
            {
                bExist = pInfoList.ContainsKey(strAcc);                
            }
            return bExist;
        }

        public static int GetAccCount()
        {
            int iRet = 0;
            lock (pInfoList)
            {
                iRet = pInfoList.Count;
            }
            return iRet;
        }

        public static string GetAllAccount()
        {
            string strRet = "";
            lock (pInfoList)
            {
                foreach (KeyValuePair<string, _GoodsFreshStr> infoNode in pInfoList)
                {
                    strRet += infoNode.Key + ",";
                }
            }
            return strRet;
        }

        public static string GetAllNoSynAccount()
        {
            string strRet = "";
            lock (pInfoList)
            {
                foreach (KeyValuePair<string, _GoodsFreshStr> infoNode in pInfoList)
                {
                    if(infoNode.Value.iSynchronizeCount < 1)
                    {
                        strRet += infoNode.Key + ",";
                    }                    
                }
            }
            return strRet;
        }


        public static void InitFreshTime(int iFreshCount)
        {
            lock (pInfoList)
            {
               foreach (string strAcc in pInfoList.Keys)
                {
                    pInfoList[strAcc].iFreshCount = iFreshCount;
                    pInfoList[strAcc].iFreshSucCount = 0;
                    pInfoList[strAcc].iFreshFailCount = 0;
                }
            }
        }
       
        public static bool GetDataCache(string strAcc, ref _GoodsFreshStr objInfo)
        {
            bool bExist = false;
            lock (pInfoList)
            {
                bExist = pInfoList.ContainsKey(strAcc);
                if (bExist)
                {
                    objInfo = pInfoList[strAcc];
                }
            }
            return bExist;
        }
        /// <summary>
        /// 添加信息到缓存
        /// </summary>
        /// <param name="objUser"></param>
        public static void AddDataCache(string strFlagAcc, _GoodsFreshStr objInfo)
        {
            lock (pInfoList)
            {
                pInfoList[strFlagAcc] = objInfo;
            }
        }
        /// <summary>
        /// 删除缓存中的信息
        /// </summary>
        /// <param name="strUserKey"></param>
        public static void DeleteDataCache(string strKey)
        {
            lock (pInfoList)
            {
                if (pInfoList.ContainsKey(strKey))
                {
                    pInfoList.Remove(strKey);
                }
            }
        }


        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        public static string GetFirstAccount()
        {
            string strRet = "";
            lock (m_strFreshArray)
            {
                if (m_strFreshArray.Count < 1)
                {
                    return "";
                }
                strRet = m_strFreshArray[0];
                m_strFreshArray.RemoveAt(0);
            }
            return strRet;
        }

        public static void InitFreshAccount()
        {
            string strAccString = GetAllAccount();
            lock(m_strFreshArray)
            {
                m_strFreshArray.Clear();
                string[] strArray = strAccString.Split(',');
                foreach (string strAcc in strArray)
                {
                    if (strAcc != "")
                    {
                        m_strFreshArray.Add(strAcc);
                    }
                }
            }            
        }

        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        //m_strSynArray
        public static string GetFirstSynAccount()
        {
            string strRet = "";
            lock (m_strSynArray)
            {
                if (m_strSynArray.Count < 1)
                {
                    return "";
                }
                strRet = m_strSynArray[0];
                m_strSynArray.RemoveAt(0);
            }
            return strRet;
        }

        public static int GetSynCount()
        {
            int iRet = 0;
            lock (m_strSynArray)
            {
                iRet = m_strSynArray.Count;
            }
            return iRet;
        }

        public static void DeleteAllAccount()
        {
            m_strSynArray.Clear();
            pInfoList.Clear();
        }

        public static void InitSynAccount()
        {
            string strAccString = GetAllNoSynAccount();

            lock (m_strSynArray)
            {
                m_strSynArray.Clear();
                string[] strArray = strAccString.Split(',');
                foreach (string strAcc in strArray)
                {
                    if (strAcc != "")
                    {
                        m_strSynArray.Add(strAcc);
                    }
                }
            }
        }
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        public static string GetFirstModifyAccount()
        {
            string strRet = "";
            lock (m_strModifyArray)
            {
                if (m_strModifyArray.Count < 1)
                {
                    return "";
                }
                strRet = m_strModifyArray[0];
                m_strModifyArray.RemoveAt(0);
            }
            return strRet;
        }

        public static int GetModifyCount()
        {
            int iRet = 0;
            lock (m_strModifyArray)
            {
                iRet = m_strModifyArray.Count;
            }
            return iRet;
        }
        public static void InitModifyAccount()
        {
            string strAccString = GetAllAccount();
            lock (m_strModifyArray)
            {
                m_strModifyArray.Clear();
                string[] strArray = strAccString.Split(',');
                foreach (string strAcc in strArray)
                {
                    if (strAcc != "")
                    {
                        m_strModifyArray.Add(strAcc);
                    }
                }
            }
        }
    }
}
