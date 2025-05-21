/**
 * Author: Quang Nhat Ngo
 * Date: 2025-05-21
 * Source: https://judge.yosupo.jp/submission/272063
 * Description: Generic segment tree beats with range chmin, chmax, add and range sum
 * Status: tested
*/
#pragma once
const int inf = 1e18;

struct info {
	int mx, mx2, mn, mn2, cmx, cmn, sum, lazy;
	info() {
		mx = mn = cmx = cmn = sum = lazy = 0;
		mx2 = -inf; mn2 = inf;
	}
	info(int x) {
		mx = mn = sum = x;
		cmx = cmn = 1;
		mx2 = -inf; mn2 = inf;
		lazy = 0;
	}
	void merge(const info &l, const info &r) {
		mx = max(l.mx, r.mx);
		mn = min(l.mn, r.mn);
		sum = l.sum + r.sum;
		cmx = l.cmx * (mx == l.mx) + r.cmx * (mx == r.mx);
		cmn = l.cmn * (mn == l.mn) + r.cmn * (mn == r.mn);
		
		if(mx == l.mx) {
			if(mx == r.mx) mx2 = max(l.mx2, r.mx2);
			else mx2 = max(l.mx2, r.mx);
		} else mx2 = max(l.mx, r.mx2);
		if(mn == l.mn) {
			if(mn == r.mn) mn2 = min(l.mn2, r.mn2);
			else mn2 = min(l.mn2, r.mn);
		} else mn2 = min(l.mn, r.mn2);
	}
};
struct SGT {
	int n; vector<info> t;
	SGT(vector<int> a) : n(a.size()), t(n << 2) {
		build(1, 1, n, a);
	}
	void build(int v, int l, int r, const vector<int> &a) {
		if(l == r) {
			t[v] = info(a[l - 1]);
			return;
		}
		int m = (l + r) >> 1;
		build(v << 1, l, m, a);
		build(v << 1 | 1, m + 1, r, a);
		t[v].merge(t[v << 1], t[v << 1 | 1]);
	}
	void push_sum(int v, int l, int r, int x) {
		if(x == 0) return;
		t[v].sum += x * (r - l + 1);
		t[v].mx += x;
		t[v].mn += x;
		if(t[v].mx2 != -inf) t[v].mx2 += x;
		if(t[v].mn2 != inf) t[v].mn2 += x;
		t[v].lazy += x;
	}
	void push_min(int v, int x, bool f) {
		if(x <= t[v].mn) return;
		t[v].sum += t[v].cmn * (x - t[v].mn);
		t[v].mn = x;
		if(f) {
			t[v].mx = x;
		} else {
			if(x >= t[v].mx) {
				t[v].mx = x;
			} else if(x > t[v].mx2) {
				t[v].mx2 = x;
			}
		}
	}
	void push_max(int v, int x, bool f) {
		if(x >= t[v].mx) return;
		t[v].sum -= t[v].cmx * (t[v].mx - x);
		t[v].mx = x;
		if(f) {
			t[v].mn = x;
		} else {
			if(x <= t[v].mn) {
				t[v].mn = x;
			} else if(x < t[v].mn2) {
				t[v].mn2 = x;
			}
		}
	}
	void push(int v, int l, int r) {
		int m = (l + r) >> 1;
		push_sum(v << 1, l, m, t[v].lazy);
		push_sum(v << 1 | 1, m + 1, r, t[v].lazy);
		t[v].lazy = 0;
		push_min(v << 1, t[v].mn, l == m);
		push_min(v << 1 | 1, t[v].mn, m + 1 == r);
		push_max(v << 1, t[v].mx, l == m);
		push_max(v << 1 | 1, t[v].mx, m + 1 == r);
	}
	void chmin(int v, int l, int r, int ll, int rr, int x) {
		if(l > rr || r < ll || t[v].mx <= x) return;
		if(l >= ll && r <= rr && t[v].mx2 < x) {
			push_max(v, x, l == r);
			return;
		}
		push(v, l, r);
		int m = (l + r) >> 1;
		chmin(v << 1, l, m, ll, rr, x);
		chmin(v << 1 | 1, m + 1, r, ll, rr, x);
		t[v].merge(t[v << 1], t[v << 1 | 1]);
	}
	void chmin(int l, int r, int x) {
		chmin(1, 1, n, l, r, x);
	}
	void chmax(int v, int l, int r, int ll, int rr, int x) {
		if(l > rr || r < ll || t[v].mn >= x) return;
		if(l >= ll && r <= rr && t[v].mn2 > x) {
			push_min(v, x, l == r);
			return;
		}
		push(v, l, r);
		int m = (l + r) >> 1;
		chmax(v << 1, l, m, ll, rr, x);
		chmax(v << 1 | 1, m + 1, r, ll, rr, x);
		t[v].merge(t[v << 1], t[v << 1 | 1]);
	}
	void chmax(int l, int r, int x) {
		chmax(1, 1, n, l, r, x);
	}
	void add(int v, int l, int r, int ll, int rr, int x) {
		if(l > rr || r < ll) return;
		if(l >= ll && r <= rr) {
			push_sum(v, l, r, x);
			return;
		}
		push(v, l, r);
		int m = (l + r) >> 1;
		add(v << 1, l, m, ll, rr, x);
		add(v << 1 | 1, m + 1, r, ll, rr, x);
		t[v].merge(t[v << 1], t[v << 1 | 1]);
	}
	void add(int l, int r, int x) {
		add(1, 1, n, l, r, x);
	}
	int get_sum(int v, int l, int r, int ll, int rr) {
		if(l > rr || r < ll) return 0;
		if(l >= ll && r <= rr) return t[v].sum;
		push(v, l, r);
		int m = (l + r) >> 1;
		return get_sum(v << 1, l, m, ll, rr) + get_sum(v << 1 | 1, m + 1, r, ll, rr);
	}
	int get_sum(int l, int r) {
		return get_sum(1, 1, n, l, r);
	}
};