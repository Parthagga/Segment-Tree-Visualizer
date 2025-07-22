// Segment Tree Visualizer in C++ with SUM, MIN, MAX trees using OOP
#include <iostream>
#include <vector>
#include <climits>
#include <string>
using namespace std;

class SegmentTree {
protected:
    vector<int> tree, lazy, arr;
    int n;

public:
    SegmentTree(const vector<int>& input) {
        arr = input;
        n = input.size();
        tree.resize(4 * n);
        lazy.resize(4 * n, 0);
    }

    virtual void build(int node, int l, int r) = 0;
    virtual void updateIndex(int node, int l, int r, int idx, int val) = 0;
    virtual int queryRange(int node, int l, int r, int ql, int qr) = 0;
    virtual void updateRange(int node, int l, int r, int ql, int qr, int val) = 0;
    virtual void visualize() = 0;
    virtual ~SegmentTree() {}

    void buildVisualTree(int node, int l, int r, int col, int row, vector<string>& output, int offset) {
        if (node >= 4 * n || l > r || tree[node] == INT_MAX || tree[node] == INT_MIN) return;

        string val = to_string(tree[node]);
        for (size_t i = 0; i < val.size(); ++i) {
            if (col + i < output[row].size())
                output[row][col + i] = val[i];
        }

        int mid = (l + r) / 2;
        if (l != r) {
            output[row + 1][col - offset / 2] = '/';
            buildVisualTree(2 * node, l, mid, col - offset, row + 2, output, offset / 2);

            output[row + 1][col + offset / 2] = '\\';
            buildVisualTree(2 * node + 1, mid + 1, r, col + offset, row + 2, output, offset / 2);
        }
    }

    void visualizeTree() {
        cout << "\nVisualized Segment Tree (Horizontal):\n";
        vector<string> output(20, string(120, ' '));
        buildVisualTree(1, 0, n - 1, 60, 0, output, 30);
        for (const string& line : output) {
            if (line.find_first_not_of(' ') != string::npos)
                cout << line << endl;
        }
    }
};

class SumSegmentTree : public SegmentTree {
public:
    SumSegmentTree(const vector<int>& input) : SegmentTree(input) {}

