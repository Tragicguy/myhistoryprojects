var varSpgateStore;
SmsSpgateInfo = function(node)
{
    if (IsTabExist(node.id)) {
        return;
    }
    	
	var fields = ["GateNumber","GateName","BusinessID","GateType","IsLong","IsSubport","IsSurportMo","MsgLen","GateInfo"];
	varSpgateStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsSpgate/SmsSpgateInfo.aspx",
                method:"POST"
           }),
           reader:new Ext.data.JsonReader(
   		   {
				fields:fields,
                root:"data",
                id:"GateNumber",           
                totalProperty:"totalCount"
           })
	});
	//加载时参数
	 //varSpgateStore.load({params:{start:0,limit:pageSize}})	 
	 varSpgateStore.on('beforeload', function() {
        var varSpgateNumber = SpgateNumber.getValue();
        var varBusiness = SpgateBusiness.getValue();
        var varType = SpgateType.getValue();
        Ext.apply(this.baseParams,{type:varType});
        Ext.apply(this.baseParams,{business:varBusiness});
        Ext.apply(this.baseParams,{spgate:varSpgateNumber});
    });
	//--------------------------------------------------列选择模式
	var sm = new Ext.grid.CheckboxSelectionModel({
		dataIndex:"GateNumber"
	});
	//--------------------------------------------------列头
	var cm = new Ext.grid.ColumnModel([
		sm,{
		header:"通道号",
		width:190,
		dataIndex:"GateNumber"
	},{
		header:"通道名",
		width:100,
		dataIndex:"GateName"
	},{
		header:"通道类型",
		width:100,
		dataIndex:"BusinessID",
        renderer:function(value)
        {
            if (value==0) {
                return "网关通道";
            }
            else{
                return "Http通道";
            }
        }
	},{
		header:"支持号段",
		width:100,
		dataIndex:"GateType",
        renderer:function(value)
        {
            switch(parseInt(value))
            {
                case 1:return "<font color='red'>全网</font>";
                case 2: return "<font color='green'>三网</font>";
                case 3:return "移动";
                case 4: return "联通";
                case 5:return "电信";
                case 6: return "小灵通";
            }
        }
	},{
		header:"长短信",
		width:80,
		dataIndex:"IsLong",
        renderer:function(value)
        {            
            if (value=='1') {
                return "支持";
            }
            else{
                return "不支持";
            }
        } 
	},{
		header:"子端口",
		width:70,
		dataIndex:"IsSubport",
        renderer:function(value)
        {
             if (value=='1') {
                return "支持";
            }
            else{
                return "不支持";
            }
        }
	},{
		header:"上行",
		width:70,
		dataIndex:"IsSurportMo",
        renderer:function(value)
        {
            if (value=='1') {
                return "支持";
            }
            else{
                return "不支持";
            }
        }
	},{
		header:"长度",
		width:60,
		dataIndex:"MsgLen"
	},{
		header:"备注",
		width:200,
		dataIndex:"GateInfo"
	}]);
	
   //----右键菜单
    var RightClickFunc = new Ext.menu.Menu
        ({
                items:
                [{
                    text: '添加通道',
                    iconCls:'addicon',
                    handler:AddSpgate
                },{
                    text:'编辑通道',
                    iconCls:'editicon',
                    handler:EditSpgateInfoFn
                },{
                    text:'删除通道',
                    iconCls:'deleteicon',
                    handler:DelSpgateInfoFn
                }]
         });
     var SpgateNumber = new Ext.form.NumberField({
        id:'SpgateNumber',
        style: 'text-align:left;',
		width : 100,
		emptyText : ''
	});
   var SpgateBusiness = new Ext.form.ComboBox({   
                id : 'SpgateBusiness_Combo',
                xtype:"combo",
                fieldLabel: '',   
                hiddenName:'',
                store: new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["全部","-1"],["Http通道","1"],["网关通道","0"]]
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
	var SpgateType = new Ext.form.ComboBox({   
                id : 'SpgateType_Combo',
                xtype:"combo",
                fieldLabel: '',   
                hiddenName:'',
                store: new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["全部","-1"],["全网","1"],["三网","2"],["移动","3"],["联通","4"],["电信","5"],["小灵通","6"]]
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
	var SpgateInfoGrid = new Ext.grid.GridPanel({
	    id:"SpgateInfoGrid",
		store:varSpgateStore,
		sm:sm,
		cm:cm,
		loadMask:true,
		enableHdMenu:false,
		autoScroll:true,
		border:false,
		//分页
		bbar:new Ext.PagingToolbar({
			store:varSpgateStore,
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
		//new Ext.Toolbar.Fill(),
		{
			text:"添加通道",
			tooltip:"添加通道",
			iconCls:"addicon",
			handler:AddSpgate
		},"","-","",{
			text:"编辑通道",
			tooltip:"编辑通道",
			iconCls:"editicon",
			handler:EditSpgateInfoFn
		},"","-","",{
			text:"删除通道",
			tooltip:"删除通道",
			iconCls:"deleteicon",
			handler:DelSpgateInfoFn
		},"","-","",
		new Ext.Toolbar.TextItem('通道类型: '),SpgateBusiness,"","-","",
		new Ext.Toolbar.TextItem('支持号段: '),SpgateType,"","-","",
		new Ext.Toolbar.TextItem('通道号: '),SpgateNumber,"","-","",			
		{
			text:" 查询 ",
			tooltip:" 查询 ",
			iconCls:"queryicon",
			handler:function(){varSpgateStore.reload({params:{start:0,limit:pageSize}});}
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
	TabShow(node,SpgateInfoGrid);
	 varSpgateStore.reload({params:{start:0,limit:pageSize}});	 
}
//添加账号
function AddSpgate()
{
    SmsSpgateEdit('','0');
}
//编辑账号
function EditSpgateInfoFn()
{
        var row=Ext.getCmp("SpgateInfoGrid").getSelectionModel().getSelections();
        if(row.length==0)
        {
            ExtAlert("您没有选中任何行!");
        }
        else if(row.length>1){  
            ExtAlert("对不起只能选择一个!");
        }else if(row.length==1)
        {
           SmsSpgateEdit(row[0].data.GateNumber,'1');
        }    
}
function DelSpgateInfoFn()
{
   var row=Ext.getCmp("SpgateInfoGrid").getSelectionModel().getSelections();
    if(row.length==0)
    {
        ExtAlert("请至少选择一项!");
    }
    else{  
            Ext.Msg.confirm("提示!","您确定要删除选中通道吗?",function(btn){ 
            if(btn=="yes")
            {
               //执行删除操作
               var varSpgateList = '';
               for(var i=0;i<row.length;i++)
               {
                   varSpgateList += row[i].data.GateNumber+',';
               }
               //提交
               ExtShow('正在删除,请稍后...');
               Ext.Ajax.request({
                            url:"./Data/SmsSpgate/SpgateOprate.aspx",
                            method:"POST",
                            params:{
                                type:'2',//删除
                                spgateno:varSpgateList
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.util.JSON.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    Ext.MessageBox.hide();
                                    varSpgateStore.reload();
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
