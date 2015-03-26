SmsManagerEdit = function(varUserID)
{
var oldType = '';
var ManagerInfoEdiPanel = new Ext.FormPanel({   
    id:'ManagerInfoEdiPanel',
    labelAlign:'right',
    layout:"form",   
    buttonAlign:'center',   
    bodyStyle:'padding:5px',   
    width: 470,
    frame:true,   
    labelWidth:80,
    items: [{
        layout:'column',
        border:false,
        labelSeparator:'：',   
        items:[{   
            columnWidth:.5,  //该列占用的宽度，标识为50％   
            layout: 'form',   
            border:false,   
            items: [{                     //这里可以为多个Item，表现出来是该列的多行   
                xtype:'textfield',
                fieldLabel: '账户名',
                //disabled:true,
                readOnly:true,
                name: 'ManagerID'
            },{
                xtype:'textfield',
                fieldLabel: '姓名',
                name: 'UserName'
            },{
                xtype:'textfield',
                fieldLabel: '公司名',
                name: 'CompanyName'
            }]
        },{
            columnWidth:.5,
            layout: 'form',
            border:false, 
            items: [{
                xtype:'textfield',
                inputType:'password',
                fieldLabel: '密码',
                disabled:true,
                name: 'ManagerPwd',
                value:'123456'
            },{                
                name:"ManagerType",
				xtype:"combo",
				fieldLabel:"类型",
				width:90,
			    editable: false,
			    typeAhead: true,
				//传入后台真实值value field /value
				hiddenName:"ManagerType",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:new Ext.data.SimpleStore({
					fields:["name","value"],
					data:[["管理员","0"],["代理商","1"]]
				})
            },{
                xtype:'textfield',
                fieldLabel: '电话',
                name: 'Phone'
            }]   
        }]   
    },{
                xtype:'textfield',
                width:360,
                fieldLabel: '地址',
                name: 'Address'
    },{
                xtype:'textarea',
                fieldLabel: '备注',
                width:360,
                height:50,
                name: 'ExtraInfo'
    }],
    buttons:[{
       text : '保存',
       width:60,
       handler : function() {
              var values = Ext.getCmp('ManagerInfoEdiPanel').getForm().getValues();
			  if (values.UserName == '') {
			        ExtAlert('姓名不能为空');
			        return;
			  }
			  if (values.Phone == '') {
			        ExtAlert('电话不能为空');
			        return;
			  }
 			  if (values.ManagerType != oldType) {
 			    Ext.Msg.confirm("提示!","您改变了类型,账户权限将被清空,继续吗?",function(btn){
 			        if(btn=="yes"){
 			             doSaveManagerInfo(values);
 			        }
 			    });
			  }
			  else{
			      doSaveManagerInfo(values);
			  }           
        }
    },{
       text : '重置',
       width:60,
       handler : function() {
              ManagerInfoEdiPanel.form.reset();
              loadData();
              }
    },{
       text : '取消',
       width:60,
       handler : function() {
              ManagerInfoEditWin.close();
              }
    }]
});   
    var ManagerInfoEditWin=new Ext.Window({
			title:"编辑账户信息-"+varUserID,
			width:485,
			height:251,
			plain:true,
			layout:"form",
			iconCls:"editicon",			
			resizable:false,
			defaultType:"textfield",
			labelWidth:100,
			closeAction : 'close',
            closable:true,
            modal: 'true', 
			buttonAlign:"center",
			bodyStyle:"padding:0px 0 0 0px",
			items:[ManagerInfoEdiPanel]
	});
	ManagerInfoEditWin.show();
	loadData();
	function loadData() {
        ManagerInfoEditWin.setTitle("编辑账户信息-"+varUserID+'<font color="red"> 加载中....</font>');
        Ext.Ajax.request({
           url:"./Data/SmsManager/ManagerOprate.aspx",
            method:"POST",
            params:{
                userid:varUserID,
                type:'3'
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    Ext.getCmp('ManagerInfoEdiPanel').getForm().setValues(responseArray.data[0]);
                    oldType = responseArray.data[0].ManagerType;
                }else{                
                    ExtAlert(responseArray.errorInfo);
                }
               ManagerInfoEditWin.setTitle("编辑账户信息-"+varUserID);
            },
            failure:function(response,option)
            {
                ExtAlert("网络错误");
                ManagerInfoEditWin.setTitle("编辑账户信息-"+varUserID);
            }
        }); 
    }
    function doSaveManagerInfo(values)
    {
        ExtShow("正在提交,请稍后...");
                           Ext.Ajax.request({
                           url:"./Data/SmsManager/ManagerOprate.aspx",
                            method:"POST",
                            params:{
                                jdata:Ext.encode(values),
                                type:'1'
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    Ext.MessageBox.hide();
                                    ManagerInfoEditWin.close();
                                    ManagerInfoStore.reload();
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
}