    void build(int node, int l, int r) override {
        if (l == r) {
            tree[node] = arr[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid);
        build(2 * node + 1, mid + 1, r);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void push(int node, int l, int r) {
        if (lazy[node] != 0) {
            tree[node] += (r - l + 1) * lazy[node];
            if (l != r) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void updateIndex(int node, int l, int r, int idx, int val) override {
        push(node, l, r);
        if (l == r) {
            tree[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid)
            updateIndex(2 * node, l, mid, idx, val);
        else
            updateIndex(2 * node + 1, mid + 1, r, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    int queryRange(int node, int l, int r, int ql, int qr) override {
        push(node, l, r);
        if (qr < l || ql > r) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return queryRange(2 * node, l, mid, ql, qr) + queryRange(2 * node + 1, mid + 1, r, ql, qr);
    }

    void updateRange(int node, int l, int r, int ql, int qr, int val) override {
        push(node, l, r);
        if (qr < l || ql > r) return;
        if (ql <= l && r <= qr) {
            lazy[node] += val;
            push(node, l, r);
            return;
        }
        int mid = (l + r) / 2;
        updateRange(2 * node, l, mid, ql, qr, val);
        updateRange(2 * node + 1, mid + 1, r, ql, qr, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void visualize() override {
        visualizeTree();
    }
};

class MinSegmentTree : public SumSegmentTree {
public:
    MinSegmentTree(const vector<int>& input) : SumSegmentTree(input) {}

    void build(int node, int l, int r) override {
        if (l == r) {
            tree[node] = arr[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid);
        build(2 * node + 1, mid + 1, r);
        tree[node] = min(tree[2 * node], tree[2 * node + 1]);
    }

    void updateIndex(int node, int l, int r, int idx, int val) override {
        push(node, l, r);
        if (l == r) {
            tree[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid)
            updateIndex(2 * node, l, mid, idx, val);
        else
            updateIndex(2 * node + 1, mid + 1, r, idx, val);
        tree[node] = min(tree[2 * node], tree[2 * node + 1]);
    }

    int queryRange(int node, int l, int r, int ql, int qr) override {
        push(node, l, r);
        if (qr < l || ql > r) return INT_MAX;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return min(queryRange(2 * node, l, mid, ql, qr),
                   queryRange(2 * node + 1, mid + 1, r, ql, qr));
    }

    void updateRange(int node, int l, int r, int ql, int qr, int val) override {
        push(node, l, r);
        if (qr < l || ql > r) return;
        if (ql <= l && r <= qr) {
            lazy[node] += val;
            push(node, l, r);
            return;
        }
        int mid = (l + r) / 2;
        updateRange(2 * node, l, mid, ql, qr, val);
        updateRange(2 * node + 1, mid + 1, r, ql, qr, val);
        tree[node] = min(tree[2 * node], tree[2 * node + 1]);
    }

    void visualize() override {
        visualizeTree();
    }
};


class MaxSegmentTree : public SumSegmentTree {
public:
    MaxSegmentTree(const vector<int>& input) : SumSegmentTree(input) {}

    void build(int node, int l, int r) override {
        if (l == r) {
            tree[node] = arr[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid);
        build(2 * node + 1, mid + 1, r);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    void updateIndex(int node, int l, int r, int idx, int val) override {
        push(node, l, r);
        if (l == r) {
            tree[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid)
            updateIndex(2 * node, l, mid, idx, val);
        else
            updateIndex(2 * node + 1, mid + 1, r, idx, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    int queryRange(int node, int l, int r, int ql, int qr) override {
        push(node, l, r);
        if (qr < l || ql > r) return INT_MIN;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return max(queryRange(2 * node, l, mid, ql, qr),
                   queryRange(2 * node + 1, mid + 1, r, ql, qr));
    }

    void updateRange(int node, int l, int r, int ql, int qr, int val) override {
        push(node, l, r);
        if (qr < l || ql > r) return;
        if (ql <= l && r <= qr) {
            lazy[node] += val;
            push(node, l, r);
            return;
        }
        int mid = (l + r) / 2;
        updateRange(2 * node, l, mid, ql, qr, val);
        updateRange(2 * node + 1, mid + 1, r, ql, qr, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    void visualize() override {
        visualizeTree();
    }
};



int main() {
    cout << "Enter number of elements (e.g., 6): ";
    int n; cin >> n;
    vector<int> arr(n);
    cout << "Enter elements separated by space (e.g., 1 3 5 7 9 11): ";
    for (int i = 0; i < n; ++i) cin >> arr[i];

    cout << "\nChoose Tree Type:\n1. Sum\n2. Min\n3. Max\nChoice: ";
    int choice; cin >> choice;

    SegmentTree* segTree;
    if (choice == 1) segTree = new SumSegmentTree(arr);
    else if (choice == 2) segTree = new MinSegmentTree(arr);
    else segTree = new MaxSegmentTree(arr);

    segTree->build(1, 0, n - 1);
    segTree->visualize();

    while (true) {
        cout << "\n1. Update Index\n2. Range Query\n3. Range Update\n4. Exit\nEnter Operation: ";
        int op; cin >> op;
        if (op == 1) {
            int idx, val;
            cout << "Enter index and value (e.g., 2 10): "; cin >> idx >> val;
            segTree->updateIndex(1, 0, n - 1, idx, val);
            segTree->visualize();
        }
        else if (op == 2) {
            int l, r;
            cout << "Enter range [l r] (e.g., 1 4): "; cin >> l >> r;
            cout << "Result: " << segTree->queryRange(1, 0, n - 1, l, r) << endl;
        }
        else if (op == 3) {
            int l, r, val;
            cout << "Enter range [l r] and value to add (e.g., 1 3 5): "; cin >> l >> r >> val;
            segTree->updateRange(1, 0, n - 1, l, r, val);
            segTree->visualize();
        }
        else break;
    }

    delete segTree;
    return 0;
}
