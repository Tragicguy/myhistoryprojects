using System;
using System.Web;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using System.Xml.Serialization;
using System.Text.RegularExpressions;
namespace SmsServer.BLL
{
    public class SmsHttpSend
    {
       // public static HttpWebRequest objRequest = (HttpWebRequest)HttpWebRequest.Create(SmsSetIni.KeepOnlineUrl);
        /// <summary>
        /// 唯一的对外的HTTP发送接口
        /// </summary>
        /// <param name="strURL"></param>
        /// <param name="strParams"></param>
        /// <param name="iTimeOut"></param>
        /// <param name="iEncodeType"></param>
        /// <param name="iType"></param>
        /// <returns></returns>
        public static string HttpSendMessge(ref string strURL, ref string strParams, int iTimeOut, int iEncodeType,int iType)
        {
            try
            {
                switch (iType)
                {
                    case 1:
                        return HttpPostSendMessge(ref strURL, ref strParams, iTimeOut, iEncodeType);
                    case 2:
                        return HttpGetSendMessge(ref strURL, ref strParams, iTimeOut, iEncodeType);
                    case 3:
                        return "暂时不支持SOAP方式";
                    default:
                        return "暂时不支持SOAP方式";
                }
            }
            catch (System.Exception e)
            {
                return "PostError[-1111]," + e.Message;//表示发送信息时POST异常
            }
        }

        private static string HttpPostSendMessge(ref string strURL, ref string strParams, int iTimeOut, int iEncodeType)
        {
            HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(strURL);
            request.Method = "POST";
            SetWebRequest(request, iTimeOut);            
            byte[] data = EncodeParams(ref strParams);
            WriteRequestData(request, data);
            return PreProcessRetString(request.GetResponse(), iEncodeType);
        }


        private static string HttpGetSendMessge(ref string strURL, ref string strParams, int iTimeOut, int iEncodeType)
        {            
            HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(strURL + strParams);
            request.Method = "GET";
            SetWebRequest(request, iTimeOut);
            return PreProcessRetString(request.GetResponse(),iEncodeType);
        }

//         public static string KeepOnline(int iTimeOut, int iEncodeType)
//         {
//             objRequest = (HttpWebRequest)HttpWebRequest.Create(SmsSetIni.KeepOnlineUrl);
//             objRequest.Method = "GET";
//             SetWebRequest(objRequest, iTimeOut);
//             return PreProcessRetString(objRequest.GetResponse(), iEncodeType);
//         }
        /// <summary>
        /// 初步处理返回值,去掉Webservice的XML标签
        /// </summary>
        /// <param name="strRet"></param>
        /// <returns></returns>
        public static string PreProcessRetString(WebResponse webRet,int iType)
        {
            StreamReader sr = new StreamReader(webRet.GetResponseStream(), iType == 0?Encoding.UTF8:Encoding.GetEncoding("gb2312"));
            string retStr = sr.ReadToEnd();
            sr.Close();
            //处理返回值，去掉XML标签
            Regex regex = new Regex(@"<[^>]*>");
            if (regex.IsMatch(retStr))
            {
                retStr = regex.Replace(retStr, "");
            }
            retStr = retStr.Replace("\r\n", "");
            return retStr;
        }

        public static void SetWebRequest(HttpWebRequest request, int iTimeOut)
        {
            request.ContentType = "application/x-www-form-urlencoded";
            request.Credentials = CredentialCache.DefaultCredentials;
            request.Timeout = iTimeOut;
        }
        private static void WriteRequestData(HttpWebRequest request, byte[] data)
        {
            request.ContentLength = data.Length;
            Stream writer = request.GetRequestStream();
            writer.Write(data, 0, data.Length);
            writer.Close();
        }
        private static byte[] EncodeParams(ref string strParams)
        {
            return Encoding.UTF8.GetBytes(strParams);
        }
    }
}
