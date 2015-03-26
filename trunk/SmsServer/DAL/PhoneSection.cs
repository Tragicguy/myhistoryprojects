using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using NBearLite;//请先添加引用
namespace SmsServer.DAL
{
    public class PhoneSection
    {
        public DataSet GetPhoneSection()
        {
            return MyDataBase.db.Select(SmsPlatForm.PhoneSection).ToDataSet();
        }

        public bool Update(Model.PhoneSection model)
        {
            try
            {
                MyDataBase.db.Update(SmsPlatForm.PhoneSection)
                .AddColumn(SmsPlatForm.PhoneSection.MobileHeader, model.MobileHeader)
                .AddColumn(SmsPlatForm.PhoneSection.UnicomHeader, model.UnicomHeader)
                .AddColumn(SmsPlatForm.PhoneSection.TelecomHeader, model.TelecomHeader).Execute();
                return true;
            }
            catch (System.Exception e)
            {
                //记录日志
                return false;
            }
        }
    }
}
