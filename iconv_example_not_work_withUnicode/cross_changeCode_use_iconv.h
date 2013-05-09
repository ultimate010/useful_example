/*
 * 跨平台的编码转换函数，原作者不详，linux下测试失败
 * Zaks Wang
 * 2013-5-6
 */
#ifndef __CROSS_CHANGECODE_H_
#define __CROSS_CHANGECODE_H_
#ifdef WINDOWS

#include <windows.h>

#include <stdio.h>

#include <ctype.h>

#else

#include <iconv.h>

#include <wctype.h>
#include <wchar.h>
#include <errno.h>

#endif




//wchar_t转成UTF-8
int FW2UTF8Convert( const wchar_t* a_szSrc, int a_nSrcSize, char* a_szDest, int a_nDestSize )
{
#ifdef WINDOWS
  return WideCharToMultiByte( CP_UTF8, 0, a_szSrc, -1, a_szDest, a_nDestSize, NULL, NULL );
#else
  size_t result;
  iconv_t env;
  env = iconv_open("UTF-8","WCHAR_T");
  if (env==(iconv_t)-1)
  {
    printf("iconv_open WCHAR_T->UTF8 error%s %d/n",strerror(errno),errno) ;
    return -1;
  }
  result = iconv(env,(char**)&a_szSrc,(size_t*)&a_nSrcSize,(char**)&a_szDest,(size_t*)&a_nDestSize);
  if (result==(size_t)-1)
  {
    printf("iconv WCHAR_T->UTF8 error %d/n",errno) ;
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}





//UTF-8转成wchar_t
int FUTF82WConvert( const char* a_szSrc, wchar_t* a_szDest, int *a_nDestSize )
{
#ifdef WINDOWS
  return MultiByteToWideChar( CP_UTF8, 0, a_szSrc, -1, a_szDest, *a_nDestSize );
#else
  size_t result;
  iconv_t env;
  int size = strlen(a_szSrc)+1 ;
  env = iconv_open("WCHAR_T","UTF-8");
  if (env==(iconv_t)-1)
  {
    printf("iconv_open UTF8->WCHAR_T error %d/n",errno) ;
    return -1;
  }
  result = iconv(env,(char**)&a_szSrc,(size_t*)&size,(char**)&a_szDest,(size_t*)a_nDestSize);
  if (result==(size_t)-1)
  {
    printf("iconv UTF8->WCHAR_T error %d/n",errno) ;
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}





//wchar_t转成utf16
int FW2UConvert( const wchar_t* a_szSrc, int  *a_nSize,char* a_szDest, int *a_nDestSize )
{
#ifdef WINDOWS
  memcpy_s((wchar_t*)a_szDest,*a_nDestSize,a_szSrc,*a_nSize);
  return *a_nSize ;
#else
  size_t result;
  iconv_t env;
  env = iconv_open("UTF-16LE","WCHAR_T");
  if (env==(iconv_t)-1)
  {
    printf("iconv_open WCHAR_T->UTF16 error%s %d/n", strerror(errno),errno);
    return -1;
  }
  result = iconv(env,(char**)&a_szSrc,(size_t*)a_nSize,(char**)&a_szDest,(size_t*)a_nDestSize);
  if (result==(size_t)-1)
  {
    fprintf(stderr,"iconv WCHAR_T->UTF16 error %s %d/n", strerror(errno), errno);
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}





//utf16转成wchar_t
int FU2WConvert( const  char* a_szSrc, int a_nSize, wchar_t* a_szDest, int a_nDestSize )
{
#ifdef WINDOWS
  memcpy_s(a_szDest,a_nDestSize,(const wchar_t*)a_szSrc,a_nSize);
  return a_nSize ;
#else
  size_t result;
  iconv_t env;
  env = iconv_open("UCS-4-INTERNAL","UCS-2-INTERNAL");
  if (env==(iconv_t)-1)
  {
    printf("iconv_open error %d/n",errno) ;
    return -1;
  }
  result = iconv(env,(char**)&a_szSrc,(size_t*)&a_nSize,(char**)&a_szDest,(size_t*)&a_nDestSize);
  if (result==(size_t)-1)
  {
    printf("UTF16 -> WCHAR_T conv error %d/n",errno) ;
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}
#endif
