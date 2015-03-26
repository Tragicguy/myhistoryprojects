SmsManagerPortSet = function(varUserID)
{
   var portTree = new Ext.tree.TreePanel({
         animate:true,
         enableDD:false,
         containerScroll: true,
         rootVisible:true,
         width:343,
         height:450,
         split:true,
         autoScroll:true,       
         lines: true
    });
    var portRoot=new Ext.tree.AsyncTreeNode({
        id:'0',
        text:"短信通道分配",
        loader:new Ext.tree.TreeLoader({
            url:"./Data/SmsManager/PortSet.aspx",
            listeners:{
                "beforeload":function(treeloader,node)
                {
                     treeloader.baseParams={
                     id:node.id,
                     userid:varUserID,
                     type:'0',//获取
                     method:'POST'
                     };
                }
            }
        })
    });
    
    portTree.setRootNode(portRoot);
    //选择改变
    portTree.on('checkchange',function(node)
    {
         NodeCheck(node);
    });

    //执行函数
    function NodeCheck(node,iFlag)
    {
        var checkFlag = false;           
           if (node.id != '0') {
             checkFlag = node.getUI().checkbox.checked;
           }
           if (!node.isLeaf()) {
            var childNodes = node.childNodes;
                if(childNodes || childNodes.length>0){
                    for(var i=0;i<childNodes.length;i++)
                    {
                        childNodes[i].getUI().checkbox.checked = checkFlag;                        
                    }
                }
           }
           else
           {
                //选中父结点,排除ID为0的，否则会出错
                var pNode = node.parentNode;
                if (pNode && pNode.id != '0') {
                    pNode.getUI().checkbox.checked = IsChildChecked(pNode);
                }
           } 
    }
    function IsChildChecked(node)
    {
        var childNodes = node.childNodes;
        if(childNodes || childNodes.length>0){
            for(var i=0;i<childNodes.length;i++)
            {
                if(childNodes[i].getUI().checkbox.checked)
                {
                    return true;
                }
            }
        }
        return false;
    }
    portTree.expandAll();    
    var ManagerPortInfoWin=new Ext.Window({
			title:"通道分配-"+varUserID,
			width:360,
			height:512,
			plain:true,
			iconCls:"setport",
			resizable:false,
			defaultType:"textfield",
			labelWidth:100,
			collapsible:true,
            closeAction : 'hide',
            closable:true,
            tbar:[
		    new Ext.Toolbar.Fill(),
		    {
			    text:"保存",
			    tooltip:"保存配置",
			    iconCls:"saveicon",
			    handler:function(){
			        var varSpgates= GetPowers(portRoot);
			        var objMask = MaskShow(ManagerPortInfoWin,"正在保存,请稍后...");
			            Ext.Ajax.request({
                            url:"./Data/SmsManager/PortSet.aspx",
                            method:"POST",
                            params:{
                                type:'1',//更改
                                userid:varUserID,
                                ports:varSpgates
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.decode(response.responseText);                                    
                                if(responseArray.success == true){                    
                                     ManagerPortInfoWin.close();
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
			    text:"重置",
			    tooltip:"加载数据",
			    iconCls:"refreshicon",
			    handler:function(e){portRoot.reload();portTree.expandAll();}
		    }],
			modal: 'true', 
			buttonAlign:"center",
			items:[portTree]	
	});
	ManagerPortInfoWin.show();
	function GetPowers(node)
	{
	    var varStr='';
	    if(node.id !='0')
	    {
	        if(node.getUI().checkbox.checked)
            {
                varStr+=node.id+',';
            }
	    }
	    if (!node.isLeaf()) {	
	        var childNodes = node.childNodes;
                if(childNodes || childNodes.length>0){
                    for(var i=0;i<childNodes.length;i++)
                    {
                       varStr += GetPowers(childNodes[i]);                       
                    }
                }
           }
           return varStr;      
	}
}