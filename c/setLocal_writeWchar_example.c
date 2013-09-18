/*
 * 设置local和宽字节写入例子，代码原作者不详
 * Zaks Wang修改其中bug，加入写入文件测试
 * 2013-5-5
 */
#ifdef __GNUC__

#define CSET_GBK    "GBK"
#define CSET_UTF8   "UTF-8"

#define LC_NAME_zh_CN   "zh_CN"

// ifdef __GNUC__
#elif defined(_MSC_VER)

#define CSET_GBK    "936"
#define CSET_UTF8   "65001"

#define LC_NAME_zh_CN   "Chinese_People's Republic of China"

// ifdef _MSC_VER
#endif

#define LC_NAME_zh_CN_GBK       LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8      LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

void setUTF8Local(){
  const char* locname = NULL;
  locname = setlocale(LC_ALL,LC_NAME_zh_CN_UTF8);
  if ( NULL == locname ){
    printf("setlocale() with %s failed.\n", LC_NAME_zh_CN_UTF8);
  }else{
    printf("setlocale() with %s succeed.\n", LC_NAME_zh_CN_UTF8);
  }
}
void setGBKLocal(){
  const char* locname = NULL;
  locname = setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
  if ( NULL == locname ){
    printf("setlocale() with %s failed.\n", LC_NAME_zh_CN_DEFAULT);
  }else{
    printf("setlocale() with %s succeed.\n", LC_NAME_zh_CN_DEFAULT);
  }
}


int main(int argc, char* argv[])
{
  char* locname = NULL;
  const wchar_t* strzh = L"中文字符串";
  // 使用指定的 locale
  locname = setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);
  /*
   * 设置指定为中文utf8，程序中的unicode会重定向到utf8,
   * 也就是说输出文件也是utf8
   * 注意：代码中不能同时出现printf和wprintf函数，
   * 为了一致，采用printf向标准输出流输出，
   * 用fwprintf像文件写入
   */
  if ( NULL == locname )
  {
    printf("setlocale() with %s failed.\n", LC_NAME_zh_CN_UTF8);
  }
  else
  {
    printf("setlocale() with %s succeed.\n", LC_NAME_zh_CN_UTF8);
  }
  FILE * file = fopen("/tmp/tst.file","w");
  if(file==NULL){
    printf("Open file failed\n");
  }
  wchar_t BOM = 0xFEFF;
  //fputwc(BOM,file);
  wchar_t word = 0x4E00;
  for(;word<0x9FA5;word++){
    fwprintf(file,L"%lc\n",word);
  }
  fflush(file);
  fclose(file);
  return 0;

}
