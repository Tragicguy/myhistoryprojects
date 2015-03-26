using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using Newtonsoft.Json;
using System.Collections;
namespace SmsServer.BLL
{
    public class JSonClass
    {
        public static string DataTableToJsonExEx(DataTable dt)
        {
            StringBuilder jsonBuilder = new StringBuilder();
            jsonBuilder.Append("[");
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                if (i != 0) { jsonBuilder.Append(","); }
                jsonBuilder.Append("{");
                for (int j = 0; j < dt.Columns.Count; j++)
                {
                    jsonBuilder.Append("\"");
                    jsonBuilder.Append(dt.Columns[j].ColumnName);
                    Type dType = dt.Columns[j].GetType();
                    if (typeof(string) == dType)
                    {
                        jsonBuilder.Append("\":\"");
                        jsonBuilder.Append(dt.Rows[i][j].ToString().Replace("\"", ""));
                        jsonBuilder.Append("\",");
                    }
                    else
                    {
                        jsonBuilder.Append("\":");
                        jsonBuilder.Append(dt.Rows[i][j].ToString());
                        jsonBuilder.Append(",");
                    }
                    
                }
                jsonBuilder.Remove(jsonBuilder.Length - 1, 1);
                jsonBuilder.Append("}");
            }
            jsonBuilder.Append("]");
            return jsonBuilder.ToString();
        }
        
        public static string DataTableToJson(DataTable dt,Hashtable ht)
        {
            StringBuilder jsonBuilder = new StringBuilder();
            jsonBuilder.Append("[");
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                if (i != 0) { jsonBuilder.Append(","); }
                jsonBuilder.Append("{");
                for (int j = 0; j < dt.Columns.Count; j++)
                {
                    jsonBuilder.Append("\"");
                    jsonBuilder.Append(dt.Columns[j].ColumnName);
                    Type dType = dt.Columns[j].GetType();
                    if (!ht.Contains(j+1))         //不是整数,加上双引号
                    {
                        jsonBuilder.Append("\":\"");
                        jsonBuilder.Append(dt.Rows[i][j].ToString().Replace("\"", ""));
                        jsonBuilder.Append("\",");
                    }
                    else
                    {
                        jsonBuilder.Append("\":");
                        jsonBuilder.Append(dt.Rows[i][j].ToString());
                        jsonBuilder.Append(",");
                    }

                }
                jsonBuilder.Remove(jsonBuilder.Length - 1, 1);
                jsonBuilder.Append("}");
            }
            jsonBuilder.Append("]");
            return jsonBuilder.ToString();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="dt"></param>
        /// <returns></returns>
        public static string DataTableToJsonEx(DataTable dt,int iTotalCount)
        {
            int iCount = dt.Rows.Count;
            string strHead = "{totalCount:" + iTotalCount.ToString() + ",success:true,data:";
            StringBuilder jsonBuilder = new StringBuilder();
            jsonBuilder.Append(strHead);
            jsonBuilder.Append(DataTableToJson(dt));
            jsonBuilder.Append("}");
            return jsonBuilder.ToString();
        }
        /// <summary>
        /// DT转换为JSON
        /// </summary>
        /// <param name="dt"></param>
        /// <returns></returns>
        public static string DataTableToJson(DataTable dt)
        {
            return JavaScriptConvert.SerializeObject(dt,new DataTableConverter());
        }
        /// <summary>
        /// DS转换为JSON
        /// </summary>
        /// <param name="ds"></param>
        /// <returns></returns>
        public static string DataSetToJson(DataSet ds)
        {
            return JavaScriptConvert.SerializeObject(ds, new DataSetConverter());
        }
    }
    /// <summary>
    /// DataTable Json转换适配类
    /// </summary>
    public class DataTableConverter : JsonConverter
    {
        public override void WriteJson(JsonWriter writer, object value)
        {
            DataTable dt = (DataTable)value;
            writer.WriteStartArray();
            foreach (DataRow dr in dt.Rows)
            {
                writer.WriteStartObject();
                foreach (DataColumn dc in dt.Columns)
                {
                    writer.WritePropertyName(dc.ColumnName);
                    writer.WriteValue(dr[dc].ToString());
                }
                writer.WriteEndObject();
            }
            writer.WriteEndArray();
        }

        public override bool CanConvert(Type objectType)
        {
            return typeof(DataTable).IsAssignableFrom(objectType);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="writer"></param>
        /// <param name="value"></param>
    }
    /// <summary>
    /// DataSet Json转换适配类
    /// </summary>
    public class DataSetConverter : JsonConverter
    {
        public override bool CanConvert(Type objectType)
        {
            return typeof(DataSet).IsAssignableFrom(objectType);
        }

        public override void WriteJson(JsonWriter writer, object value)
        {
            DataSet ds = (DataSet)value;
            writer.WriteStartObject();
            foreach (DataTable dt in ds.Tables)
            {
                writer.WritePropertyName(dt.TableName);
                writer.WriteStartArray();
                foreach (DataRow dr in dt.Rows)
                {
                    writer.WriteStartObject();
                    foreach (DataColumn dc in dt.Columns)
                    {
                        writer.WritePropertyName(dc.ColumnName);
                        writer.WriteValue(dr[dc].ToString());
                    }
                    writer.WriteEndObject();
                }
                writer.WriteEndArray();
            }
            writer.WriteEndObject();
        }
    }
    
}
