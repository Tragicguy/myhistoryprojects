var varUserStore;
SmsUserInfo = function(node)
{
    if (IsTabExist(node.id)) {
        return;
    }
    	
	var fields = ["UserID","ParentID","UserName","Phone","CompanyName","SendedNum","Balance","SignStr","Status","CreatTime","ExtraInfo"];
	varUserStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsUser/SmsUserInfo.aspx",
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
	 varUserStore.on('beforeload', function() {
        var varUserID = UserID.getValue();
        var varParentID = ParentID.getValue();
        var varStatus = SmsUserStatus.getValue();
        Ext.apply(this.baseParams,{userid:varUserID});
        Ext.apply(this.baseParams,{parentid:varParentID});
        Ext.apply(this.baseParams,{status:varStatus});
    });
	//--------------------------------------------------列选择模式
	var sm = new Ext.grid.CheckboxSelectionModel({
		dataIndex:"UserID"
	});
	//--------------------------------------------------列头
	var cm = new Ext.grid.ColumnModel([
		sm,{
		header:"账户名",
		width:80,
		dataIndex:"UserID"
	},{
		header:"集团账户",
		width:80,
		dataIndex:"ParentID"
	},{
		header:"姓名",
		tooltip:"姓名",
		width:70,
		dataIndex:"UserName"
	},{
		header:"电话",
		width:90,
		dataIndex:"Phone"
	},{
		header:"公司名",
		width:150,
		dataIndex:"CompanyName"
	},{
		header:"已发数",
		width:70,
		dataIndex:"SendedNum",
		sortable:true
	},{
		header:"余额",
		width:70,
		dataIndex:"Balance",
		sortable:true
	},{
		header:"签名",
		width:70,
		dataIndex:"SignStr"
	},{
		header:"状态",
		width:50,
		dataIndex:"Status",
        renderer:function(value)
        {
            if (value==0) {
                return "<font color='green'>正常</font>";
            }
            else{
                return "<font color='red'>禁用</font>";
            }
        }
	},{
		header:"创建时间",
		width:115,
		dataIndex:"CreatTime"
	},{
		header:"备注",
		width:140,
		dataIndex:"ExtraInfo"
	}]);
	
   //----右键菜单
    var RightClickFunc = new Ext.menu.Menu
        ({
                items:
                [{
                    text: '添加账户',
                    iconCls:'addicon',
                    handler:AddSmsUserFn
                },{
                    text:'编辑账户',
                    iconCls:'editicon',
                    handler:EditUserInfoFn
                },{
                    text:'删除账户',
                    iconCls:'deleteicon',
                    handler:DelUserInfoFn
                }]
         });
     var UserID = new Ext.form.TextField({
        id:'UserID',
		width : 70,
		fieldLabel:'',
		enableKeyEvents : true
	});
	var ParentID = new Ext.form.TextField({
        id:'ParentID',
		width : 70,
		fieldLabel:'',
		enableKeyEvents : true
	});
   var SmsUserStatus = new Ext.form.ComboBox({   
                id : 'SmsUserStatus_Combo',
                xtype:"combo",
                fieldLabel: '',   
                hiddenName:'',
                store: new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["全部","-1"],["正常","0"],["禁用","1"]]
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
	var UserInfoGrid = new Ext.grid.GridPanel({
	    id:"UserInfoGrid",
		store:varUserStore,
		sm:sm,
		cm:cm,
		loadMask:true,
		enableHdMenu:false,
		autoScroll:true,
		border:false,
		//分页
		bbar:new Ext.PagingToolbar({
			store:varUserStore,
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
			text:"添加账户",
			tooltip:"添加账户",
			iconCls:"addicon",
			handler:AddSmsUserFn
		},"","-","",{
			text:"编辑账户",
			tooltip:"编辑账户",
			iconCls:"editicon",
			handler:EditUserInfoFn
		},"","-","",{
			text:"删除账户",
			tooltip:"删除账户",
			iconCls:"deleteicon",
			handler:DelUserInfoFn
		},"","-","",
		new Ext.Toolbar.TextItem('用户账户: '),UserID,"","-","",
		new Ext.Toolbar.TextItem('集团账户: '),ParentID,"","-","",
		new Ext.Toolbar.TextItem('状态: '),SmsUserStatus,"","-","",			
		{
			text:" 查询 ",
			tooltip:" 查询 ",
			iconCls:"queryicon",
			handler:function(){varUserStore.reload({params:{start:0,limit:pageSize}});}
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
	TabShow(node,UserInfoGrid);
	varUserStore.reload({params:{start:0,limit:pageSize}});	 
}
//添加账号
function AddSmsUserFn()
{
   SmsUserEdit('','0');
}
//编辑账号
function EditUserInfoFn()
{
        var row=Ext.getCmp("UserInfoGrid").getSelectionModel().getSelections();
        if(row.length==0)
        {
            ExtAlert("您没有选中任何行!");
        }
        else if(row.length>1){  
            ExtAlert("对不起只能选择一个!");
        }else if(row.length==1)
        {
           SmsUserEdit(row[0].data.UserID,'1');
        }    
}
function DelUserInfoFn()
{
   var row=Ext.getCmp("UserInfoGrid").getSelectionModel().getSelections();
    if(row.length==0)
    {
        ExtAlert("请至少选择一项!");
    }
    else{  
            Ext.Msg.confirm("提示!","您确定要删除选中通道吗?",function(btn){ 
            if(btn=="yes")
            {
               //执行删除操作
               var varUserList = '';
               for(var i=0;i<row.length;i++)
               {
                   varUserList += row[i].data.UserID+',';
               }
               //提交
               ExtShow('正在删除,请稍后...');
               Ext.Ajax.request({
                            url:"./Data/SmsUser/SmsUserOprate.aspx",
                            method:"POST",
                            params:{
                                type:'2',//删除
                                userid:varUserList
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    Ext.MessageBox.hide();
                                    varUserStore.reload();
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
