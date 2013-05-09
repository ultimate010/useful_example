/*
 * 手工读取ucs2(unicode)的文件,换行
 * Zaks Wang
 * 2013-5-6
 */
#ifndef __READLINE_UNICODE_H
#define __READLINE_UNICODE_H
#include <string.h>
#define LF 0x000A  //定义回车
#define CR 0x000D  //定义换行
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
#endif
