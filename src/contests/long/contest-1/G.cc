#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define gc getchar_unlocked
using namespace std;
template <typename T> void scanint(T &x) {
	T c = gc(); while(((c < 48) || (c > 57)) && (c!='-')) c = gc();
	bool neg = false; if(c == '-') neg = true; x = 0; for(;c < 48 || c > 57;c=gc());
	for(;c > 47 && c < 58;c=gc())	x = (x*10) + (c - 48); if(neg)	x = -x;
}

const int MAXN = 2e5 + 3;

int n;
struct query {
    int type;
    int l, r, idx;
} qs[MAXN];

int cmp[MAXN];

using namespace __gnu_pbds;
typedef
tree<
  pair<int,int>,
  null_type,
  less<pair<int,int>>,
  rb_tree_tag,
  tree_order_statistics_node_update>
ordered_set;

struct node {
    ordered_set os;
} seg[4*MAXN];
int sz;

void update(pair<int, int> &val, int add, int idx, int root = 1, int l = 1, int r = sz) {
    if(add == 1) seg[root].os.insert(val);
    else seg[root].os.erase(val);
    if(l == r) {
        return ;
    }
    int mid = (l + r) / 2;
    if(idx <= mid)
        update(val, add, idx, root * 2, l, mid);
    else
        update(val, add, idx, root * 2 + 1, mid + 1, r);
}
int query(int ql, int qr, int root = 1, int l = 1, int r = sz) {
    if(ql > r or qr < l) return 0;
    else if(l >= ql and r <= qr) {
        return seg[root].os.size();
    }
    int mid = (l + r) / 2;
    return query(ql, qr, root * 2, l, mid) + query(ql, qr, root * 2 + 1, mid + 1, r);
}

int query2(int ql, int qr, int root = 1, int l = 1, int r = sz) {
    if(r <= qr) return 0;
    else if(l > qr) {
        return seg[root].os.order_of_key({qr + 1, 0});
    }
    int mid = (l + r) / 2;
    return query2(ql, qr, root * 2, l, mid) + query2(ql, qr, root * 2 + 1, mid + 1, r);
}

int added[MAXN];

int main() {
    char opt;
    scanint(n);
    int add_cnt = 0;
    for(int i = 1; i <= n; i++) {
        scanf(" %c", &opt);
        qs[i].type = opt;
        if(opt == 'D') {
            added[++add_cnt] = i;
        }
        if(opt == 'C') {
            scanint(qs[i].idx);
            qs[i].idx = added[qs[i].idx];
        } else {
            scanint(qs[i].l), scanint(qs[i].r);
            cmp[sz++] = qs[i].l;
            cmp[sz++] = qs[i].r;
        }
    }
    sort(cmp, cmp + sz);
    sz = unique(cmp, cmp + sz) - cmp;
    for(int i = 1; i <= n; i++) {
        if(qs[i].type != 'C') {
            qs[i].l = upper_bound(cmp, cmp + sz, qs[i].l) - cmp;
            qs[i].r = upper_bound(cmp, cmp + sz, qs[i].r) - cmp;
        }
    }
    pair<int, int> temp;
    for(int i = 1; i <= n; i++) {
        if(qs[i].type == 'D') {
            temp = {qs[i].l, i};
            update(temp, 1, qs[i].r);
        } else if(qs[i].type == 'C') {
            temp = {qs[qs[i].idx].l, qs[i].idx};
            update(temp, -1, qs[qs[i].idx].r);
        } else {
            printf("%d\n", query(qs[i].l, qs[i].r) + query2(qs[i].l, qs[i].r));
        }
    }
    return 0;
}
