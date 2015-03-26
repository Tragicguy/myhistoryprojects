//#define __PAOPAO
using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using ClassLibrary;
using System.Security.Policy;
using System.Collections;
using System.Text.RegularExpressions;
using System.Web;
using System.IO;
namespace PcOnlineFresh
{
    public class TestStr1
    {
        public string strAcc;
    }
    public struct TestStr2
    {
        public string strAcc;
    }
    public class _GoodsFreshStr 
    {
        public string strAcc;
        public string strPwd;
        public int iFreshCount;         //当天要刷新的总次数
        public int iFreshSucCount;
        public int iFreshFailCount;
        public int iModifyCount;        //等待修改商品的数据
        public int iModifySucCount;     //修改商品成功的数据
        public int iFreshNumCount;      //刷新商品的数量
        public int iFreshSucNumCount;   //刷新商品成功的数量
        public string strFreshStatus;   //刷新状态
        public string strModifyStatus;  //修改状态
        public string strSynStatus;     //同步状态
        public bool bModify;
        public int iHasAddCount;
        public int iNeedAddCount;       //需要添加的商品数
        public int iSynchronizeCount;        //本地已经同步好的商品数量
        public ClassHttpPost webPost;   //网络通信对象
        public string strLastFreshQuickTime;//上次一键刷新时间
        public string strQuickFreshTimeLeft;//本日还有多少次一键刷新机会
        public string strId;
    }

    public class _GoodsModifyStr
    {
        public string strNoEx; //品牌加型号
        public string strNo;
        public string strName;
        public float fLowPrice;
        public float fHighPrice;
        public float fPrice;
        public int iFailFlag;   //1表示需要添加，2表示价格超过范围,3网络或其他错误    
        public string strUrl;
        public string strNo2;
    }

    public class GoodsFresh
    {
        public ClassHttpPost m_webPost;
        string m_strIndexContent;
        string m_strGoodContent;
        public void SetWebPost(ClassHttpPost webPost)
        {
            m_webPost = webPost;
        }
        public bool IsLogin()
        {
            return true;
        }

#region PAOPAO
        public bool GetImage(string strFile,out string strError)
        {
            strError = "";
            try
            {
                string strUrl = string.Format("http://shop.pcpop.com/CodeImage.aspx?type=1&mixture={0}", DateTime.Now.ToString("yyyyMMddHHmmss"));
                Stream stream = m_webPost.GetHttpPageSteam(strUrl, "gb2312", "get", "", "", "http://shop.pcpop.com/", "*/*", "");
                if (stream == null)
                {
                    strError = "获取验证码失败,网络错误";
                    return false;
                }

                List<byte> list = new List<byte>();
                while (true)
                {
                    int data = stream.ReadByte();
                    if (data == -1)
                        break;
                    list.Add((byte)data);
                }
                byte[] bytes = list.ToArray();
                if (File.Exists(strFile))
                {
                    File.Delete(strFile);
                }
                FileStream fs = new FileStream(strFile, FileMode.Create);
                BinaryWriter bw = new BinaryWriter(fs);
                bw.Write(bytes);
                bw.Close();
                fs.Close();
                return true;
            }
            catch
            {
                strError = "获取验证码失败,获取异常";
                return false;
            }            
        }

        public bool GetImageEx(string strFile, out string strError)
        {
            strError = "";
            try
            {
                string strUrl = string.Format("https://passport.sohu.com/servlet/Captcha?time=0.{0}", DateTime.Now.ToString("yyyyMMddHHmmss"));
                Stream stream = m_webPost.GetHttpPageSteam(strUrl, "gb2312", "get", "", "", "https://passport.sohu.com/web/dispatchAction.action?registerType=Passport&showAllType=1", "*/*", "");
                if (stream == null)
                {
                    strError = "获取验证码失败,网络错误";
                    return false;
                }

                List<byte> list = new List<byte>();
                while (true)
                {
                    int data = stream.ReadByte();
                    if (data == -1)
                        break;
                    list.Add((byte)data);
                }
                byte[] bytes = list.ToArray();
                if (File.Exists(strFile))
                {
                    File.Delete(strFile);
                }
                FileStream fs = new FileStream(strFile, FileMode.Create);
                BinaryWriter bw = new BinaryWriter(fs);
                bw.Write(bytes);
                bw.Close();
                fs.Close();
                return true;
            }
            catch
            {
                strError = "获取验证码失败,获取异常";
                return false;
            }
        }

