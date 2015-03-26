#define __PAOPAO
//表示中关村
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
        public string strMid;
        
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
        public string strTicket;
        public string strPay;
        public string strSequence;
        public string strTrueprice;
        public string strMid;
        public string strOtherInfo;  //表示特价等的字符串
        public string dealerPriceId;
        public string oldstatus;
        public string refprice;
        public string productname;
        public string strAdvert;
      
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
                string strUrl = string.Format("http://mydealer.zol.com.cn/admin/include/key.php?type=1&mixture={0}", DateTime.Now.ToString("yyyyMMddHHmmss"));
                Stream stream = m_webPost.GetHttpPageSteam(strUrl, "gb2312", "get", "", "", "http://dealer.zol.com.cn/merchant_login.php", "*/*", "");
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
        //登录中关村
        public bool PaoPaoLogin(string strAcc, string strPwd, string strCode, out string strError, bool bQuickFresh)
        {
            strError = "";
            try
            {
                string strData = string.Format("renew=true&username={0}&password={1}&siteid=1&productid=18",
                strAcc,
                strPwd,
                strCode);
                string strRet = m_webPost.GetHttpPage("http://passport.yesky.com/cas/login?service=http%3A%2F%2Fmanage.jxs.yesky.com%2Fcompany%2Finfo.do&furl=http%3A%2F%2Fit365.yesky.com%2Fjsp%2FnewFindDealerf%2Fportal.comlogin.jsp",
                    "gb2312", "post", "", strData,
                    "http://it365.yesky.com/comcenter/comlogin.do",
                    "*/*", "");
                if (strRet == "")
                {
                    strError = "登录失败,网络错误";
                    return false;
                }
                if (strRet.IndexOf("刷新全部报价") < 0)
                {
                    strError = "登录失败";
                    return false;
                }
                if (!bQuickFresh)
                {
                    //strError = GetKeyString(strRet,"http://dealer.zol.com.cn/d_","/");
                    string strUrl = "http://manage.jxs.yesky.com/price/searchprice.do?searchProductPrice.type=2&searchProductPrice.status=1&searchProductPrice.pageNo=1&searchProductPrice.sort=0&clickPage=2&clickIndex=0&searchProductPrice.brandId=0&searchProductPrice.fatherId=0&searchProductPrice.relatetype=0&returnMessage=操作成功！";
                    strRet = m_webPost.GetHttpPage(strUrl,
                    "gb2312", "get", "", "",
                    "http://manage.jxs.yesky.com/price/searchprice.do",
                    "*/*", "");
                    if (strRet == "")
                    {
                        strError = "登录失败,网络错误";
                        return false;
                    }
                    m_strGoodContent = strRet;
                    //获取页面成功
                    return true;
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
        public bool Login(string strAcc,string strPwd,bool bQuickFresh)
        {
            string strData = string.Format("return=http%3A%2F%2Fm.pconline.com.cn%2Fmember%2FloginPrompt.htm&auto_login=&username={0}&password={1}",
                strAcc,
                strPwd );
            string strRet = m_webPost.GetHttpPage("http://passport2.pconline.com.cn/passport2/passport/login.jsp",
                "utf-8", "post", "", strData,
                "http://m.pconline.com.cn/member/login.htm",
                "*/*", "");
            if (!( strRet.IndexOf("passport.pcauto.com.cn")>0 && strRet.IndexOf(strAcc)>0))
            {
                return false;
            }
            string strUrl = "";
            string strSucFlag = "";
            if (bQuickFresh)
            {
                strUrl = "http://m.pconline.com.cn/member/index.htm";
                strSucFlag = "欢迎您回来!";
            }
            else{
                strUrl = "http://m.pconline.com.cn/member/product/products_onsell.htm?&pageSize=60&pageNo=1";
                strSucFlag = string.Format("{0}[老板]",strAcc);
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
            else{
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
                //http://manage.jxs.yesky.com/dwr/exec/priceAjax.updateDealerPriceDate
                //刷新全部报价</A>(9)次
                string strBefore = "刷新全部报价</A>(";
                string strAfter = ")次";
                string strNumLeft = GetKeyString(m_strGoodContent, strBefore, strAfter);
                int iNumLeft = 0;
                strLeft = strNumLeft;
                int.TryParse(strNumLeft, out iNumLeft);
                if (iNumLeft < 1)
                {
                    strError = "您今天的刷新次数已用完";
                    return -1;
                }
                string strUrl = "";
                string strType = "post";
                string strData = "";
                string strReffer = "";
                /*
                 callCount=1
c0-scriptName=priceAjax
c0-methodName=updateDealerPriceDate
c0-id=9067_1299819179658
c0-param0=number:112752
xml=true
                 */

                //var cid = 112752;
                string strId = GetKeyString(m_strGoodContent, "var cid = ", ";");
#if __PAOPAO
                long jsBeginTick = DateTime.Parse("1970-1-1").Ticks;
                DateTime dt2 = DateTime.Now.ToLocalTime();
                
                long dt2Ticks = dt2.ToUniversalTime().Ticks;
                long dt2JsTicks = (dt2Ticks - jsBeginTick) / (1000 * 10);
                strUrl = string.Format("http://manage.jxs.yesky.com/dwr/exec/priceAjax.updateDealerPriceDate");//
                //1299041534823
                strReffer = "http://manage.jxs.yesky.com/company/info.do";
                strData = string.Format("callCount=1\r\nc0-scriptName=priceAjax\r\nc0-methodName=updateDealerPriceDate\r\nc0-id=9067_{0}\r\nc0-param0=number:{1}\r\nxml=true",
                    dt2JsTicks,
                    strId
                    );
#else
#endif
                string strRet = m_webPost.GetHttpPage(strUrl,
                    "gb2312", strType, "", strData,
                    strReffer,
                    "*/*", "");

                if (strRet == "")
                {
                    strError = "刷新失败";
                    return -1;
                }
                else if (strRet.IndexOf("var s0=") >= 0)
                {
                    return 0;
                }
                return -1;
               
            }
            catch (System.Exception ex)
            {
                strError = "刷新异常";
                return -1;
            }
            
        }

        public int Synchronize(out string strError, out int iFreshNum, out int iGoodCount, out bool bBreak,
            int iCurPage, ref int iNetErrorCount, ref Hashtable hSynTable, ref Hashtable hSynDuplicateTable,string strMid)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            try
            {
                string strBefore = "找到相符的产品";
                string strAfter = "个";

                if (m_strGoodContent.IndexOf("找到相符的产品") < 0)
                {
                    strError = "数据错误1";
                    return -1;  //数据错误，停止
                }
                // string strTemp = m_strGoodContent.Substring(m_strGoodContent.IndexOf("我的商品总数"));
                string strNum = GetKeyString(m_strGoodContent, strBefore, strAfter);
                bool bRet = int.TryParse(strNum, out iGoodCount);
                if (!bRet)
                {
                    strError = "数据错误2";
                    return -1;  //数据错误，停止
                }

                string strUrl = string.Format("http://manage.jxs.yesky.com/price/searchprice.do?searchProductPrice.type=2&searchProductPrice.status=1&searchProductPrice.pageNo={0}&searchProductPrice.sort=0&clickPage=2&clickIndex=0&searchProductPrice.brandId=0&searchProductPrice.fatherId=0&searchProductPrice.relatetype=0&returnMessage=操作成功！", iCurPage);
                AnalyzeGoods(m_strGoodContent, ref hSynTable, ref hSynDuplicateTable, iCurPage - 1);
                //<span id="numpage_t">1/56 <a
                string strTemp = m_strGoodContent.Substring(m_strGoodContent.IndexOf("<span id=\"numpage_t\">"));
                string strNumLeft = GetKeyString(strTemp, ">", "/").Trim();
                string strNumRight = GetKeyString(strTemp, "/", "<a").Trim();
                if (strNumLeft == strNumRight) //已经同步完
                {
                    bBreak = true;
                    return 0;
                }

                string strRet = m_webPost.GetHttpPage(strUrl,
                    "gb2312", "get", "", "",
                    "",
                    "*/*", "");
                //如果页码相同,则可以判断已经完成了
                //页次 49/49 每页
#if __PAOPAO
                
#else
                if (strRet.IndexOf("无相关商品") > 0) //已经同步完
                {
                    bBreak = true;
                }
#endif
                
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
                return 0;
            }
            catch (System.Exception ex)
            {
                strError = "程序异常";
                return -1;  //数据错误，停止
            }
            
        }
        public int FreshPaoPao(out string strError, out int iFreshNum, out int iGoodCount, out bool bBreak,
           int iCurPage, bool bIsModify, ref Hashtable hWaitTable, ref Hashtable hFailTable, ref int iNetErrorCount,string strMid,string strAcc)
        {
            iFreshNum = 0;
            iGoodCount = 0;
            strError = "";
            bBreak = false;
            bool bRet = false;


            string strDirectory = System.AppDomain.CurrentDomain.BaseDirectory + "modifyfail";
            
            try
            {   
                string strBefore = "共<span>";
                string strAfter = "</span>";
                if (m_strGoodContent.IndexOf(string.Format("http://dealer.zol.com.cn/d_{0}/",strMid)) < 0)
                {
                    strError = "数据错误";
                    return -1;  //数据错误，停止
                }
                string strTemp = m_strGoodContent.Substring(m_strGoodContent.IndexOf("<strong>产品搜索 : </strong>"));
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
                strBuilder.Append("");
                string strBakData = "";
                int iGoodFreshCount = 0;    //本次要刷新的商品数
                List<string> strModifyNameList = new List<string>(); //本次刷新的商品名称列表

                Hashtable hTableSynDuplicate = null;

                //HttpUtility.UrlEncode(objModify.strName, Encoding.GetEncoding("gb2312"))
                //拿前20个，直至拿完
                iCurPage = 101;
                string strPostUrl = "";
                foreach (DictionaryEntry dEntry in hWaitTable)
                {
                    if (strModifyNameList.Count == 1)  //只拿1个
                    {
                        break;
                    }
                    objModify = (_GoodsModifyStr)dEntry.Value;
                    strModifyNameList.Add((string)dEntry.Key);
                    strPostUrl = string.Format("http://mydealer.zol.com.cn/admin/ajax/ajax_response.php?type=2&mid={0}&pro_id={1}&price={2}&old_price=undefined&sequence={3}&ispay={4}&have_ticket={5}&istrueprice={6}",
                        objModify.strMid,
                        objModify.strNo,
                        objModify.fPrice,
                        objModify.strSequence,
                        objModify.strPay,
                        objModify.strTicket,
                        objModify.strTrueprice);
                }
               // strBuilder.Append("]");
                iFreshNum = strModifyNameList.Count;
                if (iFreshNum < 1)
                {
                    bBreak = true;
                    return 0;
                }

                /************************************************************************/
                /*                                                                      */
                /************************************************************************/
                string strData = "";
                string strRet = m_webPost.GetHttpPage(strPostUrl,
                    "gb2312", "post", "", strData,
                    string.Format("http://mydealer.zol.com.cn/admin/index.php?action=price"),
                    "*/*", "");

                if (strRet == "") //网络错误
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
                            StringBuilder strBuilderEx = new StringBuilder();
                            strBuilderEx.Append(string.Format("{0} --- {1} --- {2}", objModify.strNoEx, objModify.fPrice, strError));
                            WriteInfo(strDirectory, strBuilderEx, strAcc);
                        }
                        return -1;  //出错超过3次,停止
                    }
                    return -3;
                }
                
                if (strRet.Length > 10 || strRet == "1" || strRet == "4")
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
                    return 0;
                }
                //!
                else if (strRet == "5")
                {
                    strError = "同一款产品每天报价不能超过两次！";
                    foreach (string strName in strModifyNameList)
                    {
                        hWaitTable.Remove(strName);
                    }
                    StringBuilder strBuilderEx = new StringBuilder();
                    strBuilderEx.Append(string.Format("{0} --- {1} --- {2}", objModify.strNoEx, objModify.fPrice, strError));
                    WriteInfo(strDirectory, strBuilderEx, strAcc);
                    return -2;
                }
                else if (strRet == "3")
                {
                    strError = "两次报价金额相同！";
                    foreach (string strName in strModifyNameList)
                    {
                        hWaitTable.Remove(strName);
                    }
                    StringBuilder strBuilderEx = new StringBuilder();
                    strBuilderEx.Append(string.Format("{0} --- {1} --- {2}", objModify.strNoEx, objModify.fPrice, strError));
                    WriteInfo(strDirectory, strBuilderEx, strAcc);
                    return -2;
                }
                else if (strRet == "6")
                {
                    strError = "价格不在报价区间！";
                    foreach (string strName in strModifyNameList)
                    {
                        hWaitTable.Remove(strName);
                    }
                    StringBuilder strBuilderEx = new StringBuilder();
                    strBuilderEx.Append(string.Format("{0} --- {1} --- {2}", objModify.strNoEx, objModify.fPrice, strError));
                    WriteInfo(strDirectory, strBuilderEx, strAcc);
                    return -2;
                }
                else
                {
                    foreach (string strName in strModifyNameList)
                    {
                        hWaitTable.Remove(strName);
                    }
                    return 0;
                }
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
                
                _GoodsModifyStr objModify = new _GoodsModifyStr();
                _GoodsModifyStr objModifyWait = new _GoodsModifyStr();
                //通过解析得到当前的商品数据
                //m_strGoodContent
                if (!bIsModify)
                {
                    string strBefore = "找到相符的产品";
                    string strAfter = "个";

                    if (m_strGoodContent.IndexOf("找到相符的产品") < 0)
                    {
                        strError = "数据错误1";
                        return -1;  //数据错误，停止
                    }
                    // string strTemp = m_strGoodContent.Substring(m_strGoodContent.IndexOf("我的商品总数"));
                    string strNum = GetKeyString(m_strGoodContent, strBefore, strAfter);
                    bool bRet = int.TryParse(strNum, out iGoodCount);
                    if (!bRet)
                    {
                        strError = "数据错误2";
                        return -1;  //数据错误，停止
                    }

                    string strTemp = m_strGoodContent.Substring(m_strGoodContent.IndexOf("<span id=\"numpage_t\">"));
                    string strNumLeft = GetKeyString(strTemp, ">", "/").Trim();
                    string strNumRight = GetKeyString(strTemp, "/", "<a").Trim();
                    if (strNumLeft == strNumRight) //已经同步完
                    {
                        bBreak = true;
                    }
                }
                

                Hashtable hTableGoods = new Hashtable();

                StringBuilder strBuilder = new StringBuilder();
                strBuilder.Append(string.Format("clickIndex=0&clickPage=2&searchProductPrice.type=2&searchProductPrice.status=1&searchProductPrice.pageNo={0}&searchProductPrice.brandId=0&searchProductPrice.sort=0&searchProductPrice.fatherId=0&searchProductPrice.relatetype=0&searchProductPrice.productStatus=-1&searchProductPrice.productName=&productstatus=-1",iCurPage));
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
                        strBuilder.Append(string.Format("&productid={0}&price_{0}={7}&advert_{0}={1}&dealerPriceId_{0}={2}&oldstatus_{0}={3}&downflowprice_{0}={4}&upflowprice_{0}={5}&refprice_{0}={6}{8}", 
                        objModify.strNo,
                        HttpUtility.UrlEncode(objModify.strAdvert, Encoding.GetEncoding("gb2312")),
                        objModify.dealerPriceId,
                        objModify.oldstatus,
                        objModify.fLowPrice,
                        objModify.fHighPrice,
                        objModify.refprice,
                        objModify.fPrice,
                        objModify.strOtherInfo));
                        iGoodFreshCount++;
                    }
                }
                else    //修改
                {
                    //拿前60个，直至拿完
                    iCurPage = 101;
                    foreach (DictionaryEntry dEntry in hWaitTable)
                    {
                        if (strModifyNameList.Count == 30) //只拿60个
                        {
                            break;
                        }
                        objModify = (_GoodsModifyStr)dEntry.Value;
                        strBuilder.Append(string.Format("&productid={0}&price_{0}={7}&advert_{0}={1}&dealerPriceId_{0}={2}&oldstatus_{0}={3}&downflowprice_{0}={4}&upflowprice_{0}={5}&refprice_{0}={6}{8}", 
                        objModify.strNo,
                        HttpUtility.UrlEncode(objModify.strAdvert, Encoding.GetEncoding("gb2312")),
                        objModify.dealerPriceId,
                        objModify.oldstatus,
                        objModify.fLowPrice,
                        objModify.fHighPrice,
                        objModify.refprice,
                        objModify.fPrice,
                        objModify.strOtherInfo));
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
                string strReffer = string.Format("http://manage.jxs.yesky.com/price/searchprice.do");
                //string strReffer = string.Format("http://manage.jxs.yesky.com/price/searchprice.do?searchProductPrice.type=2&searchProductPrice.status=1&searchProductPrice.pageNo={0}&searchProductPrice.sort=0&clickPage=2&clickIndex=0&searchProductPrice.brandId=0&searchProductPrice.fatherId=0&searchProductPrice.relatetype=0&returnMessage=aaa", iCurPage);
                string strData = strBuilder.ToString();
                string strRet = m_webPost.GetHttpPage("http://manage.jxs.yesky.com/price/saveproductprice.do",
                    "gb2312", "post", "", strData,
                    strReffer,
                    "*/*", "");
//                 if (strRet.IndexOf("无相关商品") > 0) //已经刷完
//                 {
//                     //清除相关的待修改商品
//                     foreach (string strName in strModifyNameList)
//                     {
//                         hWaitTable.Remove(strName);
//                     }
//                     if (!bIsModify)
//                     {
//                         bBreak = true;
//                     }
//                     else        //修改
//                     {
//                         if (hWaitTable.Count < 1)
//                         {
//                             bBreak = true;
//                         }
//                     }
//                 }
                if (strRet == "") //网络错误
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
                //成功
                foreach (string strName in strModifyNameList)
                {
                    objModify.iFailFlag = 3;
                    hFailTable.Add(strName, hWaitTable[strName]);
                    hWaitTable.Remove(strName);
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
        void AnalyzeGoodsPaoPao(string strContent, ref Hashtable goodFreshList, ref Hashtable hSynDuplicateTable, int iPage,string strMid)
        {
            try
            {
                strContent = strContent.Replace("\r", "");
                strContent = strContent.Replace("\n", "");
                strContent = strContent.Replace("\r\n", "");
                Match match = Regex.Match(strContent, "><input type=\"checkbox\" name=\"product_id(.+?)报价时间：");
                
                int match_num = 0;		//总共匹配到的数量
                string matchValue = "";
                while (match.Success)
                {
                    _GoodsModifyStr goodModify = new _GoodsModifyStr();
                    //id="down_limit_6981520" type="hidden" value="774.0" /><input id="up_limit_6981520" type="hidden" value="1677.0" />
                    matchValue = match.Groups[1].Value;		//获取匹配到的字符串
                    //"><textarea id="txtRemark384380
                    goodModify.strNo = GetKeyString(matchValue, "value=\"", "\"");

                    string strTempPrice = matchValue.Substring(matchValue.IndexOf(string.Format("name=\"price{0}\" id=\"price{0}\" type=\"text\"", goodModify.strNo)));
                    //362"
                    string strPrice = GetKeyString(strTempPrice,
                        "value=\"",
                        "\"");
                    float.TryParse(strPrice, out goodModify.fPrice);
                    
                    //name="have_ticket80865" id="have_ticket80865" checked  value='1'  />带票
                    string strTempEx = matchValue.Substring(matchValue.IndexOf(string.Format("name=\"have_ticket{0}\" id=", goodModify.strNo)));

                    goodModify.strTicket = GetKeyString(strTempEx,
                        "value='",
                        "'");
                    goodModify.strPay = GetKeyString(matchValue,
                        string.Format("id=\"ispay_{0}\" checked value=\"",goodModify.strNo),
                        "\"");
                    goodModify.strSequence = GetKeyString(matchValue,
                        string.Format("id=\"sequence{0}\" class=\"txt\" type=\"text\" value=\"",goodModify.strNo),
                        "\"");
                    
                    goodModify.strTrueprice = GetKeyString(matchValue,
                        string.Format("id=\"istrueprice{0}\" checked value=\"",goodModify.strNo),
                        "\"");

                    goodModify.strMid = strMid;
                    goodModify.strNoEx = GetKeyString(matchValue,
                        "javascript:cfg_module_pic('",
                        "'");
                    goodModify.strName = goodModify.strNoEx;

                    string strTemp = GetKeyString(matchValue, "javascript:merchant_price_set(", ")");
                    string[] strArray = strTemp.Split(',');
                    if (strArray.Length > 6)
                    {
                        float.TryParse(strArray[4], out goodModify.fLowPrice);
                        float.TryParse(strArray[5], out goodModify.fHighPrice);
                    }
                    else
                    {
                        goodModify.fLowPrice = 0;
                        goodModify.fHighPrice = 0;
                    }

                    //获取最高和最低价格
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
                Match match = Regex.Match(strContent, "<input  pid=(.+?)循环结束");
                
                int match_num = 0;		//总共匹配到的数量
                string matchValue = "";
                string strPrice = "";
                //cpricestate_
                string strTemp = "";
                while (match.Success)
                {
                    _GoodsModifyStr goodModify = new _GoodsModifyStr();
                    //id="down_limit_6981520" type="hidden" value="774.0" /><input id="up_limit_6981520" type="hidden" value="1677.0" />
                    matchValue = match.Groups[1].Value;		//获取匹配到的字符串


                    goodModify.strNo = GetKeyString(matchValue, "\"", "\"");

                    strTemp = matchValue.Substring(matchValue.IndexOf("dealerPriceId_"));
                    goodModify.dealerPriceId = GetKeyString(strTemp, "value=\"", "\"");
                    
                    strTemp = matchValue.Substring(matchValue.IndexOf("oldstatus_"));
                    goodModify.oldstatus = GetKeyString(strTemp, "value=\"", "\"");

                    strTemp = matchValue.Substring(matchValue.IndexOf("refprice_"));
                    goodModify.refprice = GetKeyString(strTemp, "value=\"", "\"");

                    strTemp = matchValue.Substring(matchValue.IndexOf("productname_"));
                    goodModify.strNoEx = GetKeyString(strTemp, "value=\"", "\"");

                    strTemp = matchValue.Substring(matchValue.IndexOf("id=\"advert_"));
                    goodModify.strAdvert = GetKeyString(strTemp, "value=\"", "\"");

                    strTemp = matchValue.Substring(matchValue.IndexOf("downflowprice_"));
                    strPrice = GetKeyString(strTemp, "value=\"", "\"");
                    float.TryParse(strPrice, out goodModify.fLowPrice);

                    strTemp = matchValue.Substring(matchValue.IndexOf("upflowprice_"));
                    strPrice = GetKeyString(strTemp, "value=\"", "\"");
                    float.TryParse(strPrice, out goodModify.fHighPrice);

                    strTemp = matchValue.Substring(matchValue.IndexOf("onchange=\"changePriceData("));
                    strPrice = GetKeyString(strTemp, "value=\"", "\"");
                    float.TryParse(strPrice, out goodModify.fPrice);
                    
                    
                    goodModify.strOtherInfo = string.Format("&cpricestate_{0}=true&priority_{0}=0",goodModify.strNo);
                    
                    //特价
                    strTemp = GetKeyString(matchValue, string.Format("id=\"salestatus_{0}", goodModify.strNo), "特价");
                    if (strTemp.ToLower().IndexOf("checked")>0)
                    {
                        goodModify.strOtherInfo += string.Format("&salestatus_{0}=8", goodModify.strNo);
                    }
                    //热卖
                    strTemp = GetKeyString(matchValue, string.Format("id=\"hotstatus_{0}", goodModify.strNo), "热卖");
                    if (strTemp.ToLower().IndexOf("checked") > 0)
                    {
                        goodModify.strOtherInfo += string.Format("&hotstatus_{0}=4", goodModify.strNo);
                    }
                    //新品
                    strTemp = GetKeyString(matchValue, string.Format("id=\"newstatus_{0}", goodModify.strNo), "新品");
                    if (strTemp.ToLower().IndexOf("checked") > 0)
                    {
                        goodModify.strOtherInfo += string.Format("&newstatus_{0}=2", goodModify.strNo);
                    }
                    //下架
                    strTemp = GetKeyString(matchValue, string.Format("id=\"stopstatus_{0}", goodModify.strNo), "下架");
                    if (strTemp.ToLower().IndexOf("checked") > 0)
                    {
                        goodModify.strOtherInfo += string.Format("&stopstatus_{0}=1", goodModify.strNo);
                    }
                    //行货
                    strTemp = GetKeyString(matchValue, string.Format("id=\"qualityGoodsStatus_{0}", goodModify.strNo), "行货代票");
                    if (strTemp.ToLower().IndexOf("checked") > 0)
                    {
                        goodModify.strOtherInfo += string.Format("&qualityGoodsStatus_{0}=16", goodModify.strNo);
                    }

                    
                    /*<input   name="hotstatus_516996" id="hotstatus_516996"  type="checkbox"  value="4" onclick="thisClick(this,516996);" />热卖
                     * <input   name="newstatus_516996" id="newstatus_516996" type="checkbox" value="2" onclick="thisClick(this,516996);" />新品
                     * <input   name="stopstatus_516996" id="stopstatus_516996" onclick="stopProductPrice(this,516996);" type="checkbox" value="1" />下架
                     * <input   name="qualityGoodsStatus_516996" id="qualityGoodsStatus_516996" type="checkbox" value="16" onclick="thisClick(this,516996);" checked/>行货代票
                    */
                    /*
                    <input type="hidden" name="dealerPriceId_542009" value="17904037" id="dealerPriceId_542009">
                    <input type="hidden" name="oldstatus_542009" value="21" id="oldstatus_542009">
                    <input type="hidden" name="downflowprice_542009" value="990" id="downflowprice_542009">
                    <input type="hidden" name="upflowprice_542009" value="1755" id="upflowprice_542009">
                    <input type="hidden" name="refprice_542009" id="refprice_542009"  value="1350.0" id="refprice_542009"/>
                    <input type="hidden" name="cpricestate_542009" value="true" id="cpricestate_542009"/>
                    <input type="hidden" id="catalogid_542009" value="291"/>
                    <input type="hidden" id="brandid_542009" value="294"/>
                    <input type="hidden" id="productname_542009" value="诺基亚5230"/>  
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
                int ii = 0;
            }
            catch (System.Exception ex)
            {
            	
            }            
        }

        public void WriteInfo(string strDirectry, StringBuilder strInfoBuilder, string strAcc)
        {
            try
            {
                strInfoBuilder.Append("\r\n");
                if (!Directory.Exists(strDirectry))
                {
                    Directory.CreateDirectory(strDirectry);
                }
                string strFileName = strAcc.ToString() + ".txt";
                strFileName = strDirectry + "\\" + strFileName;
               
                StreamWriter fs = File.AppendText(strFileName);
                fs.Write(strInfoBuilder.ToString());
                fs.Close();
            }
            catch
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
        