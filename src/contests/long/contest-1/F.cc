#include <bits/stdc++.h>
using namespace std;

// Extra header files for policy based DS
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<
int,
null_type,
less<int>,
rb_tree_tag,
tree_order_statistics_node_update>
ordered_set;
// End header files

const int MAXN = 2e5 + 100;

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

int n, q;
int arr[MAXN];
pair<int, int> qs[MAXN];
ordered_set sets[2 * MAXN];

int main() {
	scanf("%d %d", &n, &q);
	Compressor<int> compressor;
	for(int i = 1; i <= n; i++) {
		scanf("%d", &arr[i]);
		compressor.Add(arr[i]);
	}
	for(int i = 0; i < q; i++) {
		scanf("%d %d", &qs[i].first, &qs[i].second);
		compressor.Add(qs[i].second);
	}
	compressor.Build();
	for(int i = 1; i <= n; i++) {
		sets[compressor.Get(arr[i])].insert(i);
	}
	for(int i = 0; i < q; i++) {
		int idx = qs[i].first;
		int val = qs[i].second;
		idx++;
		sets[compressor.Get(arr[idx])].erase(idx);
		arr[idx] = val;
		sets[compressor.Get(arr[idx])].insert(idx);
		printf("%d\n", sets[compressor.Get(arr[idx])].order_of_key(idx));
	}
	return 0;
}
