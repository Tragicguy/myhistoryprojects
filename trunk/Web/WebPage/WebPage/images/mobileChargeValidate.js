function checkform()
{   var strname1 = fm1.UserName.value.length;
    var strname2 = fm1.UserName2.value.length;
	if(fm1.UserName.value=="")
	{
		alert("请输入手机号码！");
		document.fm1.UserName.focus();
		return false;
	}
	if (isNaN(fm1.UserName.value))
	{
	alert('只能输入数字！');
	document.fm1.UserName.focus();
	return false;
    }
     if(strname1 != 11 || strname2!= 11 )
      {
        alert("请输入11位数字！");
		
        return false;
      }
	if(fm1.UserName2.value=="")
	{
		alert("请确认手机号码！");
		document.fm1.UserName2.focus();
		return false;
	}
	if (isNaN(fm1.UserName2.value))
	{
	alert('只能输入数字！');
	document.fm1.UserName2.focus();
	return false;
    }
	if(fm1.UserName2.value !=fm1.UserName.value)
	{
		alert("两次输入的手机号码不同，请更正！");
		document.fm1.UserName.focus();
		return false;
	}
var qqren,qqbank,qqnum,qqmob;

qqmob=fm1.UserName2.value;
f = document.getElementById("fm1");
for(var i = 0 ; i < f.bank_type.length; i++)
        {
          if(f.bank_type[i].checked)
          {
qqbank=f.bank_type[i].value;
          }else{
//            alert("not checked "+f.bank_type[i].value);
        }
        }

for(var i = 0 ; i < f.price.length; i++)
        {
          if(f.price[i].checked)
          {
qqnum=f.price[i].value;
          }else{
//            alert("not checked "+f.bank_type[i].value);
        }
        }

//	 return confirm("请仔细核对您的手机号码、运营商，填写错误将导致您无法充值到您的手机中，本站概不负责！"); 
return true;
}

//提交时验证form元素

//动态更改账户余额显示
function changeAcct(value){
	if('0' == value){
		showOrHidden(false,false,false);
	}else {
		for(i=0;i<arry.length-2;i++){
			if(value == arry[3*i]){
				if(arry[i*3+2] == '1'){
					showOrHidden(true,false,true);
				}else{
					showOrHidden(true,true,false);
				}
				$E('balance').innerText = arry[i*3+1]
			}
		}
	}
}

//根据参数判断页面元素显示或隐藏
function showOrHidden(b_acctBalance,b_rand,b_pwd){

	if(b_acctBalance){
		$E('acctBalance').style.display = '';
	}else{
		$E('acctBalance').style.display = 'none';
	}
	
	if(b_rand){
		$E('szx_rand').style.display = '';
	}else{
		$E('szx_rand').style.display = 'none';
	}
	
	if(b_pwd){
		$E('szx_pwd').style.display = '';
	}else{
		$E('szx_pwd').style.display = 'none';
	}
}

function setPayAmount(discount,value,obj){
$E(obj).innerText = discount*10000*value/10000;
}

//去掉字符串首尾空格
String.prototype.trim = function() 
{ 
	return this.replace(/(^\s*)|(\s*$)/g, ""); 
}