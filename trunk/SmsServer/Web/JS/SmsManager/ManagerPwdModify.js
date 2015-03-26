ManagerPwdModify = function(icon)
{
    var ManagerPwdModifyPanel = new Ext.FormPanel({   
    id:'ManagerPwdModifyPanel',
    labelAlign:'right',
    layout:"form",   
    buttonAlign:'center',   
    bodyStyle:'padding:5px',   
    width: 250,
    frame:true,
    labelWidth:70,
    items: [{                     //这里可以为多个Item，表现出来是该列的多行   
                xtype:'textfield',
                inputType:'password',
                fieldLabel: '旧密码',
                name: 'ManagerPwd'
            },{
                xtype:'textfield',
                inputType:'password',
                fieldLabel: '新密码',
                name: 'NewManagerPwd'
            },{
                xtype:'textfield',
                inputType:'password',
                fieldLabel: '确认新密码',
                name: 'ReNewManagerPwd'
           }],
    buttons:[{
       text : '保存',
       width:60,
       handler : function() {
              var values = Ext.getCmp('ManagerPwdModifyPanel').getForm().getValues();
              if (values.ManagerPwd == '') {
			        ExtAlert('旧密码不能为空');
			        return;
			  }
			  if (values.NewManagerPwd == '') {
			        ExtAlert('新密码不能为空');
			        return;
			  }
			  if (values.NewManagerPwd != values.ReNewManagerPwd) {
			        ExtAlert('新密码输入不一致');
			        return;
			  }
			  values.ReNewManagerPwd = null;
			   ExtShow("正在提交,请稍后...");
		       Ext.Ajax.request({
               url:"./Data/SmsManager/PwdModify.aspx",
                method:"POST",
                params:{
                    jdata:Ext.encode(values)
                },
                success:function(response,option)
                {           
                    var responseArray = Ext.decode(response.responseText);                                    
                    if(responseArray.success == true){
                        Ext.Msg.alert("提示",'密码修改成功',function(){
                            ManagerPwdModifyWin.close();
                        });
                    }else{
                        ExtAlert(responseArray.errorInfo);
                    }
                },
                failure:function(response,option)
                {
                    ExtAlert("网络错误");
                }
            }); 
        }
    },{
       text : '重置',
       width:60,
       handler : function() {
              ManagerPwdModifyPanel.form.reset();
              }
    },{
       text : '取消',
       width:60,
       handler : function() {
              ManagerPwdModifyWin.close();
              }
    }]
});   
var ManagerPwdModifyWin=new Ext.Window({
		title:"个人密码修改",
		width:265,
		height:169,
		plain:true,
		layout:"form",
		iconCls:icon,
		
		resizable:false,
		defaultType:"textfield",
		labelWidth:100,
		closeAction : 'close',
        closable:true,
        modal: 'true', 
		buttonAlign:"center",
		bodyStyle:"padding:0px 0 0 0px",
		items:[ManagerPwdModifyPanel]
});
ManagerPwdModifyWin.show();
}
