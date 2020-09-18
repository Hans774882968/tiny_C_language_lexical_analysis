#include <bits/stdc++.h>
using namespace std;

struct Trie{
    static const int LEN = 70,ALPHA = 30;
    int T[LEN][ALPHA],end[LEN],sz,wd_num;
    map<char,int> mp;int msz;
    Trie(){
        msz = sz = wd_num = 0;
        memset(T[0],0,sizeof T[0]);
        end[0] = -1;
    }
    
    inline int ID(char x){
        if(!mp.count(x)) return mp[x] = ++msz;
        return mp[x];
    }
    void insert(string s){
        int u = 0;
        for(auto x: s){
            int v = ID(x);
            if(!T[u][v]){
                memset(T[++sz],0,sizeof T[sz]);
                end[sz] = -1;
                T[u][v] = sz;
            }
            u = T[u][v];
        }
        if(end[u] == -1) end[u] = wd_num++;
    }
    int in_dict(string s){
        int u = 0;
        for(auto x: s){
            int v = ID(x);
            if(!T[u][v]) return -1;
            u = T[u][v];
        }
        return end[u];
    }
    int get_longest_prefix(string s){
        int u = 0,len = 0;
        for(auto x: s){
            int v = ID(x);
            if(!T[u][v]) return len;
            u = T[u][v];
            ++len;
        }
        return len;
    }
};