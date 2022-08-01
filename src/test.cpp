#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
inline int gi()
{
    char c;int num=0,flg=1;
    while((c=getchar())<'0'||c>'9')if(c=='-')flg=-1;
    while(c>='0'&&c<='9'){num=num*10+c-48;c=getchar();}
    return num*flg;
}
#define N 100005
int to[2*N],nxt[2*N],fir[N],cnt;
void adde(int a,int b)
{
    to[++cnt]=b;nxt[cnt]=fir[a];fir[a]=cnt;
}
int siz[N],son[N],top[N],fa[N],dep[N];
int dfn[N],num[N],dc;
void dfs1(int u,int f)
{
    fa[u]=f;
    dep[u]=dep[f]+1;
    siz[u]=1;
    for(int v,p=fir[u];p;p=nxt[p]){
        if((v=to[p])!=f){
            dfs1(v,u);
            siz[u]+=siz[v];
            if(siz[son[u]]<siz[v])
                son[u]=v;
        }
    }
}
void dfs2(int u)
{
    dfn[u]=++dc;num[dc]=u;
    if(son[u]){
        top[son[u]]=top[u];
        dfs2(son[u]);
    }
    for(int v,p=fir[u];p;p=nxt[p]){
        v=to[p];
        if(v!=son[u]&&v!=fa[u]){
            top[v]=v;
            dfs2(v);
        }
    }
}
int LCA(int u,int v)
{
    while(top[u]!=top[v]){
        if(dep[top[u]]<dep[top[v]])
            swap(u,v);
        u=fa[top[u]];
    }
    return dep[u]<dep[v]?u:v;
}
struct node{
    int l,r,mx,la;
}a[4*N];
void pushup(int i)
{
    a[i].mx=max(a[i<<1].mx,a[i<<1|1].mx);
}
void build(int i,int l,int r)
{
    a[i].l=l;a[i].r=r;
    if(l==r){
        a[i].mx=dep[num[l]];
        return;
    }
    int mid=(l+r)>>1;
    build(i<<1,l,mid);
    build(i<<1|1,mid+1,r);
    pushup(i);
}
void pushdown(int i)
{
    if(a[i].l==a[i].r)return;
    if(a[i].la){
        a[i<<1].mx+=a[i].la;
        a[i<<1|1].mx+=a[i].la;
        a[i<<1].la+=a[i].la;
        a[i<<1|1].la+=a[i].la;
        a[i].la=0;
    }
}
void update(int i,int l,int r,int k)
{
    if(a[i].l>r||a[i].r<l)return;
    pushdown(i);
    if(l<=a[i].l&&a[i].r<=r){
        a[i].mx+=k;
        a[i].la+=k;
        return;
    }
    update(i<<1,l,r,k);
    update(i<<1|1,l,r,k);
    pushup(i);
}
int query(int i,int l,int r)
{
    if(a[i].l>r||a[i].r<l) return 0;
    pushdown(i);
    if(l<=a[i].l&&a[i].r<=r)
        return a[i].mx;
    return max(query(i<<1,l,r),query(i<<1|1,l,r));
}
namespace LCT{
    int ch[N][2],fa[N],mipos[N];
    void pushup(int x)
    {
        mipos[x]=x;
        if(ch[x][0]&&dep[mipos[ch[x][0]]]<dep[mipos[x]])
            mipos[x]=mipos[ch[x][0]];
        if(ch[x][1]&&dep[mipos[ch[x][1]]]<dep[mipos[x]])
            mipos[x]=mipos[ch[x][1]];
    }
    bool pdc(int x){return ch[fa[x]][1]==x;}
    bool pdr(int x){return ch[fa[x]][0]!=x&&ch[fa[x]][1]!=x;}
    void rot(int x)
    {
        int y=fa[x],z=fa[y];
        bool flg=pdc(x);
        if(!pdr(y)) ch[z][pdc(y)]=x;
        if(ch[y][flg]=ch[x][flg^1])
            fa[ch[y][flg]]=y;
        ch[x][flg^1]=y;
        fa[y]=x;fa[x]=z;
        pushup(y);pushup(x);
    }
    void splay(int x,int y)
    {
        for(;!pdr(x);rot(x))
            if(!pdr(fa[x]))
                rot(pdc(x)==pdc(fa[x])?fa[x]:x);
    }
    int findroot(int x)
    {
        while(ch[x][0])x=ch[x][0];
        return x;
    }
    void Access(int x)
    {
        int tmp=0;
        for(;x;x=fa[x]){
            splay(x,0);
            if(ch[x][1]){
                int rt=mipos[ch[x][1]];
                //int rt=findroot(ch[x][1]);
                update(1,dfn[rt],dfn[rt]+siz[rt]-1,1); 
            }
            if(tmp){
                int rt=mipos[tmp];
                //int rt=findroot(tmp);
                update(1,dfn[rt],dfn[rt]+siz[rt]-1,-1); 
            }
            ch[x][1]=tmp;
            pushup(x);
            tmp=x;
        }
    }
}
int main()
{
    int n,m,x,y,i,op;
    n=gi();m=gi();
    for(i=1;i<n;i++){
        x=gi();y=gi();
        adde(x,y);adde(y,x);
    }
    dfs1(1,0);
    top[1]=1;
    dfs2(1);
    build(1,1,n);
    for(i=1;i<=n;i++)
        LCT::fa[i]=fa[i];
    while(m--){
        op=gi();
        if(op==1){
            x=gi();
            LCT::Access(x);
        }
        else if(op==2){
            x=gi();y=gi();
            int lca=LCA(x,y);
            int ans=query(1,dfn[x],dfn[x])+query(1,dfn[y],dfn[y])-2*query(1,dfn[lca],dfn[lca])+1;
            printf("%d\n",ans);
        }
        else{
            x=gi();
            printf("%d\n",query(1,dfn[x],dfn[x]+siz[x]-1));
        }
    }
}