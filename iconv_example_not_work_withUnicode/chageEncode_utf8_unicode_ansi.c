/*
 * 注意为了是wchar_t在linux下也是两字节，gcc要加上-fshort-wchar编译选项，不过函数需要四
 * 字节输入，未成功
 * Zaks Wang
 * 2013-5-6
 *
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef  _MSC_VER
#include <windows.h>
#endif
#include <string.h>
#include <locale.h>
#define BUFF_SIZE 1024
#define LF 0x000A  //定义回车
#define CR 0x000D  //定义换行
#ifdef _MSC_VER
wchar_t * ANSIToUnicode( const char* str )
{
     int textlen ;
     wchar_t * result;
     textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 );
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));
     memset(result,0,(textlen+1)*sizeof(wchar_t));
     MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen );
     return result;
}
char * UnicodeToANSI( const wchar_t* str )
{
     char* result;
     int textlen;
     textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
     result =(char *)malloc((textlen+1)*sizeof(char));
     memset( result, 0, sizeof(char) * ( textlen + 1 ) );
     WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );
     return result;
}
wchar_t * UTF8ToUnicode( const char* str )
{
     int textlen ;
     wchar_t * result;
     textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 );
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));
     memset(result,0,(textlen+1)*sizeof(wchar_t));
     MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen );
     return result;
}
char * UnicodeToUTF8( const wchar_t* str )
{
     char* result;
     int textlen;
     textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );
     result =(char *)malloc((textlen+1)*sizeof(char));
     memset(result, 0, sizeof(char) * ( textlen + 1 ) );
     WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
     return result;
}
#endif
#ifdef __GNUC__
/*宽字符转换为多字符Unicode - ANSI*/
char* w2m(const wchar_t* wcs)
{
      int len;
      char* buf;
      len =wcstombs(NULL,wcs,0);
      if (len == 0)
          return NULL;
      buf = (char *)malloc(sizeof(char)*(len+1));
      memset(buf, 0, sizeof(char) *(len+1));
      len =wcstombs(buf,wcs,len+1);
      return buf;
}
/*多字符转换为宽字符ANSI - Unicode*/
wchar_t* m2w(const char* mbs,int *wchar_len)
{
      int len;
      wchar_t* buf;
      len =mbstowcs(NULL,mbs,0);
      if (len == 0)
          return NULL;
      buf = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));
      memset(buf, 0, sizeof(wchar_t) *(len+1));
      len =mbstowcs(buf,mbs,len+1);
      *wchar_len = len;
      return buf;
}
#endif
#ifdef _MSC_VER
char* ANSIToUTF8(const char* str)
{
     return UnicodeToUTF8(ANSIToUnicode(str));
}
char* UTF8ToANSI(const char* str)
{
     return UnicodeToANSI(UTF8ToUnicode(str));
}
#endif
/*
 * 读取unicode行，如果换行碰到换行符返回
 * @return 如果文件到末尾且没有有用字符则返回-1，否则返回当前行的字符数
 * 注意：不会返回换行符
 */
int readUnicodeLine(unsigned short * buffer,int bufLen,FILE * input){
  memset(buffer,0,sizeof(unsigned short)*bufLen);//先清空
  unsigned short word = 0;
  int count = 0;
  while(!feof(input)&&count<bufLen){
    //为读完且buffer未满
    if(fread(&word,sizeof(unsigned short),1,input)!=1){
      break; //到末尾了
    }
    if(word==CR||word==LF){
      *(buffer+count) = word;count++;
      break;
    }else{
     *(buffer+count) = word; count++;
    }
  }
  if(feof(input)&&count==0){
    return -1;
  }
  return count;
}
int main()
{
     /*使用wcstombs和mbstowcs之前必须调用setlocale，以便决定内码*/
     setlocale(LC_ALL,"zh_CN.utf8"); //默认设置为utf8，即转换为utf8的多字节
     /*假定有一个Unicode(UTF-16LE)编码的文件，将其打开，重新编码为ANSI
，写入aa.txt中，再继续编码回Unicode，写入aw.txt中*/
     /*如果不存在a.txt文件，则程序出错，没有做错误处理*/
     char* filename = "text.file.uni";
     char* filenamea = "aa.txt";
     char* filenamew = "aw.txt";
     FILE*     input=fopen( filename, "rb");
     FILE*     inputa=fopen( filenamea, "wb");
     FILE*     inputw=fopen( filenamew, "wb");
     /*BOE设置，UTF-16LE的BOE为FEFF，如果不先将其读取出来，wcstombs会调用失败*/
     unsigned short BOM ;
     fread(&BOM,sizeof(unsigned short),1,input);
     fwrite(&BOM,sizeof(unsigned short),1,inputw);
     unsigned short word = 0;
     unsigned short *buffer = (unsigned short *)malloc(sizeof(unsigned short)*BUFF_SIZE);
     unsigned short *pBuffer = buffer;
     printf("%d\n",sizeof(wchar_t));
     int count = 0;
     /*开始读取文件*/
     while((count=readUnicodeLine(buffer,BUFF_SIZE,input))!=-1)
     {
       if(count>0){
         //当前行有数据
         char *buf = w2m(buffer);
         printf("%s\n",buf);
         fprintf(inputa,"%s\n",buf);
         int len;
         wchar_t * bufWideChar = m2w(buf,&len);
         fwrite(buffer,sizeof(wchar_t),count,inputw);
         free(bufWideChar);
         free(buf);
       }
     }
     /*后续处理*/
     free(buffer);
     fclose(input);
     fclose(inputa);
     fclose(inputw);
     return 0;
}
