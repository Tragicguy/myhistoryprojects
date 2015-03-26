/*根据元素ID查找元素*/
function $() {
	var elements = new Array();
	for (var i = 0; i < arguments.length; i++) {
		var element = arguments[i];
		if (typeof element == 'string')
			element = document.getElementById(element);	
	    if (arguments.length == 1)
			return element;
		elements.push(element);
  }
  return elements;
}

/*根据元素ID取得元素*/
function $E(elemid) {
	return document.getElementById(elemid);
}

/*根据元素ID取得元素的value*/
function $Vo(elemid) {
	return document.getElementById(elemid).value;
}

/*根据元素ID取得元素的innerHTML*/
function $H(elemid) {
	return document.getElementById(elemid).innerHTML;
}

/*根据元素ID隐藏该元素*/
function $Hide(id) {
    document.getElementById(id).style.display = 'none';
}

/*根据元素ID显示该元素*/
function $Display(id) {
    document.getElementById(id).style.display = 'block';
}

/*根据元素ID设置该元素为空*/
function $Dlose(id) {
    document.getElementById(id).innerHTML='';
}

/*网页转向*/
function $U(url){
	window.location.href=url;
}

/*取字符长度，一个中文字符为两个字节*/
function $Len(str){
	return (''+str).replace(/[^\x0000-\xFF00]/gi,'xx').length;
}

//用正则表达式将前后空格用空字符串替代。
function trim(strSrc)
{
	return strSrc.replace(/(^\s*)|(\s*$)/g, "");
}

//onkeypress时根据输入类型控制输入字符,"F":浮点型 "I":整型 "D":日期型
function filterKey(sType){
	var iKey = window.event.keyCode;
	
	if(sType == "F"){ //浮点型
	
		if(iKey != 45 && iKey != 46 && iKey != 13 && iKey != 11 && !(iKey>=48 && iKey<=57))
			window.event.keyCode = 0
		else{
			if(iKey == 46){
				var obj = window.event.srcElement;
				if(obj.value.indexOf(".")>=0)
					window.event.keyCode = 0;
			}
			if(iKey == 45){
				var obj = window.event.srcElement;
				if(obj.value.indexOf("-")>=0)
					window.event.keyCode = 0;
				else{
					window.event.keyCode = 0;
					obj.value = "-" + obj.value;
					if(obj.onchange != null){
						obj.onchange();
					}
				}
			}
		}
	} //end "F"
	else if(sType == "I"){ //整型
		if(iKey != 13 && iKey != 45 && iKey != 11 && !(iKey>=48 && iKey<=57))
			window.event.keyCode = 0;
		else if(iKey == 45){
			var obj = window.event.srcElement;
			if(obj.value.indexOf("-")>=0)
				window.event.keyCode = 0;
			else{
				window.event.keyCode = 0;
				obj.value = "-" + obj.value;
				if(obj.onchange != null){
					obj.onchange();
				}
			}
		}
	}	// end "I"
	else if(sType == "D"){ //日期型
		var obj = window.event.srcElement;
		var strDate = obj.value;
		
		if(strDate.length>=10){
			window.event.keyCode = 0;
			return;
		}
		else if(strDate.length<4){ //年
			if(iKey != 13 && iKey != 11 && !(iKey>=48 && iKey<=57))
				window.event.keyCode = 0;
		}
		else if(strDate.length == 4){ //分隔符
			if(iKey != 45 && iKey != 47)
				window.event.keyCode = 0;
		}
		else if(strDate.length>=5){
			if( strDate.indexOf("-") > 0 && strDate.indexOf("-") == strDate.lastIndexOf("-")){ //正输入月份
				if(strDate.length>=7 && iKey != 45){ //如果长度过长，则退出
					window.event.keyCode = 0;
					return;
				}
				
				if(iKey>=48 && iKey<=57){
					var iPos = strDate.indexOf("-");
					
					var iMonth = parseInt("" + strDate.substr(iPos+1,strDate.length-iPos-1) + (parseInt(iKey) - 48));
					
					if(strDate.length>=6 && (iMonth <1 || iMonth > 12)){
						window.event.keyCode = 0;
						return;
					}
				}
			} // end if("-")
			else if( strDate.indexOf("/") > 0 && strDate.indexOf("/") == strDate.lastIndexOf("/")){ //正输入月份
				if(strDate.length>=7 && iKey != 47){ //如果长度过长，则退出
					window.event.keyCode = 0;
					return;
				}
				
				if(iKey>=48 && iKey<=57){
					var iPos = strDate.indexOf("/");
					
					var iMonth = parseInt("" + strDate.substr(iPos+1,strDate.length-iPos-1) + (parseInt(iKey) - 48));
					
					if(strDate.length >= 6 && (iMonth <1 || iMonth > 12)){
						window.event.keyCode = 0;
						return;
					}
				}
			} // end if("/")
			else if( strDate.indexOf("-") > 0 && strDate.indexOf("-") != strDate.lastIndexOf("-")){ //正输入日期
				if(iKey>=48 && iKey<=57){
					var iPos = strDate.lastIndexOf("-");
					if(strDate.length - iPos > 2){
						window.event.keyCode = 0;
						return;
					}
					
					var iDay = parseInt("" + strDate.substr(iPos+1,strDate.length-iPos-1) + (parseInt(iKey) - 48));
					
					if(iDay > 31){
						window.event.keyCode = 0;
						return;
					}
				}
			}
			else if( strDate.indexOf("/") > 0 && strDate.indexOf("/") != strDate.lastIndexOf("/")){ //正输入日期
				if(iKey>=48 && iKey<=57){
					var iPos = strDate.lastIndexOf("/");
					if(strDate.length - iPos > 2){
						window.event.keyCode = 0;
						return;
					}
					
					var iDay = parseInt("" + strDate.substr(iPos+1,strDate.length-iPos-1) + (parseInt(iKey) - 48));
					
					if(iDay > 31){
						window.event.keyCode = 0;
						return;
					}
				}
			}
			
		}
		
		if(strDate.charAt(strDate.length-1) == "-" || strDate.charAt(strDate.length-1) == "/"){
			if(iKey != 13 && iKey != 11 && !(iKey>=48 && iKey<=57))
				window.event.keyCode = 0;
		}
		
		if(iKey == 47 || iKey == 45){
			if(strDate.indexOf("-") > 0 && iKey == 47)
				window.event.keyCode = 0;
			if(strDate.indexOf("/") > 0 && iKey == 45)
				window.event.keyCode = 0;
		}
	} // end "D"
	
	
}

