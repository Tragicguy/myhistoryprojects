using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
namespace SmsServer.BLL
{
    public class DataVerify
    {
        /// <summary>
        /// 英文加数字
        /// </summary>
        /// <param name="strIn"></param>
        /// <returns></returns>
        public static bool IsIllegalUser(string strIn)
        {
            if (strIn.Length > 12 || strIn.Length < 5)
            {
                return false;
            }
            return Regex.IsMatch(strIn, @"^[a-zA-Z0-9]+$");
        }

        public static bool IsDecimal(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return false;
            }
            return Regex.IsMatch(input, "^[0-9]+[.]?[0-9]+$");
        }

        /// <summary>
        /// 带符号的实数验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsDecimalSign(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return false;
            }
            return Regex.IsMatch(input, "^[+-]?[0-9]+[.]?[0-9]+$");
        }

        /// <summary>
        /// 正整数验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsNumber(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return false;
            }
            return Regex.IsMatch(input, "^[0-9]+$");
        }

        /// <summary>
        /// 整数验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsNumberSign(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return false;
            }
            return Regex.IsMatch(input, "^[+-]?[0-9]+$");
        }

        /// <summary>
        /// 邮编验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsPostCode(string input)
        {
            return (IsNumber(input) && (input.Length == 6));
        }

        /// <summary>
        /// 邮箱地址格式验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsEmail(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return false;
            }
            return Regex.IsMatch(input, @"^\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$");
        }

        /// <summary>
        /// IP地址验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsIP(string input)
        {
            return (!string.IsNullOrEmpty(input) && Regex.IsMatch(input.Trim(), @"^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$"));
        }

        /// <summary>
        /// URL验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsUrl(string input)
        {
            if (string.IsNullOrEmpty(input))
            {
                return false;
            }
            return Regex.IsMatch(input, @"^http(s)?://([\w-]+\.)+[\w-]+(/[\w- ./?%&=]*)?$");
        }

        /// <summary>
        /// 区号验证
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsAreaCode(string input)
        {
            return ((IsNumber(input) && (input.Length >= 3)) && (input.Length <= 5));
        }

        /// <summary>
        /// 用户名格式验证,长度[0,20],不能含有\\/\"[]:|<>+=;,?*@
        /// </summary>
        /// <param name="userName"></param>
        /// <returns></returns>
        public static bool IsValidUserName(string userName)
        {
            if (string.IsNullOrEmpty(userName))
            {
                return false;
            }
            if (userName.Length > 20)
            {
                return false;
            }
            if (userName.Trim().Length == 0)
            {
                return false;
            }
            if (userName.Trim(new char[] { '.' }).Length == 0)
            {
                return false;
            }
            string str = "\\/\"[]:|<>+=;,?*@";
            for (int i = 0; i < userName.Length; i++)
            {
                if (str.IndexOf(userName[i]) >= 0)
                {
                    return false;
                }
            }
            return true;
        }
        /// <summary>
        /// 特殊字符过滤
        /// </summary>
        /// <param name="strchar"></param>
        /// <returns></returns>
        public static string FilterBadChar(string strchar)
        {
            string input = "";
            if (string.IsNullOrEmpty(strchar))
            {
                return "";
            }
            string str = strchar;
            string[] strArray = new string[] { 
                "+", "'", "%", "^", "&", "?", "(", ")", "<", ">", "[", "]", "{", "}", "/", "\"", 
                ";", ":", "Chr(34)", "Chr(0)", "--"
             };
            StringBuilder builder = new StringBuilder(str);
            for (int i = 0; i < strArray.Length; i++)
            {
                input = builder.Replace(strArray[i], "").ToString();
            }
            return Regex.Replace(input, "@+", "@");
        }

        /// <summary>
        /// SQL注入过滤
        /// </summary>
        /// <param name="strchar"></param>
        /// <returns></returns>
        public static string FilterSqlKeyword(string strchar)
        {
            bool flag = false;
            if (string.IsNullOrEmpty(strchar))
            {
                return string.Empty;
            }
            strchar = strchar.ToLower();
            string[] strArray = new string[] { 
                "select", "update", "insert", "delete", "declare", "@", "exec", "dbcc", "alter", "drop", "create", "backup", "if", "else", "end", "and", 
                "or", "add", "set", "open", "close", "use", "begin", "retun", "as", "go", "exists", "kill"
             };
            for (int i = 0; i < strArray.Length; i++)
            {
                if (strchar.Contains(strArray[i]))
                {
                    strchar = strchar.Replace(strArray[i], "");
                    flag = true;
                }
            }
            if (flag)
            {
                return FilterSqlKeyword(strchar);
            }
            return strchar;
        }

        /// <summary>
        /// HTML编码
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string HtmlDecode(object value)
        {
            if (value == null)
            {
                return null;
            }
            return HtmlDecode(value.ToString());
        }

        /// <summary>
        /// HTML编码
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string HtmlDecode(string value)
        {
            if (!string.IsNullOrEmpty(value))
            {
                value = value.Replace("<br>", "\n");
                value = value.Replace("& gt;", ">");
                value = value.Replace("& lt;", "<");
                value = value.Replace("& nbsp;", " ");
                value = value.Replace("& #39;", "'");
                value = value.Replace("& quot;", "\"");
            }
            return value;
        }

        /// <summary>
        /// HTML解码
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string HtmlEncode(object value)
        {
            if (value == null)
            {
                return null;
            }
            return HtmlEncode(value.ToString());
        }

        /// <summary>
        /// HTML解码
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static string HtmlEncode(string str)
        {
            if (!string.IsNullOrEmpty(str))
            {
                str = str.Replace("<", "& lt;");
                str = str.Replace(">", "& gt;");
                str = str.Replace(" ", "& nbsp;");
                str = str.Replace("'", "& #39;");
                str = str.Replace("\"", "& quot;");
                str = str.Replace("\r\n", "<br>");
                str = str.Replace("\n", "<br>");
            }
            return str;
        }
    }
}