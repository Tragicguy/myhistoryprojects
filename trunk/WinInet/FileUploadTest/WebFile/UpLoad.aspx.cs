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
using System.IO;
namespace WebFile
{
    public partial class UpLoad : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            try
            {
                Stream recvStream = Request.InputStream;
                int iLength = (int)recvStream.Length;
                Byte[] bData = new Byte[iLength];
                recvStream.Read(bData, 0, iLength);
                Response.Write(CaculateFile(bData));
            }
            catch
            {
                Response.Write("0"); //表示失败
            }
        }
        private string CaculateFile(byte[] bData)
        {
            try
            {
                string strFlag = System.Text.Encoding.UTF8.GetString(bData, 0, 1);
                if (strFlag == "1")
                {
                    string strDirect = Server.MapPath("Files");
                    if (!Directory.Exists(strDirect))  //如果目录不存在
                    {
                        Directory.CreateDirectory(strDirect);
                    }
                    string strName = System.Text.Encoding.UTF8.GetString(bData, 1, bData.Length - 1);
                    string filePath = string.Format("{0}\\{1}", strDirect, strName);
                    int iLoop = 1;
                    string strNameEx = strName;
                    while (File.Exists(filePath))
                    {
                        strNameEx = string.Format("{0}_{1}", iLoop, strName);
                        filePath = string.Format("{0}\\{1}", strDirect, strNameEx);
                        iLoop++;
                    }
                    Session["FilePath"] = filePath;
                    string strUrl = Request.Url.ToString();
                    strUrl = strUrl.Substring(0,strUrl.LastIndexOf('/'));
                    strUrl += "/Files/" + strNameEx;
                    return "ok," + strUrl;
                }
                else
                {
                    string strPath = "";
                    if (Session["FilePath"] == null)
                    {
                        return "-1";
                    }
                    strPath = Session["FilePath"].ToString();
                    FileStream streamFile = new FileStream(strPath, FileMode.OpenOrCreate);
                    streamFile.Seek(0, SeekOrigin.End);
                    streamFile.Write(bData, 1, bData.Length - 1);
                    streamFile.Dispose();
                }
                return "1";
            }
            catch (System.Exception ex)
            {
                return ex.Message;
            }
            
        }

    }
}
