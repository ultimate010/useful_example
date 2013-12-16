#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <set>

using namespace std;
#define MAX_BUFFER_SIZE 1024 * 1024 * 2


int MAX_WORD_LEN = 0;

void string_replace(string & strBig, const string & strsrc, const string &strdst){
        string::size_type pos=0;
        string::size_type srclen=strsrc.size();
        string::size_type dstlen=strdst.size();
        while( (pos=strBig.find(strsrc, pos)) != string::npos){
                strBig.replace(pos, srclen, strdst);
                pos += dstlen;
        }
}
int getWordLen(const char * word){
        int len = 0;
        const char * p = word;
        while(*p != '\0'){
                if(*p & 0x80){
                        p += 2; //中文
                }else{
                        p++; //Eng
                }
                len++;
        }
        return len;
}
int getWord(const char * word,char * outBuffer,int len){
        const char * p = word;
        for(int i = 0;i < len && *p != '\0';i++){
                if(*p & 0x80){ //中文两个字节
                        *outBuffer++ = *p++;
                        *outBuffer++ = *p++;
                }else{ //英文一个字节
                        *outBuffer++ = *p++;
                }
        }
        *outBuffer++ = '\0';
        return 0;
}

bool loadWordTable(const char * dataPath,set<string> & wordSet){
        if(dataPath == NULL){
                cerr <<"WordTable path is null\n";
                return false;
        }
        ifstream input(dataPath);
        if(input.fail()){
                cerr <<"Can not open WordTable file!\n";
                return false;
        }
        string lineStr = "";
        int lineCount = 0;
        while(!input.eof()){
                getline(input,lineStr);
                if(0 == lineStr.size()){
                        continue;
                }
                if(++lineCount % 1000 == 0){
                        cout <<"\rLoading:" <<lineCount;
                }
                string_replace(lineStr,"\r","");
                if(getWordLen(lineStr.c_str()) > MAX_WORD_LEN){
                        MAX_WORD_LEN = getWordLen(lineStr.c_str());
                }
                wordSet.insert(lineStr);
        }
        cout <<endl;
        return true;
}
bool process(const char * input,char * output,set<string> & wordSet){
        //加载词典
        const char * p = input;
        char buf[256];
        while(*p != '\0'){
                for(int i = MAX_WORD_LEN;i > 1;i--){
                        memset(buf,'\0',256);
                        getWord(p,buf,i); //获取词,长度i
                        if(wordSet.find(string(buf)) != wordSet.end()){
                                //找到,输出
                                strcpy(output,buf);
                                output += strlen(buf);
                                p += strlen(buf);
                                *output++ = '|';
                                *output++ = '|';
                                *output++ = '|';
                                break;
                        }
                }
                //找不到,单字拆分
                memset(buf,'\0',256);
                getWord(p,buf,1); //获取词,长度1
                //strcpy(output,buf);
                //output += strlen(buf);
                p += strlen(buf);
                //*output++ = '\t';
        }
        return true;
}

int main(int argc,char *argv[]){

        if(argc != 4){
                cout <<"Usage: exe wordTable inputfile outFile\n";
                cout <<"处理GBK文本";
                return -1;
        }

        set<string> wordSet;
        if(loadWordTable(argv[1],wordSet) == false){
                return -1;
        }

        /*
        set<string>::iterator iter = wordSet.begin();
        for(;iter != wordSet.end();iter++){
                cout <<*iter <<"\t";
        }
        cout <<MAX_WORD_LEN <<endl;
        cout <<endl;
        */

        char * outBuffer = new char[MAX_BUFFER_SIZE];
        if(outBuffer == NULL){
                cerr <<"Can not open buffer!\n";
                return -1;
        }
        ifstream input(argv[2]);
        ofstream output(argv[3]);
        if(input.fail() || output.fail()){
                cerr <<"Can not open input or output!\n";
                return -1;
        }
        string lineStr = "";
        int lineCount = 0;
        while(!input.eof()){
                getline(input,lineStr);
                if(++lineCount % 1000 == 0){
                        cout <<"Processing: " <<lineCount <<endl;
                }
                output <<lineCount <<"###";
                string_replace(lineStr,"\r","");
                if(0 == lineStr.size()){
                        output <<"\r\n";
                        continue;
                }
                memset(outBuffer,'\0',MAX_BUFFER_SIZE);
                process(lineStr.c_str(),outBuffer,wordSet);
                output <<outBuffer <<"\r\n";
        }
        delete [] outBuffer;
}