/*自适应大小*/ 
function DrawImage(ImgD,_width,_height){
	if(!_width) _width=120;
	if(!_height) _height=120;
	var flag=false;
	var image=new Image();
	image.src=ImgD.src;
	if(image.width>0&&image.height>0){
		flag=true;
		if(image.width/image.height>=_width/_height){//120/120
			if(image.width>_width){   
				ImgD.width=_width;
				ImgD.height=(image.height*_width)/image.width;
			}else{
				ImgD.width=image.width;   
				ImgD.height=image.height;
			}
			ImgD.alt=image.width+"X"+image.height;
		}
		else{
			if(image.height>_height){   
				ImgD.height=_height;
				ImgD.width=(image.width*_height)/image.height;   
			}else{
				ImgD.width=image.width;   
				ImgD.height=image.height;
			}
			ImgD.alt=image.width+"X"+image.height;
		}
	}
}  

/*Cookie*/
function setCookie(name,value)
{
    var Days = 365;
    var exp  = new Date();    //new Date("December 31, 9998");
        exp.setTime(exp.getTime() + Days*24*60*60);
        document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString();
}
function getCookie(name)
{
    var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");
	if(arr=document.cookie.match(reg)) 
		return unescape(arr[2]);
	else 
		return null;
}
function delCookie(name)
{
    var exp = new Date();
        exp.setTime(exp.getTime() - 1);
    var cval=getCookie(name);
        if(cval!=null) 
        	document.cookie= name + "="+cval+";expires="+exp.toGMTString();
}

/*验证数字*/
function isNumber(e){
	var number = "1234567890";
	for(var i=0; i<e.length; i++){
		if (number.indexOf(e.charAt(i))<0) {
			return false;
		}
	}
	return true;
}

