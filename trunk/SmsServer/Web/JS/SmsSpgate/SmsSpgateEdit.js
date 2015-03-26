SmsSpgateEdit = function(varSpgate,varType)
{
    var oldSpgate = ''; 
    /************************************/   
    var SelectStore = new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["支持","1"],["不支持","0"]]
                       });
    var BusinessStore = new Ext.data.Store({
        proxy:new Ext.data.HttpProxy({
            url:'./Data/SmsSpgate/SpgateType_Bus_List.aspx',
            method:'POST'
        }),
        reader: new Ext.data.JsonReader({
            totalProperty:"totalCount",
            root:"data",
            fields:[{name: 'BusinessID'},{name: 'BusinessName'}]
            })
    });
    BusinessStore.on('beforeload',function(){
        BusinessStore.baseParams = {type:'1'};
    });   
    BusinessStore.load();
    /************************************/
    var ScriptStore = new Ext.data.Store({
        proxy:new Ext.data.HttpProxy({
            url:'./Data/SmsMapScript/MapScriptOprate.aspx',
            method:'POST'
        }),
        reader: new Ext.data.JsonReader({
            totalProperty:"totalCount",
            root:"data",
            fields:[{name: 'MapName'},{name: 'MapID'},{name: 'ScriptType'}]
            })
    });
    ScriptStore.on('beforeload',function(){
        ScriptStore.baseParams = {type:'4'};
    });
    ScriptStore.load();
    var MtScriptStore = new Ext.data.SimpleStore({
                         fields : ["MapName","MapID","ScriptType"]
                       });
    var MoScriptStore = new Ext.data.SimpleStore({
      fields : ["MapName","MapID","ScriptType"]
    });
    var ReportScriptStore = new Ext.data.SimpleStore({
      fields : ["MapName","MapID","ScriptType"]
    });
    ScriptStore.on('load',function(){
    MtScriptStore.removeAll();
    MoScriptStore.removeAll();
    ReportScriptStore.removeAll();
    ScriptStore.each(function(rcd){     //将通道按号段进行分类
           var vType = rcd.get('ScriptType');           
           if( vType== '1'){                
               MtScriptStore.add(rcd);
           }
           else if (vType== '2') {
               MoScriptStore.add(rcd);
           }
           else if (vType== '3') {
               ReportScriptStore.add(rcd);
           }
    });        
    });
    /************************************/
    var panelHttp=new Ext.Panel({
    layout:'form',
    labelWidth:80,
    items :[/*{
                xtype:'textfield',
                fieldLabel: '下行WEB地址',
                width:323,
                name: 'PortUrl'
            },{
                xtype:'textfield',
                fieldLabel: '上行WEB地址',
                width:323,
                name: 'MoUrl'
            },{
                xtype:'textfield',
                fieldLabel: '回执WEB地址',
                width:323,
                name: 'ReportUrl'
            },*/{
        layout : 'column',
        items : [ 
            {
                columnWidth : 0.5,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '下行WEB地址',
                width:110,
                name: 'PortUrl'
            },{
                xtype:'textfield',
                fieldLabel: '上行WEB地址',
                width:110,
                name: 'MoUrl'
            },{
                xtype:'textfield',
                fieldLabel: '回执WEB地址',
                width:110,
                name: 'ReportUrl'
            }, {
                xtype:'textfield',
                fieldLabel: '登录名参数',
                width:100,
                name: 'UseridParamString'
            },{
                xtype:'textfield',
                fieldLabel: '密码参数',
                width:100,
                name: 'PwdParamString'
            },{
                xtype:'textfield',
                fieldLabel: '手机号码参数',
                width:100,
                name: 'MobileParamString'
            },{
                xtype:'textfield',
                fieldLabel: '信息内容参数',
                width:100,
                name: 'MsgParamString'
            },{
                xtype:'textfield',
                fieldLabel: '通道号参数',
                width:100,
                name: 'GateNoParamString'
            },{
                xtype:'textfield',
                fieldLabel: '超时[毫秒]',
                width:100,
                name: 'TimeOut'
            }]   
            },{   
                columnWidth : 0.5,   
                layout : 'form',   
                items : [{
                name:"MtScriptID",
				xtype:"combo",
				fieldLabel:"下行映射脚本",
				width:100,
			    editable: false,
			    typeAhead: true,
				hiddenName:"MtScriptID",
				mode:"local",
				displayField:"MapName",
				valueField:"MapID",
				triggerAction:"all",				
				store:MtScriptStore
            },{
                name:"MoScriptID",
				xtype:"combo",
				fieldLabel:"上行映射脚本",
				width:100,
			    editable: false,
			    typeAhead: true,
				hiddenName:"MoScriptID",
				mode:"local",
				displayField:"MapName",
				valueField:"MapID",
				triggerAction:"all",				
				store:MoScriptStore
            },{
                name:"ReportScriptID",
				xtype:"combo",
				fieldLabel:"回执映射脚本",
				width:100,
			    editable: false,
			    typeAhead: true,
				hiddenName:"ReportScriptID",
				mode:"local",
				displayField:"MapName",
				valueField:"MapID",
				triggerAction:"all",				
				store:ReportScriptStore
            },{
                xtype:'textfield',
                fieldLabel: '登录账号',
                width:100,
                name: 'UserID'
            },{
                xtype:'textfield',
                fieldLabel: '登录密码',
                width:100,
                name: 'UserPwd'
            },{
                xtype:'textfield',
                fieldLabel: '号码个数参数',
                width:100,
                name: 'PhoneNumParamString'
            },{
                xtype:'textfield',
                fieldLabel: '子端口参数',
                width:100,
                name: 'SubportParamString'
            },{
                name:"SendType",
				xtype:"combo",
				fieldLabel:"发送模式",
				width:100,
			    editable: false,
			    typeAhead: true,
				hiddenName:"SendType",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"1",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["POST方式","1"],["GET方式","2"]]
                       })
            },{
                name:"EncodeType",
				xtype:"combo",
				fieldLabel:"编码类型",
				width:100,
			    editable: false,
			    typeAhead: true,
				hiddenName:"EncodeType",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["UTF-8","0"],["GB2312","1"]]
                       })
            }] 
            }]   
    }]   
});
var fieldsetHttp = new Ext.form.FieldSet({
    title : 'Http通道设置',   
    autoHeight : true,
    defaultType : 'textfield',
    items :[panelHttp]   
});
    var SpgateEditPanel = new Ext.FormPanel({   
    id:'SpgateEditPanel',
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
            columnWidth:.5,
            layout: 'form',   
            border:false,   
            items: [{
                xtype:'textfield',
                fieldLabel: '通道号',
                readOnly:varType=='1'?true:false,
                name: 'GateNumber'
            },{
                xtype:'textfield',
                fieldLabel: '信息最大长度',
                name: 'MsgLen'
            },{
                xtype:'textfield',
                fieldLabel: '移动计费字数',
                name: 'MobileLen'
            },{
                xtype:'textfield',
                fieldLabel: '电信计费字数',
                name: 'TeleComLen'
            },{
                name:"BusinessID",
				xtype:"combo",
				fieldLabel:"通道类型",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"BusinessID",
				mode:"local",
				displayField:"BusinessName",
				valueField:"BusinessID",
				triggerAction:"all",				
				store:BusinessStore,
				emptyText :'请选择类型'
            },{                
                name:"IsLong",
				xtype:"combo",
				fieldLabel:"长短信",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"IsLong",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"1",
				store:SelectStore
            },{                
                name:"IsSurportMo",
				xtype:"combo",
				fieldLabel:"上行",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"IsSurportMo",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:SelectStore
            }]
        },{
            columnWidth:.5,
            layout: 'form',
            border:false,
            items: [{
                xtype:'textfield',
                fieldLabel: '通道名称',
                name: 'GateName'
            },{
                xtype:'textfield',
                fieldLabel: '最大号码数',
                name: 'MaxPhoneNum'
            },{
                xtype:'textfield',
                fieldLabel: '联通计费字数',
                name: 'UnicomLen'
            },{
                xtype:'textfield',
                fieldLabel: '灵通计费字数',
                name: 'ChinaPhoneLen'
            },{
                name:"GateType",
				xtype:"combo",
				fieldLabel:"支持号段",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"GateType",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"1",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["全网","1"],["三网","2"],["移动","3"],["联通","4"],["电信","5"],["小灵通","6"]]
                       })
            },{                
                name:"IsSubport",
				xtype:"combo",
				fieldLabel:"子端口",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"IsSubport",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:SelectStore
            },{                
                name:"IsReport",
				xtype:"combo",
				fieldLabel:"状态报告",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"IsReport",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:SelectStore
            }]   
        }]   
    },{
        xtype:'textarea',
        fieldLabel: '备注',
        width:360,
        height:30,
        name: 'GateInfo'
    },fieldsetHttp,
    {//按钮
       xtype : 'panel',
       bodyStyle : 'margin-bottom:0px;margin-top:0px;margin-left:120px;',
       layout:"column", 
       items:[{
               text : '保存',
               xtype:'button',
               iconCls:'saveicon',
               width:60,
               handler : function() {
                      var values = Ext.getCmp('SpgateEditPanel').getForm().getValues();
 			          if (values.GateNumber == '') {
 			                ExtAlert('通道号不能为空');
 			                return;
 			          }
 			          else if (values.BusinessID == "") {
 			                ExtAlert('请选择通道类型');
 			                return;
 			          }
			          else{
			                if (values.TimeOut == ""){ values.TimeOut = "300000";}
			                if (values.MtScriptID == "") {values.MtScriptID = "0";}
                            if (values.MoScriptID == "") {values.MoScriptID = "0";}
                            if (values.ReportScriptID == "") {values.ReportScriptID = "0";}
			                doSaveManagerInfo(values);
			          }           
                }
            },
            {text : '重置',xtype:'button',iconCls:'refreshicon',width:60,handler : function() {
                    SpgateEditPanel.form.reset(); 
                    Ext.getCmp('SpgateEditPanel').getForm().setValues({TimeOut:30000});
                    if(varType=='1'){ loadData();}
            }},
            {text : '取消',xtype:'button',iconCls:'exiticon',width:60,handler : function() {SpgateEditWin.close();}}
            ]
    }       
]
});   
    var SpgateEditWin=new Ext.Window({            
			title:varType=='1'?("编辑通道信息-"+varSpgate):"添加通道",
			width:485,
			height:565,
			plain:true,
			layout:"form",
			iconCls:varType=='1'?"editicon":"addicon",			
			resizable:false,
			defaultType:"textfield",
			labelWidth:100,
			closeAction : 'close',
            closable:true,
            modal: 'true', 
			buttonAlign:"center",
			bodyStyle:"padding:0px 0 0 0px",
			items:[SpgateEditPanel]
	});
	SpgateEditWin.show();
	if(varType == '1')
	{
	    loadData();
	}
	else
	{
	    Ext.getCmp('SpgateEditPanel').getForm().setValues({TimeOut:30000});
	}
	function loadData() {
        SpgateEditWin.setTitle("编辑通道信息-"+varSpgate+'<font color="red"> 加载中....</font>');
        Ext.Ajax.request({
           url:"./Data/SmsSpgate/SpgateOprate.aspx",
            method:"POST",
            params:{
                spgate:varSpgate,
                type:'3'
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    if (responseArray.data[0].MtScriptID == "0") {responseArray.data[0].MtScriptID = "";}
                    if (responseArray.data[0].MoScriptID == "0") {responseArray.data[0].MoScriptID = "";}
                    if (responseArray.data[0].ReportScriptID == "0") {responseArray.data[0].ReportScriptID = "";}
                    Ext.getCmp('SpgateEditPanel').getForm().setValues(responseArray.data[0]);
                }else{                
                    ExtAlert(responseArray.errorInfo);
                }
               SpgateEditWin.setTitle("编辑通道信息-"+varSpgate);
            },
            failure:function(response,option)
            {
                ExtAlert("网络错误");
                SpgateEditWin.setTitle("编辑通道信息-"+varSpgate);
            }
        }); 
    }
    function doSaveManagerInfo(values)
    {
        var objMask = MaskShow(SpgateEditPanel,"正在提交,请稍后...");      
                           Ext.Ajax.request({
                           url:"./Data/SmsSpgate/SpgateOprate.aspx",
                            method:"POST",
                            params:{
                                jdata:Ext.encode(values),
                                type:varType
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    SpgateEditWin.close();
                                    varSpgateStore.reload();
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
