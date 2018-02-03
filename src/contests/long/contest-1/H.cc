#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 100;

struct Query {
	static int rootm;
	int vertex, k;
	int l, r, idx;
	bool operator<(const Query &other) const {
		if(l/rootm == other.l/rootm) {
			return r < other.r;
		}
		return l < other.l;
	}
} qs[MAXN];
int Query::rootm;

int n, m;
int color[MAXN];
int ans[MAXN];
vector<int> adj[MAXN];
int pool, st[MAXN], en[MAXN], inv[MAXN];

void Walk(int r, int p) {
	st[r] = ++pool, inv[pool] = r;
	for(int c: adj[r]) {
		if(c != p) {
			Walk(c, r);
		}
	}
	en[r] = pool;
}

struct ActiveSet {
	vector<int> cnt, blocks, data;
	int root, size;
	ActiveSet(int size) : size(size) {
		root = sqrt(size);
		data.resize(size);
		cnt.resize(size);
		blocks.resize(sqrt(size) + 3);
	}
	void Add(int val) {
		if(cnt[val] > 0) {
			data[cnt[val]]--;
			blocks[cnt[val]/root]--;
		}
		cnt[val]++;
		blocks[cnt[val]/root]++;
		data[cnt[val]]++;
	}
	void Remove(int val) {
		data[cnt[val]]--;
		blocks[cnt[val]/root]--;
		cnt[val]--;
		if(cnt[val] > 0) {
			blocks[cnt[val]/root]++;
			data[cnt[val]]++;
		}
	}
	int CountGTE(int val) {
		int res = 0;
		int up = min(size, root + (val / root) * root);
		for(int j = val; j < up; j++) {
			res += data[j];
		}
		int b_no = (val / root) + 1;
		for(int b = b_no; b < blocks.size(); b++) {
			res += blocks[b];
		}
		return res;
	}
};

int main() {
	scanf("%d %d", &n, &m);
	Query::rootm = sqrt(m);
	for(int i = 1; i <= n; i++) {
		scanf("%d", &color[i]);
	}
	for(int i = 1; i < n; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		adj[a].push_back(b); adj[b].push_back(a);
	}
	Walk(1, 0);
	for(int i = 0; i < m; i++) {
		qs[i].idx = i;
		scanf("%d %d", &qs[i].vertex, &qs[i].k);
		qs[i].l = st[qs[i].vertex], qs[i].r = en[qs[i].vertex];
	}
	sort(qs, qs + m);
	ActiveSet aset(int(1e5) + 1);
	int curr_l = 1, curr_r = 0;
	for(int i = 0; i < m; i++) {
		while(curr_l > qs[i].l) {
			aset.Add(color[inv[--curr_l]]);
		}
		while(curr_r < qs[i].r) {
			aset.Add(color[inv[++curr_r]]);
		}
		while(curr_l < qs[i].l) {
			aset.Remove(color[inv[curr_l++]]);
		}
		while(curr_r > qs[i].r) {
			aset.Remove(color[inv[curr_r--]]);
		}
		ans[qs[i].idx] = aset.CountGTE(qs[i].k);
	}
	for(int i = 0; i < m; i++) {
		printf("%d\n", ans[i]);
	}
	return 0;
}
