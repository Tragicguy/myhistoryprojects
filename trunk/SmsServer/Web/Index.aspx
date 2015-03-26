<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Index.aspx.cs" Inherits="SmsServer.Web.Index" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
    <meta http-equiv="content-type" content="text/html; charset=utf-8">
   <link rel="stylesheet" type="text/css" href="./jscss/resources/css/ext-all.css">
   <script type="text/javascript" src="./jscss/adapter/ext/ext-base.js"></script>
  <script type="text/javascript" src="./jscss/ext-all.js"></script>
  <script type="text/javascript" src="./jscss/ext-lang-zh_CN.js"></script>
</head>
<body>
    <div id="hello"></div>
  <script>
/**/
Ext.BLANK_IMAGE_URL="../jscss/resources/images/default/s.gif";
Ext.onReady(function(){            //布局    

new Ext.Viewport({
layout:"border",
items:[{region:"north",     //头部
height:60,
margins:'6 6 0 6',
title:"header"},

{region:"south",
height:50,
title:"底部面板",
margins:'0 6 6 6',
content:"aaaa"},

// {region:"center",
// margins:'0 0 0 0',
// title:"中央面板",
//  autoScroll:true},
 panel,
tree,       //控制列表

{region:"east",
width:100,
margins:'0 6 0 0',  //上.右.下 左
title:"右边面板"}
]

}); 

});

    var tree = new Ext.tree.TreePanel({
         // tree
         animate:true,
         enableDD:true,
         containerScroll: true,
         ddGroup: 'organizerDD',
         rootVisible:false,
         // layout
         region:'west',
         width:175,
         minSize:150,
         maxSize:260,
         split:true,
         // panel
         title:'控制列表',
         autoScroll:true,
        // tbar: tb,
         margins: '0 0 0 6'
    });

    var root = new Ext.tree.TreeNode({
        text: 'Albums',
        allowDrag:false,
        allowDrop:false
    });
    tree.setRootNode(root);
    root.appendChild(
        new Ext.tree.TreeNode({text:'Album 2', cls:'album-node', allowDrag:false}),
        new Ext.tree.TreeNode({text:'Album 1', cls:'album-node', allowDrag:false}),
        new Ext.tree.TreeNode({text:'Album 3', cls:'album-node', allowDrag:false})
    );
    root.appendChild(
        new Ext.tree.TreeNode({text:'Album 2', cls:'album-node', allowDrag:false}),
        new Ext.tree.TreeNode({text:'Album 1', cls:'album-node', allowDrag:false}),
        new Ext.tree.TreeNode({text:'Album 3', cls:'album-node', allowDrag:false})
    );
    root.appendChild(
        new Ext.tree.TreeNode({text:'Album 2', cls:'album-node', allowDrag:false}),
        new Ext.tree.TreeNode({text:'Album 1', cls:'album-node', allowDrag:false}),
        new Ext.tree.TreeNode({text:'Album 3', cls:'album-node', allowDrag:false})
    );
 var panel = new Ext.Panel({
        id:'images-view',
        width:600,
        height:550,
        region:"center",
        collapsible:true,
        layout:'fit',
        title:'ListView Demo'
      //  items: listView
    });
/**/
/**         
//容器
Ext.onReady(function(){ 

new Ext.Panel({
renderTo:"hello",
title:"容器组件",
layout:"column",
width:500,
height:100,
items:[{title:"列1",width:100},

{title:"列2",width:200},

{title:"列3",width:100},

{title:"列4"}]
});
});
/**/


/*//表单
Ext.onReady(function(){ 

new Ext.form.FormPanel({

renderTo:"hello",

title:"容器组件",

width:300, 

labelAlign:"right",

height:120,

defaultType: 'textfield',

items:[

{fieldLabel:"请输入姓名",name:"name"},

{fieldLabel:"请输入地址",name:"address"},

{fieldLabel:"请输入电话",name:"tel"}

]

}

);

});
/**/
/**
//Accordion布局
Ext.onReady(function(){ 

new Ext.Panel({

renderTo:"hello",

title:"容器组件",

width:500,

height:200,

layout:"accordion",

layoutConfig: {

animate: true 

},

items:[{title:"子元素1",html:"这是子元素1中的内容"},

{title:"子元素2",html:"这是子元素2中的内容"},

{title:"子元素3",html:"这是子元素3中的内容"}

]

}

);

});
/**/
/**
//Card布局
Ext.onReady(function(){ 

var panel=new Ext.Panel({

renderTo:"hello",

title:"容器组件",

width:500,

height:200,

layout:"card",

activeItem: 0,

layoutConfig: {

animate: true 

},

items:[{title:"子元素1",html:"这是子元素1中的内容"},

{title:"子元素2",html:"这是子元素2中的内容"},

{title:"子元素3",html:"这是子元素3中的内容"}

],

buttons:[{text:"切换",handler:changeTab}]

}

);

var i=1;

function changeTab()

{ 

panel.getLayout().setActiveItem(i++); 

if(i>2)i=0;

}

});
/**/

/**
//
Ext.onReady(function(){ 

var panel=new Ext.Panel({

renderTo:"hello",

title:"容器组件",

width:500,

height:200,

layout:"table", 

layoutConfig: {

columns: 3 

},

items:[{title:"子元素1",html:"这是子元素1中的内容",rowspan:2,height:100},

{title:"子元素2",html:"这是子元素2中的内容",colspan:2},

{title:"子元素3",html:"这是子元素3中的内容"},

{title:"子元素4",html:"这是子元素4中的内容"}

]

}

);

});
/**/

  </script>
</body>
</html>
