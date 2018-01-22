#include <bits/stdc++.h>
using namespace std;

const int MAXVAL = 1e6 + 1;

/* Sort queries first by the block in which
 * the left end point resides. If that block
 * is same, sort by right end point.
 */
struct QueryData {
	static int rootq;
	int l, r, idx;
	bool operator<(const QueryData &other) const {
		if(l/rootq == other.l/rootq) {
			return r < other.r;
		}
		return l < other.l;
	}
};

int QueryData::rootq = 0;

/* Active set that maintains the number of distinct elements
 * it has. `dist` stores exactly that. `cnt` is a vector
 * such that cnt[val] stores the number of times val occurs
 * in the active set.
 */
struct ActiveSet {
	int dist;
	vector<int> cnt;
	ActiveSet() : dist(0), cnt(vector<int>(MAXVAL, 0)) { }
	void Add(int val) {
		dist += (cnt[val]++ == 0) ? 1 : 0;
	}
	void Remove(int val) {
		dist -= (cnt[val]-- == 1) ? 1 : 0;
	}
};

/* `curr_l` and `curr_r` represent that the Active Set
 * stores information regarding the range [curr_l, curr_r]
 * Initially, they are set to (0, -1) respectively.
 */
int main() {
	int n, q;
	scanf("%d", &n);
	vector<int> arr(n);
	for(int i = 0; i < arr.size(); i++) scanf("%d", &arr[i]);
	scanf("%d", &q);
	QueryData::rootq = sqrt(q);
	vector<QueryData> qs(q);
	vector<int> ans(q);
	for(int qc = 0; qc < q; qc++) {
		scanf("%d %d", &qs[qc].l, &qs[qc].r);
		qs[qc].l--, qs[qc].r--; // We are using zero based indexing
		qs[qc].idx = qc;
	}
	sort(qs.begin(), qs.end());
	ActiveSet act;
	int curr_l = 0, curr_r = -1;
	for(int i = 0; i < qs.size(); i++) {
		while(curr_l > qs[i].l) {
			act.Add(arr[--curr_l]);
		}
		while(curr_r < qs[i].r) {
			act.Add(arr[++curr_r]);
		}
		while(curr_r > qs[i].r) {
			act.Remove(arr[curr_r--]);
		}
		while(curr_l < qs[i].l) {
			act.Remove(arr[curr_l++]);
		}
		ans[qs[i].idx] = act.dist;
	}
	for(int i = 0; i < qs.size(); i++) {
		printf("%d\n", ans[i]);
	}
	return 0;
}
