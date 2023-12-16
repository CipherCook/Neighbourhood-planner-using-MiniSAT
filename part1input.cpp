#include<bits/stdc++.h>
using namespace std;

int main(int argc,char** argv)
{
    string name(argv[1]);
    fstream graph((name+".graph"),ios::in);
    int n,m,k1,k2;
    graph>>n>>m>>k1>>k2;
    vector<set<int>> adj(n+1);
    for(int i=0;i<m;i++)
    {
        int x,y;
        graph>>x>>y;
        adj[x].insert(y);
        adj[y].insert(x);
    }
    graph.close();

    vector<vector<string>> clauses;
    map<int,string> var;
    vector<string> avar, bvar;
    avar.push_back("0");
    bvar.push_back("0");
    for(int i=1;i<=n;i++)
    {
        string s = "a";
        s.append(to_string(i));
        var[i] = s;
        avar.push_back(to_string(i));
    }
    for(int i=1;i<=n;i++)
    {
        string s = "b";
        s.append(to_string(i));
        var[i+n] = s;
        bvar.push_back(to_string(i+n));
    }

    for(int i=1;i<=n;i++)
    {
        vector<string> temp{"-"+to_string(i),"-"+to_string(i+n)};
        clauses.push_back(temp);
    }

    for(int i=1;i<=n;i++)
    {
        for(int j=i+1;j<=n;j++)
        {
            if(adj[i].count(j)) continue;
            vector<string> x{"-"+to_string(i),"-"+to_string(j)};
            vector<string> y{"-"+to_string(i+n),"-"+to_string(j+n)};
            clauses.push_back(x);
            clauses.push_back(y);
        }
    }
    int cur = 2*n + 1;
    int sa[n+1][k1+1];
    int sb[n+1][k2+1];
    for(int i=0; i<=n; i++)
    {
        for(int j = 0; j<=k1; j++)
        {
            sa[i][j] = cur;
            var[sa[i][j]] = "Sa_"+to_string(i)+"_"+to_string(j);
            cur++;
        }
    }

    for(int i=0; i<=n; i++)
    {
        vector<string> x = {to_string(sa[i][0])};
        clauses.push_back(x);
    }
    for(int i=1; i<=k1; i++)
    {
        vector<string> x = {"-" + to_string(sa[0][i])};
        clauses.push_back(x);
    }

    for(int j = 1; j<=k1; j++)
    {
        for(int i=1; i<=n; i++)
        {
            //xi = avar[i]
            vector<string> x1 = {("-" + to_string(sa[i][j])), to_string(sa[i-1][j]), to_string(sa[i-1][j-1])};
            vector<string> x2 = {("-" + to_string(sa[i][j])), to_string(sa[i-1][j]),  avar[i]};
            vector<string> x3 = {to_string(sa[i][j]), ("-" + to_string(sa[i-1][j]))};
            vector<string> x4 = {to_string(sa[i][j]), ("-" + to_string(sa[i-1][j-1])), ("-" + avar[i])};
            clauses.push_back(x1);
            clauses.push_back(x2);
            clauses.push_back(x3);
            clauses.push_back(x4);
        }
    }
    clauses.push_back({to_string(sa[n][k1])});

    for(int i=0; i<=n; i++)
    {
        for(int j = 0; j<=k2; j++)
        {
            sb[i][j] = cur;
            var[sb[i][j]] = "Sb_"+to_string(i)+"_"+to_string(j);
            cur++;
        }
    }

    for(int i=0; i<=n; i++)
    {
        vector<string> x = {to_string(sb[i][0])};
        clauses.push_back(x);
    }
    for(int i=1; i<=k2; i++)
    {
        vector<string> x = {"-" + to_string(sb[0][i])};
        clauses.push_back(x);
    }

    for(int j = 1; j<=k2; j++)
    {
        for(int i=1; i<=n; i++)
        {
            //xi = bvar[i]
            vector<string> x1 = {("-" + to_string(sb[i][j])), to_string(sb[i-1][j]), to_string(sb[i-1][j-1])};
            vector<string> x2 = {("-" + to_string(sb[i][j])), to_string(sb[i-1][j]),  bvar[i]};
            vector<string> x3 = {to_string(sb[i][j]), ("-" + to_string(sb[i-1][j]))};
            vector<string> x4 = {to_string(sb[i][j]), ("-" + to_string(sb[i-1][j-1])), ("-" + bvar[i])};
            clauses.push_back(x1);
            clauses.push_back(x2);
            clauses.push_back(x3);
            clauses.push_back(x4);
        }
    }
    clauses.push_back({to_string(sb[n][k2])});

    // degree of vertices
    for(int i=1; i<=n; i++)
    {
        if(adj[i].size() < k1 - 1)
        {
            vector<string> x = {"-" + to_string(i)};
            clauses.push_back(x);
        }
        if(adj[i].size() < k2 - 1)
        {
            vector<string> x = {"-" + to_string(i+n)};
            clauses.push_back(x);
        }
    }
    
    fstream satinput((name+".satinput"),ios::out);
    satinput<<"p cnf ";
    satinput<<var.size()<<" ";
    satinput<<clauses.size()<<endl;
    for(auto v: clauses)
    {
        for(auto p: v) satinput<<p<<" ";
        // {
        //     if(stoi(p)>0) satinput<<var[stoi(p)]<<" ";
        //     else satinput<<"~"<<var[-stoi(p)]<<" ";
        // }
        satinput << "0"<<endl;
    }
    satinput.close();   
    return 0;
}