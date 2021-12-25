#include <iostream>
#include <vector>
#include <string>

using namespace std;

inline void fast() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

class Hashed_string {
public:
    explicit Hashed_string(const string &str = "", long long mod = 1e9 + 19, long long power = 239) :
    str_(str), mod_(mod), power_(power) {
        powers.reserve(str.size() + 1);
        powers.push_back(0);
        for (size_t i = 0; i < str.size(); ++i) {
            powers.push_back(multiplication(powers.back(), power_));
        }

        hashes.reserve(str.size() + 1);
        hashes.push_back(0);
        for (char symbol : str) {
            hashes.push_back(add(multiplication((hashes.back()), power_), (long long)symbol));
        }
    }

    long long find_hash(size_t left, size_t right) {
        return minus(hashes[right], multiplication(hashes[left - 1], powers[right - left + 1]));
    }

protected:
    string str_;
    vector <long long> hashes, powers;
    const long long mod_, power_;

    long long add(long long left, long long right) const {
        long long ans = left + right;

        if (ans >= mod_) {
            ans -= mod_;
        }

        return ans;
    }

    long long minus(long long left, long long right) const {
        long long ans = left - right;

        if (ans < 0) {
            ans += mod_;
        }

        return ans;
    }

    long long multiplication(long long left, long long right) const {
        return (left * right) % mod_;
    }
};
