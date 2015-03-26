var SmsHistoryInfo = function(node)
{
    var varHisStore;
    
	var fields = ["UserID","ParentID","PhoneNum","FeeNum","SendResult","SpgateNo","PhoneList","MsgContent","SendTime","StatusString"];
	varHisStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsHistory/HistoryInfo.aspx",
                method:"POST"
           }),
           reader:new Ext.data.JsonReader(
   		   {
				fields:fields,
                root:"data",
                //id:"UserID",           
                totalProperty:"totalCount"
           })
	});
	//加载
	 varHisStore.on('beforeload', function() {
        Ext.apply(this.baseParams,{type:"4"});
        
    });
    
    var varUserID = new Ext.form.TextField({
        id: 'varUserID',
        width: 60
    });
    var varParentID = new Ext.form.TextField({
        id: 'varParentID',
        width: 60
    });
    var varSpgateNo = new Ext.form.NumberField({
        id: 'varSpgateNo',
        style: 'text-align:left;',
        width: 100
    });
    var varSendStatus = new Ext.form.ComboBox({   
                id : 'varSendStatus',
                xtype:"combo",
                fieldLabel: '',   
                hiddenName:'',
                store: new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["全部","-1"],["成功","0"],["失败","-2"]]
                       }),
                valueField: 'value',   
                displayField:'name', 
                editable:false,
                allowBlank : false, 
                valueNotFoundText : 0,
                value:"-1",  
                mode: 'local',   
                triggerAction: 'all',
                width:70
            });
            
    var varPhone = new Ext.form.TextField({
        id: 'varPhone',
        width: 60
    });
    var varContent = new Ext.form.TextField({
        id: 'varContent',
        width: 60
    });
    var varDate = new Ext.form.DateField({
        id: 'varDate',
        width: 80,
        fieldLabel: '',
        enableKeyEvents: true,
        emptyText: '',
        format: 'Y-m-d'
    });
	//--------------------------------------------------列头
	var cm = new Ext.grid.ColumnModel([
		{
		header:"用户账号",
		width:70,
		dataIndex:"UserID"
	},{
		header:"集团账号",
		width:70,
		dataIndex:"ParentID"
	},{
		header:"通道号",
		width:80,
		dataIndex:"SpgateNo"
	},{
		header:"发送状态",
		width:55,
		dataIndex:"SendResult",
		renderer:function(value){
		    if (value == "0") {
		        return "<font color='green'>成功</font>";
		    }
		    else{
		        return "<font color='red'>失败</font>";
		    }
		}
	},{
		header:"号码个数",
		width:55,
		dataIndex:"PhoneNum"
	},{
		header:"计费条数",
		width:55,
		dataIndex:"FeeNum"
	},{
		header:"发送时间",
		width:120,
		dataIndex:"SendTime"
	},{
		header:"手机号码",
		width:100,
		dataIndex:"PhoneList"
	},{
		header:"信息内容",
		width:150,
		dataIndex:"MsgContent"
	},{
		header:"原始返回值",
		width:100,
		dataIndex:"StatusString"
	}]);
	
	var HistoryGrid = new Ext.grid.GridPanel({
	    id:"HistoryGrid",
		store:varHisStore,
		cm:cm,
		loadMask:true,
		autoScroll:true,
		height:400,
		enableHdMenu:false,
		border:false,
		bbar:new Ext.PagingToolbar({
			store:varHisStore,
			pageSize:pageSize,
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
		tbar:[new Ext.Toolbar.TextItem('用户账号: '), varUserID, "", "-", "",
		new Ext.Toolbar.TextItem('集团账号: '), varParentID, "", "-", "",
		new Ext.Toolbar.TextItem('通道号: '), varSpgateNo,"", "-", "",
		new Ext.Toolbar.TextItem('发送状态: '), varSendStatus, "", "-", "",
		new Ext.Toolbar.TextItem('手机号: '), varPhone, "", "-", "",
		new Ext.Toolbar.TextItem('内容: '), varContent, "", "-", "",
		new Ext.Toolbar.TextItem('发送日期: '), varDate, "", "-", "",
		{
			text:"查询",
			iconCls:"queryicon",
			handler:function(){varHisStore.reload({params:{start:0,limit:pageSize}});}
		}],listeners:{
            'contextmenu':function(e)
            {
                e.stopEvent();
            },
            "rowmousedown":function(grid,rowIndex,e)
            {
                e.stopEvent();
            }
		}
	});
	TabShow(node,HistoryGrid);
}