/*验证数字*/
function isAllDigits(argvalue) {
    argvalue = argvalue.toString();
    var validChars = "0123456789";
    var startFrom = 0;
    if (argvalue.substring(0, 2) == "0x") {
       validChars = "0123456789abcdefABCDEF";
       startFrom = 2;
    } else if (argvalue.charAt(0) == "0") {
       validChars = "01234567";
       startFrom = 1;
    } else if (argvalue.charAt(0) == "-") {
        startFrom = 1;
    }
    
    for (var n = startFrom; n < argvalue.length; n++) {
        if (validChars.indexOf(argvalue.substring(n, n+1)) == -1) return false;
    }
    return true;
}

/*检查Email是否合法*/
function isEmail(s){
    if (s.length<7||s.length > 50){
            return false;
    }
     var regu = "^(([0-9a-zA-Z]+)|([0-9a-zA-Z]+[_.0-9a-zA-Z-]*[0-9a-zA-Z]+))@([a-zA-Z0-9-]+[.])+([a-zA-Z]{2}|net|NET|com|COM|gov|GOV|mil|MIL|org|ORG|edu|EDU|int|INT)$"
     var re = new RegExp(regu);
     if (s.search(re) != -1) {
           return true;
     } else {
           return false;
     }
}

/*检查字符串是否为Null*/
function isNull(s){
    if (s == null || s.length <= 0 || s.trim() == ""){
            return true;
    }
    return false;
}

/*检查字符串是否为空*/
function isEmpty(s){
    if (s == null || s.length <= 0 || s.trim() == ""){
            return true;
    }
    return false;
}

/*检查日期是否合法*/
function isValidDate(day, month, year) {
    if (month < 1 || month > 12) {
            return false;
        }
        if (day < 1 || day > 31) {
            return false;
        }
        if ((month == 4 || month == 6 || month == 9 || month == 11) &&
            (day == 31)) {
            return false;
        }
        if (month == 2) {
            var leap = (year % 4 == 0 &&
                       (year % 100 != 0 || year % 400 == 0));
            if (day>29 || (day == 29 && !leap)) {
                return false;
            }
        }
        return true;
    }

/*获得Radio的值*/
function getRadioValue(name){	
	var radios = document.getElementsByName(name);
	var i;   
	if (null == radios.length){
	  	if(radios.checked) {
	  		return radios.value;
	  	}
	}
    for(i = 0; i < radios.length; i++){
       if(radios[i].checked){
     		return radios[i].value;
       }
    }
    return 0;
}

/*设置Radio的值*/
function setRadioValue(name,value){	
	var radios = document.getElementsByName(name);
	var i;
	if (null == radios.length){
	  	if(radios.checked) {
	  		radios.checked = "checked";
	  	}
	}
    for(i=0;i<radios.length;i++){
       if(value == radios[i].value){
     		radios[i].checked = "checked";
       }
    }
    return 0;
}

/*获得CheckBox的值,多个为数组*/
function getCheckBoxValues(name){	
	var values = new Array();
	var cbs = document.getElementsByName(name);
	var i;   
	if (null == cbs) return values;	  
	if (null == cbs.length){
	  	if(cbs.checked) {
	  		values[values.length] = cbs.value;
	  	}
	  	return values;
	}	    
	var count = 0 ;  	
	for(i = 0; i<cbs.length; i++){
		if(cbs[i].checked){
			values[values.length] = cbs[i].value;
		}
	}
	return values;
}

/*设置CheckBox的值*/
function setCheckBoxValue(name,value){
	var cbs = document.getElementsByName(name);
	var i;
    if (null == cbs) return 0 ;
  	if (null == cbs.length){
  		cbs.checked = value;
  		return 0;
  	}
	for(i=0;i<cbs.length;i++){
  		cbs[i].checked = value;
  	}
  	return 0;
}

/*设置CheckBox选中状态*/
function setCheckBoxs(name,value){
	var cbs = document.getElementsByName(name);
	var i;
    if (null == cbs) return 0 ;
  	if (null == cbs.length){
  		cbs.checked = true;
  		return 0;
  	}
	for(i=0;i<cbs.length;i++){
		if(cbs[i].value == value){
			cbs[i].checked = true;
		}
  	}
  	return 0;
}

