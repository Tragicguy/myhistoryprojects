if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AddSendResult_MW]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[AddSendResult_MW]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ReadTaskMessage_MW]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[ReadTaskMessage_MW]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SaveMoMessage_MW]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[SaveMoMessage_MW]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[UpDateSendResult_MW]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[UpDateSendResult_MW]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE [AddSendResult_MW] (
@SubTime varchar(21),
--@OrgAddr varchar(30),
@DestAddr varchar(21),
@SM_Content varchar(200),
@SendTime varchar(21),
@Service_ID varchar(21),
@Valid_Time varchar(21),
@Sent_Time varchar(21),
@Msg_Status int,
@Recv_Status int,
@Recv_Time varchar(21),
@Reserve2 varchar(20),
@CreatorID varchar(10),
@MessageID varchar(50),
@msglocalID int)
as
declare @sql varchar(2000),
  @tblName varchar(21),
  @s varchar(10),
  @tempTime datetime
begin
  select @tempTime = convert(datetime,@SendTime)
  Select @s=month(@tempTime)
  if (Len(@s)<2)
  begin
    Select @s='0'+@s
  end
  Select @tblName='tbl_SMResult_'+@s
  Select @s=day(@tempTime)
  if (Len(@s)<2)
  begin
    Select @s='0'+@s
  end
  Select @tblName=@tblName+@s

if @Reserve2 is null
begin
select @Reserve2 = 'a'
end

select @sql='insert into '+@tblName+'  (SM_Type,SM_ID,SubTime,OrgAddr,DestAddr,SM_Content,SendTime,MsgID,Service_ID,Fee_UserType,Fee_Terminal_ID,Msg_Fmt,FeeType,FeeCode,Valid_Time,Sent_Time,Msg_Status,TryTimes,Recv_Status,Recv_Time,Reserve1,Reserve2,CreatorID,SMType,MessageID,DestAddrType) values(''1'',''1'','''
+cast(@SubTime as varchar(21))+''','''
+'1860'+cast(@CreatorID as varchar(10))+''','''
+cast(@DestAddr as varchar(21))+''','''
+cast(@SM_Content as varchar(200))+''','''
+cast(@SendTime as varchar(21))+''',''88888888'','''
+cast(@Service_ID as varchar(21))+''',''1'',''1'',''1'',''2'',''10'','''
+cast(@Valid_Time as varchar(21))+''','''
+cast(@Sent_Time as varchar(21))+''','''
+cast(@Msg_Status as varchar(3))+''',''1'','''
+cast(@Recv_Status as varchar(3))+''','''
+cast(@Recv_Time as varchar(21))+''',''1'','''
+cast(@Reserve2 as varchar(20))+''','''
+cast(@CreatorID as varchar(10))+''',''1'','''
+@MessageID+''',''1'')'
 exec (@sql)
delete from tbl_smsendtask where [ID]=@msglocalID
--select @sql as testsql
end
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE [ReadTaskMessage_MW]
 AS
select top 500  [ID],CreatorID,TaskName,DestAddr,SM_Content,ServiceID,SubTime,SendType ,SubOperationType  from tbl_SMSendTask where SendState = '0' and SendTime < getdate() and  (SendType = '1' or SendType = '3' or SendType = '2' or SendType = '4' or SendType = '5') order by [ID] asc
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE [SaveMoMessage_MW] 
@RecvTime varchar(21),
@OrgAddr varchar(20),
@DestAddr varchar(30),
@SM_Content varchar(300)
AS
--declare @
insert into tbl_SMReceived (OrgAddr,DestAddr,SM_Content,RecvTime,Reserve1,Reserve2,SMType,MessageID,OrgAddrType,ActionID,ActionReasonID,ServiceID,ProtocolType,Readed,Droped)values(@OrgAddr,'18600000',@SM_Content,convert(datetime,@RecvTime),'','','0','88888888','0','0','0','EIE','0','0','0')
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE [UpDateSendResult_MW](
@Recv_Time varchar(21),
@Recv_Status varchar(10),
@MessageID varchar(50))
AS
declare @tblName varchar(20),
@sql varchar(200)
select @tblName='tbl_SMResult_'+Left(@MessageID,4)
select @sql = 'update '+@tblName + ' set Recv_Time ='''+@Recv_Time+''',Recv_Status='''+@Recv_Status+'''  where MessageID='''+@MessageID+''' and Recv_Status <> ''0'''
exec(@sql)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

