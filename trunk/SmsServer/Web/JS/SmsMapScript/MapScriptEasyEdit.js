var varEasyScriptStore;
MapScriptEasyEdit = function(varMapID)
{
    
	var fields = ["id","MapID","MapType","SrcValue","MapRetValue","RetUseValue","RetString","CharNum","SplitChar"];
	varEasyScriptStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsMapScript/EasyScriptOprate.aspx",
                method:"POST"
           }),
           reader:new Ext.data.JsonReader(
   		   {
				fields:fields,
                root:"data",
                id:"id",           
                totalProperty:"totalCount"
           })
	});
	//加载时参
	 varEasyScriptStore.on('beforeload', function() {
        Ext.apply(this.baseParams,{type:"4"});
        Ext.apply(this.baseParams,{mapid:varMapID});        
    });
	//--------------------------------------------------�列选择模式
	var sm = new Ext.grid.CheckboxSelectionModel({
		dataIndex:"id"
	});
	//--------------------------------------------------列头
	var cm = new Ext.grid.ColumnModel([
		sm,{
		header:"ID",
		width:50,
		dataIndex:"id"
	},{
		header:"映射编号",
		width:60,
		dataIndex:"MapID"
	},{
		header:"匹配类别",
		width:70,
		dataIndex:"MapType"
	},{
		header:"比较值",
		width:70,
		dataIndex:"SrcValue"
	},{
		header:"映射返回值",
		width:70,
		dataIndex:"MapRetValue"
	},{
		header:"是否重发",
		width:60,
		tooltip:"配置规则",
		dataIndex:"RetUseValue",
        renderer:function(value)
        {
            if (value=='1') {
                return "是";
            }
            else{
                return "否";
            }
        }
	},{
		header:"截取字数",
		width:60,
		dataIndex:"CharNum"
	},{
		header:"分隔符",
		width:50,
		dataIndex:"SplitChar"
	},{
		header:"返回字符串",
		width:200,
		dataIndex:"RetString"
	}]);
	
	var EasyScriptGrid = new Ext.grid.GridPanel({
	    id:"EasyScriptGrid",
		store:varEasyScriptStore,
		sm:sm,
		cm:cm,
		loadMask:true,
		autoScroll:true,
		height:400,
		enableHdMenu:false,
		border:false/*,
		viewConfig:{
			columnsText:"显示/隐藏列",
			sortAscText:"正序排列",
			sortDescText:"倒序排列",
			forceFit:true
		}*/,
		//分页
		bbar:new Ext.PagingToolbar({
			store:varEasyScriptStore,
			pageSize:pageSize,
			//显示右下角信息
			displayInfo:true,
			displayMsg:'当前记录 {0} -- {1} 条 共 {2} 条记录',
		    emptyMsg:"当前无数据显示",
		    firstText:'第一页',
		    prevText:"上一页",
			nextText:"下一页",
			refreshText:"刷新",
			lastText:"最后页",
			firstText:"第一页",
			beforePageText:"当前页",
			afterPageText:"共{0}页"		
		}),
		tbar:[
		{
			text:"保存",
			iconCls:"saveicon",
			handler:SaveFunction
		},"","-","",{
			text:"重置",
			iconCls:"refreshicon",
			handler:function(){EasyScriptEditPanel.form.reset();}
		},"","-","",{
			text:"删除规则",
			iconCls:"deleteicon",
			handler:DelEasyScriptFn
		}],listeners:{
            'contextmenu':function(e)
            {
                e.stopEvent();
            },
            "rowmousedown":function(grid,rowIndex,e)
            {
                e.stopEvent();
                grid.selModel.selectRow(rowIndex, false);
                var row = grid.getSelectionModel().getSelections();
                EasyScriptEditPanel.getForm().loadRecord(row[0]);
            }
		}
	});	
    
    var EasyScriptEditPanel = new Ext.FormPanel({   
    id:'EasyScriptEditPanel',
    labelAlign:'right',
    layout:"column",   
    buttonAlign:'center',   
    bodyStyle:'padding:5px',   
    width: 780,
    frame:true,
    labelWidth:70,
    items: [{   
            columnWidth:.25,
            layout: 'form',   
            border:false,   
            items: [{
                xtype:'textfield',
                fieldLabel: 'id',
                width:70,
                readOnly:true,
                name: 'id'
            },{
                name:"RetUseValue",
				xtype:"combo",
				fieldLabel:"是否重发",
				width:70,
			    editable: false,
			    typeAhead: true,
				hiddenName:"RetUseValue",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"0",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["是","1"],["否","0"]]
                       })
            }]
        },{   
            columnWidth:.25,
            layout: 'form',   
            border:false,   
            items: [{
                name:"MapType",
				xtype:"combo",
				fieldLabel:"映射类型",
				width:100,
			    editable: false,
			    typeAhead: true,
				hiddenName:"MapType",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:"1",
				store:new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["否","0"],["是","1"]]
                       })
            },{
                xtype:'textfield',
                fieldLabel: '返回字符串',
                width:100,
                name: 'RetString'
            }]
        },{   
            columnWidth:.25,
            layout: 'form',   
            border:false,   
            items: [{
                xtype:'textfield',
                fieldLabel: '比较值',
                width:70,
                name: 'SrcValue'
            },{
                xtype:'textfield',
                fieldLabel: '截取长度',
                width:70,
                name: 'CharNum'
            }]
        },{   
            columnWidth:.25,
            layout: 'form',   
            border:false,   
            items: [{
                xtype:'textfield',
                fieldLabel: '返回值',
                width:70,
                name: 'MapRetValue'
            },{
                xtype:'textfield',
                fieldLabel: '分割符',
                width:70,
                name: 'SplitChar'
            }]
        }]
});   
    var EasyScriptEditWin=new Ext.Window({
			title:"编辑映射规则信息",
			width:785,
			height:505,
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
			items:[EasyScriptEditPanel,EasyScriptGrid]
	});
	EasyScriptEditWin.show();
	varEasyScriptStore.reload({params:{start:0,limit:pageSize}});
	
	
