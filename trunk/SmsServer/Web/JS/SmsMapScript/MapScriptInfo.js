var varMapScriptStore;
MapScriptInfo = function(node)
{
    if (IsTabExist(node.id)) {
        return;
    }
    	
	var fields = ["MapID","MapName","ScriptType","ScriptFlag","MapInfo"];
	varMapScriptStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsMapScript/MapScriptInfo.aspx",
                method:"POST"
           }),
           reader:new Ext.data.JsonReader(
   		   {
				fields:fields,
                root:"data",
                id:"MapID",           
                totalProperty:"totalCount"
           })
	});
	//加载 
	 varMapScriptStore.on('beforeload', function() {
        var varMapName = MapName.getValue();
        var varType = ScriptType.getValue();
        Ext.apply(this.baseParams,{type:varType});
        Ext.apply(this.baseParams,{mapname:varMapName});
    });
	//--------------------------------------------------�列选择模式
	var sm = new Ext.grid.CheckboxSelectionModel({
		dataIndex:"MapID"
	});
	//--------------------------------------------------列头
	var cm = new Ext.grid.ColumnModel([
		sm,{
		header:"映射编号",
		width:70,
		dataIndex:"MapID"
	},{
		header:"映射名称",
		width:70,
		dataIndex:"MapName"
	},{
		header:"映射类型",
		tooltip:"映射类型",
		width:70,
		dataIndex:"ScriptType",
        sortable:false,
        renderer:function(value)
        {
            switch(value) {
            case "1":
        	     return "<font color='red'>下行</font>";
            case "2":
                 return "<font color='green'>上行</font>";
            case "3":
                 return "<font color='blue'>状态报告</font>";
            default: return "";
            }
        }
	},{
		header:"脚本分类",
		width:70,
		dataIndex:"ScriptFlag",
        renderer:function(value)
        {
            if (value==0) {
                return "规则定义";
            }
            else{
                return "个性化设置";
            }
        }
	},{
		header:"配置规则",
		width:70,
		dataIndex:"MapID",
        renderer:function(value, metaData, record, rowIndex, colIndex, store)
        {
            if (record.data.ScriptFlag=='0') {
                return "<div align=center><a href=\"javascript:MapScriptEasyEdit('"+value+"');\"><img src='./Images/icons/edit.png' /></a></div>";
            }
            else{
                return "";
            }
        }
	},{
		header:"备注",
		width:200,
		dataIndex:"MapInfo"
	}]);
	
   //----右键菜单
    var RightClickFunc = new Ext.menu.Menu
        ({
                items:
                [{
                    text: '添加映射',
                    iconCls:'addicon',
                    handler:AddMapScriptInfo
                },{
                    text:'编辑映射',
                    iconCls:'editicon',
                    handler:EditMapScriptInfoFn
                },{
                    text:'删除映射',
                    iconCls:'deleteicon',
                    handler:DelMapScriptInfoFn
                }]
         });
     var MapName = new Ext.form.TextField({
        id:'MapName',
		width : 70,
		fieldLabel:'',
		enableKeyEvents : true,
		emptyText : ''
	});
   var ScriptType = new Ext.form.ComboBox({   
                id : 'ScriptType_Combo',
                xtype:"combo",
                fieldLabel: '',   
                hiddenName:'',
                store: new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["全部","-1"],["下行","1"],["上行","2"],["状态报告","3"]]
                       }),
                valueField: 'value',
                displayField:'name',
                editable:false,
                allowBlank : false,
                valueNotFoundText : 0,
                value:"-1",
                mode: 'local',
                triggerAction: 'all',   
                selectOnFocus:true,   
                width:70
            });
    //Grid
	var MapScriptInfoGrid = new Ext.grid.GridPanel({
	    id:"MapScriptInfoGrid",
		store:varMapScriptStore,
		sm:sm,
		cm:cm,
		loadMask:true,
		autoScroll:true,
		enableHdMenu:false,
		border:false,
		//分页
		bbar:new Ext.PagingToolbar({
			store:varMapScriptStore,
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
			text:"添加映射",
			tooltip:"添加映射",
			iconCls:"addicon",
			handler:AddMapScriptInfo
		},"","-","",{
			text:"编辑映射",
			tooltip:"编辑映射",
			iconCls:"editicon",
			handler:EditMapScriptInfoFn
		},"","-","",{
			text:"删除映射",
			tooltip:"删除映射",
			iconCls:"deleteicon",
			handler:DelMapScriptInfoFn
		},"","-","",
		new Ext.Toolbar.TextItem('映射名称: '),MapName,"","-","",
		new Ext.Toolbar.TextItem('映射类型: '),ScriptType,"","-","",
		{
			text:" 查询 ",
			tooltip:" 查询 ",
			iconCls:"queryicon",
			handler:function(){varMapScriptStore.reload({params:{start:0,limit:pageSize}});}
		}],listeners:{
            'contextmenu':function(e)
            {
                e.stopEvent();
            },
            "rowmousedown":function(grid,rowIndex,e)
            {
                e.stopEvent();
                grid.selModel.selectRow(rowIndex,false);
            },
            "rowcontextmenu":function(grid,rowIndex,e)
            {
                e.stopEvent();
                RightClickFunc.showAt(e.getXY());
            }
		}
	});	
	//传入icon样式
	TabShow(node,MapScriptInfoGrid);
	varMapScriptStore.reload({params:{start:0,limit:pageSize}});	 
}
//添加账号
function AddMapScriptInfo()
{
    MapScriptEdit('','0');
}
//编辑账号
function EditMapScriptInfoFn()
{
        var row=Ext.getCmp("MapScriptInfoGrid").getSelectionModel().getSelections();
        if(row.length==0)
        {
            ExtAlert("您没有选中任何行");
        }
        else if(row.length>1){  
            ExtAlert("对不起只能选择一行");
        }else if(row.length==1)
        {
           MapScriptEdit(row[0].data.MapID,'1');
        }    
}
function DelMapScriptInfoFn()
{
    var objGrid = Ext.getCmp("MapScriptInfoGrid");
    var row=objGrid.getSelectionModel().getSelections();
    if(row.length==0)
    {
        ExtAlert("请至少选择一行");
        return;
    }
    else{  
            Ext.Msg.confirm("提示!","您确定要删除选中通道吗?",function(btn){ 
            if(btn=="yes")
            {
               //�执行删除操作
               var varMapIDList = '';
               for(var i=0;i<row.length;i++)
               {
                   varMapIDList += row[i].data.MapID+',';
               }
               //提交
               var objMask = MaskShow(objGrid,"正在删除,请稍后...");  
               Ext.Ajax.request({
                    url:"./Data/SmsMapScript/MapScriptOprate.aspx",
                    method:"POST",
                    params:{
                        type:'2',//�删除
                        mapid:varMapIDList
                    },
                    success:function(response,option)
                    {           
                        var responseArray = Ext.util.JSON.decode(response.responseText);                                    
                        if(responseArray.success == true){
                            MaskHide(objMask);
                            Ext.MessageBox.hide();
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
        })
    }
}