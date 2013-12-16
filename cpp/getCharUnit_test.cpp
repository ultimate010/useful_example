/*
 * getCharUnit 的测试
 */
#include "getCharUnit.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc,char **argv){
  string str = "This \ti\ta s a test\r\n";
  cout <<"The str is : " <<str <<"End" <<endl;
  cout <<"The vector of sentence is " <<endl;
  vector<string> myvec = segSentence(str,string(" "));
  for(vector<string>::iterator iter = myvec.begin();iter != myvec.end();iter++){
    cout <<*iter <<endl;
  }

  cout <<"The length of str utf8 is: " <<getSentLen(str.c_str(),CHAR_UTF8) <<endl;
  cout <<"The length of str gbk is: " <<getSentLen(str.c_str(),CHAR_GBK) <<endl;
  str = delEnter(str);
  cout <<"After delete enter" <<endl;
  cout <<"The str is : " <<str <<"End" <<endl;
  cout <<"The length of str utf8 is: " <<getSentLen(str.c_str(),CHAR_UTF8) <<endl;
  cout <<"The length of str gbk is: " <<getSentLen(str.c_str(),CHAR_GBK) <<endl;

  str = "这是中 文测试\r\n";
  cout <<"The str is : " <<str <<"End" <<endl;
  cout <<"The length of str utf8 is: " <<getSentLen(str.c_str(),CHAR_UTF8) <<endl;
  cout <<"The length of str gbk is: " <<getSentLen(str.c_str(),CHAR_GBK) <<endl;
  str = delEnter(str);
  cout <<"After delete enter" <<endl;
  cout <<"The str is : " <<str <<"End" <<endl;
  cout <<"The length of str utf8 is: " <<getSentLen(str.c_str(),CHAR_UTF8) <<endl;
  cout <<"The length of str gbk is: " <<getSentLen(str.c_str(),CHAR_GBK) <<endl;

  if(argc != 2){
    cerr <<"Usage: inputfile.gbk outputfile.gbk" <<endl;
    return -1;
  }
  ifstream input(argv[1]);
  ofstream output(argv[2]);
  if(input.fail() || output.fail()){
    cerr <<"Can not open inputfile or outputfile" <<endl;
    return -1;
  }
  string line;
  while(!input.eof()){
    getline(input,line);
    str = line;
    output <<"The str is : " <<str <<"End" <<endl;
  //  cout <<"The length of str utf8 is: " <<getSentLen(str.c_str(),CHAR_UTF8) <<endl;
    output <<"The length of str gbk is: " <<getSentLen(str.c_str(),CHAR_GBK) <<endl;
    str = delEnter(str);
    output <<"After delete enter" <<endl;
    output <<"The str is : " <<str <<"End" <<endl;
   // cout <<"The length of str utf8 is: " <<getSentLen(str.c_str(),CHAR_UTF8) <<endl;
    output <<"The length of str gbk is: " <<getSentLen(str.c_str(),CHAR_GBK) <<endl;
  }
  return 0;
}
