using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using NBearLite;//请先添加引用

namespace SmsServer.DAL
{
    public class TreeNode
    {
        public DataSet GetTreeNode(string strUserID,int iParentID)
        {
            string strPowers = MyDataBase.db.Select(SmsPlatForm.SmsManager, SmsPlatForm.SmsManager.Power).Where(SmsPlatForm.SmsManager.ManagerID == strUserID).ToScalar().ToString();
            string[] strArray = strPowers.Split(',');
            return MyDataBase.db.Select(SmsPlatForm.TreeNode).Where(SmsPlatForm.TreeNode.ParentID == iParentID && SmsPlatForm.TreeNode.id.In(strArray)).OrderBy(SmsPlatForm.TreeNode.Number.Asc).ToDataSet();
        }
        /// <summary>
        /// 返回账号类型为指定类型的所有可能的权限,其中2为两者共有的权限
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="iParentID"></param>
        /// <returns></returns>
        public DataSet GetTreeNodeEx(string strUserID, int iParentID, out string strPower)
        {
            DataSet ds = MyDataBase.db.Select(SmsPlatForm.SmsManager, SmsPlatForm.SmsManager.ManagerType, SmsPlatForm.SmsManager.Power).Where(SmsPlatForm.SmsManager.ManagerID == strUserID).ToDataSet();
            int iType = Int32.Parse(ds.Tables[0].Rows[0]["ManagerType"].ToString());
            iType = ( iType + 1) % 2;
            strPower = ds.Tables[0].Rows[0]["Power"].ToString();
            return MyDataBase.db.Select(SmsPlatForm.TreeNode).Where(SmsPlatForm.TreeNode.ParentID == iParentID && SmsPlatForm.TreeNode.belongto != iType).ToDataSet();
        }
    }
}