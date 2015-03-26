using System;
using System.Collections.Generic;
using System.Text;
using NBearLite;//请先添加引用
namespace SmsServer.DAL
{
    class MyDataBase
    {
        public static Database db = new Database("ConnString");
    }
}
