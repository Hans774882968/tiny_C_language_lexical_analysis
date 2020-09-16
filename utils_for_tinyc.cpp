#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
#define rep(i,a,b) for(int i = (a);i <= (b);++i)
#define re_(i,a,b) for(int i = (a);i < (b);++i)

//regex_replace不能正确匹配若干反斜杠这种字符串
string myreplace(string s,string old,string cur){
    int pos = 0;string ret;
    while(pos < s.size()){
        int pos1 = s.find(old,pos);
        if(pos1 == string::npos){ret += s.substr(pos);break;}
        else ret += s.substr(pos,pos1 - pos) + cur,pos = pos1 + old.size();
    }
    return ret;
}

string clean_str(string s){
    s = myreplace(s,"\\\\","\\");
    s = myreplace(s,"\\\"","\"");
    return s;
}

bool judge_binary(string d){
    re_(i,0,d.size())
        if(isalpha(d[i]) || d[i] - '0' >= 2)
            return false;
    return true;
}
bool judge_octal(string d){
    re_(i,0,d.size())
        if(isalpha(d[i]) || d[i] - '0' >= 8)
            return false;
    return true;
}
bool judge_hex(string d){
    re_(i,0,d.size())
        if(isalpha(d[i]) && tolower(d[i]) - 'a' >= 6)
            return false;
    return true;
}
bool judge_integer(string d){
    if(d[0] == '0'){
        if(isalpha(d[1])){
            if(tolower(d[1]) == 'x'){
                bool legal = judge_hex(d.substr(2));
                if(!legal){
                    printf("Illegal hex number: %s\n",d.c_str());
                    return false;
                }
            }
            else if(tolower(d[1]) == 'b'){
                bool legal = judge_binary(d.substr(2));
                if(!legal){
                    printf("Illegal binary number: %s\n",d.c_str());
                    return false;
                }
            }
            else{
                printf("Illegal number: %s\n",d.c_str());
                return false;
            }
        }
        else{
            bool legal = judge_octal(d.substr(1));
            if(!legal){
                printf("Illegal octal number: %s\n",d.c_str());
                return false;
            }
        }
    }
    return true;
}

//'\n'长度4,'a'长度3
bool judge_char(string s){
    if(s[1] == '\\') return s.size() == 4;
    return s.size() == 3;
}