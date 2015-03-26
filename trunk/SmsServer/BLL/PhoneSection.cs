using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using Newtonsoft.Json;
namespace SmsServer.BLL
{
    public class PhoneSection
    {
        private readonly DAL.PhoneSection dal = new DAL.PhoneSection();
        public DataSet GetPhoneSection()
        {
            return dal.GetPhoneSection();
        }
        public bool Update(string strJson, out string strout)
        {
            Model.PhoneSection model = (Model.PhoneSection)JavaScriptConvert.DeserializeObject(strJson, typeof(Model.PhoneSection));
            strout = "更新号段信息失败";
            bool bRet = dal.Update(model);
            if (bRet)
            {
                //清除缓存
                DataCache.InitCacheSection();
            }
            return bRet;
        }

        public string GetPhoneSectionFields()
        {
            DataSet ds = dal.GetPhoneSection();
            string strJson = "{success:true,data:";
            strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
            return strJson;
        }
    }
}
