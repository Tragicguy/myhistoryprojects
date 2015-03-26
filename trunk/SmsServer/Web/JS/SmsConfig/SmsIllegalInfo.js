SmsIllegalInfo = function(node)
{
    var varIllegalStore;
    
	var fields = ["id","KeyWord"];
	varIllegalStore = new Ext.data.Store({		 
		 proxy:new Ext.data.HttpProxy(
           {
                url:"./DATA/SmsConfig/IllegalInfo.aspx",
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
	
	 varIllegalStore.on('beforeload', function() {
        Ext.apply(this.baseParams,{type:"4"});
        
    });
    
    var varIllegalName = new Ext.form.TextField({
        id: 'varIllegalName',
        width: 70,
        fieldLabel: '',
        enableKeyEvents: true
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
		header:"关键词",
		width:360,
		dataIndex:"KeyWord"
	}]);
	
	var IllegalGrid = new Ext.grid.GridPanel({
	    id:"IllegalGrid",
		store:varIllegalStore,
		sm:sm,
		cm:cm,
		loadMask:true,
		autoScroll:true,
		height:400,
		enableHdMenu:false,
		border:false,
		bbar:new Ext.PagingToolbar({
			store:varIllegalStore,
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
		tbar:[new Ext.Toolbar.TextItem('关键词: '), varIllegalName, "", "-", "",
		{
			text:"保存",
			iconCls:"saveicon",
			handler:SaveFunction
		},"","-","",{
			text:"重置",
			iconCls:"refreshicon",
			handler:function(){Ext.getCmp("varIllegalName").setValue("");}
		},"","-","",{
			text:"删除选中",
			iconCls:"deleteicon",
			handler:DelIllegalFn
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
                Ext.getCmp("varIllegalName").setValue(row[0].data.KeyWord);
            }
		}
	});
	TabShow(node,IllegalGrid);
	varIllegalStore.reload({params:{start:0,limit:pageSize}});	
function DelIllegalFn()
{
    var objGrid = Ext.getCmp("IllegalGrid");
    var row=objGrid.getSelectionModel().getSelections();
    if(row.length==0)
    {
        ExtAlert("请至少选择一行");
        return;
    }
    else{  
            Ext.Msg.confirm("提示!","您确定要删除选中关键词吗?",function(btn){ 
            if(btn=="yes")
            {
               //执行删除操作
               var varIDList = '';
               for(var i=0;i<row.length;i++)
               {
                   varIDList += row[i].data.id+',';
               }             
               //提交
               Ext.Ajax.request({
                            url:"./Data/SmsConfig/IllegalOperate.aspx",
                            method:"POST",
                            params:{
                                type:'2',//�删除
                                id:varIDList
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.util.JSON.decode(response.responseText);                                    
                                if(responseArray.success == true){                                    
                                    varIllegalStore.reload();
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
        var varName = Ext.get("varIllegalName").getValue();
 			  if (varName == '') {
 			        ExtAlert('关键词不能为空');
 			        return;
 			  } 			  		  
			  else{
			      doSaveIllegal(varName);
			  }
    }

    //添加或修改关键词
    function doSaveIllegal(varName)
    {
           //var objGrid = Ext.getCmp("IllegalGrid");
           var row=IllegalGrid.getSelectionModel().getSelections();
           var vType = '0';
           var vId = '0';
           if ( row.length != 0 )
           {
                vType = '1';
                vId = row[0].data.id;
           }       
           Ext.Ajax.request({
           url:"./Data/SmsConfig/IllegalOperate.aspx",
            method:"POST",
            params:{
                type:vType,
                name:varName,
                id:vId
            },
            success:function(response,option)
            {           
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){                                   
                    varIllegalStore.reload();
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
}
