#include<bits/stdc++.h>
using namespace std;

int main(int argc,char** argv)
{
    string name(argv[1]);
    fstream graph((name+".graph"),ios::in);
    int n,m;
    graph>>n>>m;
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
    vector<string> xvar;
    xvar.push_back("0");

    for(int i=1;i<=n;i++)
    {
        string s = "x";
        s.append(to_string(i));
        var[i] = s;
        xvar.push_back(to_string(i));
    }

    for(int i=1;i<=n;i++)
    {
        for(int j=i+1;j<=n;j++)
        {
            if(adj[i].count(j)) continue;
            vector<string> x{"-"+to_string(i),"-"+to_string(j)};
            clauses.push_back(x);
        }
    }

    int cur = n + 1;
    int s[n+1][n+1];
    for(int i=0; i<=n; i++)
    {
        for(int j = 0; j<=n; j++)
        {
            s[i][j] = cur;
            var[s[i][j]] = "S_"+to_string(i)+"_"+to_string(j);
            cur++;
        }
    }

    int ans = 0;
    int l = 1;
    int r = n;
    vector<int> clique;

    while(l<=r)
    {
        int mid = (l+r)/2;
        cout<<mid<<endl;

        vector<vector<string>> c;
        for(auto v:clauses) c.push_back(v);

        for(int i=0; i<=n; i++)
        {
            vector<string> x = {to_string(s[i][0])};
            c.push_back(x);
        }
        for(int i=1; i<=mid; i++)
        {
            vector<string> x = {"-" + to_string(s[0][i])};
            c.push_back(x);
        }

        for(int j = 1; j<=mid; j++)
        {
            for(int i=1; i<=n; i++)
            {
                //xi = xvar[i]
                vector<string> x1 = {("-" + to_string(s[i][j])), to_string(s[i-1][j]), to_string(s[i-1][j-1])};
                vector<string> x2 = {("-" + to_string(s[i][j])), to_string(s[i-1][j]),  xvar[i]};
                vector<string> x3 = {to_string(s[i][j]), ("-" + to_string(s[i-1][j]))};
                vector<string> x4 = {to_string(s[i][j]), ("-" + to_string(s[i-1][j-1])), ("-" + xvar[i])};
                c.push_back(x1);
                c.push_back(x2);
                c.push_back(x3);
                c.push_back(x4);
            }
        }
        c.push_back(vector<string>{to_string(s[n][mid])});
        for(int i=1; i<=n; i++)
        {
            if(adj[i].size() < mid - 1)
            {
                vector<string> x = {"-" + to_string(i)};
                c.push_back(x);
            }
        }

        fstream satinput((name+".satinput"),ios::out);
        satinput<<"p cnf ";
        satinput<<var.size()<<" ";
        satinput<<c.size()<<endl;
        for(auto v: c)
        {
            for(auto p: v)
            {
                satinput<<p<<" ";
                // {
                //     if(stoi(p)>0) satinput<<var[stoi(p)]<<" ";
                //     else satinput<<"~"<<var[-stoi(p)]<<" ";
                // }
            }
            satinput << "0"<<endl;
        }
        satinput.close();

        string cmd = "./minisat "+name+".satinput "+name+".satoutput";
        std::system(cmd.c_str());

        fstream satoutput((name+".satoutput"),ios::in);
        string s;
        satoutput>>s;
        if(s=="UNSAT")
        {
            r = mid - 1;
        }
        else
        {
            clique.clear();
            for(int i=0;i<n;i++)
            {
                int x;
                satoutput>>x;
                if(x>0) clique.push_back(x);
            }

            ans = mid;
            l = mid + 1;
        }
        satoutput.close();
    }

    fstream mapping((name+".mapping"),ios::out);
    if(ans==0) mapping<<0;
    else 
    {
        mapping<<"#1"<<endl;
        for(int i=0;i<clique.size()-1;i++) mapping<<clique[i]<<" ";
        mapping<<clique.back();
        // mapping<<endl;
    }
    mapping.close();
    return 0;
}