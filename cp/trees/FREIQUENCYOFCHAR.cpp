#include <bits/stdc++.h>
using namespace std;

static const int LOG = 20; // since 2^20 > 1e5

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // node characters (1-indexed)
    vector<char> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    // tree
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // LCA data
    vector<int> lvl(n + 1, 0);
    vector<vector<int>> par(n + 1, vector<int>(LOG, 0));

    // prefix frequency: cnt[u][c] = count of char c on path root -> u
    vector<array<int, 26>> cnt(n + 1);

    // DFS
    function<void(int,int)> dfs = [&](int u, int p) {
        par[u][0] = p;
        lvl[u] = lvl[p] + 1;

        if (p != 0) cnt[u] = cnt[p];
        cnt[u][a[u] - 'a']++;

        for (int v : adj[u]) {
            if (v == p) continue;
            dfs(v, u);
        }
    };

    dfs(1, 0); // root at 1

    // build binary lifting table
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            par[i][j] = par[ par[i][j - 1] ][j - 1];
        }
    }

    // lift node u by k levels
    auto lift = [&](int u, int k) {
        for (int j = 0; j < LOG; j++) {
            if (k & (1 << j)) {
                u = par[u][j];
            }
        }
        return u;
    };

    // LCA
    auto lca = [&](int u, int v) {
        if (lvl[u] < lvl[v]) swap(u, v);

        u = lift(u, lvl[u] - lvl[v]);
        if (u == v) return u;

        for (int j = LOG - 1; j >= 0; j--) {
            if (par[u][j] != par[v][j]) {
                u = par[u][j];
                v = par[v][j];
            }
        }
        return par[u][0];
    };

    int q;
    cin >> q;
    while (q--) {
        int u, v;
        char c;
        cin >> u >> v >> c;

        int l = lca(u, v);
        int ch = c - 'a';

        int ans = cnt[u][ch] + cnt[v][ch]
                  - 2 * cnt[l][ch]
                  + (a[l] - 'a' == ch);

        cout << ans << '\n';
    }

    return 0;
}