        public bool PaoPaoLogin(string strAcc, string strPwd,string strCode,out string strError)
        {
            strError = "";
            try
            {
                string strData = string.Format("txtUserName={0}&txtUserPWD={1}&txtCheckCode={2}&x=50&y=12",
                strAcc,
                strPwd,
                strCode);
                string strRet = m_webPost.GetHttpPage("http://shop.pcpop.com/login.aspx",
                    "gb2312", "post", "", strData,
                    "http://shop.pcpop.com/",
                    "*/*", "");
                if (strRet == "")
                {
                    strError = "登录失败,网络错误";
                    return false;
                }
                if (strRet.IndexOf("验证码错误") > 0)
                {
                    strError = "验证码错误";
                    return false;
                }
                if (strRet.IndexOf("用户名或密码错误")> 0)
                {
                    strError = "用户名或密码错误";
                    return false;
                }
                if (strRet.IndexOf("该用户不是经销商") > 0)
                {
                    strError = "该用户不是经销商";
                    return false;
                }
                if (!(strRet.IndexOf("短消息中心") > 0 && strRet.IndexOf("退出登录") > 0))
                {
                    strError = "登录失败";
                    return false;
                }
                string strUrl = "http://my.shop.pcpop.com/Price/PriceList.aspx";
                //登录成功
                strRet = m_webPost.GetHttpPage(strUrl,
                    "gb2312", "get", "", "",
                    "",
                    "*/*", "");
                if (!(strRet.IndexOf("短消息中心") > 0 && strRet.IndexOf("退出登录") > 0))
                {
                    strError = "登录失败";
                    return false;
                }
                m_strGoodContent = strRet;
                //获取页面成功
                return true;
            }
            catch (System.Exception ex)
            {
                strError = string.Format("登录时出现异常:{0}",ex.Message);
            }            
            return false;
        }
#endregion
        public bool Login(string strAcc,string strPwd,bool bQuickFresh,out string strId)
        {
            strId = "";
            string strData = string.Format("username={0}&password={1}&B1=1&x=6&y=12",
                HttpUtility.UrlEncode(strAcc, Encoding.GetEncoding("gb2312")),
                strPwd );
            string strRet = m_webPost.GetHttpPage("http://user.younet.com/login.php",
                "gb2312", "post", "", strData,
                "http://user.younet.com/login.php",
                "*/*", "");
            if (strRet.IndexOf("已经成功登录，请稍候") < 0)
            {
                return false;
            }
            string strUrl = "";
            string strSucFlag = "";
            if (strRet.IndexOf("shop.younet.com/manage/")<0)
            {
                return false;
            }
            strId = GetKeyString(strRet, "shop.younet.com/manage/?cid=", "'");
            
            //获取页面成功
            return true;
        }        

        
        //
        public int FreshQuick(out string strError,out string strLeft)
        {
            strError = "";
            strLeft = "";
            try
            {                
                string strTemp = m_strIndexContent.Substring(m_strIndexContent.IndexOf("function updateData(){"));
                string leftFlag = GetKeyString(strTemp, "if(", "!");
                string rightFlag = GetKeyString(strTemp, "!=", ")");
                if (leftFlag != rightFlag)
                {
                    strError = "没有一键刷新权限";
                    return -1;
                }

                string strBefore = "name=\"refreshProductCount\" id=\"refreshProductCount\" value=\"";
                string strAfter = "\"";
                string strNumLeft = GetKeyString(m_strIndexContent, strBefore, strAfter);
                int iNumLeft = 0;
                strLeft = strNumLeft;
                int.TryParse(strNumLeft, out iNumLeft);
                if (iNumLeft < 1)
                {
                    strError = "您今天的刷新次数已用完";
                    return -1;
                }
                string strRet = m_webPost.GetHttpPage("http://m.pconline.com.cn/member/product/ajax_update_data.htm",
                    "gb2312", "get", "", "",
                    "",
                    "*/*", "");
                if (strRet.IndexOf("您尚未登录，请先登录!") > 0)
                {
                    m_strIndexContent = "";
                    return -2;
                    //未登录
                }
                if (strRet.IndexOf("{\"refreshProductCount\"") < 0)
                {
                    strError = "数据错误";
                    return -3;
                }
                strNumLeft = GetKeyString(strRet, "refreshProductCount\":", ",");
                strLeft = strNumLeft;
                string strFreshed = GetKeyString(strRet, "refreshSum\":", ",");
                if (strFreshed == "0")//刷新失败
                {
                    strError = "您今天的刷新次数已用完";
                    return -1;
                }
                return 0;
            }
            catch (System.Exception ex)
            {
                strError = "刷新异常";
                return -1;
            }
            
        }

