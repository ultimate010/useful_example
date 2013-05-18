/*
 * 根据分好词的句子创建索引
 * Zaks wang
 * 2013-5-16
 */
#include "getFileName.h"
#include "hashmap.h"
#include <assert.h>
#if defined(DEBUG)
#include "leak_detector_c.h"
#endif


#define INPUTPATH "/home/nlp/projects/map_test/"
#define INDEXFILE "/home/nlp/projects/pattern_mining/index.bin"
#define TEMPPATH "/home/nlp/temp.buildIndex."  //临时文件夹
#define MAXKEYLEN (32)
#define BUFFERSIZE (256)
#define BUCKETSIZE (200000ul)
static int CURRENTINDEX=0; //

/*
 * 结构如下
 |---------------|      |------------------|       |-------------------|
 |map_t:  map    |----->|int:   wordId     |------>|int:    wordId     |
 |int:  wordCount|      |map_t: nextWordMap|       |int:    count      |
 |---------------|      |int:   count      |       |floate: probability|
 *                      |float: probability|       |-------------------|
 *                      |------------------|
 */
typedef struct{
  char wordId[MAXKEYLEN];
  map_t nextWordMap;
  int count;
}WordNode_t;

typedef struct{
  char wordId[MAXKEYLEN];
  int count;
  float probability;
}NextWordNode_t;

typedef struct{
  map_t map;
  long wordCount;
}Head_t;

void doBuiltIndex(const char * fileName,FILE *indexFile,Head_t *head);
int freeNode(any_t item,any_t data); //删除结点
int freeWordNode(any_t item,any_t data); //删除单词结点
void printIndex(FILE* fileOut,Head_t *head);
int printNode(any_t item,any_t data); //输出信息
int printNextNode(any_t item,any_t data); //打印后接单词
void iniMap(Head_t *head); //新建
void freeMap(Head_t *head); //删除


