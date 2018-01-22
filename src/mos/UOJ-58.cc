#include <bits/stdc++.h>
#define LL long long
#define gc getchar_unlocked
using namespace std;
template <typename T> void in(T &x) {
	T c = gc(); while(((c < 48) || (c > 57)) && (c!='-')) c = gc();
	bool neg = false; if(c == '-') neg = true; x = 0; for(;c < 48 || c > 57;c=gc());
	for(;c > 47 && c < 58;c=gc())	x = (x*10) + (c - 48); if(neg)	x = -x;
}

const int MAXN = 2e5 + 3;
const int MAXLOGN = 20;
int n, m, q, st[MAXN], en[MAXN], inv[MAXN];
LL w[MAXN], c[MAXN], v[MAXN];
vector<int> adj[MAXN];
int pa[MAXN][MAXLOGN];
int depth[MAXN] = {0};
int tame;
bool vis[MAXN];
LL ans[MAXN];
void walk(int r, int p) {
	st[r] = ++tame, inv[tame] = r;
	for(int i = 0; i < adj[r].size(); i++) {
		int elem = adj[r][i];
		if(elem != p) walk(elem, r);
	}
	en[r] = ++tame, inv[tame] = r;
}
/* LCA BEGIN */
void lca_dfs(int node = 1, int prev = -1, int d = 0) {
	depth[node] = d;
	pa[node][0] = prev;
	int sz = int(adj[node].size());
	for(int i = 0; i < sz; i++) {
		if(adj[node][i] == prev)	continue;
		lca_dfs(adj[node][i], node, d+1);
	}
}
void preprocess_lca() {
	std::fill(&pa[0][0], &pa[0][0] + MAXN*MAXLOGN, -1);
	lca_dfs();
	for(int j = 1; j < MAXLOGN; j++)
		for(int i = 1; i <= n; i++)
			if(pa[i][j-1] != -1) pa[i][j] = pa[pa[i][j-1]][j-1];
}
int LCA(int u, int v) {
	if(depth[u] < depth[v]) swap(u,v);
	int diff = depth[u]-depth[v];
	for(int i = 0; i < MAXLOGN; i++) if((diff>>i)&1) u = pa[u][i];
	if(u == v) return u;
	for(int i = MAXLOGN-1; i > -1; i--) if(pa[u][i] != pa[v][i])
		u = pa[u][i], v = pa[v][i];
	return pa[u][0];
}
/* LCA END */

int BLOCK_SIZE;

struct update {
	int t, v, p, now;
	bool operator<(const update &other) const {
		return t < other.t;
	}
} up[MAXN];;
struct query {
	int t, l, r, lca;
	bool flag;
	bool operator<(const query &other) const {
		if(l/BLOCK_SIZE == other.l/BLOCK_SIZE) {
			if(r/BLOCK_SIZE == other.r/BLOCK_SIZE) return t < other.t;
			return r < other.r;
		}
		return l < other.l;
	}
} qs[MAXN];
int type[MAXN], freq[MAXN];
LL res = 0;

void add(int node) {
	if(vis[node]) { // remove it
		int val = c[node];
		res -= w[freq[val]--] * v[val];
	} else {
		int val = c[node];
		res += w[++freq[val]] * v[val];
	}
	vis[node] ^= true;
}
void change(int node, int v) {
	if(vis[node]) {
		add(node);
		c[node] = v;
		add(node);
	} else {
		c[node] = v;
	}
}
int TEMP[MAXN];

int main() {
	int x, y, op;
	in(n), in(m), in(q);
	BLOCK_SIZE = pow(n, 2.0/3.0);
	BLOCK_SIZE = 2154;
	for(int i = 1; i <= m; i++) in(v[i]);
	for(int i = 1; i <= n; i++) in(w[i]);
	for(int i = 1; i < n; i++) {
		in(x), in(y);
		adj[x].push_back(y), adj[y].push_back(x);
	}
	preprocess_lca();
	walk(1, 0);
	for(int i = 1; i <= n; i++) in(c[i]), TEMP[i] = c[i];
	int q_size = 0;
	for(int i = 1; i <= q; i++) {
		in(op), in(x), in(y);
		type[i] = op;
		if(op == 1) { // Answer
			if(st[x] > st[y]) swap(x, y);
			int l = LCA(x, y);
			if(l == x) qs[q_size++] = {i, st[x], st[y], l, false};
			else qs[q_size++] = {i, en[x], st[y], l, true};
		} else {
			up[i] = {i, x, TEMP[x], y}; // TODO
			TEMP[x] = y;
		}
	}
	sort(qs, qs + q_size);
	// END INPUT
	int curr_l = 1, curr_r = 0, now = 0;
	for(int i = 0; i < q_size; i++) {
		while(now < qs[i].t) {
			now++;
			if(type[now] == 0) change(up[now].v, up[now].now);
		}
		while(qs[i].t < now) {
			if(type[now] == 0) change(up[now].v, up[now].p);
			now--;
		}
		while(curr_l > qs[i].l) { // Adding
			curr_l--;
			add(inv[curr_l]);
		}
		while(curr_r < qs[i].r) { // Adding
			curr_r++;
			add(inv[curr_r]);
		}
		while(curr_l < qs[i].l) {
			add(inv[curr_l]);
			curr_l++;
		}
		while(curr_r > qs[i].r) {
			add(inv[curr_r]);
			curr_r--;
		}
		if(qs[i].flag) {
			add(qs[i].lca);
			ans[qs[i].t] = res;
			add(qs[i].lca);
		} else {
			ans[qs[i].t] = res;
		}
	}
	for(int i = 1; i <= q; i++) {
		if(type[i] == 1) printf("%lld\n", ans[i]);
	}
	return 0;
}
