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

struct QueryData {
	int l, r, k, v;
	int type;
} qs[MAXN];

struct node {
	ordered_set s;
} segtree[MAXN*4];
int arr[MAXN];

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

void Insert(int val, int idx, int root, int l, int r) {
	segtree[root].s.insert(idx);
	if(l == r) return ;
	int mid = (l + r) >> 1;
	if(val <= mid) Insert(val, idx, root*2, l, mid);
	else Insert(val, idx, root*2+1, mid+1, r);
}
void Remove(int val, int idx, int root, int l, int r) {
	segtree[root].s.erase(idx);
	if(l == r) return ;
	int mid = (l + r) >> 1;
	if(val <= mid) Remove(val, idx, root*2, l, mid);
	else Remove(val, idx, root*2+1, mid+1, r);
}
int Query(int ql, int qr, int qk, int root, int l, int r) {
	if(l == r) {
		return l;
	}
	int mid = (l + r) >> 1;
	int num_elems = segtree[root*2].s.order_of_key(qr + 1) -
		segtree[root*2].s.order_of_key(ql);
	if(num_elems >= qk) {
		return Query(ql, qr, qk, root * 2, l, mid);
	} else {
		return Query(ql, qr, qk - num_elems, root * 2 + 1, mid + 1, r);
	}
}
void Update(int idx, int val, int c_size) {
	Remove(arr[idx], idx, 1, 1, c_size);
	arr[idx] = val;
	Insert(arr[idx], idx, 1, 1, c_size);
}

int main() {
	int n, opt, q, l, r, k, v;
	Compressor<int> compressor;
	scanf("%d", &n);
	for(int i = 1; i <= n; i++) {
		scanf("%d", &arr[i]);
		compressor.Add(arr[i]);
	}
	scanf("%d", &q);
	for(int i = 0; i < q; i++) {
		scanf("%d", &opt);
		qs[i].type = opt;
		if(opt == 1) {
			scanf("%d %d", &qs[i].l, &qs[i].v);
			compressor.Add(qs[i].v);
		} else {
			scanf("%d %d %d", &qs[i].l, &qs[i].r, &qs[i].k);
		}
	}
	compressor.Build();
	for(int i = 1; i <= n; i++) {
		arr[i] = compressor.Get(arr[i]);
		Insert(arr[i], i, 1, 1, compressor.size());
	}
	for(int i = 0; i < q; i++) {
		if(qs[i].type == 2) { // Query
			int temp = Query(qs[i].l, qs[i].r, qs[i].k, 1, 1, compressor.size());
			printf("%d\n", compressor.Uncompress(temp));
		} else {            // Update
			Update(qs[i].l, compressor.Get(qs[i].v), compressor.size());
		}
	}
	return 0;
}
