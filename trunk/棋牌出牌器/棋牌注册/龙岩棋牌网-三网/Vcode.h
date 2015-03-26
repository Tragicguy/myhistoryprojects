//导入函数表静态库文件
//#pragma comment(lib, "Vcode.lib")

extern "C"
{
//载入一个新的验证码库
	int _stdcall LoadDe(char* strFilePath,char* strFilePath2);

//取验证码库信息
	int _stdcall DeInfo(char* Info);

//取验证码库错误信息
	int _stdcall DeError(char* Info);


//PicPath:验证码图片的URL
	int _stdcall GetUrlVcode(char* PicPath,int Index,char* result);


//PicPath:可以只是包含验证码URL的一部分特征文本
	int _stdcall GetWebVcode(void* WebHWnd,char* PicPath,int Index,char* result);


	int _stdcall GetFileVcode(char* PicName,int Index,char* result);


	int _stdcall GetByteVcode(void* ByteData,int ByteLength,int Index,char* result);

	int _stdcall GetRecNum(void* ImageDate,int ImageLength,int BYTEflag,char* result);
}

