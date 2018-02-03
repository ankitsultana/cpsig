#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;

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

struct Tower {
	int x, r, f;
	bool operator<(const Tower &other) const {
		return r > other.r;
	}
} towers[MAXN];

ordered_set per_freq[int(1e4 + 10)];
int n, k;

int main() {
	scanf("%d %d", &n, &k);
	for(int i = 1; i <= n; i++) {
		scanf("%d %d %d", &towers[i].x, &towers[i].r, &towers[i].f);
	}
	sort(towers + 1, towers + 1 + n);
	long long ans = 0;
	for(int i = 1; i <= n; i++) {
		int low = towers[i].x - towers[i].r, high = towers[i].x + towers[i].r;
		for(int ff = max(1, towers[i].f - k); ff <= min(10000, towers[i].f + k); ff++) {
			ans += per_freq[ff].order_of_key(high + 1) - per_freq[ff].order_of_key(low);
		}
		per_freq[towers[i].f].insert(towers[i].x);
	}
	printf("%lld\n", ans);
	return 0;
}
