SmsUserEdit = function(varSmsUser,varType)
{
   var oldPort = '';
   var PhoneMapStore = new Ext.data.SimpleStore({
                         fields : ["name","value"],
                         data : [["移动","0"],["联通","1"],["电信","2"],["小灵通","3"]]
                       });
    var SpgateStore = new Ext.data.Store({
        proxy:new Ext.data.HttpProxy({
            url:'./Data/SmsSpgate/SpgateOprate.aspx',
            method:'POST'
        }),
        reader: new Ext.data.JsonReader({
            totalProperty:"totalCount",
            root:"data",
            fields:[{name: 'value'},{name: 'name'},{name: 'gatetype'}]
            })
    });
    SpgateStore.on('beforeload',function(){
        SpgateStore.baseParams = {type:'4'};
    });
    SpgateStore.load();    
    var MobileStore = new Ext.data.SimpleStore({
                         fields : ["name","value","gatetype"]
                       });
    var UnicomStore = new Ext.data.SimpleStore({
      fields : ["name","value","gatetype"]
    });
    var TelecomStore = new Ext.data.SimpleStore({
      fields : ["name","value","gatetype"]
    });
    var ChinaPhoneStore = new Ext.data.SimpleStore({
      fields : ["name","value","gatetype"]
    });
    SpgateStore.on('load',function(){
    MobileStore.removeAll();
    UnicomStore.removeAll();
    TelecomStore.removeAll();
    ChinaPhoneStore.removeAll();
    SpgateStore.each(function(rcd){     //将通道按号段进行分类
           var vType = rcd.get('gatetype');           
           if( vType== '1' || vType == '2' || vType == '3'){                
               MobileStore.add(rcd);
           }
           if (vType== '1' || vType == '2' || vType == '4') {
               UnicomStore.add(rcd);
           }
           if (vType== '1' || vType == '2' || vType == '5') {
               TelecomStore.add(rcd);
           }
           if (vType== '1' || vType == '6') {
               ChinaPhoneStore.add(rcd);
           }
    });        
    });

    var BaseInfoPanel=new Ext.Panel({
    layout:'form',
    labelWidth:40,
    labelAlign:'right',
    bodyStyle:"padding:5px 5px 5px 5px",
    items : [  
               { xtype:'textfield',
                fieldLabel: '用户ID',
                width:100,
                name: 'UserID'
            },{
                xtype:'textfield',
                fieldLabel: '集团ID',
                width:100,
                name: 'ParentID'
            },{
                xtype:'textfield',
                fieldLabel: '姓名',
                width:100,
                name: 'UserName'
            },{
                xtype:'textfield',
                fieldLabel: '公司名',
                width:100,
                name: 'CompanyName'
            },{
                xtype:'textfield',
                fieldLabel: '电话',
                width:100,
                name: 'Phone'
            },{
                xtype:'textfield',
                fieldLabel: '备注',
                width:100,
                name: 'ExtraInfo'
            }]
});
var ContrlInfoPanel=new Ext.Panel({
    layout:'form',
    labelAlign:'right',
    items :[{
        layout : 'column',
        items : [  
            {
                columnWidth : 0.5,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '最大号码数',
                width:50,
                name: 'MaxCount'
            },{
                xtype:'textfield',
                fieldLabel: '信息最大长度',
                width:50,
                name: 'MaxLen'
            },{
                xtype:'checkbox',
                fieldLabel: '状态',
                width:50,
                boxLabel:'禁用',
                name: 'Status'
            },{
                xtype:'checkbox',
                fieldLabel: '信息审核',
                width:50,
                boxLabel:'审核',
                name: 'IsAudit'
            },{
                xtype:'textfield',
                fieldLabel: '审核阈值',
                width:50,
                name: 'AuditNum'
            }]//1
            },{
                columnWidth : 0.5,   
                layout : 'form',   
                items : [{
                xtype:'textfield',
                fieldLabel: '最小号码数',
                width:50,
                name: 'MinCount'
            },{
                xtype:'textfield',
                fieldLabel: '签名',
                width:70,
                name: 'SignStr'
            },{
                xtype:'checkbox',
                labelWidth:0,
                boxLabel:'拆分号码',
                fieldLabel: '历史保存',
                width:75,
                name: 'HistoryType'
            },{
                xtype:'checkbox',
                fieldLabel: '信息拆分',
                boxLabel:'加序号',
                width:70,
                name: 'HasIndex'
            },{
                xtype:'checkbox',
                fieldLabel: '非法关键词',
                boxLabel:'检测',
                width:50,
                name: 'IsCheckIllegal'
            },{
                xtype:'checkbox',
                width:75,
                fieldLabel: '扣费字数设置',
                boxLabel:'跟随通道',
                name: 'IsSpgateFee'
            }]
            }]
            }]   
});
var SpgateSetPanel = new Ext.Panel({
    layout:'form',
    labelWidth:70,
    labelAlign:'right',
    items :[{
        layout : 'column',
        items : [
            {
                columnWidth : 0.85,
                layout : 'form',
                items : [{
                name:"MobileSpgate",
				xtype:"combo",
				fieldLabel:"移动通道",
				width:360,
			    editable: false,
			    typeAhead: true,
				hiddenName:"MobileSpgate",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				store:MobileStore,
				listeners:{
				"select":function(){
				    var varPort = this.getValue();
				    if(oldPort != varPort)
				    {
				        //更新此通道的数据
				        RefreshPortData(varPort,'0');
				    }
				},
				"beforeselect":function(){oldPort = this.getValue();}
				}
            },{
                name:"UnicomSpgate",
				xtype:"combo",
				fieldLabel:"联通通道",
				width:360,
			    editable: false,
			    typeAhead: true,
				hiddenName:"UnicomSpgate",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",				
				store:UnicomStore,
				listeners:{
				"select":function(){
				    var varPort = this.getValue();
				    if(oldPort != varPort)
				    {
				        //更新此通道的数据
				        RefreshPortData(varPort,'1');
				    }
				},
				"beforeselect":function(){oldPort = this.getValue();}
				}
            },{
                name:"TelecomSpgate",
				xtype:"combo",
				fieldLabel:"电信通道",
				width:360,
			    editable: false,
			    typeAhead: true,
				hiddenName:"TelecomSpgate",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",				
				store:TelecomStore,
				listeners:{
				"select":function(){
				    var varPort = this.getValue();
				    if(oldPort != varPort)
				    {
				        //更新此通道的数据
				        RefreshPortData(varPort,'2');
				    }
				},
				"beforeselect":function(){oldPort = this.getValue();}
				}
            },{
                name:"ChinaPhoneSpgate",
				xtype:"combo",
				fieldLabel:"小灵通通道",
				width:360,
			    editable: false,
			    typeAhead: true,
				hiddenName:"ChinaPhoneSpgate",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",				
				store:ChinaPhoneStore,
				listeners:{
				"select":function(){
				    var varPort = this.getValue();
				    if(oldPort != varPort)
				    {
				        //更新此通道的数据
				        RefreshPortData(varPort,'3');
				    }
				},
				"beforeselect":function(){oldPort = this.getValue();}
				}
            }]
            },{
            labelWidth:40,
            columnWidth : 0.15,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '子端口',
                width:30,
                name: 'MobileSubport'
            },{
                xtype:'textfield',
                fieldLabel: '子端口',
                width:30,
                name: 'UnicomSubport'
            },{
                xtype:'textfield',
                fieldLabel: '子端口',
                width:30,
                name: 'TelecomSubport'
            },{
                xtype:'textfield',
                fieldLabel: '子端口',
                width:30,
                name: 'ChinaPhoneSubport'
            }]
         }]   
    }]
});
//号段映射
var PhoneMapPanel = new Ext.Panel({
    layout:'form',
    labelWidth:40,
    labelAlign:'right',
    items :[{
        layout : 'column',
        items : [  
            {
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                name:"MobileMap",
				xtype:"combo",
				fieldLabel:"移动",
				width:60,
			    editable: false,
			    typeAhead: true,
				hiddenName:"MobileMap",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:'3',				
				store:PhoneMapStore
            }]
         },{
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                name:"UnicomMap",
				xtype:"combo",
				fieldLabel:"联通",
				width:60,
			    editable: false,
			    typeAhead: true,
				hiddenName:"UnicomMap",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:'4',				
				store:PhoneMapStore
            }]
         },{
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                name:"TelecomMap",
				xtype:"combo",
				fieldLabel:"电信",
				width:60,
			    editable: false,
			    typeAhead: true,
				hiddenName:"TelecomMap",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:'5',			
				store:PhoneMapStore
            }]
         },{
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                name:"ChinaPhoneMap",
				xtype:"combo",
				fieldLabel:"小灵通",
				width:60,
			    editable: false,
			    typeAhead: true,
				hiddenName:"ChinaPhoneMap",
				mode:"local",
				displayField:"name",
				valueField:"value",
				triggerAction:"all",
				value:'6',
				store:PhoneMapStore
            }]
         }]
      }]
});
var FeeSetPanel = new Ext.Panel({
    layout:'form',
    labelWidth:40,
    labelAlign:'right',
    items :[{
        layout : 'column',
        items : [  
            {
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '移动',
                width:50,
                name: 'MobileLen'
            }]
         },{
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '联通',
                width:50,
                name: 'UnicomLen'
            }]
         },{
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '电信',
                width:50,
                name: 'TelecomLen'
            }]
         },{
                columnWidth : 0.25,
                layout : 'form',
                items : [{
                xtype:'textfield',
                fieldLabel: '小灵通',
                width:50,
                name: 'ChinaPhoneLen'
            }]
         }]
    }]
});

