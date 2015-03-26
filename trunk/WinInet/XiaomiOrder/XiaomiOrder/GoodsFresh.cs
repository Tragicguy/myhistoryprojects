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
using System.Windows.Forms;
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
        public string goodId;
        public string skuId;
        public string strNum;
        public string strReceiver;
        public string strPhone;

        public string strEmail;
        public string strType;


        public string strPostCode;
        public string strProvince;
        public string strCity;
        public string strTown;
        public string strAddr;
        public string strOrderID;
        public string strGoodName;
        public ListViewItem listItem;

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
    }

    public class GoodsFresh
    {
        public Form1 m_objForm;
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
                Stream stream = m_webPost.GetHttpPageSteam(strUrl, "gb2312", "get", "", "", "http://shop.pcpop.com/", "*/*", "", null);
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
                Stream stream = m_webPost.GetHttpPageSteam(strUrl, "gb2312", "get", "", "", "https://passport.sohu.com/web/dispatchAction.action?registerType=Passport&showAllType=1", "*/*", "", null);
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
        public bool Login(string strAcc,string strPwd, bool bQuickFresh)
        {
            
            string strUrl = "https://login.vancl.com/login/login.aspx?http://www.vancl.com/";
            string strRet = m_webPost.GetHttpPage(strUrl, "utf-8", "get", "", "", "", "*/*", "");
            if (strRet.IndexOf("会员登录") < 0)
            {
                return false;
            }
            string strData = string.Format("Loginasync=true&LoginUserName={0}&UserPassword={1}&Validate={2}&type=web",
                strAcc,
                strPwd);
            strRet = m_webPost.GetHttpPage("http://passport2.pconline.com.cn/passport2/passport/login.jsp",
                "utf-8", "post", "", strData,
                "http://m.pconline.com.cn/member/login.htm",
                "*/*", "");
            if (!(strRet.IndexOf("passport.pcauto.com.cn") > 0 && strRet.IndexOf(strAcc) > 0))
            {
                return false;
            }
            string strSucFlag = "";
            if (bQuickFresh)
            {
                strUrl = "http://m.pconline.com.cn/member/index.htm";
                strSucFlag = "一键刷新";
            }
            else
            {
                strUrl = "http://m.pconline.com.cn/member/product/products_onsell.htm?&pageSize=60&pageNo=1";
                strSucFlag = string.Format("{0}[老板]", strAcc);
                strSucFlag = "出售中的商品";
            }
            //登录成功
            strRet = m_webPost.GetHttpPage(strUrl,
                "gb2312", "get", "", "",
                "",
                "*/*", "");
            if (strRet.IndexOf(strSucFlag) < 0)
            {
                return false;
            }
            if (bQuickFresh)
            {
                m_strIndexContent = strRet;
            }
            else
            {
                m_strGoodContent = strRet;
            }
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
            int iCurPage, ref int iNetErrorCount, ref Hashtable hSynTable, ref Hashtable hSynDuplicateTable)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            try
            {
                string strBefore = "共";
                string strAfter = "条记录";
                bool bRet = false;
#if __PAOPAO
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
#else
                strBefore = "共<i class=\"num\">";
                strAfter = "</i>";

                if (m_strGoodContent.IndexOf("出售中的商品") < 0)
                {
                    strError = "数据错误1";
                    return -1;  //数据错误，停止
                }
                string strTemp = "";// m_strGoodContent.Substring(m_strGoodContent.IndexOf("我的商品总数"));
                string strNum = GetKeyString(m_strGoodContent, strBefore, strAfter);
                bRet = int.TryParse(strNum, out iGoodCount);
                if (!bRet)
                {
                    strError = "数据错误2";
                    return -1;  //数据错误，停止
                }
#endif
                string strCompleteFlag = "无相关商品";
                string strUrl = string.Format("http://m.pconline.com.cn/member/product/products_onsell.htm?&pageSize=60&pageNo={0}", iCurPage);
#if __PAOPAO
                strUrl = string.Format("http://my.shop.pcpop.com/Price/PriceList.aspx?page={0}", iCurPage);
                strCompleteFlag = "";
                AnalyzeGoodsPaoPao(m_strGoodContent, ref hSynTable, ref hSynDuplicateTable, iCurPage - 1);
#else
                AnalyzeGoods(m_strGoodContent, ref hSynTable, ref hSynDuplicateTable, iCurPage - 1);
#endif

                string strRet = m_webPost.GetHttpPage(strUrl,
                    "gb2312", "get", "", "",
                    "",
                    "*/*", "");
                //如果页码相同,则可以判断已经完成了
                //页次 49/49 每页
#if __PAOPAO
                strTemp = strTemp.Substring(strTemp.IndexOf("页次"));
                string strNumLeft = GetKeyString(strTemp, "页次 ", "/").Trim();
                string strNumRight = GetKeyString(strTemp, "/", "每页").Trim();
                if (strNumLeft == strNumRight) //已经同步完
                {
                    bBreak = true;
                }
#else
                if (strRet.IndexOf("无相关商品") > 0) //已经同步完
                {
                    bBreak = true;
                }
#endif
                
                else if (strRet == "") //网络错误
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

        public int Fresh(out string strError,out int iFreshNum,out int iGoodCount, out bool bBreak,
            int iCurPage, bool bIsModify, ref Hashtable hWaitTable, ref Hashtable hFailTable, ref int iNetErrorCount)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            try
            {
                string strBefore = "共<i class=\"num\">";
                string strAfter = "</i>";

                if (m_strGoodContent.IndexOf("出售中的商品") < 0)
                {
                    strError = "数据错误1";
                    return -1;  //数据错误，停止
                }
                string strTemp = "";// m_strGoodContent.Substring(m_strGoodContent.IndexOf("我的商品总数"));
                string strNum = "";
                if (!bIsModify)
                {
                    strNum = GetKeyString(m_strGoodContent, strBefore, strAfter);
                    bool bRet = int.TryParse(strNum, out iGoodCount);
                    if (!bRet)
                    {
                        strError = "数据错误2";
                        return -1;  //数据错误，停止
                    }
                }
                
                _GoodsModifyStr objModify = new _GoodsModifyStr();
                _GoodsModifyStr objModifyWait = new _GoodsModifyStr();
                //通过解析得到当前的商品数据
                //m_strGoodContent
                Hashtable hTableGoods = new Hashtable();

                StringBuilder strBuilder = new StringBuilder();
                strBuilder.Append("method=save");
                string strBakData = "";
                int iGoodFreshCount = 0;    //本次要刷新的商品数
                List<string> strModifyNameList = new List<string>(); //本次刷新的商品名称列表

                Hashtable hTableSynDuplicate = null;
                if (bIsModify == false)   //刷新
                {
                    AnalyzeGoods(m_strGoodContent, ref hTableGoods, ref hTableSynDuplicate, 1);
                    iFreshNum = hTableGoods.Count;
                    foreach (DictionaryEntry dEntry in hTableGoods)
                    {
                        objModify = (_GoodsModifyStr)dEntry.Value;
                        strBuilder.Append(string.Format("&pid={0}&title{0}={1}&retailPrice{0}={2}&count_{0}=10", objModify.strNo, HttpUtility.UrlEncode(objModify.strName, Encoding.GetEncoding("gb2312")), objModify.fPrice));
                        iGoodFreshCount++;
                    }
                }
                else    //修改
                {
                    //拿前60个，直至拿完
                    iCurPage = 101;
                    foreach (DictionaryEntry dEntry in hWaitTable)
                    {
                        if (strModifyNameList.Count == 60)  //只拿60个
                        {
                            break;
                        }
                        objModify = (_GoodsModifyStr)dEntry.Value;
                        strBuilder.Append(string.Format("&pid={0}&title{0}={1}&retailPrice{0}={2}&count_{0}=10", objModify.strNo, HttpUtility.UrlEncode(objModify.strName, Encoding.GetEncoding("gb2312")), objModify.fPrice));
                        strModifyNameList.Add((string)dEntry.Key);
                    }
                    iFreshNum = strModifyNameList.Count;
                    if (iFreshNum < 1)
                    {
                        bBreak = true;
                        return 0;
                    }
                }

                /************************************************************************/
                /*                                                                      */
                /************************************************************************/
                string strData = strBuilder.ToString();
                string strRet = m_webPost.GetHttpPage("http://m.pconline.com.cn/member/product/products_onsell.htm",
                    "gb2312", "post", "", strData,
                    string.Format("http://m.pconline.com.cn/member/product/products_onsell.htm?&pageSize=60&pageNo={0}", iCurPage),
                    "*/*", "");
                if (strRet.IndexOf("无相关商品") > 0) //已经刷完
                {
                    //清除相关的待修改商品
                    foreach (string strName in strModifyNameList)
                    {
                        hWaitTable.Remove(strName);
                    }
                    if (!bIsModify)
                    {
                        bBreak = true;
                    }
                    else        //修改
                    {
                        if (hWaitTable.Count < 1)
                        {
                            bBreak = true;
                        }
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
                if (strRet.IndexOf("别人正在另一台服务器上修改您所修改的数据") > 0)
                {
                    strError = "其他人正在修改";
                    if (iNetErrorCount++ > 30)
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
                m_strGoodContent = strRet;
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


        void AnalyzeGoods(string strContent, ref Hashtable goodFreshList, ref Hashtable hSynDuplicateTable,int iPage)
        {
            try
            {
                strContent = strContent.Replace("\r", "");
                strContent = strContent.Replace("\n", "");
                strContent = strContent.Replace("\r\n", "");
                strContent = strContent.Replace("\t", "");
                Match match = Regex.Match(strContent, "<i>商品型号：</i>(.+?)删除商品");
                
                int match_num = 0;		//总共匹配到的数量
                string matchValue = "";
                string strPrice = "";
                while (match.Success)
                {
                    _GoodsModifyStr goodModify = new _GoodsModifyStr();
                    //id="down_limit_6981520" type="hidden" value="774.0" /><input id="up_limit_6981520" type="hidden" value="1677.0" />
                    matchValue = match.Groups[1].Value;		//获取匹配到的字符串


                    goodModify.strNo = GetKeyString(matchValue, "\"down_limit_", "\"");
                    if (matchValue.IndexOf("自定义商品")>0)
                    {
                        //索尼爱立信 XPERIA E15i（自定义商品）
                        goodModify.fHighPrice = 100000;
                        goodModify.fLowPrice = 0;
                        goodModify.strNoEx = matchValue.Substring(0, matchValue.IndexOf("</div>"));
                    }
                    else
                    {
                        
                        strPrice = GetKeyString(matchValue,
                            string.Format("id=\"down_limit_{0}\" type=\"hidden\" value=\"", goodModify.strNo),
                            "\"");
                        float.TryParse(strPrice, out goodModify.fLowPrice);
                        strPrice = GetKeyString(matchValue,
                            string.Format("id=\"up_limit_{0}\" type=\"hidden\" value=\"", goodModify.strNo),
                            "\"");
                        float.TryParse(strPrice, out goodModify.fHighPrice);             

                        // +"    " + iPage.ToString();
                        goodModify.strNoEx = GetKeyString(matchValue,
                           "target=\"_blank\">",
                            "</a>");
                    }
                    strPrice = GetKeyString(matchValue,
                            string.Format("id=\"retailPrice{0}\" type=\"text\" value=\"", goodModify.strNo),
                            "\"");
                    float.TryParse(strPrice, out goodModify.fPrice);
                    goodModify.strName = GetKeyString(matchValue,
                            string.Format("id=\"title{0}\" type=\"text\" value=\"", goodModify.strNo),
                            "\"");
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

        public bool SetTransTypeAndPayType(_GoodsFreshStr goodInfo)
        {
            string strCheckOutUrl = "https://shopping.vancl.com/checkout";
            string strDeleverUrl = "https://shopping.vancl.com/checkout/setdelivery?transferId=01&transferTime=AnyTime";
            string strRet = m_webPost.GetHttpPage(strDeleverUrl,
                    "utf-8", "get", "", "",
                    strCheckOutUrl,
                    "*/*", "");
            if (strRet.IndexOf("<h2>出错了</h2>") > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf("<h2>出错了</h2>"));
                goodInfo.strFreshStatus = GetKeyString(strRet, "<p>", "</p>");
                return false;
            }
            else if (strRet.IndexOf("配送方式：") < 0)
            {
                goodInfo.strFreshStatus = "设置寄送方式出错";
                return false;
            }

            string strPayTypeUrl = "https://shopping.vancl.com/checkout/setpayment?identifier=-1%24-1%24False%24False";
            strRet = m_webPost.GetHttpPage(strPayTypeUrl,
                    "utf-8", "get", "", "",
                    strCheckOutUrl,
                    "*/*", "");
            if (strRet.IndexOf("<h2>出错了</h2>") > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf("<h2>出错了</h2>"));
                goodInfo.strFreshStatus = GetKeyString(strRet, "<p>", "</p>");
                return false;
            }
            else if (strRet.IndexOf("在线支付") < 0)
            {
                goodInfo.strFreshStatus = "设置支付方式出错";
                return false;
            }
            goodInfo.iFreshNumCount = 0;
            return SubmitOrder(goodInfo, "");            
        }

        public bool SubmitOrder(_GoodsFreshStr goodInfo, string strCheckCode)
        {
            string strCheckOutUrl = "https://shopping.vancl.com/checkout";
            string strData = "";
            strData = "comments=&requireInvoice=true&InvoiceCustom=Personal&invoicetitle=%E4%B8%AA%E4%BA%BA&InvoiceContent=Details&InvoiceTaxNo=&requireedm=true";
            if (strCheckCode != "")
                strData += string.Format("&verifycode={0}", strCheckCode);

            string strSubmitUrl = "https://shopping.vancl.com/checkout/save";
            string strRet = m_webPost.GetHttpPage(strSubmitUrl,
                    "utf-8", "post", "", strData,
                    strCheckOutUrl,
                    "*/*", "");

            /*
             <span>   <label class="error">请输入验证码</label></span>
                    <span>  <a id="verify-code-link">换一题</a> </span>
                    <span>  <input type="text" id="verify-code-txt" name="verifycode" />  </span>
                    <span>  <img id="verify-code-img" alt="" src="/VerifyingCode.ashx?380"/> </span>
                    <span id="verify-code" data-valid-error="答案错误,请重新输入"> </span>
                    <span id="verify-code-tip">验证码:</span>
             */
            strRet = strRet.ToLower();
            string strFlag = "请输入验证码";
            string strFlag1 = "<img id=\"verify-code-img\"";
            if (strRet.IndexOf(strFlag) > 0 && strRet.IndexOf(strFlag1) > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf(strFlag1));
                string strCheckCodeUrl = "https://shopping.vancl.com" + GetKeyString(strRet, "src=\"", "\"");
                m_objForm.GetCheckCode(m_webPost, "", out goodInfo.strFreshStatus, out strCheckCode, strCheckCodeUrl, strCheckOutUrl);
                goodInfo.iFreshNumCount++;
                if (goodInfo.iFreshNumCount > 3)
                    return false;
                return SubmitOrder(goodInfo, strCheckCode);
            }

            if (strRet.IndexOf("<h2>出错了</h2>") > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf("<h2>出错了</h2>"));
                goodInfo.strFreshStatus = GetKeyString(strRet, "<p>", "</p>");
                return false;
            }
            else if (strRet.IndexOf("订单已提交成功") < 0)
            {
                goodInfo.strFreshStatus = "提交订单出错，可能提交订单太频繁，需要验证码，【不要同时使用多个软件，用同样的账号】，也可以考虑在失败后换IP";
                return false;
            }
            strRet = strRet.Substring(strRet.IndexOf("订单已提交成功"));
            strRet = strRet.Substring(strRet.IndexOf("tbody"));
            goodInfo.strOrderID = GetKeyString(strRet, "<td>", "</td>");
            goodInfo.strFreshStatus = "下单成功：" + goodInfo.strOrderID;
            return true;
        }

        public bool AddAddress(_GoodsFreshStr goodInfo)
        {
            string strCheckOutUrl = "https://shopping.vancl.com/checkout";
            string strRet = m_webPost.GetHttpPage(strCheckOutUrl,
                    "utf-8", "get", "", "",
                    "http://shopping.vancl.com/mycart?showbuy=false",
                    "*/*", "");
            if (strRet.IndexOf("确认提交订单") < 0)
            {
                goodInfo.strFreshStatus = "加载定单填写页面失败";
                return false;
            }
            string strName = goodInfo.strReceiver;
            string strProvince = goodInfo.strProvince;
            string strCity = goodInfo.strCity;
            string strTown = goodInfo.strTown;
            string strAddr = goodInfo.strAddr;
            string strPostCode = goodInfo.strPostCode;
            string strMobile = goodInfo.strPhone;

            strName = HttpUtility.UrlEncode(strName, Encoding.GetEncoding("UTF-8"));
           
            strProvince = HttpUtility.UrlEncode( HttpUtility.UrlEncode(strProvince, Encoding.GetEncoding("UTF-8")) );
            strCity = HttpUtility.UrlEncode( HttpUtility.UrlEncode(strCity, Encoding.GetEncoding("UTF-8")) );
            strTown = HttpUtility.UrlEncode( HttpUtility.UrlEncode(strTown, Encoding.GetEncoding("UTF-8")) );
            strAddr = HttpUtility.UrlEncode(strAddr, Encoding.GetEncoding("UTF-8"));
            string strAddrData = string.Format("model.FullName={0}&model.Province={1}&model.City={2}&model.Area={3}&model.Address={4}&model.PostalCode={5}&model.MobilePhone={6}&model.Phone=&model.AddressId=0",
                strName,
                strProvince,
                strCity,
                strTown,
                strAddr,
                strPostCode,
                strMobile);
            string strAddrUrl = "https://shopping.vancl.com/checkout/saveinlandaddress";
            strRet = m_webPost.GetHttpPage(strAddrUrl,
                    "utf-8", "post", "", strAddrData,
                    strCheckOutUrl,
                    "*/*", "");
            if (strRet.IndexOf("<h2>出错了</h2>") > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf("<h2>出错了</h2>"));
                goodInfo.strFreshStatus = GetKeyString(strRet, "<p>", "</p>");
                return false;
            }
            else if (strRet.IndexOf("收货地址：") < 0)
            {
                goodInfo.strFreshStatus = "设置收货地址出错";
                return false;
            }
            return true;
        }

        public bool AddToCart(_GoodsFreshStr goodInfo)
        {
            string strCartUrl = "http://shopping.vancl.com/mycart";
            string strCartDelUrl = "http://shopping.vancl.com/mycart/batchdelcart";

            string strRet = m_webPost.GetHttpPage(strCartDelUrl,
                    "utf-8", "get", "", "",
                    strCartUrl,
                    "*/*", "");
            
            string strGoodUrl = string.Format("http://item.vancl.com/{0}.html", goodInfo.goodId);
            
             strRet = m_webPost.GetHttpPage(strGoodUrl,
                    "utf-8", "get", "", "",
                    "",
                    "*/*", "");
            if (strRet.IndexOf("<h2>出错了</h2>") > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf("<h2>出错了</h2>"));
                goodInfo.strFreshStatus = GetKeyString(strRet, "<p>", "</p>");
                return false;
            }
            else if (strRet == "")
            {
                goodInfo.strFreshStatus = "加载商品页面出错";
                return false;
            }

            strRet = strRet.ToLower();
            goodInfo.strGoodName = GetKeyString(strRet, "<title>", "</title>");//<title>流金幻彩松紧女短靴 咖啡色 - 鞋 女鞋 靴子 时尚女靴 - VANCL 凡客诚品</title>
            string strPrice = "2.00";
            string strAddtoCartUrl = string.Format("http://shopping.vancl.com/mycart/batchadd?sku={0}&price={1}&qty={2}&async=true&callback=jQuery171013180430320374498_1343624828578&_={3}",
                goodInfo.skuId,
                strPrice,
                goodInfo.strNum,
                DateTime.Now.ToFileTime());
            strRet = m_webPost.GetHttpPage(strAddtoCartUrl,
                    "utf-8", "get", "", "",
                    strGoodUrl,
                    "*/*", "");
            strRet = strRet.ToLower();

            //jQuery171013180430320374498_1343624828578({"Type":"Normal","Title":"出错了","Content":"您要订购的商品不存在或者已下架。","LinkUrl":null})
            if (strRet.IndexOf("出错了") > 0)
            {
                strRet = strRet.Substring(strRet.IndexOf("\"出错了"));
                goodInfo.strFreshStatus = GetKeyString(strRet, "content\":\"", "\"");
                return false;
            }
            else if (strRet.IndexOf("\"totalproducts\"") < 0)
            {
                goodInfo.strFreshStatus = "加入购物车出错";
                return false;
            }
            return true;
        }
    }
}
        
