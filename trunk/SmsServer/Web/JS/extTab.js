//创建TABPANEL
var tabPanel = new Ext.TabPanel({
		region:'center',
		deferredRender:false,
		autoScroll: true, 
		margins:'0 6 0 0',
		activeTab:0,
		enableTabScroll:true,
		items:[{
			id:'tabShow',
			iconCls:'gonggao',
    		title: '初始面板',
    		html:'你想来吗？',
    		closable:false,
    		autoScroll:true
		}]
    });

//显示一个TAB,内部内容为grid,样式类为icon
TabShow=function(node,grid){
    var tab = tabPanel.getItem('tab_'+node.id);
	    if(!tab){
		   tab = tabPanel.add({
			    id:'tab_'+node.id,
			    iconCls:node.attributes.iconCls,
			    xtype:"panel",
			    title:node.text,
			    closable:true,
			    layout:"fit",
			    items:[grid]			   
		    });
	    }
  tabPanel.setActiveTab(tab);
 }
 
 function IsTabExist(nodeid)
 {
    var tab = tabPanel.getItem('tab_'+nodeid);
	    if(tab){
		   tabPanel.setActiveTab(tab);
		   return true;
	    }
    return false;
 }