Attribute VB_Name = "modFindePass"
Public Type GetDrvInfo
    sBack_SystemuDiskWord As String
    sBack_DrvKeyNumber As String
    sBack_DrvVenInfo As String
    sBack_DrvProdIDInfo As String
    sBack_DrvRevSNInfo As String
    sBack_EncryptVer As String
    sBack_Encryptlevel As String
    sBack_MakeDate As Date
    sBack_MakeTime As Date
    sBack_MakeComputerName As String
    sBack_CheckUseTimes As Boolean
    sBack_SetedMaxUseTimes As Long
    sBack_CheckUseDays As Boolean
    sBack_SetedMaxUseDays As Boolean
    sBack_CheckAnyWhereStarEvent As Boolean
    sBack_CanUseTimes As Long
    sBack_CanUseDays As Long
    sBack_SaveLogInfo As Boolean
    sBack_CurUseDiskWord As String
End Type

