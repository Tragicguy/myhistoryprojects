SmsManagerAdd = function()
{
var ManagerInfoAddPanel = new Ext.FormPanel({   
    id:'ManagerInfoAddPanel',
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
                name: 'ManagerPwd'
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
              var values = Ext.getCmp('ManagerInfoAddPanel').getForm().getValues();
              if (values.ManagerID == '') {
			        ExtAlert('账号不能为空');
			        return;
			  }
			  if (values.ManagerPwd == '') {
			        ExtAlert('密码不能为空');
			        return;
			  }
			  if (values.UserName == '') {
			        ExtAlert('姓名不能为空');
			        return;
			  }
			  if (values.Phone == '') {
			        ExtAlert('电话不能为空');
			        return;
			  }
			   ExtShow("正在提交,请稍后...");
		       Ext.Ajax.request({
               url:"./Data/SmsManager/ManagerOprate.aspx",
                method:"POST",
                params:{
                    jdata:Ext.encode(values),
                    type:'0'
                },
                success:function(response,option)
                {           
                    var responseArray = Ext.decode(response.responseText);                                    
                    if(responseArray.success == true){
                        Ext.MessageBox.hide();
                        ManagerInfoAddWin.close();
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
    },{
       text : '重置',
       width:60,
       handler : function() {
              ManagerInfoAddPanel.form.reset();
              }
    },{
       text : '取消',
       width:60,
       handler : function() {
              ManagerInfoAddWin.close();
              }
    }]
});   
var ManagerInfoAddWin=new Ext.Window({
		title:"添加账户信息",
		width:485,
		height:251,
		plain:true,
		layout:"form",
		iconCls:"addicon",
		
		resizable:false,
		defaultType:"textfield",
		labelWidth:100,
		closeAction : 'close',
        closable:true,
        modal: 'true', 
		buttonAlign:"center",
		bodyStyle:"padding:0px 0 0 0px",
		items:[ManagerInfoAddPanel]
});
ManagerInfoAddWin.show();
}