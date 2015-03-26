unit uWebInc;
interface

const
  RSPSIMPLESERVICE = 1;
  RSPUNREGISTERSERVICE = 0;
  //获得绝对位置
  offsetJsStr = 'function Offset(id){var e=document.getElementById(id);var t = e.offsetTop;var l = e.offsetLeft;'+
                'var w = e.offsetWidth;var h = e.offsetHeight - 2;while(e = e.offsetParent){'+
                't += e.offsetTop;l += e.offsetLeft;}return l+","+t}';
  //输入关键字
  setKeyWords = 'function setKeyWords(id){document.getElementById(id).value="%s"}';
  //通过web地址获得<a>元素的定位
  getElementPOffsetByUrl = 'function getElementPOffsetByUrl(Url){var i;var Elmts = document.getElementsByTagName("FONT");' +
                    'for (i = 0; i < Elmts.length; i++){if (Elmts.item(i).innerText.indexOf(Url, 0) >= 0) ' +
                    '{var f = Elmts.item(i);var e = f.parentNode.firstChild;var t = e.offsetTop;var l = e.offsetLeft;'+
                    'var w = e.offsetWidth;var h = e.offsetHeight - 2;while(e = e.offsetParent){'+
                    't += e.offsetTop;l += e.offsetLeft;}return l+","+t}}}';
  //获得相对位置
  getElementPosByUrl = 'function getElementPosByUrl(Url){var i;var Elmts = document.getElementsByTagName("FONT");' +
                    'for (i = 0; i < Elmts.length; i++){if (Elmts.item(i).innerText.indexOf(Url, 0) >= 0) ' +
                    '{var f = Elmts.item(i);var e = f.parentNode.firstChild;var l = e.offsetLeft;var c = e.offsetParent;'+
                    'while (c !== null){l += c.offsetLeft;c = c.offsetParent;'+
                    '};if (document.compatMode == "BackCompat"){var esl=document.body.scrollLeft;'+
                    '}else{var esl=document.documentElement.scrollLeft;};'+
                    'var t = e.offsetTop;c = e.offsetParent;'+
                    'while (c !== null){t += c. offsetTop;c = c.offsetParent;};'+
                    'if (document.compatMode == "BackCompat"){var est=document.body.scrollTop;'+
                    '}else{var est=document.documentElement.scrollTop;'+
                    '};return ((l-esl)+","+(t-est));}}}';
  getBrowserVersion = 'function getVer(){return parseFloat(navigator.appVersion.split("MSIE")[1]);}';

  test = 'function test(){return s;}';

  bodyScrollTop = 'function bodyScrollTop(pos){document.documentElement.scrollTop = pos;}';
implementation
end.
