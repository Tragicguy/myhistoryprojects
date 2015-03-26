MapScriptEdit = function(varMapID,varType)
{
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
    
    var MapScriptEditPanel = new Ext.FormPanel({   
    id:'MapScriptEditPanel',
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
                fieldLabel: '映射编号',
                readOnly:varType=='1'?true:false,
                name: 'MapID'
            },{
                name:"ScriptType",
				xtype:"combo",
				fieldLabel:"脚本类型",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"ScriptType",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"1",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["下行","1"],["上行","2"],["状态报告","3"]]
                       })
            }]
        },{
            columnWidth:.5,
            layout: 'form',
            border:false,
            items: [{
                xtype:'textfield',
                fieldLabel: '映射名称',
                name: 'MapName'
            },{
                name:"ScriptFlag",
				xtype:"combo",
				fieldLabel:"脚本类型",
				width:90,
			    editable: false,
			    typeAhead: true,
				hiddenName:"ScriptFlag",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["规则定义","0"],["个性化设置","1"]]
                       })
            }]   
        }]   
    },{
                xtype:'textfield',
                fieldLabel: '上行匹配通道',
                width:360,
                name: 'MoSpgate'
            },{
                xtype:'textarea',
                fieldLabel: '备注',
                width:360,
                height:45,
                name: 'MapInfo'
            },{
                xtype:'textarea',
                fieldLabel: '脚本内容',
                width:360,
                height:285,
                name: 'ScriptCode'
            }         
],
    buttons:[{
       text : '保存',
       width:60,
       handler : function() {
              var values = Ext.getCmp('MapScriptEditPanel').getForm().getValues();
 			  if (values.MapID == '') {
 			        ExtAlert('映射编号不能为空');
 			        return;
 			  }
			  else{
			      doSaveManagerInfo(values);
			  }           
        }
    },{
       text : '重置',
       width:60,
       handler : function() {
              MapScriptEditPanel.form.reset();
              if(varType=='1'){
                    loadData();
              }
          }
    },{
       text : '克隆规则脚本',
       width:60,
       handler : function() {                
                if (varMapID == '') {return;}
                var objMask = MaskShow(MapScriptEditPanel,"正在获取脚本,请稍后...");
                   Ext.Ajax.request({
                   url:"./Data/SmsMapScript/MapScriptOprate.aspx",
                    method:"POST",
                    params:{
                        mapid:varMapID,
                        type:'3'
                    },
                    success:function(response,option)
                    {           
                        var responseArray = Ext.decode(response.responseText);                                    
                        if(responseArray.success == true){
                           if (responseArray.data[0]) {
                                Ext.getCmp('MapScriptEditPanel').getForm().setValues({ScriptCode:responseArray.data[0].EasyScriptCode});
                           }                           
                           MaskHide(objMask);    
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
    },{
       text : '取消',
       width:60,
       handler : function() {
              MapScriptEditWin.close();
              }
    }]
});   
    var MapScriptEditWin=new Ext.Window({            
			title:varType=='1'?("编辑映射信息-"+varMapID):"添加映射",
			width:485,
			height:505,
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
			items:[MapScriptEditPanel]
	});
	MapScriptEditWin.show();
	if(varType == '1')
	{
	    loadData();
	}
	else
	{
	    Ext.getCmp('MapScriptEditPanel').getForm().setValues({TimeOut:20000});
	}
	function loadData() {
        MapScriptEditWin.setTitle("编辑映射信息-"+varMapID+'<font color="red"> 加载中....</font>');
        Ext.Ajax.request({
           url:"./Data/SmsMapScript/MapScriptOprate.aspx",
            method:"POST",
            params:{
                mapid:varMapID,
                type:'3'
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    Ext.getCmp('MapScriptEditPanel').getForm().setValues(responseArray.data[0]);                
                }else{                
                    ExtAlert(responseArray.errorInfo);
                }
               MapScriptEditWin.setTitle("编辑映射信息-"+varMapID);
            },
            failure:function(response,option)
            {
                ExtAlert("网络错误");
                MapScriptEditWin.setTitle("编辑映射信息-"+varMapID);
            }
        }); 
    }
    function doSaveManagerInfo(values)
    {
       var objMask = MaskShow(MapScriptEditPanel,"正在提交,请稍后...");
           Ext.Ajax.request({
           url:"./Data/SmsMapScript/MapScriptOprate.aspx",
            method:"POST",
            params:{
                jdata:Ext.encode(values),
                type:varType
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    MapScriptEditWin.close();
                    varMapScriptStore.reload();
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