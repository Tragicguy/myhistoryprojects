

//使用时,先在外面分配好空间, 解压后的内容就放在里面
//int gzuncompress(char* filename,char*&buf,int &len);

int gzuncompress( string & strBuffer, int & nlen);