int main(){
#if defined(DEBUG)
  atexit(report_mem_leak);
#endif
  _fileNameNode *pFiles = getFileNameByDir(INPUTPATH); //获取要处理的输入文件
  _fileNameNode *pCur = pFiles;
  FILE * indexFile = fopen(INDEXFILE,"wb");
  if(indexFile==NULL) exit(-1);
  Head_t head;
  while(pCur){
    printf("Processing %s\n",pCur->fileName);
    iniMap(&head); //新建map
    doBuiltIndex(pCur->fileName,indexFile,&head);
    freeMap(&head); //删除map
    pCur=pCur->next; //处理下一个
  }
  freefileNameList(pFiles);
  fclose(indexFile);
  pFiles=pCur=NULL;
  return 0;
}
void freeMap(Head_t *head){
  hashmap_iterate(head->map,freeWordNode,(void *)NULL); //遍历删除结点
  hashmap_free(head->map);
}
void iniMap(Head_t *head){
  head->wordCount=0;
  head->map=hashmap_new(BUCKETSIZE);
  if(!head->map){
    fprintf(stderr,"No mem\n");
    exit(-1);
  }
}
int freeWordNode(any_t item,any_t data){
  //删除单词结点
  WordNode_t *p = (WordNode_t*)data;
  map_t * nextMap = p->nextWordMap;
  hashmap_iterate(nextMap,freeNode,(void *)NULL); //遍历后接结点，并删除
  hashmap_free(nextMap);
  free(p);
  return MAP_OK;
}
int freeNode(any_t item,any_t data){ //删除结点
  NextWordNode_t *p = (NextWordNode_t*)data;
  free(p);
  return MAP_OK;
}
void doBuiltIndex(const char * fileName,FILE *indexFile,Head_t *head){
  char temp[256];
  char *buffer=(char*)malloc(sizeof(char)*BUFFERSIZE);
  assert(buffer);
  sprintf(temp,"%s%s",INPUTPATH,fileName);
  FILE *fileIn=fopen(temp,"r"); //打开输入文件
  assert(fileIn);
  sprintf(temp,"%s%d",TEMPPATH,CURRENTINDEX++); //临时文件夹
  FILE *fileOut=fopen(temp,"w"); //打开输出文件
  assert(fileOut);
  memset(buffer,0,BUFFERSIZE);
  char *pI,*pJ;
  int errorCode = -1;
  char preWordId[MAXKEYLEN]={0},curWordId[MAXKEYLEN]={0};
  while(fgets(buffer,BUFFERSIZE,fileIn)){
    //每一行的开始
    memset(preWordId,0,MAXKEYLEN),memset(curWordId,0,MAXKEYLEN);
    pI=pJ=buffer;
    while(*pI!='\0'&&*pI!='\n'&&*pI!='\r'){
      while(*pI=='\t'||*pI==' '){
        pJ++,pI++; //跳过开始的空白
      }
      if(*pJ=='\r'||*pJ=='\n'||*pJ=='\0') break; //处理到末尾
      while(*pI!=' '&&*pI!='\t'&&*pI!='\n'&&*pI!='\0'&&*pI!='\r')pI++;
      memset(curWordId,0,MAXKEYLEN);
      strncpy(curWordId,pJ,pI-pJ);
      pJ=pI;
      head->wordCount+=1; //总单词计数+1
      WordNode_t * ptemp = NULL;
      //stepOne:修改当前词节点
      if(hashmap_get(head->map,curWordId,(any_t *)&ptemp)==MAP_MISSING){
        //第一次碰到这个词，首先新建一个节点，并且把该节点加入
        ptemp = (WordNode_t*)malloc(sizeof(WordNode_t));
        assert(ptemp); //不能为空
        memset(ptemp->wordId,0,MAXKEYLEN);
        strcpy(ptemp->wordId,curWordId);
        ptemp->count=1;
        ptemp->nextWordMap = hashmap_new(BUFFERSIZE);
        if(!ptemp->nextWordMap){
          fprintf(stderr,"No mem\n");
          exit(-1);
        }
        errorCode=hashmap_put(head->map,ptemp->wordId,ptemp);
        assert(errorCode==MAP_OK); //插入成功
      }else{
        //存在
        assert(ptemp);
        ptemp->count+=1; //出现次数加一
      }
      //stepTwo:把当前词加入前一个词的节点
      if(*preWordId=='\0'){//如果第一个词的话
      }else{
        //先获取前一个词
        errorCode=hashmap_get(head->map,preWordId,(any_t *)&ptemp);
        assert(errorCode==MAP_OK);
        assert(ptemp->nextWordMap); //测试不为空
        //stepOne:查看是否存curword这个后续节点
        NextWordNode_t * nextWord = NULL;
        if(hashmap_get(ptemp->nextWordMap,curWordId,(any_t *)&nextWord)==MAP_MISSING){
          assert(nextWord==NULL);
          nextWord = (NextWordNode_t*)malloc(sizeof(NextWordNode_t));
          assert(nextWord);
          strcpy(nextWord->wordId,curWordId);
          nextWord->count=1;
          errorCode=hashmap_put(ptemp->nextWordMap,nextWord->wordId,nextWord);
          assert(errorCode==MAP_OK);
        }else{
          //存在的话就把数据+1
          assert(nextWord);
          nextWord->count+=1;
        }
      }
      memset(preWordId,0,MAXKEYLEN);
      strcpy(preWordId,curWordId);
    }

    //fgets(buffer,BUFFERSIZE,fileIn); //跳过第二行
    memset(buffer,0,BUFFERSIZE);
  }
  //建索引结束
  //输出索引
  printIndex(fileOut,head);
  free(buffer);
  fclose(fileOut);
  fclose(fileIn);
}
void printIndex(FILE* fileOut,Head_t *head){
  assert(fileOut);
  assert(head);
  //先打印总单词个数
  fprintf(fileOut,"totalWordsCount:%d\n\n",head->wordCount);
  //然后打印单词
  hashmap_iterate(head->map,printNode,fileOut); //输出
}
//打印结点
int printNode(any_t file,any_t data){
  FILE *fileOut = (FILE *) file;
  assert(fileOut);
  WordNode_t * node=(WordNode_t*)data;
  assert(node);
  //打印单词id和单词出现个数
  fprintf(fileOut,"currentWord:%s\t%d\n\n",node->wordId,node->count);
  //首先打印后接单词个数
  assert(node->nextWordMap);
  fprintf(fileOut,"NextWordCoutn:%d\n",hashmap_length(node->nextWordMap));
  hashmap_iterate(node->nextWordMap,printNextNode,file);
  return MAP_OK;
}

int printNextNode(any_t file,any_t data){
  FILE* fileOut=(FILE*)file;
  NextWordNode_t * nextNode= (NextWordNode_t*)data;
  assert(nextNode);
  fprintf(fileOut,"nextWord:%s\t%d\n",nextNode->wordId,nextNode->count);
  return MAP_OK;
}