var BaseInfoFieldSet = new Ext.form.FieldSet({
    title : '基本信息设置',   
    autoHeight : true,
    width:180,
    defaultType : 'textfield',
    items :[BaseInfoPanel]   
});

var ContrlInfoFieldSet = new Ext.form.FieldSet({
    title : '业务逻辑设置',
    layout:"form", 
    autoHeight : false,
    labelAlign:'right',
    //height:199,   
    defaultType : 'textfield',
    width:405,
    style:'border-left:0px;',   
    items :[ContrlInfoPanel]
});

var SpgateFieldSet = new Ext.form.FieldSet({
    title : '通道设置',
    autoHeight : true,
    defaultType : 'textfield',
    items :[SpgateSetPanel]
});

var SpgateFeeSet = new Ext.form.FieldSet({
    title : '扣费字数设置',   
    autoHeight : true,   
    defaultType : 'textfield',
    items :[FeeSetPanel]
});

var PhoneMapFieldSet = new Ext.form.FieldSet({
    title : '号段映射设置',   
    autoHeight : true,   
    defaultType : 'textfield',
    items :[PhoneMapPanel]
});

var SmsUserEditPanel = new Ext.FormPanel({
    id:'SmsUserEditPanel',
    labelAlign:'right',
    layout:"form",   
    buttonAlign:'center',
    frame:true,   
    items:[
    	{
        columnWidth : 0.5,
        layout : 'column',          
        items : [BaseInfoFieldSet,ContrlInfoFieldSet]
      },SpgateFieldSet,PhoneMapFieldSet,SpgateFeeSet],
      buttons:[{
      text : '保存',
      width:60,
      handler : function() {
              var values = Ext.getCmp('SmsUserEditPanel').getForm().getValues();
              if (values.UserID == '') { ExtAlert('用户ID不能为空'); return;}
 			  if (values.ParentID == '') { ExtAlert('集团ID不能为空'); return;}
 			  if (values.UserName == '') { ExtAlert('用户姓名不能为空'); return;}
 			  if (values.Phone == '') { ExtAlert('电话不能为空'); return;}
 			  /*if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}
 			  if (values.UserID == '') { ExtAlert('用户名不能为空'); return;}*/
			  doSaveManagerInfo(values);
        }
    },{
       text : '重置',
       width:60,
       handler : function() {
              SmsUserEditPanel.form.reset();
              SpgateStore.reload();
              if(varType=='1'){
                    loadData();
              }
          }
    },{
       text : '取消',
       width:60,
       handler : function() {
              SmsUserEditWin.close();
              }
    }]
    });    
    var SmsUserEditWin=new Ext.Window({
			title:varType=='1'?("编辑用户信息-"+varSmsUser):"添加用户",
			width:575,
			height:550,
			plain:true,
			layout:"form",
			iconCls:varType=='1'?"editicon":"addicon",			
			resizable:false,
			defaultType:"textfield",
			closeAction : 'close',
            closable:true,
            modal: 'true', 
			buttonAlign:"center",
			items:[SmsUserEditPanel]
	});
	SmsUserEditWin.show();	
	//ContrlInfoFieldSet.setHeight(BaseInfoFieldSet.getHeight());
	if(varType == '1')
	{
	    loadData();
	}
	function loadData() {
        SmsUserEditWin.setTitle("编辑用户信息-"+varSmsUser+'<font color="red"> 加载中....</font>');
        Ext.Ajax.request({
           url:"./Data/SmsUser/SmsUserOprate.aspx",
            method:"POST",
            params:{
                userid:varSmsUser,
                type:'3'
            },
            success:function(response,option)
            {
                var responseArray = Ext.decode(response.responseText);                                    
                if(responseArray.success == true){
                    Ext.getCmp('SmsUserEditPanel').getForm().setValues(responseArray.data[0]);
                }else{
                    ExtAlert(responseArray.errorInfo);
                }
               SmsUserEditWin.setTitle("编辑用户信息-"+varSmsUser);
            },
            failure:function(response,option)
            {
                ExtAlert("网络错误");
                SmsUserEditWin.setTitle("编辑用户信息-"+varSmsUser);
            }
        }); 
    }
    function doSaveManagerInfoEx(values)
    {
        ExtShow("正在提交,请稍后...");
                   Ext.Ajax.request({
                   url:"./Data/SmsUser/SmsUserOprate.aspx",
                    method:"POST",
                    params:{
                        jdata:Ext.encode(values),
                        type:varType
                    },
                    success:function(response,option)
                    {
                        var responseArray = Ext.decode(response.responseText);                                    
                        if(responseArray.success == true){
                            Ext.MessageBox.hide();
                            SmsUserEditWin.close();
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
    function doSaveManagerInfo(values)
    {
        //判断号段映射是否合法
        var bRet = IsPhoneMapRight(values);
        if(!bRet)
        {
            return;
        }
        bRet = IsSelectMapRight(values,'6');
        if(!bRet)
        {
            Ext.Msg.confirm("提示!","电信号段映射与通道配置不匹配,继续吗?",function(btn){ 
                if(btn=="yes")
                {
                    //判断各号段计费条数是否已设置，并给出一定范围[可以后台配置]
                    doSaveManagerInfoEx(values);
                }
                else
                {
                    return false;
                }
            });
        }
        else
        {
            doSaveManagerInfoEx(values);
        }
    }
    //判断指定号段映射是否正确，mytype表示映射本体标识
    IsSelectMapRight = function(values,myType)
    {
        var destType = '';
        var destPort = '';
        switch(myType)
        {
            case '3':
            destType = values.MobileMap;
            break;
            case '4':
            destType = values.UnicomMap;
            break;
            case '5':
            destType = values.TelecomMap;
            break;
            case '6':
            destType = values.ChinaPhoneMap;
            break;
            default:
            return true;
            break;
        }
        switch(destType)
        {
            case '3':
            destPort = values.MobileSpgate;
            break;
            case '4':
            destPort = values.UnicomSpgate;
            break;
            case '5':
            destPort = values.TelecomSpgate;
            break;
            case '6':
            destPort = values.ChinaPhoneSpgate;
            break;
            default:
            return true;
            break;
        }
        var index = SpgateStore.find("value",destPort);
        var vRecord = SpgateStore.getAt(index);
        var vPortType = '';
       // var vType='';
        if(vRecord)
        {
           vPortType = vRecord.get("gatetype"); 
        }        
        if(vPortType == '1' || vPortType == '2' || vPortType == myType) //判断号段映射是否合法
        {
            return true;
        }
        return false;
    }   
    //判断各号段的映射是否合法
    IsPhoneMapRight = function(values)
    {
        var bRet = IsSelectMapRight(values,'3');
        if(!bRet)
        {
            ExtAlert('移动号段映射与通道配置不匹配');
            return false;
        }
        bRet = IsSelectMapRight(values,'4');
        if(!bRet)
        {
            ExtAlert('联通号段映射与通道配置不匹配');
            return false;
        }
        bRet = IsSelectMapRight(values,'5');
        if(!bRet)
        {
            ExtAlert('电信号段映射与通道配置不匹配');
            return false;
        }       
        return true;
    }
    RefreshPortData = function(varPort,type)
    {
       //先判断一下，其他通道是否已经配置了此通道
        var vForm =  Ext.getCmp('SmsUserEditPanel').getForm();        
        var values = vForm.getValues();
        var iError = 0;
        var GateName = '';
        switch(type)
        {
            case "0":
            if(varPort == values.UnicomSpgate ||varPort == values.TelecomSpgate ||varPort == values.ChinaPhoneSpgate)
            { 
                iError = 1; 
                vForm.setValues({MobileSpgate:oldPort});
            }
            break;            
            case "1":
            if(varPort == values.MobileSpgate ||varPort == values.TelecomSpgate ||varPort == values.ChinaPhoneSpgate)
            { 
                iError = 1;
                vForm.setValues({UnicomSpgate:oldPort});
            }
            break;
            case "2":
            if(varPort == values.UnicomSpgate ||varPort == values.MobileSpgate ||varPort == values.ChinaPhoneSpgate)
            { 
                iError = 1;
                vForm.setValues({TelecomSpgate:oldPort});
            }
            break;
            case "3":
            if(varPort == values.MobileSpgate ||varPort == values.TelecomSpgate ||varPort == values.UnicomSpgate)
            {
                iError = 1;
                vForm.setValues({ChinaPhoneSpgate:oldPort});
            }
            break;
            default:
            ExtAlert("未知通道号");
            return;
            break;
        }
        if(iError == 1)
        {
            ExtAlert("此通道已绑定在其他号段上,请通过设置号段映射来绑定!");
            return;
        }
        //得到显示选择项的通道类型
        var index = SpgateStore.find("value",varPort);
        var vRecord = SpgateStore.getAt(index);
        var vType = vRecord.get("gatetype");
        //自动改变号段映射关系
        ChangePhoneMap(vForm,vType,type);
        //更新计费字数
        if(values.IsSpgateFee == 'on')
        {
            var objMask = MaskShow(SmsUserEditPanel,"正在获取通道数据,请稍后...");  
               Ext.Ajax.request({
               url:"./Data/SmsSpgate/SpgateOprate.aspx",
                method:"POST",
                params:{
                    spgateno:varPort,
                    type:'5'
                },
                success:function(response,option)
                {   
                    var responseArray = Ext.decode(response.responseText);                              
                    if(responseArray.success == true){
                        RefreshData(vForm,responseArray.data[0],type); //更新数据 
                        MaskHide(objMask);                       
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
    }
    //设置号段映射
    ChangePhoneMap = function(vForm,vType,type)
    {
        switch(vType)
        {
            case '1':   //all
            {
                vForm.setValues({MobileMap:type,UnicomMap:type,TelecomMap:type,ChinaPhoneMap:type});
                break;
            }
            case '2':   //three
            {
                vForm.setValues({MobileMap:type,UnicomMap:type,TelecomMap:type});
                break;
            }
            case '3':
            {
                vForm.setValues({MobileMap:type});
                break; 
            }
            case '4':
            {
                vForm.setValues({UnicomMap:type});
                break;
            }
            case '5':
            {
                vForm.setValues({TelecomMap:type});
                break;
            }
            case '6':
            {
                vForm.setValues({ChinaPhoneMap:type});
                break;
            }
            default:
                break;
        }
    }
    //更新计费
    RefreshData = function(vForm,obj,type)
    {   
        switch(obj.GateType)
        {
            case '1':   //all
            {
                vForm.setValues({MobileLen:obj.MobileLen,UnicomLen:obj.UnicomLen,TelecomLen:obj.TeleComLen,ChinaPhoneLen:obj.ChinaPhoneLen});
                break;
            }
            case '2':   //three
            {
                vForm.setValues({MobileLen:obj.MobileLen,UnicomLen:obj.UnicomLen,TelecomLen:obj.TeleComLen});
                break;
            }
            case '3':
            {
                vForm.setValues({MobileLen:obj.MobileLen});
                break; 
            }
            case '4':
            {
                vForm.setValues({UnicomLen:obj.UnicomLen});
                break;
            }
            case '5':
            {
                vForm.setValues({TelecomLen:obj.TeleComLen});
                break;
            }
            case '6':
            {
                vForm.setValues({ChinaPhoneLen:obj.ChinaPhoneLen});
                break;
            }
            default:
                break;
        }
    }
}