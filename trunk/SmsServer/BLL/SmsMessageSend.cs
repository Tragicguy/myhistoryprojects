using System;
using System.Collections.Generic;
using System.Text;
using SmsUserData = SmsServer.Model.SmsUser;
using SmsMsgSendStr = SmsServer.Model.SmsMsgSend;
using SmsSpgateEx = SmsServer.Model.SmsSpgate;
using SmsTaskEx = SmsServer.Model.SmsTaskA;
namespace SmsServer.BLL
{
    public class SmsMessageSend
    {
        private readonly SmsTaskA objTaskBll = new SmsTaskA();
        /// <summary>
        /// 对外的信息发送接口
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="strPwd"></param>
        /// <param name="strPhone"></param>
        /// <param name="strMsg"></param>
        /// <param name="strSubport"></param>
        /// <returns></returns>
        public string SendMessage(string strUserID,string strPwd,string strPhone,string strMsg,string strSubport)
        {
            //此处传进来的参数已经经过了空参数与非法参数的检查
            if(string.IsNullOrEmpty(strUserID) ||
                string.IsNullOrEmpty(strPwd) ||
                string.IsNullOrEmpty(strPhone) ||
                string.IsNullOrEmpty(strMsg))
            {
                return "-1";  //参数为空
            }
            //验证账号密码、子端口格式
            if (!DataVerify.IsIllegalUser(strUserID) || !DataVerify.IsIllegalUser(strPwd))
            {
                return "-2";    //账号或者密码非法
            }
            else if(!string.IsNullOrEmpty(strSubport)) 
            {
                if (!DataVerify.IsNumber(strSubport) || strSubport.Length > 6 )
                {
                    return "-3";    //子端口格式非法
                }
                //strSubport = "";
            }           
            //获取用户信息,判断密码是否正确
            SmsUserData objUser = new SmsUserData();
            DataCache.GetCacheUser(strUserID, strPwd, ref objUser);
            if(object.Equals(null,objUser))
            {
                //用户名或密码错误
                return "-11";
            }
            //判断账号是否被禁用
            if (objUser.Status == 1)//账号被禁用,0正常，1禁用
            {
                return "-12";
            }
            //如果余额已经小于等于0
            if (objUser.Balance <= 0)
            {
                return "-15";   //余额不足
            }
            //判断信息内容是否超长[加上签名后的],也就是说可以发的长度是设置长度减去签名长度
            int iMsgLen = 0;
            strMsg = strMsg + objUser.SignStr.Trim();
            iMsgLen = strMsg.Length;
            if (iMsgLen > objUser.MaxLen)
            {
                return "-13";
            }
            //判断是否有非法关键词
            if (objUser.IsCheckIllegal == 1)   //需要检测非法关键词
            {
                if (IsContainIllegal(strMsg))
                {
                    return "-14";
                }
            }
            string[] strPhoneArray = strPhone.Split(',');
            if (strPhoneArray.Length > objUser.MaxCount)
            {
                return "-16"; //发送号码数太多
            }
            if (strPhoneArray.Length < objUser.MinCount)
            {
                return "-17"; //发送号码数太少
            }
            SmsMsgSendStr objMsg = new SmsMsgSendStr();
            GetPhoneList(strPhoneArray, objUser, objMsg);
            //获取通道信息
            if(!GetSpgateInfo(objUser, objMsg))
            {
                return "-18";   //有通道未绑定
            }
            //获取信息拆分结果
            GetSpliteMsg(objUser, objMsg, strMsg);
            //计算扣费数
            int iTotalCount = ComputeTotalFee(objUser, objMsg);//进行扣费处理，此处难在信息的拆分上[计算一条信息要扣多少条]
            if (iTotalCount == 0)
            {
                return "-19";     //无有效号码
            }
            //验证完毕，扣余额。开始发送，如果余额不足，则不扣，在存储过程中扣费
            if(!new SmsUser().UpdateBalance(strUserID,iTotalCount))
            {
                return "-15";  //扣除余额失败,余额不足
            }
            //发送 
            SendMessage(objUser, objMsg, strSubport,iTotalCount);
            return "1";
        }
        private string SendMessage(SmsUserData objUser,SmsMsgSendStr objMsg,string strSubport,int iTotalCount)
        {
            //1。根据审核标识来判断是写入审核表，还是进行发送处理
//             if (objUser.IsAudit == 1)
//             {
//                 if (iTotalCount >= objUser.AuditNum)
//                 {
//                     //插入等待审核表
//                     return "12";//发送成功
//                 }
//             }

            //以上判断是否插入审核表
            SmsTaskEx objSmsTask = new SmsTaskEx();
            objSmsTask.SendFlag = 0;        //标识是否号码是正确的
            objSmsTask.SaveType = objUser.HistoryType;
            //objSmsTask.SubPort = strSubport;
            objSmsTask.SubPortEx = strSubport;
            objSmsTask.SendTime = DateTime.Now;            
            objSmsTask.SubTime = DateTime.Now;
            objSmsTask.UserID = objUser.UserID;
            objSmsTask.ParentID = objUser.ParentID;
            for (int i = 0; i < 4;i++ )     //发送真实信息
            {
                if (objMsg.PhoneCount[i] > 0)   //如果有号码需要发送
                {
                    objSmsTask.SpgateNo = objMsg.PhoneSpgate[i].GateNumber;
                    objSmsTask.PhoneList = objMsg.PhoneList[i].ToString();
                    objSmsTask.PhoneNum = objMsg.PhoneCount[i];
                    switch (i)
                    {
                        case 0:
                            objSmsTask.SubPort = objUser.MobileSubport;
                            break;
                        case 1:
                            objSmsTask.SubPort = objUser.UnicomSubport;
                            break;
                        case 2:
                            objSmsTask.SubPort = objUser.TelecomSubport;
                            break;
                        case 3:
                            objSmsTask.SubPort = objUser.ChinaPhoneSubport;
                            break;
                    }
                    for (int j = 0; j < 4;j++ )
                    {
                        if (objMsg.FeeMap[j] == i)      //如果j号段映射到了i号段上，那么在i通道上的j号段号码数为j号段扣费号码数，i通道上j号段的拆分扣费数
                        {
                            switch (j)
                            {
                                case 0:
                                    objSmsTask.SubPort = objUser.MobileSubport;
                                    objSmsTask.MobileNum = objMsg.FeePhoneCount[j];
                                    break;
                                case 1:
                                    objSmsTask.SubPort = objUser.UnicomSubport;
                                    objSmsTask.UnicomNum = objMsg.FeePhoneCount[j];
                                    break;
                                case 2:
                                    objSmsTask.SubPort = objUser.TelecomSubport;
                                    objSmsTask.TelecomNum = objMsg.FeePhoneCount[j];
                                    break;
                                case 3:
                                    objSmsTask.SubPort = objUser.ChinaPhoneSubport;
                                    objSmsTask.ChinaNum = objMsg.FeePhoneCount[j];
                                    break;
                            }
                        }                        
                    }
                    SendMessageQueue(objSmsTask, objMsg.MessageArray[i], objMsg.SingleMsgFeeNum, objMsg.MsgArrayLen[i], objMsg.PhoneSpgate[i].BusinessID);
                }
            }
            //发送错误号码的信息，统一流程
            if (objMsg.PhoneCount[4] > 0)
            {
                objSmsTask.SendFlag = 1;
                objSmsTask.SubPort = objUser.MobileSubport;
                objSmsTask.SpgateNo = objMsg.PhoneSpgate[0].GateNumber;
                objSmsTask.PhoneList = objMsg.PhoneList[4].ToString();
                objSmsTask.PhoneNum = objMsg.PhoneCount[4];
                objSmsTask.MobileNum = objMsg.FeePhoneCount[4]; //将扣费不发送号码全分到移动上
                objSmsTask.UnicomNum = 0;
                objSmsTask.TelecomNum = 0;
                objSmsTask.ChinaNum = 0;
                SendMessageQueue(objSmsTask, objMsg.MessageArray[0], objMsg.SingleMsgFeeNum, objMsg.MsgArrayLen[0], objMsg.PhoneSpgate[0].BusinessID);
            }
            return "1";
        }
        /// <summary>
        /// 对信息进行拆分发送
        /// </summary>
        /// <param name="objSmsTask"></param>
        /// <param name="strMsgArray"></param>
        /// <param name="iSingleFeenum"></param>
        /// <param name="iArrayLen"></param>
        /// <param name="iBusinessID"></param>
        private void SendMessageQueue(SmsTaskEx objSmsTask, string[] strMsgArray, int[][] iSingleFeenum, int iArrayLen, int iBusinessID)
        {
            for (int i=0;i<iArrayLen;i++)
            {
                objSmsTask.MsgContent = strMsgArray[i];
                objSmsTask.MsgLen = strMsgArray[i].Length;
                objSmsTask.SingleMobileFee = iSingleFeenum[0][i];
                objSmsTask.SingleUnicomFee = iSingleFeenum[1][i];
                objSmsTask.SingleTelecomFee = iSingleFeenum[2][i];
                objSmsTask.SingleChinaFee = iSingleFeenum[3][i];
                SendMessageQueueEx(objSmsTask, iBusinessID);
            }           
        }
        /// <summary>
        /// 往发送队列或者中转表中存储
        /// </summary>
        /// <param name="objSmsTask"></param>
        /// <param name="iBusinessID"></param>
        private void SendMessageQueueEx(SmsTaskEx objSmsTask,int iBusinessID)
        {
            if (iBusinessID != 1 || QueueManage<SmsTaskEx>.IsOverSelectLen(SmsSetIni.TaskQueuePath, SmsSetIni.QueueMaxlen))//如果不是HTTP类型或者消息队列已满
            {
                //添加进等待发送表
                if( objTaskBll.Add(objSmsTask))
                {
                    //添加成功
                }
                else
                {
                    //添加失败
                }
            }
            else   //添加进消息队列
            {
                QueueManage<SmsTaskEx>.SendQueue(SmsSetIni.TaskQueuePath, objSmsTask);
            }
        }

