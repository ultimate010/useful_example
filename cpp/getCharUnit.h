#ifndef __GETCHARUNIT_H__
#define __GETCHARUNIT_H__
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <iostream>

using namespace std;


enum{
  CHAR_GBK, //For gbk
  CHAR_UTF8, //For utf8
};

/*  Function define */
void pstring(string& s);
void pstring(char * str);

vector<string> segSentence(const string & sent,const string & pattern);

string delEnter(const string & sent);
void string_replace(string & strBig, const string & strsrc, const string &strdst);

int getSentLen(const char * sent,const char char_type);
int getCharUnitLen(const char * sent,const char char_type);

int _getWordLenGBK(const char * word);
int _getSentLenGBK(const char * sent);
int _getWordLenUTF8(const char *src);
int _getSentLenUTF8(const char *src);

/*  For debug gdb to print string or char */
void pstring(string& s){
  for (int i = 0; i < s.length(); i ++){
    int len = 0;
    if(s[i] < 0) {// multi-bytes utf-8 stream
      unsigned char h = static_cast<unsigned char>(s[i]);
      if(h >> 4 == 0x0E) len = 3; // 1110XXXX 10XXXXXX 10XXXXXX
      if(h >> 5 == 0x06) len = 2; // 110XXXXX 10XXXXXX
      if(h >> 3 == 0x1E) len = 4; // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
      if(h >> 2 == 0x3E) len = 5; // 111110xx 10XXXXXX 10XXXXXX 10XXXXXX10XXXXXX
      if(h >> 1 == 0x7E) len = 6; // 1111110x 10XXXXXX 10XXXXXX 10XXXXXX10XXXXXX 10XXXXXX
    }
    else if (s[i] == 0)
    return;
    else len = 1; // ascii, 0XXXXXXX
    string tmp = s.substr(i,len);
    tmp += "\0";
    cout<<tmp;
  }
  cout<<'\n';
  fflush(stdout);
}

void pstring(char * str){
  string s=str;
  for (int i = 0; i < s.length(); i ++){
    int len = 0;
    if(s[i] < 0) {// multi-bytes utf-8 stream
      unsigned char h = static_cast<unsigned char>(s[i]);
      if(h >> 4 == 0x0E) len = 3; // 1110XXXX 10XXXXXX 10XXXXXX
      if(h >> 5 == 0x06) len = 2; // 110XXXXX 10XXXXXX
      if(h >> 3 == 0x1E) len = 4; // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
      if(h >> 2 == 0x3E) len = 5; // 111110xx 10XXXXXX 10XXXXXX 10XXXXXX10XXXXXX
      if(h >> 1 == 0x7E) len = 6; // 1111110x 10XXXXXX 10XXXXXX 10XXXXXX10XXXXXX 10XXXXXX
    }
    else if (s[i] == 0)
    return;
    else len = 1; // ascii, 0XXXXXXX

    string tmp = s.substr(i,len);
    tmp += "\0";
    cout<<tmp;
  }
  cout<<'\n';
  fflush(stdout);
}



/*	Public function	    */
/**
 * @brief   拆分句子成短语
 *
 * @param sent	    : 输入字串
 * @param pattern   : 拆分字符
 *
 * @return  拆分后的vector
 *
 *
 */
vector<string> segSentence(const string & sent,const string & pattern){
  vector<string> myvec;
  string::size_type begin = 0,end = sent.size();
  while((end = sent.find(pattern,begin)) != string::npos){
    if(begin != end){
      string tempStr = sent.substr(begin,end - begin);
      myvec.push_back(tempStr);
    }
    begin = end + 1;
  }
  myvec.push_back(sent.substr(begin,end));
  return myvec;
}

/**
 * @brief   删掉句子末尾换行符号
 *
 * @param sent	    : 输入字串
 *
 * @return  删掉句子末尾换行符号后的字符串
 *
 * 注意:输入的stl string,处理windows下的'\r\n'和linux下的'\n'
 *
 */
string delEnter(const string & sent){
  string tempSent;
  int len = sent.size() - 1;
  while(sent[len] == '\n' || sent[len] == '\r'){
    len--;
  }
  tempSent = sent.substr(0,len + 1);
  return tempSent;
}

/**
 * @brief   替换字符串中的部分为目的字符串
 *
 * @param strBig	: 输入待处理的字串
 * @param strsrc	: 输入替换字串
 * @param strdst	: 输入目标字串
 *
 * @return	void
 *
 * 注意:输入都是的stl string,输入待处理字符串被修改
 */
void string_replace(string & strBig, const string & strsrc, const string &strdst){
  string::size_type pos=0;
  string::size_type srclen=strsrc.size();
  string::size_type dstlen=strdst.size();
  while( (pos=strBig.find(strsrc, pos)) != string::npos){
    strBig.replace(pos, srclen, strdst);
    pos += dstlen;
  }
}


/**
 * @brief   获取一个字的字符长度
 *
 * @param sent	    : 输入字串
 * @param char_type : 字符集
 *
 * @return 字的字符长度
 *
 * 注意:输入的字符以'\0'结束,只能处理gbk,utf8
 *
 */
int getCharUnitLen(const char * sent,const char char_type){
  if(char_type == CHAR_GBK){
    return _getWordLenGBK(sent);
  }
  if(char_type == CHAR_UTF8){
    return _getWordLenUTF8(sent);
  }
  return -1;
}

/**
 * @brief   获取一个句子的字个数
 *
 * @param sent	    : 输入句子
 * @param char_type : 字符集
 *
 * @return 句子的字个数
 *
 * 注意:输入的字符以'\0'结束,只能处理gbk,utf8
 *
 */
int getSentLen(const char * sent,const char char_type){
  if(char_type == CHAR_GBK){
    return _getSentLenGBK(sent);
  }
  if(char_type == CHAR_UTF8){
    return _getSentLenUTF8(sent);
  }
  return -1;
}


/*	Private function	*/
int _getWordLenGBK(const char * word){
  unsigned char h = static_cast<unsigned char>(*word);
  if(h & 0x80){
    return 1;
  }else if(*word != '\0'){
    return 2;
  }else{
    return 0;
  }
}

int _getSentLenGBK(const char * sent){
  int len = 0;
  const char * p = sent;
  while(*p != '\0'){
    if(static_cast<unsigned char>(*p) & 0x80){
      p += 2; //中文
    }else{
      p++; //Eng
    }
    len++;
  }
  return len;
}

int _getWordLenUTF8(const char *src){
  int len = 0;
  if(*src < 0) {// multi-bytes utf-8 stream
    unsigned char h = static_cast<unsigned char>(*src);
    if(h >> 4 == 0x0E) len = 3; // 1110XXXX 10XXXXXX 10XXXXXX
    if(h >> 5 == 0x06) len = 2; // 110XXXXX 10XXXXXX
    if(h >> 3 == 0x1E) len = 4; // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
    if(h >> 2 == 0x3E) len = 5; // 111110xx 10XXXXXX 10XXXXXX 10XXXXXX10XXXXXX
    if(h >> 1 == 0x7E) len = 6; // 1111110x 10XXXXXX 10XXXXXX 10XXXXXX10XXXXXX 10XXXXXX
  }else if (*src == 0){
    return 0;
  }else{
    len = 1; // ascii, 0XXXXXXX
  }
  return len;
}

int _getSentLenUTF8(const char *src){
  int i = 0;
  int count = 0;
  while(*(src + i) != '\0'){
    i += _getWordLenUTF8(src+i);
    count++;
  }
  return count;
}

#endif

