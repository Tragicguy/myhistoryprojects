// JScript File
function Search()
{
  var str = document.getElementById("key").value;
  if(str.length==0) return false;
  var url = "http://search.csdn.net/search/";
  if(window.encodeURIComponent)
    url += encodeURIComponent(str) +"/1/lt/";
  else
    url += (str) +"/1/lt/";

  var form = document.createElement("FORM");
  form.target = "_blank";
  form.method = "GET";
  form.action = url;
  document.body.appendChild(form);
  form.submit();
  setTimeout(function(){form.parentNode.removeChild(form);}, 50)
  return false;
}

function Collect()
{
  var d=document, keyit;
  var t=d.selection?(d.selection.type!='None'?d.selection.createRange().text:''):(d.getSelection?d.getSelection():'');
  void(keyit=window.open('http://www.365key.com/storeit.aspx?t='+escape(d.title)
    +'&amp;u='+escape(d.location.href)+'&amp;c='+escape(t),'keyit',
    'scrollbars=no,width=475,height=575,left=75,top=20,status=no,resizable=yes'));
  keyit.focus();
  return false;
}
function Print(){window.print(); return false;}
function Close(){window.opener="meizz"; window.close(); return false;}

//google ad
google_ad_client = "pub-1076724771190722";
google_alternate_ad_url = "http://topic.csdn.net/include/google_default_160_600.htm";
google_ad_width = 160;
google_ad_height = 600;
google_ad_format = "160x600_as";
google_ad_type = "text_image";
google_color_border = "FFFFFF";
google_color_bg = "FFFFFF";
google_color_link = "0000CC";
google_color_url = "008000";
google_color_text = "6F6F6F";

window.onload = function()
{ 
  if (!document.getElementsByTagName) return; 
  var anchors = document.getElementsByTagName("a"); 
  for (var i=0, n=anchors.length; i<n; i++)
  { 
    if(anchors[i].getAttribute("href") && 
       anchors[i].getAttribute("rel") == "external") 
       anchors[i].target = "_blank"; 
  } 
} 