function DelEasyScriptFn()
{
    var objGrid = Ext.getCmp("EasyScriptGrid");
    var row=objGrid.getSelectionModel().getSelections();
    if(row.length==0)
    {
        ExtAlert("请至少选择一行");
        return;
    }
    else{  
            Ext.Msg.confirm("提示!","您确定要删除选中规则吗?",function(btn){ 
            if(btn=="yes")
            {
               //执行删除操作
               var varIDList = '';
               var varEditId = Ext.getCmp('EasyScriptEditPanel').getForm().getValues().id;
               var bExist = false;
               for(var i=0;i<row.length;i++)
               {
                   if (bExist == false && varEditId == row[i].data.id) {
                        bExist = true;
                   }
                   varIDList += row[i].data.id+',';
               }
               //提交
               Ext.Ajax.request({
                            url:"./Data/SmsMapScript/EasyScriptOprate.aspx",
                            method:"POST",
                            params:{
                                type:'2',//�删除
                                id:varIDList,
                                mapid:varMapID
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.util.JSON.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    if (bExist == true) {
                                        Ext.getCmp('EasyScriptEditPanel').getForm().setValues({id:""});
                                    }
                                    //Ext.MessageBox.hide();
                                    varEasyScriptStore.reload();
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
        })
    }
}
//添加
function SaveFunction()
{
    var values = Ext.getCmp('EasyScriptEditPanel').getForm().getValues();
	  if (values.SrcValue == '') {
	        ExtAlert('比较值不能为空');
	        return;
	  }
	  if (values.MapRetValue == '') {
	        ExtAlert('返回值不能为空');
	        return;
	  }
	  else if (values.id == '') {    //添加
	        values.id = -1;
	        doSaveManagerInfo(values,varMapID,'0',EasyScriptEditPanel);
	  } 			  
	  else{
	      doSaveManagerInfo(values,varMapID,'1',EasyScriptEditPanel);
	  }
    }
}
function doSaveManagerInfo(values,varMapID,varType,varObj)
{
        var objMask = MaskShow(varObj,"正在提交,请稍后...");      
                           Ext.Ajax.request({
                           url:"./Data/SmsMapScript/EasyScriptOprate.aspx",
                            method:"POST",
                            params:{
                                jdata:Ext.encode(values),
                                type:varType,
                                mapid:varMapID
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    MaskHide(objMask);
                                    varEasyScriptStore.reload();
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
    

