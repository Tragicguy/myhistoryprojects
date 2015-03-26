using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Collections.Generic;
using SmsServer.DataAdapter;
using Microsoft.JScript;
using SmsServer.Model;
using SmsServer.BLL;
namespace SmsServer.Web
{
    public partial class WebForm1 : System.Web.UI.Page
    {
        public string ProductCode;
        public static DataAdapterClass testObj = new DataAdapterClass();
        protected void Page_Load(object sender, EventArgs e)
        {
            ProductCode = "aaaa";
//             SmsServer.Model.SmsUser objUser = new SmsServer.BLL.SmsUser().GetSelectSmsUser("wawa", "wawa");
//             if (objUser == null)
//             {
//                 Response.Write("0");
//             }
//             else{
//                 Response.Write(objUser.UserID);
//             }
//             Dictionary<string, int> pIllegalList = new Dictionary<string, int>();
//             pIllegalList["aa"] = 1;
//             pIllegalList["bb"] = 2;
//             Dictionary<string, int> pIllegalList2 = pIllegalList;
//             Response.Write(pIllegalList2["aa"].ToString()); 
//             Response.Write(pIllegalList2["bb"].ToString());
//             if (pIllegalList2.ContainsKey("cc"))
//             {
//                 Response.Write(pIllegalList2["cc"].ToString());
//             }
            if (!IsPostBack)
            {
                TextBox3.Text = @"var GetRetValue = function(varSrcInfo){
                                    varSrcInfo = unescape(varSrcInfo);
                                    var retValue = 0;
                                    var needResend = 22;
                                    return {RetValue:retValue,NeedResend:needResend ,RetInfo:varSrcInfo}
                                }";
                TextBox1.Text = "你好吗？";
                if (SendThreadMgr.m_bExit)
                {
                    Button7.Enabled = true;
                    Button8.Enabled = false;
                    Response.Write("发送线程未启动");
                }
                else
                {
                    Button7.Enabled = false;
                    Button8.Enabled = true;
                    Response.Write("发送线程启动于：" + SendThreadMgr.StartDate.ToString());
                }
            }
            
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            string strSrc = TextBox1.Text;

//              Dictionary<int , string> pUserList = new Dictionary<int, string>();
//              lock (pUserList)
//              {
//                  pUserList[1] = "aaa";
//                  Response.Write(pUserList[1]);
//                  pUserList[2] = "bbb";
//                  Response.Write(pUserList[2]);
//                  pUserList[1] = "ccc";
//                  Response.Write(pUserList[1]);
//                  pUserList.Remove(8);
//              }
           
            string[][] strMessageArray = new string[4][];
            strMessageArray[0] = strSrc.Split(',');
//             if(SmsServer.BLL.DataVerify.IsIllegalUser(strSrc))
//             {
//                 Response.Write("1");
//             }
//             else{Response.Write("0");}
        }

        protected void Button2_Click(object sender, EventArgs e)
        {
            SmsServer.Model.SmsSpgate model = new SmsServer.Model.SmsSpgate();
            model.GateNumber = "old text ";
            //TestTest(ref model);
            TestTestEx(model);
            Response.Write(model.GateNumber);
            
        }
        private void TestTest(ref SmsServer.Model.SmsSpgate model)
        {
           //model = new SmsServer.BLL.SmsSpgate().GetModelByCache("106570003");
        }
        private void TestTestEx(SmsServer.Model.SmsSpgate model)
        {
            model.GateNumber = "AAAAAAAAAA";
        }
        private bool Testaa()
        {
            return true;
        }
        protected void Button3_Click(object sender, EventArgs e)
        {
//             int iType = 0;
//             if (Testaa() && iType == 1)
//             {
//                 Response.Write("aa");
//             }
            string[,] strMessageArray = new string[20,20];
            int iRowLen = strMessageArray.GetLength(0);
            int iColumnLen = strMessageArray.GetLength(1);
            for (int i = 0; i < iRowLen; i++)
            {
                for (int j = 0; j < iColumnLen; j++)
                {
                    strMessageArray[i, j] = i.ToString().PadLeft(2, '0') + "-" + j.ToString().PadLeft(2, '0');
                }
            }
            /*foreach (string strA in (string[])strMessageArray.GetValue(0))
            {
                foreach (string strB in (string[])strMessageArray.GetValue(1))
                {
                    Response.Write(strA[0]);
                }
            }     */
            for (int i = 0; i < iRowLen;i++ )
            {
                for (int j = 0; j < iColumnLen;j++ )
                {
                    Response.Write(strMessageArray[i,j] + " ");
                }
                Response.Write("\r\n<br/>");
            }
          
            //Response.Write(((string[])strMessageArray.GetValue(0))[2]);
        //strMessageArray[0][0] = "111";
        //Response.Write(strMessageArray.Length.ToString() + strMessageArray[0][0]);
        }

        protected void Button4_Click(object sender, EventArgs e)
        {
            string strScript = TextBox3.Text;
            string strSrc = TextBox1.Text;

            SendRetValue retObj = new SendRetValue();
            testObj.AdapterSendResult(strScript, strSrc, ref retObj);
            Response.Write(retObj.OldRetValue +"<br/>"
                +retObj.RetValue.ToString() +"<br/>"
                + retObj.IsNeedResend.ToString() + "<br/>"
                +retObj.RetInfo +"<br/>");
//             Microsoft.JScript.ArrayObject oo = (Microsoft.JScript.ArrayObject)testObj.RunJscriptCode(strScript);
//             for (int i = 0; i < (int)oo.length;i++ )
//             {
//                 if (i==0)
//                 {
//                     Response.Write(((Microsoft.JScript.JSObject)oo[i])["aaaa"].ToString() + "\r\n<br/>");  
//                 }
//                 else
//                 Response.Write(oo[i].ToString()+"\r\n<br/>");                
//             }
            //oo.
            //Response.Write(oo["aaa"].ToString());
        }

        protected void Button6_Click(object sender, EventArgs e)
        {
            string strAA = (sender as Button).CommandArgument;
        }

        protected void Button8_Click(object sender, EventArgs e)
        {
            SendThreadMgr.StopSendThreads();
            new SmsServer.BLL.SmsLog().WriteDataBaseLog(2, "手动停止发送线程", "", "");
            Button7.Enabled = true;
            Button8.Enabled = false;
            Response.Write("发送线程未启动");            
        }

        protected void Button7_Click(object sender, EventArgs e)
        {
            SendThreadMgr.StartSendThreads();
            new SmsServer.BLL.SmsLog().WriteDataBaseLog(2, "手动启动发送线程", "", "");
            Button7.Enabled = false;
            Button8.Enabled = true;
            Response.Write("发送线程启动于：" + SendThreadMgr.StartDate.ToString());
        }
    }
}
