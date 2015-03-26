Ext.MessageBox.minWidth = 250;
var pageSize = 13;
ExtShow = function(varMsg)
{
Ext.MessageBox.show(
    {
        msg: varMsg,
        progressText: 'Saving...',
        width:300,
        wait:true,
        waitConfig: {interval:200},
        icon:'download',
        animEl: 'saving'
    }
    );
}

ExtAlert = function(varMsg)
{
    Ext.MessageBox.alert('提示',varMsg);
}
ExtAlertEx = function(objMask,varMsg)
{
    Ext.MessageBox.alert('提示',varMsg);
    objMask.hide();
}
MaskShow = function(objForm,varMsg)
{
   var objMask = new Ext.LoadMask(objForm.getEl(), { msg: varMsg});
   objMask.show(); 
   return objMask;
}

MaskHide = function(objMask)
{
    objMask.hide();
}