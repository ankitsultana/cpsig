// Segtree solution to: http://codeforces.com/contest/19/problem/D
// This gives TLE, see Cfast.cc for a BIT based solution
#include <bits/stdc++.h>
#define gc getchar
using namespace std;
template <typename T> void in(T &x) {
	T c = gc(); while(((c < 48) || (c > 57)) && (c!='-')) c = gc();
	bool neg = false; if(c == '-') neg = true; x = 0; for(;c < 48 || c > 57;c=gc());
	for(;c > 47 && c < 58;c=gc())	x = (x*10) + (c - 48); if(neg)	x = -x;
}

template<typename T>
struct Compressor {
	vector<T> cmp;
	void Add(T val) { cmp.push_back(val); }
	void Build() {
		sort(cmp.begin(), cmp.end());
		cmp.resize(unique(cmp.begin(), cmp.end()) - cmp.begin());
	}
	int Get(T val) {
		return upper_bound(cmp.begin(), cmp.end(), val) - cmp.begin();
	}
	int Uncompress(int val) {
		return cmp[val - 1];
	}
	int size() { return cmp.size(); }
};

const int MAXN = 4e5 + 10;
const int INF = numeric_limits<int>::max();
set<pair<int, int> > segtree[MAXN*4];

void Add(pair<int, int> &pt, int root, int l, int r) {
	segtree[root].insert(pt);
	if(l == r) return ;
	int mid = (l + r) >> 1;
	if(pt.second <= mid) Add(pt, root * 2, l, mid);
	else Add(pt, root * 2 + 1, mid + 1, r);
}
void Remove(const pair<int, int> &pt, int root, int l, int r) {
	segtree[root].erase(pt);
	if(l == r) return ;
	int mid = (l + r) >> 1;
	if(pt.second <= mid) Remove(pt, root * 2, l, mid);
	else Remove(pt, root * 2 + 1, mid + 1, r);
}
pair<int, int> Query(pair<int, int> &pt, int root, int l, int r) {
	int mid = (l + r) >> 1;
	if(l >= pt.second) {
		auto it = segtree[root].lower_bound(pt);
		if(it == segtree[root].end()) return make_pair(INF, INF);
		return *it;
	}
	pair<int, int> ans = make_pair(INF, INF);
	if(mid >= pt.second) {
		ans = Query(pt, root * 2, l, mid);
	}
	ans = min(ans, Query(pt, root * 2 + 1, mid + 1, r));
	return ans;
}

char buf[20];
int n;

pair<int, pair<int, int> > qs[MAXN];

int main() {
	int x, y;
	Compressor<int> compressor;
	in(n);
	for(int i = 0; i < n; i++) {
		scanf("%s", buf);
		in(x), in(y);
		if(buf[0] == 'a') qs[i].first = 1;
		else if(buf[0] == 'r') qs[i].first = -1;
		else if(buf[0] == 'f') qs[i].first = 0, x++, y++;
		else assert(false);
		qs[i].second = make_pair(x, y);
		compressor.Add(x);
		compressor.Add(y);
	}
	compressor.Build();
	for(int i = 0; i < n; i++) {
		qs[i].second.first = compressor.Get(qs[i].second.first);
		qs[i].second.second = compressor.Get(qs[i].second.second);
	}
	for(int i = 0; i < n; i++) {
		if(qs[i].first == 1) {
			Add(qs[i].second, 1, 1, compressor.size());
		} else if(qs[i].first == 0) {
			pair<int, int> ans = Query(qs[i].second, 1, 1, compressor.size());
			if(ans.first == INF) puts("-1");
			else {
				ans.first = compressor.Uncompress(ans.first);
				ans.second = compressor.Uncompress(ans.second);
				printf("%d %d\n", ans.first, ans.second);
			}
		} else {
			Remove(qs[i].second, 1, 1, compressor.size());
		}
	}
	return 0;
}
