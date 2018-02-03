# BITS CPSIG Long Contest 1

Here is the [link](vjudge.net/contest/209797) for the contest.

**Errata:** There was a small issue with the solution of problem C
that we discussed in lecture. I will address this in the next lecture.
Meanwhile, you can find the solution below and in this repo though.

### Notes

---

**[Problem 1:](http://acm.hdu.edu.cn/showproblem.php?pid=5412)** Create a segtree
over values with a set (policy based) in each node. It turns out using a segtree
with policy based ds leads to MLE in this problem.

---

**[Problem 3:](http://codeforces.com/problemset/problem/19/D)** Create a segtree
on **y** coordinate values, so that every segtree node that represents a range
`[l, r]` will store all the points which have their y coordinate value in the range
`[l, r]`.
