Ext.BLANK_IMAGE_URL="./Ext/resources/images/default/s.gif";
//控制树
var tree = new Ext.tree.TreePanel({
         animate:true,
         enableDD:false,
         containerScroll: true,
         collapsible : true,
         rootVisible:true,
         // layout
         region:'west',
         width:175,
         minSize:150,
         maxSize:260,
         split:true,
         // panel
         title:'控制列表',
         autoScroll:true,
         margins: '0 0 0 6',
         lines: true
    });
    var root=new Ext.tree.AsyncTreeNode({
        id:'0',
        text:"短信管理系统",
        iconCls:"hotelmanageicon",
        loader:new Ext.tree.TreeLoader({
            url:"./Data/TreeShow.aspx",
            listeners:{
                "beforeload":function(treeloader,node)
                {
                     treeloader.baseParams={
                     id:node.id,
                     method:'POST'
                     };
                }
            }
        })
    });
    
    tree.setRootNode(root);
    tree.on('click', function(node){
           if (node.isLeaf()) {
              eval(node.attributes.Url);
           }
           else {
              node.toggle();
           } 
    });
    
