#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class DisjointSetUnion {
private:
    vector<int> parent;

public:
    explicit DisjointSetUnion(int n) : parent(n) {
        fill(parent.begin(), parent.end(), -1);
    }

    int FindSet(int x) {
        vector<int> buf;
        while (parent[x] >= 0) {
            buf.push_back(x);
            x = parent[x];
        }
        for (int i: buf) {
            parent[i] = x;
        }
        return x;
    }

    void Union(int x, int y) {
        x = FindSet(x);
        y = FindSet(y);

        if (x != y) {
            if (parent[x] > parent[y]) {
                swap(x, y);
            }

            parent[x] += parent[y];
            parent[y] = x;
        }
    }

    bool IsOneUnion() {
        bool foundA = false, foundB = false;
        for (int i: parent) {
            if (i < 0) {
                if (!foundA) {
                    foundA = true;
                    continue;
                } else {
                    foundB = true;
                    break;
                }
            }
        }
        return !(foundA && foundB);
    }
};

int main() {
    std::ifstream in("input.txt");
    int n, m, q;
    in >> n;
    in >> m;
    in >> q;
    int k = 3;
    vector<vector<int>> a(k, vector<int>(m));

    for (int j = 0; j < m; j++) {
        in >> a[0][j];
        a[0][j]--;
        in >> a[1][j];
        a[1][j]--;
        a[2][j] = 1;
    }


    vector<int> earthquakes(q);

    for (int i = q - 1; i >= 0; i--) {
        in >> earthquakes[i];
        earthquakes[i]--;
    }
    in.close();

    DisjointSetUnion country(n);

    for (int i = 0; i < q; i++) {
        a[2][earthquakes[i]] = 0;
    }

    for (int i = 0; i < m; i++) {
        if (a[2][i] == 1)
            country.Union(a[0][i], a[1][i]);
    }

    ofstream out("output.txt");
    vector<bool> reverse;
    bool t = country.IsOneUnion();
    reverse.push_back(t);
    for (int i = 0; i < q - 1; i++) {
        int s = a[0][earthquakes[i]];
        int b = a[1][earthquakes[i]];
        if (country.FindSet(s) == country.FindSet(b))
            reverse.push_back(t);
        else {
            country.Union(s, b);
            t = country.IsOneUnion();
            reverse.push_back(t);
        }
    }

    for (int i = q - 1; i >= 0; i--)
        out << reverse[i];
    out.close();
}