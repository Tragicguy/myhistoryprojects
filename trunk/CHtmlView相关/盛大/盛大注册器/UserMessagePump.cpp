#include "StdAfx.h"
#include ".\usermessagepump.h"

CUserMessagePump *CUserMessagePump:: m_Instance=NULL;
CCriticalSection CUserMessagePump::m_slockInstance;
CUserMessagePump::CUserMessagePump(void)
{
}

CUserMessagePump::~CUserMessagePump(void)
{

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserMessagePump * CUserMessagePump::GetInstance()
{
        if(m_Instance==NULL)
        {
                m_slockInstance.Lock();
                if(m_Instance==NULL)
                {
                        m_Instance=new CUserMessagePump;
                }
                m_slockInstance.Unlock();
        }
        return m_Instance;
}

void CUserMessagePump::SetHwnd(UINT windowID,HWND hwnd)
{
        _MessageHWND msgHwnd;
        msgHwnd.DstWindowID = windowID;
        msgHwnd.hwnd = hwnd;
        m_lockHwnd.Lock();
        m_listHwnd.push_back(msgHwnd);
        m_lockHwnd.Unlock();
}

//发送消息
void CUserMessagePump::AddToMessagePump(UINT windowID,CString msgStr)
{
        HWND dstHwnd = NULL;

        list<_MessageHWND>::iterator it;
        m_lockHwnd.Lock();
        it = m_listHwnd.begin();
        while(it!=m_listHwnd.end())
        {
                _MessageHWND msgHwnd = (*it);
                if (msgHwnd.DstWindowID == windowID)
                {
                        dstHwnd = msgHwnd.hwnd;
                        break;
                }
                it++;
        }
        m_lockHwnd.Unlock();

        if (dstHwnd != NULL)
        {
                _MessagePumpStr msgStru;
                msgStru.DstWindowID = windowID;
                msgStru.hwnd = dstHwnd;
                msgStru.msgString = msgStr;
                m_lockMsg.Lock();
                m_listMsg.push_back(msgStru);
                m_lockMsg.Unlock();
                ::PostMessage(dstHwnd,windowID,0,0);
        }
}

//取消息
void CUserMessagePump::GetPumpMessage(UINT windowID,HWND hwnd,CString &msgStr)
{
        list<_MessagePumpStr>::iterator it;
        m_lockMsg.Lock();
        it = m_listMsg.begin();
        while(it!=m_listMsg.end())
        {
                _MessagePumpStr msgStru = (*it);
                if (msgStru.DstWindowID == windowID && msgStru.hwnd == hwnd)
                {
                        msgStr = msgStru.msgString;
                        m_listMsg.erase(it);
                        break;
                }
                it++;
        }
        m_lockMsg.Unlock();
}