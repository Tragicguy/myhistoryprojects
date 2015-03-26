PhoneSectionEdit = function(node)
{
    var SectionEditPanel = new Ext.FormPanel({   
    id:'SectionEditPanel',
    labelAlign:'right',
    layout:"form",   
    //title:'号段编辑',
    buttonAlign:'center',   
    bodyStyle:'padding:5px',   
    width: 470,
    frame:true,   
    labelWidth:80,
    items: [{
                xtype:'textarea',
                fieldLabel: '移动号段',
                width:360,
                name: 'MobileHeader'
            },{
                xtype:'textarea',
                fieldLabel: '联通号段',
                width:360,
                name: 'UnicomHeader'
            },{
                xtype:'textarea',
                fieldLabel: '电信号段',
                width:360,
                name: 'TelecomHeader'
            }, {//按钮
                   xtype : 'panel',
                   bodyStyle : 'margin-bottom:0px;margin-top:0px;margin-left:120px;',
                   layout:"column", 
                   items:[{
                           text : '保存',
                           xtype:'button',
                           iconCls:'saveicon',
                           width:60,
                           handler : function() {
                                  var values = Ext.getCmp('SectionEditPanel').getForm().getValues();
 			                      if (values.MapID == '') {
 			                            ExtAlert('映射编号不能为空');
 			                            return;
 			                      }
			                      else{
			                          doSaveSectionInfo(values);
			                      }        
                            }
                        },
                        {text : '重置',xtype:'button',iconCls:'refreshicon',width:60,handler : function() {
                                SectionEditPanel.form.reset();
                                loadData();
                        }},
                        {text : '取消',xtype:'button',iconCls:'exiticon',width:60,handler : function() {SectionEditWin.close();}}
                        ]
    }         
]
});   
    var SectionEditWin=new Ext.Window({            
			title:"号段信息配置",
			width:485,
			height:285,
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
			items:[SectionEditPanel]
	});
	SectionEditWin.show();
    loadData();	
	function loadData() {
        SectionEditWin.setTitle("号段信息配置-<font color='red'> 加载中....</font>");
        Ext.Ajax.request({
           url:"./Data/SmsConfig/PhoneSectionEdit.aspx",
            method:"POST",
            params:{
                type:'3'
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    Ext.getCmp('SectionEditPanel').getForm().setValues(responseArray.data[0]);                    
                }else{                
                    ExtAlert(responseArray.errorInfo);
                }
               SectionEditWin.setTitle("号段信息配置");
            },
            failure:function(response,option)
            {
                ExtAlert("网络错误");
                SectionEditWin.setTitle("号段信息配置");
            }
        }); 
    }
    function doSaveSectionInfo(values)
    {
       var objMask = MaskShow(SectionEditPanel,"正在提交,请稍后...");
           Ext.Ajax.request({
           url:"./Data/SmsConfig/PhoneSectionEdit.aspx",
            method:"POST",
            params:{
                jdata:Ext.encode(values),
                type:'1'
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    SectionEditWin.close();
                }else{                
                   ExtAlertEx(objMask,responseArray.errorInfo);
                }
            },
            failure:function(response,option)
            {
                ExtAlertEx(objMask,"网络错误");
            }
        }); 
    }
}