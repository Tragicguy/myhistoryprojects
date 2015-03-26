using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.JScript;
using SmsServer.Model;
namespace SmsServer.DataAdapter
{
    public class DataAdapterClass : JsEvalClass
    {
        //匹配发送返回值
        //匹配上行信息
        //匹配状态报告

        /// <summary>
        /// 匹配发送返回值
        /// </summary>
        /// <param name="strFuncName"></param>
        /// <param name="strFunProc"></param>
        /// <param name="strRet"></param>
        public bool AdapterSendResult(string strFunProc,string strRet, ref SendRetValue objRetValue)
        {
            try
            {
                objRetValue.OldRetValue = strRet;
                strRet = GlobalObject.escape(strRet);
                //strRet = strRet.Replace("\\", "\\\\");
                //strRet = strRet.Replace("\"", "\\\"");
                string strJsCode = string.Format("{0} \r\nGetRetValue(\"{1}\");", strFunProc, strRet);
                JSObject retJsObj = (JSObject)RunJscriptCode(strJsCode);
                objRetValue.RetValue = Int32.Parse(retJsObj["RetValue"].ToString());
                objRetValue.IsNeedResend = Int32.Parse(retJsObj["NeedResend"].ToString());
                objRetValue.RetInfo = retJsObj["RetInfo"].ToString();
                return true;
            }
            catch (System.Exception e)
            {
                objRetValue.RetInfo = e.Message;
                objRetValue.RetValue = -1;
                objRetValue.IsNeedResend = 0;
                return false;
            }            
        }

        //public 
        //全部返回JS对象，或者JS对象数组
    }
}
