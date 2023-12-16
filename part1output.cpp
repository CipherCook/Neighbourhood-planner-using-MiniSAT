#include<bits/stdc++.h>
using namespace std;

int main(int argc,char** argv)
{
    string name(argv[1]);
    fstream graph((name+".graph"),ios::in);
    int n,m,k1,k2;
    graph>>n>>m>>k1>>k2;
    graph.close();

    fstream satoutput((name+".satoutput"),ios::in);
    fstream mapping((name+".mapping"),ios::out);
    string s;
    satoutput>>s;
    if(s=="UNSAT") mapping<<0;
    else
    {
        vector<int> c1(n),c2(n);
        int cnt1 = 0,cnt2 = 0;
        for(int i=0;i<n;i++)
        { 
            satoutput>>c1[i];
            cnt1 += (c1[i]>0);
        }           
        for(int i=0;i<n;i++)
        { 
            satoutput>>c2[i];
            cnt2 += (c2[i]>0);
        }

        int t1=0,t2=0;
        mapping<<"#1"<<endl;
        for(auto e:c1)
        {
            if(e>0)
            {
                mapping<<e;
                if(++t1<cnt1) mapping<<" "; 
            }
        }
        mapping<<endl;
        mapping<<"#2"<<endl;
        for(auto e:c2)
        {
            if(e>0)
            {
                mapping<<e-n;
                if(++t2<cnt2) mapping<<" "; 
            }
        }
        // mapping<<endl;
    }
    satoutput.close();
    mapping.close();
    return 0;
}