        public int Synchronize(out string strError, out int iFreshNum, out int iGoodCount, out bool bBreak,
            int iCurPage, ref int iNetErrorCount, ref Hashtable hSynTable, ref Hashtable hSynDuplicateTable,string strId)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            int iAnalyzeCount = 0;
            try
            {
                string strBefore = "共";
                string strAfter = "条记录";
                bool bRet = false;
                string strUrl = string.Format("http://shop.younet.com/c818/manage/list2.php?cid={0}&page={1}", strId, iCurPage);
                string strRet = m_webPost.GetHttpPage(strUrl,
                    "gb2312", "get", "", "",
                    "",
                    "*/*", "");
                //如果页码相同,则可以判断已经完成了
                //页次 49/49 每页
               
                if (strRet == "") //网络错误
                {
                    strError = (strRet == "" ? "网络异常" : "数据错误");
                    if (iNetErrorCount++ > 3)
                    {
                        iNetErrorCount = 0;
                        return -1;  //出错超过3次,停止
                    }
                    return -3;
                }
                m_strGoodContent = strRet;
                if (m_strGoodContent.IndexOf("<title>久腾网手机营业厅 管理后台") < 0)
                {
                    return -1;
                }
               
                string strCompleteFlag = "无相关商品";
                

                AnalyzeGoods(m_strGoodContent, ref hSynTable, ref hSynDuplicateTable, iCurPage - 1, out iAnalyzeCount);
                if (iAnalyzeCount < 1)  //已经同步完成
                {
                    bBreak = true;
                    return 0;
                }
                iFreshNum = iGoodCount = hSynTable.Count;
                
                return 0;
            }
            catch (System.Exception ex)
            {
                strError = "程序异常";
                return -1;  //数据错误，停止
            }
            
        }
        public int FreshPaoPao(out string strError, out int iFreshNum, out int iGoodCount, out bool bBreak,
           int iCurPage, bool bIsModify, ref Hashtable hWaitTable, ref Hashtable hFailTable, ref int iNetErrorCount)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            bool bRet = false;
            try
            {
                string strBefore = "共";
                string strAfter = "条记录";
                if (m_strGoodContent.IndexOf("退出登录") < 0)
                {
                    strError = "数据错误";
                    return -1;  //数据错误，停止
                }
                string strTemp = m_strGoodContent.Substring(m_strGoodContent.IndexOf("全选/反选"));
                string strNum = GetKeyString(strTemp, strBefore, strAfter).Trim();
                bRet = int.TryParse(strNum, out iGoodCount);
                if (!bRet)
                {
                    strError = "数据错误";
                    return -1;  //数据错误，停止
                }

                _GoodsModifyStr objModify = new _GoodsModifyStr();
                _GoodsModifyStr objModifyWait = new _GoodsModifyStr();
                //通过解析得到当前的商品数据
                //m_strGoodContent
                Hashtable hTableGoods = new Hashtable();

                StringBuilder strBuilder = new StringBuilder();
                strBuilder.Append("[");
                string strBakData = "";
                int iGoodFreshCount = 0;    //本次要刷新的商品数
                List<string> strModifyNameList = new List<string>(); //本次刷新的商品名称列表

                Hashtable hTableSynDuplicate = null;

                //HttpUtility.UrlEncode(objModify.strName, Encoding.GetEncoding("gb2312"))
                //拿前20个，直至拿完
                iCurPage = 101;
                foreach (DictionaryEntry dEntry in hWaitTable)
                {
                    if (strModifyNameList.Count == 20)  //只拿20个
                    {
                        break;
                    }
                    objModify = (_GoodsModifyStr)dEntry.Value;
                    string strInfo = "{\"index\":"
                        +(strModifyNameList.Count+1).ToString()
                        +",\"productId\":\""
                        +objModify.strNo.ToString()
                        + "\",\"price\":"
                        +objModify.fPrice.ToString()
                        +",\"link\":\"\",\"remark\":\"\"}";
                        strBuilder.Append(strInfo);
                    strModifyNameList.Add((string)dEntry.Key);
                    if (strModifyNameList.Count != 20 && strModifyNameList.Count < hWaitTable.Count)  //只拿20个
                    {
                        strBuilder.Append(",");
                    }
                }
                strBuilder.Append("]");
                iFreshNum = strModifyNameList.Count;
                if (iFreshNum < 1)
                {
                    bBreak = true;
                    return 0;
                }

                /************************************************************************/
                /*                                                                      */
                /************************************************************************/
                string strData = "action=savePriceList&jsonData=" + HttpUtility.UrlEncode(strBuilder.ToString(), Encoding.GetEncoding("gb2312"));
                string strRet = m_webPost.GetHttpPage("http://my.shop.pcpop.com/Handler/Price/Price.ashx",
                    "gb2312", "post", "", strData,
                    string.Format("http://my.shop.pcpop.com/Price/PriceList.aspx"),
                    "*/*", "");
                if (strRet.IndexOf("{res") >= 0) //刷新成功
                {
                    //清除相关的待修改商品
                    foreach (string strName in strModifyNameList)
                    {
                        hWaitTable.Remove(strName);
                    }
                    if (hWaitTable.Count < 1)
                    {
                        bBreak = true;
                    }
                }
                else if (strRet == "") //网络错误
                {
                    strError = (strRet == "" ? "网络异常" : "数据错误");
                    if (iNetErrorCount++ > 3)
                    {
                        iNetErrorCount = 0;
                        //添加修改失败的商品名
                        foreach (string strName in strModifyNameList)
                        {
                            objModify.iFailFlag = 3;
                            hFailTable.Add(strName, hWaitTable[strName]);
                            hWaitTable.Remove(strName);
                        }
                        return -1;  //出错超过3次,停止
                    }
                    return -3;
                }
                //m_strGoodContent = strRet;
                return 0;
            }
            catch (System.Exception ex)
            {
                strError = "程序异常";
                return -1;  //数据错误，停止
            }

        }
        public int FreshYouNet(string strPid, out string strError, ref int iNetErrorCount,string strId)
        {
            strError = "";
            try
            {
                long jsBeginTick = DateTime.Parse("1970-1-1").Ticks;
                DateTime dt2 = DateTime.Now.ToLocalTime();

                long dt2Ticks = dt2.ToUniversalTime().Ticks;
                long dt2JsTicks = (dt2Ticks - jsBeginTick) / (1000 * 10);
                string strUrl = string.Format("http://shop.younet.com/c818/manage/_do.php?pid={0}&act=red&ts={1}", strPid, dt2JsTicks);
                string strRet = m_webPost.GetHttpPage(strUrl,
                 "gb2312", "get", "", "",
                 string.Format("http://shop.younet.com/c818/manage/list2.php?cid={0}", strId),
                 "*/*", "");
                if (strRet == "")
                {
                    strError = (strRet == "" ? "网络异常" : "数据错误");
                    if (iNetErrorCount++ > 5)
                    {
                        iNetErrorCount = 0;
                        
                        return -1;  //出错超过3次,停止
                    }
                    return -3;
                }
                if (strRet.IndexOf("ok") < 0)
                {
                    return -1;
                }
                return 0;
            }
            catch (System.Exception ex)
            {
                strError = ex.Message;
            }
            return -1;
        }
        public int Fresh(out string strError,out int iFreshNum,out int iGoodCount, out bool bBreak,
            int iCurPage, bool bIsModify, ref Hashtable hWaitTable, ref Hashtable hFailTable, ref int iNetErrorCount,string strId)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            int iAnalyzeCount = 0;
            try
            {        
                string strBefore = ">";
                string strAfter = "</a>";

                string strUrl = string.Format("http://shop.younet.com/c818/manage/list2.php?cid={0}&page={1}", strId, iCurPage);
                //加载
                string strRet = "";
                
                if (!bIsModify)
                {
                    strRet = m_webPost.GetHttpPage(strUrl,
                     "gb2312", "get", "", "",
                     string.Format("http://shop.younet.com/c818/manage/list2.php?cid={0}", strId),
                     "*/*", "");
                    if (strRet == "") //网络错误
                    {
                        strError = (strRet == "" ? "网络异常" : "数据错误");
                        if (iNetErrorCount++ > 5)
                        {
                            iNetErrorCount = 0;
                            return -1;  //出错超过3次,停止
                        }
                        return -3;
                    }
                    m_strGoodContent = strRet;
                    if (m_strGoodContent.IndexOf("<title>久腾网手机营业厅 管理后台") < 0)
                    {
                        strError = "刷新失败";
                        return -1;
                    }
                }
                
                _GoodsModifyStr objModify = new _GoodsModifyStr();
                _GoodsModifyStr objModifyWait = new _GoodsModifyStr();
                //通过解析得到当前的商品数据
                //m_strGoodContent
                Hashtable hTableGoods = new Hashtable();

                StringBuilder strBuilder = new StringBuilder();
               // strBuilder.Append("method=save");
                string strBakData = "";
                int iGoodFreshCount = 0;    //本次要刷新的商品数
                List<string> strModifyNameList = new List<string>(); //本次刷新的商品名称列表

                Hashtable hTableSynDuplicate = null;
                if (bIsModify == false)   //刷新
                {
                    AnalyzeGoods(m_strGoodContent, ref hTableGoods, ref hTableSynDuplicate, 1, out iAnalyzeCount);
                    iFreshNum = hTableGoods.Count;
                    if (iFreshNum < 1)  //刷新完成
                    {
                        bBreak = true;
                        return 0;
                    }
                    foreach (DictionaryEntry dEntry in hTableGoods)
                    {
                        objModify = (_GoodsModifyStr)dEntry.Value;
                        strBuilder.Append(string.Format("price%5B%5D={0}&topic%5B%5D={1}&url%5B%5D={2}&proid%5B%5D={3}&status%5B{4}%5D=&id%5B%5D={4}&",
                            objModify.fPrice, HttpUtility.UrlEncode(objModify.strName, Encoding.GetEncoding("gb2312")),
                            HttpUtility.UrlEncode(objModify.strUrl, Encoding.GetEncoding("gb2312")), objModify.strNo2, objModify.strNo, objModify.strNo));
                        iGoodFreshCount++;
                    }
                }
                else    //修改
                {
                    //拿前60个，直至拿完
                    iCurPage = 101;
                    foreach (DictionaryEntry dEntry in hWaitTable)
                    {
                        if (strModifyNameList.Count == 200)  //只拿200个
                        {
                            break;
                        }
                        objModify = (_GoodsModifyStr)dEntry.Value;
                        strBuilder.Append(string.Format("price%5B%5D={0}&topic%5B%5D={1}&url%5B%5D={2}&proid%5B%5D={3}&status%5B{4}%5D=&id%5B%5D={4}&",
                            objModify.fPrice, HttpUtility.UrlEncode(objModify.strName, Encoding.GetEncoding("gb2312")),
                            HttpUtility.UrlEncode(objModify.strUrl, Encoding.GetEncoding("gb2312")), objModify.strNo,objModify.strNo2));
                        strModifyNameList.Add((string)dEntry.Key);
                    }
                    iFreshNum = strModifyNameList.Count;
                    if (iFreshNum < 1)
                    {
                        bBreak = true;
                        return 0;
                    }
                }

                string strLast = string.Format("act=edit&cid={0}&pr=edit&links=list2.php%3Fcid%3D1548%26tradeid%3D%26page%3D1",strId);
                strBuilder.Append(strLast);
                /************************************************************************/
                /*                                                                      */
                /************************************************************************/
                //string strData = "price%5B%5D=2111&topic%5B%5D=%D5%FD%B9%E6%B4%F3%C2%BD%D0%D0%BB%F5%A3%AC%B4%F8%B7%A2%C6%B1%A3%AC%C8%AB%B9%FA%C1%AA%B1%A3%A3%AC%CF%ED%CA%DC%B9%FA%BC%D2%C8%FD%B0%FC%A1%A3%D6%A7%B3%D6%BB%F5%B5%BD%B8%B6%BF%EE%A3%AC%C9%CF%C3%C5%D7%D4%CC%E1%A1%A3&url%5B%5D=http%3A%2F%2Fwww.c818.com%2FProductShow.asp%3FProID%3D3984&proid%5B%5D=22413&status%5B21847%5D=&id%5B%5D=21847&act=edit&cid=1548&pr=edit&links=list2.php%3Fcid%3D1548%26tradeid%3D%26page%3D1";

                string strData = strBuilder.ToString();
                strRet = m_webPost.GetHttpPage("http://shop.younet.com/c818/manage/price_man1.php",
                    "gb2312", "post", "", strData,
                    string.Format("http://shop.younet.com/c818/manage/list2.php?cid={0}", strId),
                    "*/*", "");

                if (strRet == "") //网络错误
                {
                    strError = (strRet == "" ? "网络异常" : "数据错误");
                    if (iNetErrorCount++ > 5)
                    {
                        iNetErrorCount = 0;
                        //添加修改失败的商品名
                        foreach (string strName in strModifyNameList)
                        {
                            objModify.iFailFlag = 3;
                            hFailTable.Add(strName, hWaitTable[strName]);
                            hWaitTable.Remove(strName);
                        }
                        return -1;  //出错超过3次,停止
                    }
                    return -3;
                }
                if (strRet.IndexOf("操作成功") < 0)
                {
                    //失败,重试3次
                    if (!bIsModify)
                    {
                        strError = "刷新失败";
                    }
                    else
                    {
                        strError = "修改失败";
                    }
                    if (iNetErrorCount++ > 5)
                    {
                        iNetErrorCount = 0;
                        //添加修改失败的商品名
                        foreach (string strName in strModifyNameList)
                        {
                            objModify.iFailFlag = 3;
                            hFailTable.Add(strName, hWaitTable[strName]);
                            hWaitTable.Remove(strName);
                        }
                        return -1;  //出错超过3次,停止
                    }
                    return -3;
                    
                }
                //成功了
                foreach (string strName in strModifyNameList)
                {
                    hWaitTable.Remove(strName);
                }
                //修改完成
                if ( bIsModify )
                {
                    if (hWaitTable.Count < 1)
                    {
                        bBreak = true;
                        return 0;
                    }                    
                }
                return 0;
            }
            catch (System.Exception ex)
            {
                strError = "程序异常"+ex.Message;
                return -1;  //数据错误，停止
            }
            
        }
        void AnalyzeGoodsPaoPao(string strContent, ref Hashtable goodFreshList, ref Hashtable hSynDuplicateTable, int iPage)
        {
            try
            {
                strContent = strContent.Replace("\r", "");
                strContent = strContent.Replace("\n", "");
                strContent = strContent.Replace("\r\n", "");
                Match match = Regex.Match(strContent, "产品描述</a></div>(.+?)报价时间");
                
                int match_num = 0;		//总共匹配到的数量
                string matchValue = "";
                while (match.Success)
                {
                    _GoodsModifyStr goodModify = new _GoodsModifyStr();
                    //id="down_limit_6981520" type="hidden" value="774.0" /><input id="up_limit_6981520" type="hidden" value="1677.0" />
                    matchValue = match.Groups[1].Value;		//获取匹配到的字符串
                    //"><textarea id="txtRemark384380
                    goodModify.strNo = GetKeyString(matchValue, "productInfoChanged('", "'");
                    string strPrice = GetKeyString(matchValue,
                        string.Format("class=\"currentPrice\" value=\""),
                        "\"");
                    float.TryParse(strPrice, out goodModify.fPrice);
                    

                    goodModify.strNoEx = GetKeyString(matchValue,
                        string.Format("id=\"txtProductName{0}\" value=\"", goodModify.strNo),
                        "\"");// +"    " + iPage.ToString();
                    goodModify.strName = goodModify.strNoEx;


                    //获取最高和最低价格
                    string strData = string.Format("action=getLimitPrice&productId={0}", goodModify.strNo);
                    try
                    {
                       string strRet = m_webPost.GetHttpPage("http://my.shop.pcpop.com/Handler/Price/Price.ashx",
                                        "gb2312", "post", "", strData,
                                        "",
                                        "*/*", "");
                       //{type:"2",res:"620 - 930 "}
                        if (strRet.IndexOf("res:\"") < 0)
                        {
                            goodModify.fLowPrice = goodModify.fHighPrice = goodModify.fPrice;
                        }
                        else{
                            strRet = strRet.Substring(strRet.IndexOf("res:\""));
                            strPrice = GetKeyString(strRet,
                        "res:\"",
                        " -");
                            float.TryParse(strPrice, out goodModify.fLowPrice);
                            strPrice = GetKeyString(strRet,
                                " - ",
                                " ");
                            float.TryParse(strPrice, out goodModify.fHighPrice);
                        }
                    }
                    catch (System.Exception ex)
                    {
                    	
                    }
                    //string strPageNum = 
                    match_num++;		//匹配数增加
                    if (!goodFreshList.ContainsKey(goodModify.strNo))
                    {
                        goodFreshList.Add(goodModify.strNo, goodModify);
                    }
                    else
                    {
                        if (hSynDuplicateTable != null && !hSynDuplicateTable.ContainsKey(goodModify.strNo))
                        {
                            hSynDuplicateTable.Add(goodModify.strNo, goodModify);
                        }
                    }
                    match = match.NextMatch();
                }
                int ii = 0;
            }
            catch (System.Exception ex)
            {

            }
        }


        void AnalyzeGoods(string strContent, ref Hashtable goodFreshList, ref Hashtable hSynDuplicateTable,int iPage,out int iGoodCount)
        {
            iGoodCount = 0;
            try
            {
                strContent = strContent.Replace("\r", "");
                strContent = strContent.Replace("\n", "");
                strContent = strContent.Replace("\r\n", "");
                strContent = strContent.Replace("\t", "");
                Match match = Regex.Match(strContent, "套红</a>(.+?)</ul>");
                
                int match_num = 0;		//总共匹配到的数量
                string matchValue = "";
                string strPrice = "";
                while (match.Success)
                {
                    _GoodsModifyStr goodModify = new _GoodsModifyStr();
                    matchValue = match.Groups[1].Value;		//获取匹配到的字符串
                    //target="_blank">摩托罗拉 A1890</a>
                    goodModify.strNoEx = GetKeyString(matchValue, "target=\"_blank\">", "</a>");
                    goodModify.strNo = GetKeyString(matchValue, "type=\"hidden\" name=\"proid[]\" value=\"", "\"");
                    goodModify.strNo2 = GetKeyString(matchValue, "type=\"hidden\" name=\"id[]\" value=\"", "\"");
                    string strTemp = matchValue.Substring(matchValue.IndexOf("<input name=\"price[]\" type=\"text\""));

                    strPrice = GetKeyString(strTemp,
                            "value=\"",
                            "\"");
                    float.TryParse(strPrice, out goodModify.fPrice);

                    strTemp = matchValue.Substring(matchValue.IndexOf("<input name=\"topic[]\" type=\"text\""));
                    goodModify.strName = GetKeyString(strTemp,
                           "value=\"",
                            "\"");
                    strTemp = matchValue.Substring(matchValue.IndexOf("<input name=\"url[]\" type=\"text\""));
                    goodModify.strUrl = GetKeyString(strTemp,
                           "value=\"",
                            "\"");
                    /*
                     <input type="hidden" name="proid[]" value="22413">
<input type="hidden" name="status[21847]" id="status21847" value="">
<input type="hidden" name="id[]" value="21847"></li>
                     */
                    //string strPageNum = 
                    match_num++;		//匹配数增加
                    if (!goodFreshList.ContainsKey(goodModify.strNo))
                    {
                        goodFreshList.Add(goodModify.strNo, goodModify);
                    }
                    else
                    {
                        if (hSynDuplicateTable != null && !hSynDuplicateTable.ContainsKey(goodModify.strNo))
                        {
                            hSynDuplicateTable.Add(goodModify.strNo, goodModify);
                        }
                    }
                    match = match.NextMatch();
                }
                iGoodCount = match_num;
                int ii = 0;
            }
            catch (System.Exception ex)
            {
            	
            }            
        }

        private string GetKeyString(string strSrc, string strBefore, string strAfter)
        {
            int iPosBefore = 0;
            int iPosAfter = 0;
            iPosBefore = strSrc.IndexOf(strBefore) + strBefore.Length;
            iPosAfter = strSrc.IndexOf(strAfter, iPosBefore);
            if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0)
            {
                return "";
            }
            else
            {
                return strSrc.Substring(iPosBefore, iPosAfter - iPosBefore);
            }
        }
    }
}
        