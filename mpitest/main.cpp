/*
 * Master Slave Framework
 * Zaks Wang
 * 2013-9-18
 */
#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define COM_TAG 0
#define MES_TAG 1
#define MES_FINISH 2

int G_COUNT = 0;
bool hasNext();
void next();
int master();
int slave();
int main(int argc,char **argv)
{
  int node,nodeSize;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&node);
  MPI_Comm_size(MPI_COMM_WORLD,&nodeSize);
  if(node == 0){
    master();
  }else{
    slave();
  }
  MPI_Finalize();
  return 0;
}
int master(){
  int size,rank,nslave,buf;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  nslave = size-1;
#ifdef _INFO
  cout <<"****************************************\n";
  cout <<"\tMaster:\t Slave size: " <<nslave <<endl;
  cout <<"****************************************\n";
#endif
  while(nslave>0){
#ifdef _INFO
    cout <<"Master:\t**************Waiting for request**************\n";
#endif
    MPI_Recv(&rank,1,MPI_INT,MPI_ANY_SOURCE,COM_TAG,MPI_COMM_WORLD,&status);
#ifdef _INFO
    cout <<"Master:\tReceive " <<rank <<" request\n";
#endif
    if(hasNext()){
#ifdef _INFO
      cout <<"Master:\tTry to assign task to " <<rank <<endl;
#endif
      MPI_Send(&rank,1,MPI_INT,rank,COM_TAG,MPI_COMM_WORLD);
      MPI_Send((void *)&G_COUNT,sizeof(G_COUNT),MPI_CHAR,rank,MES_TAG,MPI_COMM_WORLD);
#ifdef _INFO
      cout <<"Master:\tAlready assign task to " <<rank <<endl;
#endif
      next();
    }else{
#ifdef _INFO
      cout <<"Master:\t**********No task, send exit message to " <<rank <<"***********" <<endl;
#endif
      MPI_Send(&rank,1,MPI_INT,rank,MES_FINISH,MPI_COMM_WORLD);
      nslave--;
    }
  }
  return 0;
}
int slave(){
  char buf[256];
  MPI_Status status;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  char temp[20] = {0};
  sprintf(temp,"%d",rank);
  string path(temp);
  path = "log.node" + path;
  ofstream out(path.c_str());
  while(1){
    /*轮询获取消息*/
    MPI_Send(&rank,1,MPI_INT,0,COM_TAG,MPI_COMM_WORLD);
#ifdef _INFO
    out <<"Slave:\t" <<rank <<" send request to master\n";
#endif
    MPI_Recv(&rank,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
#ifdef _INFO
    out <<"Slave:\t" <<rank <<" get response from master\n";
#endif
    if( status.MPI_TAG == MES_FINISH ){
#ifdef _INFO
      out <<"Slave:\t" <<rank <<" receive exit message\n";
#endif
      break;
    }else{
      MPI_Recv(buf,256,MPI_CHAR,0,MES_TAG,MPI_COMM_WORLD,&status);
#ifdef _INFO
      out <<"Slave:\t" <<rank <<" receive task " <<(int)*buf << endl;
      out <<"Slave:\t" <<rank <<" doing task " <<(int)*buf <<"\n";
#endif
      for(int i = 0;i < 1000;i++);
    }
  }
  return 0;
}

bool hasNext(){
  if(G_COUNT > 10){
    return false;
  }
  return true;
}
void next(){
  G_COUNT++;
}
