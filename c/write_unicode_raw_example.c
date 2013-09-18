/*
 * 测试手工输出unicode
 * Zaks Wang
 * 2013-5-5
 *
 */


#include <stdio.h>
#define WCHAR_T (unsigned short)

int main(){
  FILE * file = fopen("/tmp/tst.file","wb");
  WCHAR_T myChar = 0;
  myChar = 0xFEFF;
  fwrite(&myChar,sizeof(WCHAR_T),1,file);
  WCHAR_T *huanhang = (WCHAR_T*)L"\n";
  for(myChar=0x4E00;myChar<0x9FA5;++myChar){
    fwrite(&myChar,sizeof(WCHAR_T),1,file);
    fwrite(huanhang,sizeof(WCHAR_T),1,file);
  }
  fflush(file);
  fclose(file);
  return 0;
}
