#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

inline void fast() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

inline size_t left_son(size_t now) {
    return now * 2 + 1;
}

inline size_t right_son(size_t now) {
    return now * 2 + 2;
}

template <typename type>
struct Tree {
    type help_build_tree(size_t now, size_t left, size_t right) {
        if (left < right) {
            if (right - left == 1) {
                return tree[now] = massive[left];
            } else {
                size_t middle = (left + right) / 2;
                return tree[now] = relax(help_build_tree(left_son(now), left, middle),
                                         help_build_tree(right_son(now), middle, right));
            }
        } else {
            return tree[now] = default_value;
        }
    }

    type build_tree() {
        return help_build_tree(0, 0, massive.size());
    }

    explicit Tree(const vector <type> &m, const type &d = type(),
                  const function <type (const type &, const type &)> &r = [](const type &left, const type &right)
                          {return left + right;}) {
        relax = r;
        massive = m;
        default_value = d;
        tree.resize(massive.size() * 4);
        build_tree();
    }

    type help_ans(size_t want_left, size_t want_right, size_t now_left, size_t now_right, size_t now) {
        if (want_left <= now_left && now_right <= want_right) {
            return tree[now];
        } else if (now_right <= want_left || want_right <= now_left) {
            return default_value;
        } else {
            size_t middle = (now_left + now_right) / 2;
            return relax(help_ans(want_left, want_right, now_left, middle, left_son(now)),
                         help_ans(want_left, want_right, middle, now_right, right_son(now)));
        }
    }

    type ans(size_t want_left, size_t want_right) {
        return help_ans(want_left, want_right, 0, massive.size(), 0);
    }

    type help_refresh(size_t pos, const type &elem, size_t now_left, size_t now_right, size_t now) {
        if (now_left <= pos && pos < now_right) {
            if (now_right - now_left == 1) {
                massive[pos] = elem;
                return tree[now] = elem;
            } else {
                size_t middle = (now_left + now_right) / 2;
                return tree[now] = relax(help_refresh(pos, elem, now_left, middle, left_son(now)),
                                         help_refresh(pos, elem, middle, now_right, right_son(now)));
            }
        } else {
            return tree[now];
        }
    }

    type refresh(size_t pos, const type &elem) {
        return help_refresh(pos, elem, 0, massive.size(), 0);
    }

    function <type(const type &, const type &)> relax;
    vector <type> massive, tree;
    type default_value;
};

int main() {
    fast();

    size_t n, k;
    cin >> n >> k;

    Tree <size_t> tree(vector <size_t>(n, 0), 0);
    for (size_t i = 0; i < k; ++i) {
        char c;
        cin >> c;
        if (c == 'A') {
            size_t j, x;
            cin >> j >> x;
            --j;
            tree.refresh(j, x);
        } else {
            size_t left, right;
            cin >> left >> right;
            --left;
            cout << tree.ans(left, right) << '\n';
        }
    }

    return 0;
}
