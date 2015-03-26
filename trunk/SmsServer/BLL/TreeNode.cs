using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
using System.Collections;
namespace SmsServer.BLL
{
	/// <summary>
	/// 业务逻辑类TreeNode 的摘要说明。
	/// </summary>
	public class TreeNode
	{
        private readonly SmsServer.DAL.TreeNode dal = new SmsServer.DAL.TreeNode();
		public TreeNode()
		{}
		#region  成员方法

        public string GetTreeNode(string strUserID, int iParentID)
        {
            DataSet ds = dal.GetTreeNode(strUserID, iParentID);
            Hashtable hashDt = new Hashtable();
            hashDt.Add(1, 1);
            hashDt.Add(2, 1);
            hashDt.Add(4, 1);
            hashDt.Add(6, 1);
            return JSonClass.DataTableToJson(ds.Tables[0],hashDt);
        }
        /// <summary>
        /// 获取设置权限处的值
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="iParentID"></param>
        /// <returns></returns>
        public string GetTreeNodeEx(string strUserID, int iParentID)
        {
            string strPower = "";
            DataSet ds = dal.GetTreeNodeEx(strUserID, iParentID, out strPower);
            string[] strArray = strPower.Split(',');
            Hashtable powerDt = new Hashtable();
            for (int i = 0; i < strArray.Length;i++ )
            {
                powerDt.Add(strArray[i], 1);
            }
            int iCount = ds.Tables[0].Rows.Count;
            foreach (DataRow row in ds.Tables[0].Rows)
            {

                if (powerDt.Contains(row["id"].ToString()))
                {
                    row["checked"] = 1;
                }
                else
                {
                    row["checked"] = 0;
                }
            }
            Hashtable hashDt = new Hashtable();
            hashDt.Add(1, 1);
            hashDt.Add(2, 1);
            hashDt.Add(4, 1);
            hashDt.Add(6, 1);
            hashDt.Add(7, 1);
            return JSonClass.DataTableToJson(ds.Tables[0], hashDt);
        }
        
		#endregion  成员方法
	}
}

