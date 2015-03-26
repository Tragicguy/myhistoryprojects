using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
namespace UDiskDemo
{
    class CUDiskClass
    {
        [DllImport("UDiskDll.dll")]//
        private static extern bool GetDiskSerial(string sDisk, ref byte bSerial);
        [DllImport("UDiskDll.dll")]//
        private static extern bool GetDisks(ref byte sDisk);

        public static bool GetDiskSerials(string sDisk,ref string strSerial)
        {
            byte[] szRet = new byte[50];
            bool bRet = GetDiskSerial(sDisk, ref szRet[0]);
            if (bRet)
            {
               strSerial = System.Text.Encoding.GetEncoding("gb2312").GetString(szRet).ToString();
               int iPos = strSerial.IndexOf('\0');
               if (iPos >= 0)
               {
                   strSerial = strSerial.Substring(0, iPos);
               }
            }
            return bRet;
        }

        public static string GetDisks()
        {
            byte[] szRet = new byte[50];
            bool bRet = GetDisks(ref szRet[0]);
            if (bRet)
            {
                string strRet = System.Text.Encoding.GetEncoding("gb2312").GetString(szRet).ToString();
                int iPos = strRet.IndexOf('\0');
                if (iPos >= 0)
                {
                    strRet = strRet.Substring(0, iPos);
                }
                return strRet;
            }
            return "";
        }
    }
}
