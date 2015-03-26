using System;
using System.Collections.Generic;
using System.Text;

namespace FunctionCode
{
    public class Functions
    {
        public static string GetKeyString(string strSrc, string strBefore, string strAfter)
        {
            int iPosBefore = 0;
            int iPosAfter = 0;
            int iPos = strSrc.IndexOf(strBefore);
            if (iPos < 0)
            {
                return "";
            }
            iPosBefore = iPos + strBefore.Length;
            iPosAfter = strSrc.IndexOf(strAfter, iPosBefore);
            if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0)
            {
                return "";
            }
            else
            {
                return strSrc.Substring(iPosBefore, iPosAfter - iPosBefore);
            }
        }
    }
}
