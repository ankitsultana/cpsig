#include <bits/stdc++.h>
using namespace std;

const int MAXN = 4e5 + 10;
const int INF = numeric_limits<int>::max();

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

set<pair<int, int> > bit[MAXN];

void Add(const pair<int, int> &pt) {
	int y = pt.second;
	for(; y > 0; y -= y & -y) bit[y].insert(pt);
}
void Remove(const pair<int, int> &pt) {
	int y = pt.second;
	for(; y > 0; y -= y & -y) bit[y].erase(pt);
}
pair<int, int> Query(const pair<int, int> &pt) {
	int y = pt.second;
	pair<int, int> ans = make_pair(INF, INF);
	for(; y < MAXN; y += y & -y) {
		auto it = bit[y].lower_bound(pt);
		if(it != bit[y].end()) {
			ans = min(ans, *it);
		}
	}
	return ans;
}

int n;
char buf[20];
pair<int, pair<int, int> > qs[MAXN];

int main() {
	Compressor<int> compressor;
	scanf("%d", &n);
	for(int i = 0; i < n; i++) {
		scanf(" %s", buf);
		scanf("%d %d", &qs[i].second.first, &qs[i].second.second);
		if(buf[0] == 'a') qs[i].first = 1;
		else if(buf[0] == 'r') qs[i].first = -1;
		else qs[i].first = 0, qs[i].second.first++, qs[i].second.second++;
		compressor.Add(qs[i].second.first);
		compressor.Add(qs[i].second.second);
	}
	compressor.Build();
	for(int i = 0; i < n; i++) {
		qs[i].second.first = compressor.Get(qs[i].second.first);
		qs[i].second.second = compressor.Get(qs[i].second.second);
	}
	for(int i = 0; i < n; i++) {
		if(qs[i].first == 1) {
			Add(qs[i].second);
		} else if(qs[i].first == -1) {
			Remove(qs[i].second);
		} else {
			pair<int, int> ans = Query(qs[i].second);
			if(ans.first == INF) puts("-1");
			else {
				ans.first = compressor.Uncompress(ans.first);
				ans.second = compressor.Uncompress(ans.second);
				printf("%d %d\n", ans.first, ans.second);
			}
		}
	}
	return 0;
}
