using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Text;
namespace SmsServer.Web
{
    public class TestClass
    {
        private StringBuilder strMobileList = new StringBuilder();
        public StringBuilder MobileList
        {
            set { strMobileList = value; }
            get { return strMobileList; }
        }
    }
    
}
