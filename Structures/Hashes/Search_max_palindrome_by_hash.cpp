#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

inline void fast() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

// Видео про хеши: https://youtu.be/-6kVnVxrCv0
struct Hashed_string {
    long long add(long long left, long long right) const {
        // 0 <= left, right < mod_
        long long ans = left + right;
        // 0 <= ans <= 2 * mod_ - 2
        if (ans >= mod_) {
            ans -= mod_;
        }

        return ans;
    }

    long long minus(long long left, long long right) const {
        // 0 <= left, right < mod_
        long long ans = left - right;
        // 1 - mod_ <= ans <= mod_ - 1
        if (ans < 0) {
            ans += mod_; // корректный МАТЕМАТИЧЕСКИЙ остаток (т.к. 0 <= <Остаток> < <Делитель>)
            // в духе остаток от деления (-1) на 3 равен 2
        }

        return ans;
    }

    long long multiplication(long long left, long long right) const {
        return (left * right) % mod_;
    }

    vector <long long> count_hashes(const string &str) {
        vector <long long> ans;
        ans.reserve(str.size() + 1);

        ans.push_back(0);
        for (char symbol : str) {
            ans.push_back(add(multiplication((ans.back()), power_), (long long)symbol));
        }
        // ans = {0, str[0], str[0] * power + str[1], str[0] * power ^ 2 + str[1] * power + str[0], ...
        return ans;
    }

    explicit Hashed_string(string str = "", long long mod = 1e9 + 39, long long power = 239) :
    str_(str), mod_(mod), power_(power) {
        powers.reserve(str.size() + 1);
        powers.push_back(1);
        for (size_t i = 0; i < str.size(); ++i) {
            powers.push_back(multiplication(powers.back(), power_));
        }

        hashes = count_hashes(str);
        reverse(str.begin(), str.end());
        rev_hashes = count_hashes(str);
    }



    long long find_hash(size_t left, size_t right) const {
        // Нумерация с ЕДИНИЦЫ
        // right >= left

        // ans = str[left] * powers[right - left] + str[left + 1] * powers[right - left - 1] + ... + str[right]

        // hashes[right] = str[0] * powers[right] + str[1] * powers[right - 1] + ...
        // + str[left - 1] * powers[right - left + 1] + str[left] * powers[right - left] + ...
        // + str[right]

        // hashes[left - 1] = str[0] * powers[left - 1] + str[1] * powers[left - 2] + ... + str[left - 1]

        // hashes[left - 1] * powers[right - left + 1] = str[0] * powers[right] + str[1] * powers[right - 1] + ...
        // + str[left - 1] * powers[right - left + 1]

        // hashes[right] - hashes[left - 1] * powers[right - left + 1] =
        // str[left] * powers[right - left] + str[left + 1] * powers[right - left - 1] + ... + str[right]
        // А это и есть то, что нам нужно

        return minus(hashes[right], multiplication(hashes[left - 1], powers[right - left + 1]));
    }

    size_t find_rev_index(size_t index) const {
        // index < hashes.size()
        // <index> -> <rev_index>

        // 1 -> hashes.size() - 1
        // 2 -> hashes.size() - 2
        // ...
        // hashes.size() - 2 -> 2
        // hashes.size() - 1 -> 1

        // => <index> + <rev_index> = hashes.size()
        // => <rev_index> = hashes.size() - <index>

        return hashes.size() - index;
    }

    long long find_rev_hash(size_t left, size_t right) const {
        size_t rev_left = find_rev_index(right), rev_right = find_rev_index(left);
        return minus(rev_hashes[rev_right],
                     multiplication(rev_hashes[rev_left - 1], powers[rev_right - rev_left + 1]));
    }

    size_t size() const {
        return str_.size();
    }

    string str_;
    vector <long long> hashes, rev_hashes, powers;
    const long long mod_, power_;
};

inline bool check(const Hashed_string &hashed, size_t left, size_t right) {
    size_t middle = (left + right) / 2;
    return hashed.find_hash(left, middle) == hashed.find_rev_hash(middle + (left + right) % 2, right);
    // middle + (left + right) % 2: Потому что при чётной длине middle нужно подвинуть на 1 вперёд для правой половины
}

inline string check(const Hashed_string &hashed, size_t size) {
    string ans;
    for (size_t left = 1; left + size - 1 <= hashed.str_.size() && ans.empty(); ++left) {
        if (check(hashed, left, left + size - 1)) {
            ans = hashed.str_.substr(left - 1, size);
        }
    }
    return ans;
}

string find_max_size(const Hashed_string &hashed, const vector <size_t> &sizes) {
    string ans;
    size_t left = 0, right = sizes.size();
    while (right - left > 1) {
        size_t middle = (left + right) / 2;
        auto candidate = check(hashed, sizes[middle]);
        if (candidate.empty()) {
            right = middle;
        } else {
            left = middle;
            ans = candidate;
        }
    }

    return ans;
}

string solve(const string &str) {
    vector <vector <size_t>> sizes(2);
    for (size_t mod = 0; mod < sizes.size(); ++mod) {
        sizes.reserve(str.size() / 2 + 1);
        sizes[mod].push_back(mod);
        while (sizes[mod].back() + 2 <= str.size()) {
            sizes[mod].push_back(sizes[mod].back() + 2);
        }
    }

    Hashed_string hashed(str);
    string ans;
    for (const auto &size : sizes) {
        auto candidate = find_max_size(hashed, size);
        if (candidate.size() > ans.size()) {
            ans = candidate;
        }
    }

    return ans;
}

int main() {
    fast();

    size_t size;
    cin >> size;
    string str;
    cin >> str;

    auto ans = solve(str);
    cout << ans.size(); // << '\n' << ans;

    return 0;
}