        //判断是否存在非法关键词
        private bool IsContainIllegal(string strMsg)
        {
            //Dictionary<string, int> IllegalList = new Dictionary<string, int>();
            //DataCache.GetCacheIllegal(ref IllegalList);
            foreach (KeyValuePair<string, int> objIllegal in DataCache.pIllegalList)
            {
                if(strMsg.Contains(objIllegal.Key))
                {
                    return true;
                }
            }
            return false;
        }
        /// <summary>
        /// 获取各号段号码数及计费号码数，及各号段号码列表
        /// </summary>
        /// <param name="strPhoneArray"></param>
        /// <param name="objUser"></param>
        /// <param name="objMsg"></param>
        private void GetPhoneList(string[] strPhoneArray,SmsUserData objUser,SmsMsgSendStr objMsg)
        {
            int iCheckLevel = 1;
            //先获取号段列表
            int iFlag = -1;
            string strPhone = string.Empty;
            int iType = -1;
            int iCurrNum = -1;
            foreach (string strMobile in strPhoneArray )
            {
                iFlag = GetPhoneSectionNum(strMobile, DataCache.pPhoneSection);
                strPhone = strMobile + ",";
                iCurrNum++;
                switch (iFlag)
                {                 
                case -1://有非法字符
                        switch (iCheckLevel)
                        {
                        case 1: //低
                            objMsg.PhoneList[4].Append(strPhone);
                            objMsg.PhoneCount[4]++;
                            objMsg.FeePhoneCount[4]++;
                            objMsg.FeeMap[4] = 0;
                            objMsg.MsgIdOffset[4].Append(iCurrNum.ToString() + ",");
                        	break;
                        }
                	break;
                case -2:    //长度不符合
                        //判断长度，结合长度差距与判断等级来确定是否扣费但是不发
                    break;
                case 3:
                    objMsg.FeePhoneCount[0]++;
                    iType = objUser.MobileMap == null ? iFlag : (int)objUser.MobileMap;
                    objMsg.FeeMap[0] = iType;
                    AdapterPhones(iType, strPhone, objMsg, iCurrNum);//根据号段映射来判断扣费增加的号段 
                    break;
                case 4:
                    objMsg.FeePhoneCount[1]++;
                    iType = objUser.UnicomMap == null ? iFlag : (int)objUser.UnicomMap;
                    objMsg.FeeMap[1] = iType;
                    AdapterPhones(iType, strPhone, objMsg, iCurrNum);
                    break;
                case 5:
                    objMsg.FeePhoneCount[2]++;
                    iType = objUser.TelecomMap == null ? iFlag : (int)objUser.TelecomMap;
                    objMsg.FeeMap[2] = iType;
                    AdapterPhones(iType, strPhone, objMsg, iCurrNum);
                    break;
                case 6:
                    objMsg.FeePhoneCount[3]++;
                    iType = objUser.ChinaPhoneMap == null ? iFlag : (int)objUser.ChinaPhoneMap;
                    objMsg.FeeMap[3] = iType;
                    //
                    AdapterPhones(iType, strPhone, objMsg, iCurrNum);
                    break;
                default:    ///默认扣费不发送
                    objMsg.PhoneList[4].Append(strPhone);
                    objMsg.PhoneCount[4]++;
                    objMsg.FeePhoneCount[4]++;
                    objMsg.FeeMap[4] = 0;
                    objMsg.MsgIdOffset[4].Append(iCurrNum.ToString() + ",");
                    break;
                }
            }
        }
        /// <summary>
        /// 获取指写号码所属号段
        /// </summary>
        /// <param name="strPhone"></param>
        /// <param name="pPhoneSection"></param>
        /// <returns></returns>
        private int GetPhoneSectionNum(string strPhone, Dictionary<string, int> pPhoneSection)
        {
            //判断是否有非法字符
            if (!DataVerify.IsNumber(strPhone))
            {
                return -1;
            }
            int iLen = strPhone.Length;
            if (iLen > 12 || iLen < 11)
            {
                return -1;
            }
            string strHeader = strPhone.Substring(0, 3);
            try
            {

                char[] charArray = strPhone.ToCharArray();
                if (0x30 == charArray[0])//第一位是0
                {
                    if (0x30 == charArray[1]) //第二位也是0
                    {
                        return -1;
                    }
                    else if (0x31 == charArray[1] && 0x30 != charArray[2]) //第二位是1，但是第三位不是0
                    {
                        return -1;
                    }
                    else
                    {
                        if (iLen < 10 || iLen > 12)
                        {
                            return -1;//错误
                        }
                        else
                        {
                            return  6;	//小灵通
                        }                        
                    }
                }
                if (0x31 == charArray[0] && 11 == iLen)	//手机号
                {
                    //匹配手机号段
                    if (pPhoneSection.ContainsKey(strHeader))
                    {
                        return pPhoneSection[strHeader];
                    }
                    else
                    {
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }               
            }
            catch
            {
            }
            return -1;
        }
        /// <summary>
        /// 根据号码所在号段进行匹配,加入对应该的发送号段
        /// </summary>
        /// <param name="iType"></param>
        /// <param name="strPhone"></param>
        /// <param name="objMsg"></param>
        private void AdapterPhones(int iType, string strPhone, SmsMsgSendStr objMsg,int iCurrNum)
        {
            switch (iType)
            {
                case 0:
                case 1:                   
                case 2:                   
                case 3:
                    objMsg.PhoneList[iType].Append(strPhone);
                    objMsg.PhoneCount[iType]++;
                    objMsg.MsgIdOffset[iType].Append(iCurrNum.ToString() + ",");
                    break;
                default:    ///默认扣费不发送
                    objMsg.PhoneList[4].Append(strPhone);
                    objMsg.PhoneCount[4]++;
                    objMsg.FeePhoneCount[4]++;
                    objMsg.FeeMap[4] = 0;
                    objMsg.MsgIdOffset[4].Append(iCurrNum.ToString() + ",");
                    break;
            }
        }

        /// <summary>
        /// 获取通道信息
        /// </summary>
        /// <param name="objUser"></param>
        /// <param name="objMsg"></param>
        /// <returns></returns>
        private bool GetSpgateInfo(SmsUserData objUser, SmsMsgSendStr objMsg)
        {
            string strSpgateNumber = "";
            int iPhoneCount = 0;
            for (int i = 0; i < 4;i++ )
            {
                switch(i)
                {
                    case 0:
                        strSpgateNumber = objUser.MobileSpgate;
                        break;
                    case 1:
                        strSpgateNumber = objUser.UnicomSpgate;
                        break;
                    case 2:
                        strSpgateNumber = objUser.TelecomSpgate;
                        break;
                    case 3:
                        strSpgateNumber = objUser.ChinaPhoneSpgate;
                        break;
                    default:
                        strSpgateNumber = "";
                        break;
                }
                if (i == 0)
                {
                    iPhoneCount = objMsg.PhoneCount[i] + objMsg.PhoneCount[4];
                }
                else
                {
                    iPhoneCount = objMsg.PhoneCount[i];
                }
                if (iPhoneCount > 0)
                {
                    if (strSpgateNumber == "")
                    {
                        return false;
                    }
                    if (!DataCache.GetCacheSpgate(strSpgateNumber, ref objMsg.PhoneSpgate[i]))
                    {
                        return false;
                    }
                }
            }            
            return true;
        }
        /// <summary>
        /// 取得各通道拆分后的信息
        /// </summary>
        /// <param name="objUser"></param>
        /// <param name="objMsg"></param>
        /// <param name="strMsg"></param>
        private void GetSpliteMsg(SmsUserData objUser, SmsMsgSendStr objMsg, string strMsg)
        {
            SmsSpgate spgate = new SmsSpgate();
            int iPhoneCount = 0;
            for (int i = 0; i < 4; i++)
            {
                if (i == 0)
                {
                    iPhoneCount = objMsg.PhoneCount[i] + objMsg.PhoneCount[4];
                }
                else
                {
                    iPhoneCount = objMsg.PhoneCount[i];
                }
                if (iPhoneCount > 0)       //绑定好了通道,//拆分好信息保存进数组
                {                    
                    SplitMessage(strMsg, objMsg.PhoneSpgate[i].MsgLen, objUser.HasIndex, ref objMsg.MessageArray[i], out objMsg.MsgArrayLen[i]);
                }
            }
        }
        /// <summary>
        /// 拆分信息存入数组
        /// </summary>
        /// <param name="strMsg"></param>
        /// <param name="iSplitLen"></param>
        /// <param name="iIndexFlag"></param>
        /// <param name="retArray"></param>
        /// <param name="iNum"></param>
        private void SplitMessage(string strMsg, int iSplitLen,int iIndexFlag,ref string[] retArray, out int iNum)
        {
            iNum = 0;
            int iLen = strMsg.Length;
            string strTemp = string.Empty;
            int iNewSplitLen = 0;
            if (iLen <= iSplitLen)
            {
                iIndexFlag = 0;
            }
            if (iIndexFlag == 1)
            {
                iNewSplitLen = iSplitLen - 3;
            }
            else
            {
                iNewSplitLen = iSplitLen;
            }
            while(true)
            {
                if (strMsg.Length == 0 || iNum > 9)
                {
                    break;
                }
                if (strMsg.Length <= iNewSplitLen)
                {
                    retArray[iNum] = strMsg;
                    iNum++;
                    break;
                }
                else
                {
                    retArray[iNum] = strMsg.Substring(0, iNewSplitLen);
                    iNum++;
                    strMsg = strMsg.Substring(iNewSplitLen, strMsg.Length - iNewSplitLen);
                }
            }
            for (int i = 0; i < iNum; i++)
            {
                if (iIndexFlag == 1)
                {
                    retArray[i] = string.Format("{0}/{1}", i + 1, iNum) + retArray[i];
                }                
            }
        }

        /// <summary>
        /// 计算应扣费总额
        /// </summary>
        /// <param name="objUser"></param>
        /// <param name="objMsg"></param>
        /// <returns></returns>
        private int ComputeTotalFee(SmsUserData objUser, SmsMsgSendStr objMsg)
        {
            int iTotalCount = 0;
            int iFeeMap = -1;
            int iSplitLen = 0;
            int iMobileSingleFee = 0;
            for (int i = 0; i < 6;i++ )
            {
                if (objMsg.FeePhoneCount[i] > 0)
                {
                    switch (i)
                    {
                        case 0:
                            //iFeeMap = objUser.MobileMap == null ? i : (int)objUser.MobileMap;                            
                            iSplitLen = (int)objUser.MobileLen;
                            break;
                        case 1:
                            //iFeeMap = objUser.UnicomMap == null ? i : (int)objUser.UnicomMap;
                            iSplitLen = (int)objUser.UnicomLen;
                            break;
                        case 2:
                            //iFeeMap = objUser.TelecomMap == null ? i : (int)objUser.TelecomMap;
                            iSplitLen = (int)objUser.TelecomLen;
                            break;
                        case 3:
                            //iFeeMap = objUser.ChinaPhoneMap == null ? i : (int)objUser.ChinaPhoneMap;
                            iSplitLen = (int)objUser.ChinaPhoneLen;
                            break;
                        case 4:
                            //iFeeMap = 0;
                            iSplitLen = (int)objUser.MobileLen;
                            break;
                        case 5:
                            //iFeeMap = 0;
                            iSplitLen = (int)objUser.MobileLen;
                            break;
                    }
                    iFeeMap = objMsg.FeeMap[i];
                    if (i == 0)
                    {
                        iMobileSingleFee = GetFee(iSplitLen, objMsg, iFeeMap, 0);
                        iTotalCount += iMobileSingleFee * objMsg.FeePhoneCount[i];
                    }
                    else if (i>3)
                    {
                        if (iMobileSingleFee == 0 )
                        {
                            iMobileSingleFee = GetFee(iSplitLen, objMsg, iFeeMap, 0);
                        }
                        iTotalCount += iMobileSingleFee * objMsg.FeePhoneCount[i];
                    }
                    else
                    {
                        iTotalCount += GetFee(iSplitLen, objMsg, iFeeMap, i) * objMsg.FeePhoneCount[i];
                    }                    
                }
            }
            return iTotalCount;
        }
        /// <summary>
        /// 获取计费长度[单通道]
        /// </summary>
        /// <param name="iSplitLen"></param>
        /// <param name="iPhoneCount"></param>
        /// <param name="strMsgArray"></param>
        /// <param name="iArrayLen"></param>
        /// <returns></returns>
        private int GetFee(int iSplitLen, SmsMsgSendStr objMsg, int iFeeMap,int iMapIndex)
        {
            int iArrayLen = objMsg.MsgArrayLen[iFeeMap];
            if(iArrayLen < 1)
            {
                return 0;
            }
            string[] strMsgArray = objMsg.MessageArray[iFeeMap];
            int  iLenTemp = 0;
            int iFirstCount = 0;
            int iLastCount = 0;             
            iLenTemp = strMsgArray[0].Length;
            iFirstCount = iLenTemp / iSplitLen + 1;
            if (iLenTemp % iSplitLen == 0)
            {
                iFirstCount--;
            }
            if(iArrayLen == 1)
            {
                objMsg.SingleMsgFeeNum[iMapIndex][0] = iFirstCount;
                return iFirstCount;
            }
            iLenTemp = strMsgArray[iArrayLen - 1].Length;
            iLastCount = iLenTemp / iSplitLen + 1;
            if (iLenTemp % iSplitLen == 0)
            {
                iLastCount--;
            }
            //给每段拆分的信息计算扣费数
            for (int i = 0; i < iArrayLen-1;i++ )
            {
                objMsg.SingleMsgFeeNum[iMapIndex][i] = iFirstCount;
            }
            objMsg.SingleMsgFeeNum[iMapIndex][iArrayLen - 1] = iLastCount;
            return iFirstCount * (iArrayLen - 1) + iLastCount;
        }
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
	}	
}