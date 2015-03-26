using System;
using System.Data;
using System.Text;
using NBearLite;//请先添加引用

using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// 数据访问类SmsTaskA。
	/// </summary>
	public class SmsTaskA
	{
		public SmsTaskA()
		{}
		#region  成员方法

		/// <summary>
		/// 增加一条数据
		/// </summary>
		public bool Add(SmsServer.Model.SmsTaskA model)
		{
            //return 0;
            try
            {
                MyDataBase.db.Insert(SmsPlatForm.SmsTaskA)
                .AddColumn(SmsPlatForm.SmsTaskA.PhoneList, model.PhoneList)
                .AddColumn(SmsPlatForm.SmsTaskA.MsgContent, model.MsgContent)
                .AddColumn(SmsPlatForm.SmsTaskA.SubTime, model.SubTime)
                .AddColumn(SmsPlatForm.SmsTaskA.SendTime, model.SendTime)
                .AddColumn(SmsPlatForm.SmsTaskA.UserID, model.UserID)
                .AddColumn(SmsPlatForm.SmsTaskA.ParentID, model.ParentID)
                .AddColumn(SmsPlatForm.SmsTaskA.SpgateNo, model.SpgateNo)
                .AddColumn(SmsPlatForm.SmsTaskA.SubPort, model.SubPort)
                .AddColumn(SmsPlatForm.SmsTaskA.SubPortEx, model.SubPortEx)
                .AddColumn(SmsPlatForm.SmsTaskA.FeeNum, model.FeeNum)
                .AddColumn(SmsPlatForm.SmsTaskA.PhoneNum, model.PhoneNum)
                .AddColumn(SmsPlatForm.SmsTaskA.MsgLen, model.MsgLen)
                .AddColumn(SmsPlatForm.SmsTaskA.HasIndex, model.HasIndex)
                .AddColumn(SmsPlatForm.SmsTaskA.SendType, model.SendType)
                .AddColumn(SmsPlatForm.SmsTaskA.SendFlag, model.SendFlag)
                .AddColumn(SmsPlatForm.SmsTaskA.SaveType, model.SaveType)
                .AddColumn(SmsPlatForm.SmsTaskA.MobileNum, model.MobileNum)
                .AddColumn(SmsPlatForm.SmsTaskA.UnicomNum, model.UnicomNum)
                .AddColumn(SmsPlatForm.SmsTaskA.TelecomNum, model.TelecomNum)
                .AddColumn(SmsPlatForm.SmsTaskA.ChinaNum, model.ChinaNum)
                .AddColumn(SmsPlatForm.SmsTaskA.SingleMobileFee, model.SingleMobileFee)
                .AddColumn(SmsPlatForm.SmsTaskA.SingleUnicomFee, model.SingleUnicomFee)
                .AddColumn(SmsPlatForm.SmsTaskA.SingleTelecomFee, model.SingleTelecomFee)
                .AddColumn(SmsPlatForm.SmsTaskA.SingleChinaFee, model.SingleChinaFee).Execute();                
                return true;
            }
            catch (System.Exception e)
            {
                throw new Exception(e.Message);
               // return false;
            }
            
		}
		/// <summary>
		/// 更新一条数据
		/// </summary>
		public void Update(SmsServer.Model.SmsTaskA model)
		{
			
		}

		/// <summary>
		/// 删除一条数据
		/// </summary>
		public void Delete(long ID)
		{
			
		}
		/// <summary>
		/// 得到一个对象实体
		/// </summary>
		public SmsServer.Model.SmsTaskA GetModel(long ID)
		{

            return null;
		}

		/// <summary>
		/// 获得数据列表
		/// </summary>
		public DataSet GetList(string strWhere)
		{
            return null;
		}

		/// <summary>
		/// 获得前几行数据
		/// </summary>
		public DataSet GetList(int Top,string strWhere,string filedOrder)
		{
            return null;
		}

		#endregion  成员方法
	}
}

