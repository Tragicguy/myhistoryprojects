// JScript 文件
var ManagerInfoStore;
SmsManagerInfo=function(node){
    //此处先判断这个TAB是否已经存在
    if (IsTabExist(node.id)) {
        return;
    }
    	
	//指定列参数
	var fields = ["ManagerID","ManagerType","UserName","CompanyName","Phone","Address","CreatTime","ExtraInfo","Balance","Used","PortPower"];
	ManagerInfoStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsManager/SmsManagerInfo.aspx",
                method:"POST"
           }),
           reader:new Ext.data.JsonReader(
   		   {
				fields:fields,
                root:"data",
                id:"ManagerID",                 
                totalProperty:"totalCount"
           })
	});
	//加载时参数
	 //ManagerInfoStore.load({params:{start:0,limit:pageSize}});
	 
	 ManagerInfoStore.on('beforeload', function() { 
        var varUserID = ManagerInfo_UserID.getValue();
        var varType = ManagerInfo_UserType.getValue();
        Ext.apply(this.baseParams,{type:varType});
        Ext.apply(this.baseParams,{username:varUserID});
    });
	//--------------------------------------------------列选择模式
	var sm = new Ext.grid.CheckboxSelectionModel({
		dataIndex:"ManagerID"
	});
	//--------------------------------------------------列头
	var cm = new Ext.grid.ColumnModel([
		sm,{
		header:"账户名",
		width:80,
		dataIndex:"ManagerID"
	},{
		header:"类型",
		width:80,
		dataIndex:"ManagerType",
        sortable:true,
        renderer:function(value)
        {
            if (value==1) {
                return "<font color=blue>代理商</font>";
            }
            else{
                return "<font color=red>管理员</font>";
            }
        }
	},{
		header:"已使用",
		width:60,
		dataIndex:"Used"
	},{
		header:"余额",
		width:60,
		dataIndex:"Balance"
	},{
		header:"姓名",
		width:80,
		dataIndex:"UserName"
	},{
		header:"公司名",
		width:130,
		dataIndex:"CompanyName"
	},{
		header:"电话号码",
		width:100,
		dataIndex:"Phone"
	},{
		header:"地址",
		width:220,
		dataIndex:"Address"
	},{
		header:"创建时间",
		width:130,
		dataIndex:"CreatTime"
	},{
		header:"备注",
		width:250,
		dataIndex:"ExtraInfo"
	},{
		header:"通道分配",
		width:70,
		dataIndex:"PortPower",		
		renderer:function(value)
        {
            var varArray = value.split(',');
            if (varArray[0]==1) {
                return "<div align=center><a href=\"javascript:SmsManagerPortSet('"+varArray[1]+"');\"><img src='./Images/icons/setport.gif' /></a></div>";
            }
            else{
                return "";
            }
        }
	}]);
	
   //----右键菜单
    var ManagerInfoRightClick = new Ext.menu.Menu
        ({
                items:
                [{
                    text: '添加账户',
                    iconCls:'addicon',
                    handler:AddManager
                },{
                    text:'编辑账户',
                    iconCls:'editicon',
                    handler:EditManagerInfoFn
                },{
                    text:'权限管理',
                    iconCls:'powericon',
                    handler:AuthManagerInfoFn
                },{
                    text:'删除账户',
                    iconCls:'deleteicon',
                    handler:DelManagerInfoFn
                }]
         });
     var ManagerInfo_UserID = new Ext.form.TextField({
        id:'ManagerInfo_tool_userID',
		width : 70,
		fieldLabel : 'ManagerInfo_tool_userID',
		enableKeyEvents : true,
		emptyText : ''
	});
    // 创建数据集的结构
       var ManagerTypeStore = new Ext.data.SimpleStore( {
         fields : ["name","value"],
         data : [["全部","-1"],["管理员","0"],["代理商","1"]]
       });   
	var ManagerInfo_UserType = new Ext.form.ComboBox({   
                id : 'ManagerInfo_tool_userType',
                xtype:"combo",
                fieldLabel: '',   
                hiddenName:'',
                store: ManagerTypeStore,
                valueField: 'value',   
                displayField:'name', 
                editable:false,
                allowBlank : false, 
                valueNotFoundText : 0,
                value:"-1",  
                mode: 'local',   
                triggerAction: 'all',   
                emptyText:'请选择类型',
                selectOnFocus:true,   
                width:75
            });
    //----------------------------------------------------定义grid
	var ManagerInfogrid = new Ext.grid.GridPanel({
	    id:"ManagerInfogrid",
		store:ManagerInfoStore,
		sm:sm,
		cm:cm,
		loadMask:true,
		enableHdMenu:false,
		//超过长度带自动滚动条
		autoScroll:true,
		border:false,
		viewConfig:{
			columnsText:"显示/隐藏列",
			sortAscText:"正序排列",
			sortDescText:"倒序排列",
			forceFit:true
		},
		//分页
		bbar:new Ext.PagingToolbar({
			store:ManagerInfoStore,
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
			text:"添加新账户",
			tooltip:"添加新账户",
			iconCls:"addicon",
			handler:AddManager
		},"","-","",{
			text:"编辑账户",
			tooltip:"编辑当前账户",
			iconCls:"editicon",
			handler:EditManagerInfoFn
		},"","-","",{
			text:"权限管理",
			tooltip:"权限管理",
			iconCls:"powericon",
			handler:AuthManagerInfoFn
		},"","-","",{
			text:"删除账户",
			tooltip:"删除账户",
			iconCls:"deleteicon",
			handler:DelManagerInfoFn
		},"","-","",
		new Ext.Toolbar.TextItem('账户类型: '),ManagerInfo_UserType,"","-","",
		new Ext.Toolbar.TextItem('账户名: '),ManagerInfo_UserID,"","-","",			
		{
			text:" 查询 ",
			tooltip:" 查询 ",
			iconCls:"queryicon",
			handler:function(){ManagerInfoStore.reload({params:{start:0,limit:pageSize}});}
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
                ManagerInfoRightClick.showAt(e.getXY());
            }
		}
	});	
	TabShow(node,ManagerInfogrid);
	ManagerInfoStore.reload({params:{start:0,limit:pageSize}});
}
//添加账号
function AddManager()
{
    SmsManagerAdd();
}
//编辑账号
function EditManagerInfoFn()
{
        var row=Ext.getCmp("ManagerInfogrid").getSelectionModel().getSelections();
        if(row.length==0)
        {
            ExtAlert("您没有选中任何行!");
        }
        else if(row.length>1){  
            ExtAlert("对不起只能选择一个!");
        }else if(row.length==1)
        {
           SmsManagerEdit(row[0].data.ManagerID);
        } 
    
}
//编辑账号权限
function AuthManagerInfoFn()
{
        var row=Ext.getCmp("ManagerInfogrid").getSelectionModel().getSelections();
        if(row.length==0)
        {
            ExtAlert("您没有选中任何行!");
        }
        else if(row.length>1){  
            ExtAlert("对不起只能选择一个!");
        }else if(row.length==1)
        {
            SmsManagerAuth(row[0].data.ManagerID);
        } 
    
}
function DelManagerInfoFn()
{
   var row=Ext.getCmp("ManagerInfogrid").getSelectionModel().getSelections();
    if(row.length==0)
    {
        ExtAlert("请至少选择一项!");
    }
    else{  
            Ext.Msg.confirm("提示!","您确定要删除选中账户吗?",function(btn){
            if(btn=="yes")
            {
               //执行删除操作
               var varManagerList = '';
               for(var i=0;i<row.length;i++)
               {
                   varManagerList += row[i].data.ManagerID+',';
               }
               //提交
               ExtShow('正在删除,请稍后...');
               Ext.Ajax.request({
                            url:"./Data/SmsManager/ManagerOprate.aspx",
                            method:"POST",
                            params:{
                                type:'2',//删除
                                userid:varManagerList
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.util.JSON.decode(response.responseText);                                    
                                if(responseArray.success == true){
                                    Ext.MessageBox.hide();
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
        })
    }
}