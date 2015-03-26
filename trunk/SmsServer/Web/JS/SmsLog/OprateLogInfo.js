OprateLogInfo = function(node) {
    if (IsTabExist(node.id)) {
        return;
    }
    var fields = ["log_id", "log_type", "log_detail", "log_time", "operatorID","op_module"];
    var varOprateLogStore = new Ext.data.Store({
        proxy: new Ext.data.HttpProxy(
           {
               url: "Data/SmsLog/OprateLogInfo.aspx",
               method: "POST"
           }),
        reader: new Ext.data.JsonReader(
   		   {
   		       fields: fields,
   		       root: "data",
   		       id: "log_id",
   		       totalProperty: "totalCount"
   		   })
    });
    //加载时参数
    varOprateLogStore.on('beforeload', function() {
        var varOperateID = OperateID.getValue();
        var varType = OperateType.getValue();        
        Ext.apply(this.baseParams, { opratorID: varOperateID });
        Ext.apply(this.baseParams, { type: varType });
        Ext.apply(this.baseParams, { optdate: Ext.util.Format.date(Ext.getCmp("OptDate").getValue(), 'Y-m-d') });
    });
    //--------------------------------------------------列头
    var cm = new Ext.grid.ColumnModel([
         {
            header: "日志类型",
            width: 100,
            dataIndex: "log_type",
            renderer: function(value) {
                switch (value) {
                    case '1':
                        return "<font color='green'>操作日志</font>";
                    case '0':
                        return "<font color='red'>异常日志</font>";
                    case '2':
                        return "事务日志";
                    default:
                        return "未知类型";
                }
            }
        }, {
            header: "操作员",
            width: 100,
            dataIndex: "operatorID"
        }, {
            header: "记录时间",
            width: 110,
            dataIndex: "log_time"
        }, {
            header: "操作模块",
            width: 110,
            dataIndex: "op_module"
        }, {
            header: "日志详情",
            tooltip: "姓名",
            width: 500,
            dataIndex: "log_detail"
}]);
    var OperateID = new Ext.form.TextField({
        id: 'OperateID',
        width: 70,
        fieldLabel: '',
        enableKeyEvents: true
    });
    var OperateType = new Ext.form.ComboBox({
        id: 'OperateType_Combo',
        xtype: "combo",
        fieldLabel: '',
        hiddenName: '',
        store: new Ext.data.SimpleStore({
            fields: ["name", "value"],
            data: [["全部", "-1"], ["操作日志", "1"],["事务日志", "2"],["异常日志", "0"]]
        }),
        valueField: 'value',
        displayField: 'name',
        editable: false,
        allowBlank: false,
        valueNotFoundText: 0,
        value: "-1",
        mode: 'local',
        triggerAction: 'all',
        selectOnFocus: true,
        width: 70
    });

    //Grid
    var OprateInfoGrid = new Ext.grid.GridPanel({
        id: "OprateInfoGrid",
        store: varOprateLogStore,
        cm: cm,
        height: 300,
        enableHdMenu:false,
        loadMask: true,
        autoScroll: true,
        renderto: 'divshow',
        border: false,
        //分页
        bbar: new Ext.PagingToolbar({
            store: varOprateLogStore,
            pageSize: pageSize,
            //显示右下角信息
            displayInfo: true,
            displayMsg: '当前记录 {0} -- {1} 条 共 {2} 条记录',
            emptyMsg: "当前无数据显示",
            firstText: '第一页',
            prevText: "上一页",
            nextText: "下一页",
            refreshText: "刷新",
            lastText: "最后页",
            firstText: "第一页",
            beforePageText: "当前页",
            afterPageText: "共{0}页"

        }),
        tbar: [ 
        new Ext.Toolbar.TextItem('操作员: '), OperateID, "", "-", "",
		new Ext.Toolbar.TextItem('日志类型: '), OperateType, "", "-", "",
		new Ext.Toolbar.TextItem('记录日期: '), { xtype: 'datefield', id: 'OptDate', format: "Y-m-d" }, "", "-", "",
		{
		    text: " 查询 ",
		    tooltip: " 查询 ",
		    iconCls: "queryicon",
		    handler: function() { varOprateLogStore.reload({ params: { start: 0, limit: pageSize} }); }
}], listeners: {
    'contextmenu': function(e) {
        e.stopEvent();
    },
    "rowmousedown": function(grid, rowIndex, e) {
        e.stopEvent();
        grid.selModel.selectRow(rowIndex, false);
    },
    "rowcontextmenu": function(grid, rowIndex, e) {
        e.stopEvent();
    }
}
    });
    //
    //传入icon样式
    TabShow(node, OprateInfoGrid);
    varOprateLogStore.reload({ params: { start: 0, limit: pageSize} });
}
