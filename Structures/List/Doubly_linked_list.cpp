#include <iostream>

using namespace std;

inline void fast() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

template <typename type>
struct Node {
    explicit Node(type value = type(), Node *prev = nullptr, Node *next = nullptr) : value_(value), prev_(prev), next_(next) {}
    type value_;
    Node *prev_, *next_;

    void insert_front(type value) {
        // prev <-> now
        Node *new_prev = new Node <type>(value, prev_, this);
        if (prev_ != nullptr) {
            prev_ -> next_ = new_prev;
        }
        prev_ = new_prev;
        // prev <-> new_prev <-> now
    }

    void insert_back(type value) {
        // now <-> next
        Node *new_next = new Node <type>(value, this, next_);
        if (next_ != nullptr) {
            next_ -> prev_ = new_next;
        }
        next_ = new_next;
        // now <-> new_next <-> next
    }

    bool erase_front();

    bool erase_back();
};

template <typename type>
bool erase_Node(Node <type> *node) {
    if (node == nullptr) {
        return false;
    } else {
        // prev <-> now <-> next
        if (node -> prev_ != nullptr) {
            node -> prev_ -> next_ = node -> next_;
        }
        if (node -> next_ != nullptr) {
            node -> next_ -> prev_ = node -> prev_;
        }

        delete node;

        return true;
        // prev <-> next
    }
}

template <typename type>
bool Node <type>::erase_front()  {
    // prev_0 <-> prev_1 <-> now
    return erase_Node(prev_);
    // prev_0 <-> now
}

template <typename type>
bool Node <type>::erase_back() {
    // now <-> next_0 <-> next_1
    return erase_Node(next_);
    // now <-> next_1
}

template <typename type>
class List {
public:
    explicit List() {
        // empty
        size_ = 0;
        back_end_ = new Node <type>;
        front_end_ = new Node <type>;
        front_end_ -> next_ = back_end_;
        back_end_ -> prev_ = front_end_;
        // front_end <-> back_end
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void push_back(const type &elem) {
        // elem0 <-> back_end_
        ++size_;
        back_end_ -> value_ = elem;
        back_end_ -> next_ = new Node <type>(elem, back_end_);
        back_end_ = back_end_ -> next_;
        // elem0 <-> elem <-> back_end_
    }

    void push_front(const type &elem) {
        // front_end_ <-> elem0
        ++size_;
        front_end_ -> value_ = elem;
        front_end_ -> prev_ = new Node <type>(elem, nullptr, front_end_);
        front_end_ = front_end_ -> prev_;
        // front_end_ <-> elem <-> elem0
    }

    bool pop_back() {
        if (size_ > 0) {
            // elem0 <-> elem <-> back_end_
            --size_;
            return back_end_ -> erase_front();
            // elem0 <-> back_end_
        } else {
            return false;
        }
    }

    bool pop_front() {
        if (size_ > 0) {
            // front_end_ <-> elem <-> elem0
            --size_;
            return front_end_ -> erase_back();
            // front_end_ <-> elem0
        } else {
            return false;
        }
    }

    void clear() {
        while (!empty()) {
            pop_back();
        }
    }

    bool erase(size_t i) {
        if (empty()) {
            return false;
        } else {
            i %= size_;
            if (i == 0) {
                i = size_;
            }

            auto now = front_end_;
            for (size_t j = 0; j <= i; ++j) {
                now = now -> next_;
            }
            --size_;
            return now->erase_front();
        }
    }

    string show() {
        string str_form;
        for (auto now = front_end_ -> next_; now != back_end_; now = now -> next_) {
            str_form += to_string(now -> value_) + " ";
        }
        return str_form;
    }

    ~List() {
        clear();
        delete front_end_;
        delete back_end_;
    }

private:
    size_t size_;
    Node <type> *front_end_, *back_end_;
    // Это барьерные элементы, т.е. сразу после front_end'а и перед back_end'ом идут добавленные в список элементы
    // (front_end_; back_end_): front_end_ <-> elem_1 <-> elem_2 <-> elem_3 <-> ... <-> back_end_
    // Благодаря тому, что есть барьерные элементы, писать код одно удовольствие
};

int main() {
    fast();

    const string preview = "\n2 size of list"
                           "\n3 print list"
                           "\n4 push_back(elem)"
                           "\n5 push_front(elem)"
                           "\n6 pop_back"
                           "\n7 pop_front"
                           "\n8 clear"
                           "\n9 erase before i"
                           "\nOther input - close program\n";
    List <int> list;
    bool work = true;
    while (work) {
        cerr << preview << "========" << endl;

        size_t command;
        cin >> command;

        if (command == 2) {
            cout << list.size() << endl;
        } else if (command == 3) {
            cout << list.show() << endl;
        } else if (command == 4) {
            cout << "input elem = ";
            cout.flush();
            int elem;
            cin >> elem;
            list.push_back(elem);
        } else if (command == 5) {
            cout << "input elem = ";
            cout.flush();
            int elem;
            cin >> elem;
            list.push_front(elem);
        } else if (command == 6) {
            if (list.pop_back()) {
                cout << "OK" << endl;
            } else {
                cout << "It is empty again!" << endl;
            }
        } else if (command == 7) {
            if (list.pop_front()) {
                cout << "OK" << endl;
            } else {
                cout << "It is empty again!" << endl;
            }
        } else if (command == 8) {
            list.clear();
        } else if (command == 9) {
            cout << "input i = ";
            cout.flush();
            size_t i;
            cin >> i;
            list.erase(i);
        } else {
            work = false;
        }
        cout << "now list: " << list.show() << endl;
        cout << "======" << endl;
    }


    return 0;
}
