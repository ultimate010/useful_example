#ifndef getFileName_h
#define getFileName_h
#include    <dirent.h>
#include    <sys/stat.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#if defined(DEBUG)
#include "leak_detector_c.h"
#endif


typedef struct node{
  struct node * next;
  char fileName[256];
} _fileNameNode;
void freefileNameList(_fileNameNode *head){
  _fileNameNode * pCur;
  while(pCur = head){
    head = head->next;
    free(pCur);
  }
}


_fileNameNode * getFileNameByDir(const char*dir){
  _fileNameNode * head = NULL,* pCurrent,* pLast;
  DIR *dp;
  struct dirent *entry;
  struct stat   statbuf;
  /*打开目录 */
  if((dp=opendir(dir))==NULL){
    perror("opendir error");
    return NULL;
  }
  /*进入目录*/
  chdir(dir);
  while((entry = readdir(dp))!=NULL){
    lstat(entry->d_name,&statbuf);
    if(S_ISDIR(statbuf.st_mode)){
      /*是目录*/
    }else{
      /*返回非隐藏文件*/
      if(entry->d_name[0]!= '.'){
        /*doing something*/
        //printf("%s\n %d \n",entry->d_name,sizeof(entry->d_name));
        pCurrent = (_fileNameNode *)malloc(sizeof(_fileNameNode));
        pCurrent->next = NULL;
        strcpy(pCurrent->fileName,entry->d_name);
        if(head==NULL){
          head = pLast = pCurrent;
        }else{
          pLast->next = pCurrent;
          pLast = pCurrent;
        }
      }
    }
  }
  chdir("..");
  closedir(dp);
  return head;
}
/*
int main(void){
  _fileNameNode * pFile,* pCur;
  pCur = pFile = getFileNameByDir("lib");
  while(pCur){
    printf("%s\n",pCur->fileName);
    pCur = pCur->next;
  }
  freefileNameList(pFile);
  pFile = pCur = NULL;
  return 0;
}
*/
#endif
