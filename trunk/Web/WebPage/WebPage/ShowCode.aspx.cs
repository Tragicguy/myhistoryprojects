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
using System.Xml.Serialization;
using ClassLibrary;
using System.Collections.Generic;
namespace WebPage
{
    public partial class ShowCode : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            try
            {
                Stream stream = ClassHttpPost.GetHttpPageSteam("http://www.ofcard.com/image.jsp?", "gb2312", "get", "", "", "", "*/*", "");
                if (stream == null)
                {
                    return;
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
                Response.Clear();
                Response.BinaryWrite(bytes);
            }
            catch (System.Exception ex)
            {

            } 
        }
    }
}
