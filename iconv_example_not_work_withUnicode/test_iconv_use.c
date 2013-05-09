/*
 * 测试iconv函数，从utf8到unicode，未成功
 * Zaks Wang
 * 2013-5-6
 */
#include <stdio.h>
#include <string.h>
#include "setLocale.h"
#include "cross_ChangeCode.h"
//#include "changeCode.h"
#define BOM 0xfeff
#define BUFFSIZE 2048

int main(){
  setUTF8Local();

  FILE * inFIle = fopen("t.txt","r");
  FILE * outFile = fopen("tout.txt","wb");
  wchar_t ch = BOM;
  //fwrite(&ch,sizeof(wchar_t),1,outFile); //写入bom
  char * pBuf = calloc(BUFFSIZE,1);
  while(fgets(pBuf,BUFFSIZE,inFIle)){
    int size = (strlen(pBuf)+1)*4;
    int sizeW = size*2;
    wchar_t *bufW = calloc(size,1);
    wchar_t *bufWstart = bufW;
    char *pBufStart = pBuf;
    int bfsizeW = sizeW;
    FUTF82WConvert(pBuf,bufW,&sizeW);
    bfsizeW-=sizeW;
    char *bufutf16  = calloc(size,1);
    char *bufutf16start = bufutf16;
    FW2UConvert(bufW,&bfsizeW,bufutf16,&size);
    fwrite(bufutf16,sizeof(char),size,outFile);
    free(bufWstart);
    free(bufutf16start);
    pBuf = pBufStart;
  }
  fclose(inFIle);fclose(outFile);free(pBuf);
  return 0;
}
