//JS
SmsManagerAuth = function(varUserID)
{
var treeAuth = new Ext.tree.TreePanel({
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
    var rootAuth=new Ext.tree.AsyncTreeNode({
        id:'0',
        text:"短信管理系统",
        loader:new Ext.tree.TreeLoader({
            url:"./Data/SmsManager/PowerSet.aspx",
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
    
    treeAuth.setRootNode(rootAuth);
    //选择改变
    treeAuth.on('checkchange',function(node)
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
     //重载NODE的双击事件，不让他折叠
     Ext.tree.TreeNodeUI.prototype.onDblClick = function(e){
       e.preventDefault();     
       if(this.disabled){     
           return;
       }     
       if(this.checkbox){     
           this.toggleCheck();     
       }
//     if(!this.animating && this.node.hasChildNodes()){     
//         this.node.toggle();     
//     }   
       this.fireEvent("dblclick", this.node, e); 
    }
    treeAuth.on("dblclick",function(node){if(!node.isLeaf()){node.toggle();}});
    treeAuth.expandAll();    
    var AuthManagerInfoWin=new Ext.Window({
			title:"权限管理-"+varUserID,
			width:360,
			height:512,
			plain:true,
			iconCls:"usericon",
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
			        var varPower= GetPowers(rootAuth);
			        ExtShow('正在保存,请稍后...');
			            Ext.Ajax.request({
                            url:"./Data/SmsManager/PowerSet.aspx",
                            method:"POST",
                            params:{
                                type:'1',//更改
                                userid:varUserID,
                                power:varPower
                            },
                            success:function(response,option)
                            {           
                                var responseArray = Ext.decode(response.responseText);                                    
                                if(responseArray.success == true){                    
                                    Ext.MessageBox.alert("提示","保存成功",function() { 
                                       AuthManagerInfoWin.close();
                                    });                                 
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
		    },{
			    text:"重置",
			    tooltip:"加载数据",
			    iconCls:"refreshicon",
			    handler:function(e){rootAuth.reload();treeAuth.expandAll();}
		    }],
			modal: 'true', 
			buttonAlign:"center",
			items:[treeAuth]	
	});
	AuthManagerInfoWin.show();
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