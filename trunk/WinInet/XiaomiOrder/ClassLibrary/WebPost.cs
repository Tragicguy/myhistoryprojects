using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using System.Xml.Serialization;
using System.Text.RegularExpressions;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;
namespace ClassLibrary
{
    public class ClassHttpPost
    {

        public bool CheckValidationResult(object sender, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors)
        {   // 总是接受    
            return true;
        }
        public bool m_bRedirect = false;
        public string m_strRedirectUrl = "";
        public string m_cookiestr = "";
        public CookieContainer m_Cookie;
        public CookieCollection m_CookieCollecgion;

        public string GetHttpPage(string strUrl, string strEncode, string strType,
            string strCookie, string strData, string strReferer, string strAcceptType,
            string strExtendHead)
        {
            return "";
        }

        public string GetHttpPage(string strUrl, string strEncode, string strType,
            string strCookie, string strData, string strReferer, string strAcceptType,
            string strExtendHead, CookieCollection cookies)
        {
            try
            {
                Stream stream = GetHttpPageSteam(strUrl, strEncode, strType, strCookie, strData, strReferer, strAcceptType, strExtendHead, cookies);
                while (m_bRedirect)
                {
                    stream = GetHttpPageSteam(m_strRedirectUrl, strEncode, "get", strCookie, "", "", strAcceptType, strExtendHead, null);
                }
                if (stream == null)
                {
                    return "";
                }
               
                StreamReader sr = new StreamReader(stream, strEncode == "gb2312" ? Encoding.GetEncoding("gb2312") : Encoding.UTF8);
                string retStr = sr.ReadToEnd();
                sr.Close();
                return retStr; // UnZip(retStr);
            }
            catch (System.Exception e)
            {
                return "";
            }
            
        }

        public Stream GetHttpPageSteam(string strUrl, string strEncode, string strGetType,
            string strCookie, string strData, string strReferer, string strAcceptType,
            string strExtendHead, CookieCollection cookies)
        {
            try
            {
                //  ServicePointManager.ServerCertificateValidationCallback = new System.Net.Security.RemoteCertificateValidationCallback(CheckValidationResult);

                m_bRedirect = false;
                HttpWebRequest m_Request = (HttpWebRequest)HttpWebRequest.Create(strUrl);
                strGetType = strGetType.ToUpper();
                m_Request.Method = strGetType;
                m_Request.Timeout = 65000;
                if (strExtendHead == "")
                {
                    strExtendHead = "application/x-www-form-urlencoded";
                }
                m_Request.ContentType = strExtendHead;
                m_Request.UserAgent = "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; EmbeddedWB 14.52 from: http://www.bsalsa.com/ EmbeddedWB 14.52; Win64; x64; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)";
                m_Request.Accept = strAcceptType;// "*/*";
                m_Request.Referer = strReferer;
                m_Request.KeepAlive = true;
                m_Request.ServicePoint.Expect100Continue = false;
                m_Request.Headers.Add("Accept-Language", "zh-cn");
                m_Request.Headers.Add("Cache-Control", "no-cache");
                m_Request.Headers.Add("UA-CPU", "x86");
                m_Request.AllowAutoRedirect = false;

                
                //添加到请求里
                // m_Request.ClientCertificates.Add(objx509);

                if (m_Cookie == null)
                {
                    m_Request.CookieContainer = new CookieContainer();
                    m_Cookie = m_Request.CookieContainer;
                }
                else
                {
                    m_Request.CookieContainer = m_Cookie;
                }
                if (cookies != null)
                {
                    m_Request.CookieContainer.Add(cookies);
                }
                

                if (strGetType != "GET")
                {
                    WriteRequestData(m_Request, EncodeParams(ref strData));
                }

                HttpWebResponse response = m_Request.GetResponse() as HttpWebResponse;
                if (response.StatusCode == HttpStatusCode.Found)
                {
                    m_bRedirect = true;
                    m_strRedirectUrl = response.Headers[HttpResponseHeader.Location];
                }
                
                Stream strem = response.GetResponseStream();

                return strem;
            }
            catch (System.Exception ex)
            {
            	
            }
            return null;            
        }
       
        private void WriteRequestData(HttpWebRequest request, byte[] data)
        {
            request.ContentLength = data.Length;
            Stream writer = request.GetRequestStream();
            writer.Write(data, 0, data.Length);
            writer.Close();
        }

        private byte[] EncodeParams(ref string strParams)
        {
            return Encoding.UTF8.GetBytes(strParams);
        }

        public static string UnZip(string value)
        {
            //Transform string into byte[]  
            byte[] byteArray = new byte[value.Length];
            int indexBA = 0;
            foreach (char item in value.ToCharArray())
            {
                byteArray[indexBA++] = (byte)item;
            }


            //Prepare for decompress  
            System.IO.MemoryStream ms = new System.IO.MemoryStream(byteArray);
            System.IO.Compression.GZipStream sr = new System.IO.Compression.GZipStream(ms,
                System.IO.Compression.CompressionMode.Decompress);


            //Reset variable to collect uncompressed result  
            byteArray = new byte[byteArray.Length];


            //Decompress  
            int rByte = sr.Read(byteArray, 0, byteArray.Length);


            //Transform byte[] unzip data to string  
            System.Text.StringBuilder sB = new System.Text.StringBuilder(rByte);
            //Read the number of bytes GZipStream red and do not a for each bytes in  
            //resultByteArray;  
            for (int i = 0; i < rByte; i++)
            {
                sB.Append((char)byteArray[i]);
            }
            sr.Close();
            ms.Close();
            sr.Dispose();
            ms.Dispose();
            return sB.ToString();
        }
    }
}
