#include <bits/stdc++.h>
#include "utils_for_tinyc.cpp"
using namespace std;
typedef long long LL;
#define rep(i,a,b) for(int i = (a);i <= (b);++i)
#define re_(i,a,b) for(int i = (a);i < (b);++i)
const int SZ = 10000 + 5;

struct Node{
    string token;int code;
};

class Lexer{
    private:
        string s;int idx;
        static const int KWDNUM = 12,SYMNUM = 40,QUONUM = 2;
        static const int INTCODE = KWDNUM + SYMNUM + QUONUM + 1;
        const string kwd[KWDNUM] = {"int","char","void","if","else","while","for","do","for","struct","const","return"};
        const string symbol[SYMNUM] = {"?",":","~","<",">","!=",">=","<=","==","!","&","&&","|","||",",",".",";","(",")","[","]","{","}","+","-","*","/","%","<<",">>","++","--","+=","-=","*=","/=","%=","<<=",">>=","="};
        const string quot[QUONUM] = {"'","\""};
        set<char> symb_ch;
        vector<Node> res;
    public:
        Lexer(){idx = 0;init();}
        Lexer(string s):s(s){idx = 0;init();}
        
        vector<Node> getRes(){return res;}
        void init(){
            re_(i,0,SYMNUM){
                for(auto x: symbol[i]) symb_ch.insert(x);
            }
        }
        int is_symbol_str(string symb){
            re_(i,0,SYMNUM) if(symbol[i] == symb) return i;
            return -1;
        }
        int is_keyword_str(string token){
            re_(i,0,KWDNUM) if(kwd[i] == token) return i;
            return -1;
        }
        int is_quote(string token){
            re_(i,0,QUONUM) if(quot[i][0] == token[0]) return i;
            return -1;
        }
        int get_token_id(string token){
            int idx = is_keyword_str(token);
            if(~idx) return idx + 1;
            idx = is_symbol_str(token);
            if(~idx) return idx + KWDNUM + 1;
            idx = is_quote(token);
            if(~idx) return idx + KWDNUM + SYMNUM + 1;
            return 0;
        }
        const char* get_token_type_name(int code){
            if(!code) return "variable";
            if(code <= KWDNUM) return "keyword";
            if(code <= KWDNUM + SYMNUM) return "symbol";
            if(code == KWDNUM + SYMNUM + 1) return "const_character";
            if(code == KWDNUM + SYMNUM + 2) return "const_string";
            return "integer";
        }
        void print_result(){
            for(auto rs: res) printf("(%s,%s,%d)\n",rs.token.c_str(),get_token_type_name(rs.code),rs.code);
        }
        bool is_symbol(char ch){
            return symb_ch.find(ch) != symb_ch.end();
        }
        
        string get_identifier(){
            int tidx = idx;
            for(;idx < s.size() && (s[idx] == '_' || isalpha(s[idx]) || isdigit(s[idx]));++idx);
            return s.substr(tidx,idx - tidx);
        }
        //数字的合法性，在遍历res的时候判定
        string get_int(){
            int tidx = idx;
            for(;idx < s.size() && (isdigit(s[idx]) || isalpha(s[idx]));++idx);
            return s.substr(tidx,idx - tidx);
        }
        string get_symbol(){
            int tidx = idx;
            if(s[idx] == ',' || s[idx] == ';' || s[idx] == '(' || s[idx] == ')' || s[idx] == '[' || s[idx] == ']' || s[idx] == '{' || s[idx] == '}'){
                ++idx;
                return s.substr(tidx,idx - tidx);
            }
            //注释符号必须被截断
            if(s.substr(idx,2) == "//" || s.substr(idx,2) == "/*"){
                idx += 2;
                return s.substr(tidx,2);
            }
            for(;idx < s.size() && is_symbol(s[idx]);++idx){
                if(s[idx] == ',' || s[idx] == ';' || s[idx] == '(' || s[idx] == ')' || s[idx] == '[' || s[idx] == ']' || s[idx] == '{' || s[idx] == '}')
                    break;
            }
            return s.substr(tidx,idx - tidx);
        }
        string get_const_str(){
            int tidx = idx,slash_state = 0;
            for(;idx < s.size();++idx){
                if(s[idx] == '\\')
                    slash_state = !slash_state;
                else if(idx > tidx){
                    if(s[idx] == s[tidx]){//易错点：引号类型应该相同
                        if(!slash_state) break;
                        else slash_state = 0;
                    }
                    else if(slash_state) slash_state = 0;
                }
            }
            ++idx;
            return s.substr(tidx,idx - tidx);
        }
        
        int get_next_token_type(char ch){
            if(ch == '\\') return -1;
            if(ch == '_' || isalpha(ch)) return 1;
            if(isdigit(ch)) return 2;
            if(is_symbol(ch)) return 3;
            if(ch == '\'' || ch == '"') return 4;
            return 0;
        }
        void skip_comment(string symb){
            if(symb == "//"){
                for(;idx < s.size() && s[idx] != '\n';++idx);
                return;
            }
            //注释以"/*"开头
            int comm_state = 0;
            for(;idx < s.size() && s.substr(idx - 2,2) != "*/";++idx);
        }
        bool post_analyze(){
            for(auto rs: res){
                if(rs.code == INTCODE){
                    bool legal = judge_integer(rs.token);
                    if(!legal) return false;
                }
                else if(rs.token[0] == '\''){
                    bool legal = judge_char(rs.token);
                    if(!legal){
                        printf("Multi-character character constant: %s\n",rs.token.c_str());
                        return false;
                    }
                }
            }
            return true;
        }
        bool analyze(){
            idx = 0;
            while(idx < s.size()){
                int typ = get_next_token_type(s[idx]);
                switch(typ){
                    case -1:{
                        puts("Backslash out of constant string.");
                        return false;
                        break;
                    }
                    case 1:{
                        string iden = get_identifier();
                        res.push_back({iden,get_token_id(iden)});
                        break;
                    }
                    case 2:{
                        string d = get_int();
                        res.push_back({d,INTCODE});
                        break;
                    }
                    case 3:{
                        string symb = get_symbol();
                        if(symb == "//" || symb == "/*"){
                            skip_comment(symb);
                        }
                        else{
                            if(symb.size() > 3 || is_symbol_str(symb) == -1){
                                printf("Invalid symbol: %s\n",symb.c_str());
                                return false;
                            }
                            res.push_back({symb,get_token_id(symb)});
                        }
                        break;
                    }
                    //为了方便，不去掉引号
                    case 4:{
                        string const_str = get_const_str();
                        res.push_back({const_str,get_token_id(const_str)});
                        break;
                    }
                    default:{
                        ++idx;
                        break;
                    }
                }
            }
            return post_analyze();
        }
};

template<typename Type>inline void read(Type &xx){
    Type f = 1;char ch;xx = 0;
    for(ch = getchar();ch < '0' || ch > '9';ch = getchar()) if(ch == '-') f = -1;
    for(;ch >= '0' && ch <= '9';ch = getchar()) xx = xx * 10 + ch - '0';
    xx *= f;
}

int main(int argc, char** argv) {
    while(1){
        int n;read(n);
        if(n <= 0) break;
        string s;
        rep(_,1,n){
            char s0[SZ];fgets(s0,SZ,stdin);
            s += s0;
        }
        Lexer lxr(s);
        bool legal = lxr.analyze();
        cout << "is legal: " << (legal ? "true" : "false") << endl;
        if(!legal) continue;
        lxr.print_result();
    }
    return 0;
}