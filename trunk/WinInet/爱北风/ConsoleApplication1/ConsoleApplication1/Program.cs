using System;
using System.Collections.Generic;
using System.Text;
using ClassLibrary;
using System.Threading;
namespace ConsoleApplication1
{
    class TestClass
    {
        string strTest;
    }
    class Program
    {
        public static Thread[] triggerThread = new Thread[20];
        public static TestClass m_objTest;
        public static Int32 m_iRegNum = 1000;
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                m_iRegNum = 1000;
            }
            else
            {
                m_iRegNum = Int32.Parse(args[0]);
            }
            m_objTest = new TestClass();
            for (int i = 0; i < 20; i++)
            {
                ThreadStart myThreadStart = new ThreadStart(FreshProc);
                triggerThread[i] = new Thread(myThreadStart);
                triggerThread[i].Start();
                Thread.Sleep(30);
            }            
        }


        public static void FreshProc()
        {
            //注册成功
            int i = 0;
            for (; ; )
            {
                i++;
                lock (m_objTest)
                {
                    m_iRegNum--;
                    if (m_iRegNum < 1)
                    {
                        return;
                    }
                }
                ClassHttpPost webPost = new ClassHttpPost();
                string strUser = "akdj" + new Random().Next(5000).ToString() + i.ToString();
                string strData = string.Format("username={0}&email={0}1@{0}.com&password=jjkad123&confirm_password=jjkad123&other%5Bmsn%5D=&other%5Bqq%5D=&other%5Boffice_phone%5D=&other%5Bhome_phone%5D=&other%5Bmobile_phone%5D=&way=3&othertxt=&agreement=1&act=act_register&Submit=",
                    strUser
                    );
                string strRet = webPost.GetHttpPage("http://www.ibeifeng.com/user.php",
                        "gb2312", "POST", "", strData,
                        "http://www.ibeifeng.com/user.php?act=register",
                        "*/*", "");
                if (strRet.IndexOf("注册成功") > 0)
                {
                    Console.WriteLine(string.Format("{0}: {1}", m_iRegNum, strUser));
                }
            }
        }
    }
}