function htmlEncode(text) {
	return text.replace(/&/g, '&amp;').replace(/"/g, '&quot;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

var Request = new Object();
Request.send = function(url, method, callback, data, urlencoded) {
    var req;
    if (window.XMLHttpRequest) {
        req = new XMLHttpRequest();
        if (req.overrideMimeType) {
			req.overrideMimeType('text/xml');
		}
    } else if (window.ActiveXObject) {
        req = new ActiveXObject("Microsoft.XMLHTTP");
    }
    req.onreadystatechange = function() {
        if (req.readyState == 4) {
            if (req.status < 400) {
                (method=="POST") ? callback(req) : callback(req,data);
            } else {
            }
        }
    }
    if (method=="POST") {
        req.open("POST", url, true);
        if (urlencoded) req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
        req.send(data);
    } else {
        req.open("GET", url, true);
        req.send(null);
    }
   // return req;
}

Request.sendRawPOST = function(url, data, callback) {
    Request.send(url, "POST", callback, data, false);
}
Request.sendPOST = function(url, data, callback) {
    Request.send(url, "POST", callback, data, true);
}
Request.sendGET = function(url,callback) {
    Request.send(url, "GET", callback, null, null);
}

/*分析status文件的内容*/
function parseResponseStatus(documentElement){
     var dataobj = new Object();
	 //parse status
	 var status = documentElement.getElementsByTagName("status");
	 if(status && status.length > 0){
		 if(status[0].firstChild){
			dataobj.status = status[0].firstChild.nodeValue;			 
		 }
		else{
			dataobj.status = "";			 
		}
	 }
	 return dataobj;	
}

//分页
function page(datastr){
	eval("var obj = "+datastr+";");
	dojo.io.bind({
		url: obj.url,
		load: function(type, data, evt) {
			$E(obj.id).innerHTML = data;
		},
		error: function(type, error) { 
			alert("error");
		},
		mimetype: "text/plain"	
	});
}

//提交form，把得到的数据放在指定的ID上.
function ajaxFormRequest(datastr) {
	try{
	
		eval("var obj = "+datastr+";");
		
		if (obj.id == null)
	  		obj.id = "paginationResult";

		dojo.io.bind({
		url: obj.url,
		useCache: false,
		preventCache: false,
		encoding:'UTF-8',
		load: function(type, data, evt) {
			
			if(document.getElementById(obj.id) == null){
		  		return;
		  	}
			$E(obj.id).innerHTML = data;
			
			loadScripts(data);
		},
		
		error: function(type, error) { alert("error" + error);},
		mimetype: "text/plain",
		formNode: $E(obj.form)
		});
	
	}catch(e){
		
	}
}


//Ajax请求
function ajaxRequest(obj) {
	dojo.io.bind({
	url: obj.url,
	useCache: false,
	preventCache: true,
	load: function(type, data, evt) {
	document.getElementById(obj.id).innerHTML=data;
	//dlg.show();
	},
	error: function(type, error) { alert("没有找到该会员");},
	mimetype: "text/plain"
	// and many more options!
	});
}

//将显示分页的结果
function pagination(obj)
{
  if (obj.id == null||obj.id=="null")
  obj.id = "paginationResult";
  document.getElementById(obj.id).innerHTML='<div align="center"> 查询中........</div>';
  ajaxRequest(obj);
}
//分页时,请求到第几页数据
function goPage(dataObject)
{
   var page=1;
   if (document.getElementById('gopage').value != null &&document.getElementById('gopage').value != '')	
     page=document.getElementById('gopage').value;   
   dataObject.url=dataObject.url+'&page='+page;
   pagination(dataObject);
}	

//执行文本字符中的<script>对.
function loadScripts(text) {
	match = new RegExp('(?:<script.*?>)((\n|.)*?)(?:<\/script>)', 'im'); 
	var scripts  = text.match(match); 
	if(scripts && scripts.length){
		for (var i = 0; i < scripts.length; i++){ 
			if(scripts[i] && scripts[i].match(match) && scripts[i].match(match)[1]){
		    	eval(scripts[i].match(match)[1]); 
		    }
		}   
	}  
}

function AjaxCall(location,callback) { // give as many args as you want
	var args = arguments;
	dojo.io.bind({
    	url: location,
    	useCache: false,
		preventCache: true,
    	load: function(type, data, http) {
      		var newArgs = [type, data, http];
      		for(var i = 1; i < args.length; i++) {
        		newArgs.push(args[i]);
      		}
      	callback.apply(this, newArgs);
    }
	});
}


function ajaxNoCacheRequest(obj) {
	dojo.io.bind({
	url: obj.url,
	//disable cache
	useCache: false,
	preventCache: true,
	load: function(type, data, evt) {
	document.getElementById(obj.id).innerHTML=data;
	//dlg.show();
	},
	error: function(type, error) { alert("connection error.");},
	mimetype: "text/plain"
	// and many more options!
	});
}

//利用ajax删除列表
function listRemove(objs) {
	if (!confirm('您真要要进行删除吗?'))
	  return; 
	dojo.io.bind({
    	url: objs.url,
    	mimetype: "text/xml",
    	useCache: false,
		preventCache: true,
    	load: function(type, data, httpreq) {    		  
    	var rss = data.documentElement;
    	var header = {      
            status:0
        };          
      	var i=data.getElementsByTagName('status')[0].firstChild.data;	      	
      	if (i=="1")      	
      	   $Hide(objs.delID);      	         	
      	else
      	 alert(i);       	
      	}
    
	});
}
//打开一个模式窗口
function openWindow(url) {
if (window.showModalDialog)
  window.showModalDialog(url,'','unadorned:yes;dialogWidth:755px;dialogHeight:550px');
else 
  window.open(url,'','width=755,height=550,toolbar=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes ,modal=yes');
}
// 获取XML某个节点中的文本
// 因为IE和Gecko对节点文本的实现方法不同，所以用这个函数进行封装
// IE: node.text ; Gecko: node.textContent
function getXmlNodeText(node) {
   var undefined;
   if (node !== null) {  // 判断节点不为空才获取
       if (undefined !== node.text) {
           return node.text;
       } else if (undefined !== node.textContent) {
           return node.textContent;
       }
   }
   else
     alert('null');   
   return undefined;
}
function getXmlData(xml, obj) {

  for (o in obj) {

    if ("object" == typeof(obj[o])) {            
      getXmlData(xml.getElementsByTagName(o), obj[o]);
    } else {
      obj[o] = getXmlNodeText(xml.getElementsByTagName(o).item(0));

    }

  }

}
function setDate(objId,dojoId){
	document.getElementById(objId).value=dojo.widget.byId(dojoId).inputNode.value;
}

/*金额标签转换函数*/
function money_convert(name){
	
	var formatName = name  + "_format";
	var strValue = document.getElementById(formatName).value;
    strValue=strValue.replace(",","");
    var regex = /^\d+(\.\d{0,2})?$/;
    if(!regex.test(strValue)){
    	document.getElementById(name).value=strValue;
    }else{
      	document.getElementById(name).value=strValue * 1000;
    }
}

/*金额标签转换函数
* 获得焦点时,去掉金额中的','符号
*/
function prepare(obj){
	var strValue = obj.value;
	strValue=strValue.replaceAll(",","");
	obj.value = strValue;
	obj.select();
}

String.prototype.replaceAll  = function(s1,s2){    
return this.replace(new RegExp(s1,"gm"),s2);    
}   

/*日期标签转换函数*/
function setDate(name){
	var formatName = "format_" + name;
    document.getElementById(name).value=dojo.widget.byId(formatName).inputNode.value;
}

String.prototype.trim=function(){
        return this.replace(/(^\s*)|(\s*$)/g, "");
}
	function focusOnDiv(id){
		var oDiv = document.getElementById("AREA" + id);
		if(null != oDiv){
			oDiv.className = "divFocus";
		}
	}	
	function blurOnDiv(id){
		var oDiv = document.getElementById("AREA" + id);
		if(null != oDiv){
			oDiv.className = "divBlur";
		}
	}
	function clearFocus(id){
		var iCount = 6;

		for(var i=0;i<iCount;i++){
			if(i != id){
				blurOnDiv(i);
			}
		}
	}	 
	function onDivFocus(id){
		clearFocus(id);
		focusOnDiv(id);
	}