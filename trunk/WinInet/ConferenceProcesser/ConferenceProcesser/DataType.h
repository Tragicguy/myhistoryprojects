#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#include <list>
#include "tinyxml/tinyxmlEx.h"
#include "../Util/Function.h"
using std::list;
/*
<?xml version="1.0" encoding="UTF-8" ?> 
- <!--  XML文档命名为 meeting.xml 
--> 
- <!--  updatetime为该XML文档更新时间， version为版本号，目前为"V1.0" 
--> 
- <root updatetime="2011-07-01 13:00:00" version="V1.0">
- <!--  boxnos记录该box下的会议信息所需要下发的终端编号，连续的终端编号以"-"连接，不连续的终端编号以";"连接 
--> 
- <box boxnos="BOX00001-BOX00010;BOX00100">
- <!--  每条会议记录为一个record 
-->  
- <record>
- <!--  会议室编号 
--> 
<roomid>0001</roomid> 
- <!--  会议室名称 
--> 
- <roomname>
- <![CDATA[ 会议室A
]]> 
</roomname>
- <!--  会议开始时间 
--> 
<starttime>10:00</starttime> 
- <!--  会议结束时间 
--> 
<endtime>11:00</endtime> 
- <!--  会议主题
--> 
- <topic>
- <![CDATA[ ACM2010亚洲论坛
]]> 
</topic>
- <!--  会议主持人 
--> 
<chairman>小王</chairman> 
- <!--  会议预定人 
--> 
<bookpeople>小王</bookpeople> 
- <!--  会议参与人员 
--> 
- <peoples>
+ <people>
- <![CDATA[ 部门A
]]> 
</people>
+ <people>
- <![CDATA[ 部门B
]]> 
</people>
</peoples>
- <!--  该会议室是否属于当前楼层, 值为true或false 
--> 
<currentfloor>true</currentfloor> 
- <!--  显示格式 
--> 
+ <displayformat>
- <!--  是否在屏幕上突出显示, 值为true或false 
--> 
<ishighlight>true</ishighlight> 
</displayformat>
</record>
- <!--  无会议信息的时间段，Record内容如下 
--> 
- <record>
<roomId>0001</roomId> 
- <roomname>
- <![CDATA[ 会议室A
]]> 
</roomname>
<starttime>11:00</starttime> 
<endtime>11:30</endtime> 
+ <topic>
- <![CDATA[ 会议室空闲
]]> 
</topic>
<chairman /> 
<bookpeople /> 
<peoples /> 
<currentfloor>true</currentfloor> 
- <displayformat>
<ishighlight>false</ishighlight> 
</displayformat>
</record>
</box>
</root>
*/

typedef struct _RoomInfo
{
	CString RoomName;
	_RoomInfo()
	{
		RoomName = "";
	}
}RoomInfo;

typedef struct _ConferenceSource 
{
	CString ConfId;
	CString ConfName;
	CString StartTime;
	CString EndTime;
	CString Department;
	RoomInfo Room;
	CString ParticipantNumber;
	CString ConfState;
	CString ConfType;
	_ConferenceSource()
	{
		ConfId.Empty();
		ConfName.Empty();
		StartTime.Empty();
		EndTime.Empty();
		Department.Empty();
		ParticipantNumber.Empty();
		ConfState.Empty();
		ConfType.Empty();
	}
}ConferenceSource;

typedef list<ConferenceSource> ConferenceSourceList;

typedef struct _DisplayFormat
{
	bool ishighlight;
}DisplayFormat;
//record
typedef struct _ConferenceRecord 
{
	CString roomid;
	CString roomname;
	CString starttime;
	CString endtime;
	CString topic;
	CString chairman;
	CString bookpeople;
	list<CString> peoples;
	bool currentfloor;
	DisplayFormat displayformat;
	_ConferenceRecord()
	{
		roomid.Empty();
		roomname.Empty();
		starttime.Empty();
		endtime.Empty();
		topic.Empty();
		chairman.Empty();
		bookpeople.Empty();
		peoples.clear();
		currentfloor = false;
	}
}ConferenceRecord;

//conference
class ConferenceInfo
{
public:
	ConferenceInfo()
	{
		boxnos.Empty();
		version.Empty();
		updatetime.Empty();
		record.clear();
	}
	bool ToXml(const char* szPath);
	bool SetRecord(TiXmlElement* boxElement, list<ConferenceRecord>::iterator dataRecord);

	CString boxnos;
	CString version;
	CString updatetime;
	list<ConferenceRecord> record;	
};
#endif