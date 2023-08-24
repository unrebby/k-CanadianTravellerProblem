#include <queue>
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

void dij(int n, vector<int> &d, int &point, vector<vector<int>> &tmp, vector<pair<int, int>> &p, priority_queue<pair<int, int>> &a) {
    vector<bool> visited(n, false);
    d[point]=0;
    a.push({d[point], point});
    while (!a.empty()) {
        auto v = a.top().second;
        a.pop();
        if (!visited[v]) {
            visited[v] = true;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && d[i] > d[v] + tmp[v][i] && tmp[v][i] != -1) {
                    p.emplace_back(i, v);
                    d[i] = d[v] + tmp[v][i];
                    a.push({-d[i], i});
                }
            }
        }
    }
}

void e1(int n, int &k, vector<int> &d, int &point, int &finish_point, vector<vector<int>> &g_curr, int &w, int &w_res, vector<int> &p_min, vector<int> &p_res, int &vertex_now, int &vertex_index) {
    int w_min_now = 1000000000, w_res_now = 0;
    pair<int, int> edge_now = {-1, -1};
    for (size_t i = 0; i < p_min.size(); ++i) {
        if (i != 0) {
            w_res_now += g_curr[p_min[i]][p_min[i - 1]];
        }
        int w_back_now = 0;
        for (int j = p_min.size() - 1; j < vertex_index; --j) {
            int is_blocked = rand() % 2;
            if (is_blocked == 1) {
                --k;
                g_curr[p_min[j]][p_min[j - 1]] = -1;
                g_curr[p_min[j - 1]][p_min[j]] = -1;
                cout << "Found a blocked edge: " << p_min[j - 1] << " - " << p_min[j] << "\n";
                break;
            }
            if (j != p_min.size() - 1) {
                if (g_curr[p_min[j]][p_min[j - 1]] != -1) {
                    w_back_now += g_curr[p_min[j]][p_min[j - 1]];
                } else {
                    break;
                }
            }
            if (g_curr[p_min[i]][p_min[j]] != -1) {
                int is_blocked = rand() % 2;
                if (is_blocked == 1) {
                    --k;
                    g_curr[p_min[i]][p_min[j]] = -1;
                    g_curr[p_min[j]][p_min[i]] = -1;
                    break;
                }
                if (w_res_now + g_curr[p_min[i]][p_min[j]] + w_back_now < w_min_now) {
                    w_min_now = w_res_now + g_curr[p_min[i]][p_min[j]] + w_back_now;
                    edge_now = {i, j};
                }
            }
        }
        if (p_min[i] == vertex_now) {
            break;
        }
    }
    if (w_min_now != 1000000000) {
        w_res = w_min_now;
        auto first = p_min.cbegin() + edge_now.first + 1;
        auto last = p_min.cbegin() + edge_now.second;
        p_min.erase(first, last);
        p_res = p_min;
    }
}

void step(int n, int &k, vector<int> &d, int &point, int &finish_point, vector<vector<int>> &g_curr, int &w, int &w_res, vector<int> &p_min, vector<int> &p_res) {
    int vertex_now = point, vertex_index = 0;
    int flag = 0;
    int i = 0;
    while (i < p_min.size() - 1) {
        if (p_min[i] == point) {
            break;
        }
        ++i;
    }
    for (; i < p_min.size() - 1; i++) {
        if (k > 0) {
            int is_blocked = rand() % 2;
            if (is_blocked == 1) {
                --k;
                flag = 1;
                g_curr[p_min[i]][p_min[i + 1]] = -1;
                g_curr[p_min[i + 1]][p_min[i]] = -1;
                cout << "Found a blocked edge: " << p_min[i] + 1 << " - " << p_min[i + 1] + 1 << "\n";
                break;
            }
            vertex_now = p_min[i + 1];
            vertex_index = i + 1;
        }
    }
    if (flag == 0) {
        w_res = w;
        p_res = p_min;
        return;
    }
    w_res = 1000000000;
    e1(n, k, d, point, finish_point, g_curr, w, w_res, p_min, p_res, vertex_now, vertex_index);
    int w_now = 0, index_new_start = -1;
    vector<int> p_min_tmp = p_min;
    for (int i = 0; i <= vertex_index; ++i) {
        if (i != 0) {
            w_now += g_curr[p_min[i]][p_min[i - 1]];
        }
        vector<int> ans;
        priority_queue<pair<int, int>> a;
        vector<pair<int, int>> p;
        vector<int> d_(n, 1000000000);
        dij(n, d_, p_min[i], g_curr, p, a);
        if (d_[finish_point] == 1000000000) {
            continue;
        } else {
            if (d_[finish_point] + w_now < w_res) {
                p_min_tmp = p_min;
                reverse(p.begin(), p.end());
                int t_curr = finish_point;
                for (int i = 0; i < p.size(); ++i) {
                    if (p[i].first == t_curr) {
                        ans.push_back(t_curr);
                        t_curr = p[i].second;
                    }
                }
                ans.push_back(p_min[i]);
                reverse(ans.begin(), ans.end());
                auto first = p_min_tmp.cbegin() + i;
                auto last = p_min_tmp.cend();
                p_min_tmp.erase(first, last);
                for (size_t j = 0; j < ans.size(); ++j) {
                    p_min_tmp.push_back(ans[j]);
                }
                w = d_[finish_point] + w_now;
                index_new_start = i;
            }
        }
    }
    if (index_new_start != -1) {
        step(n, k, d, p_min[index_new_start], finish_point, g_curr, w, w_res, p_min_tmp, p_res);
    }
}

int main() {
    int n, s, t, w, k;
    cout << "Input n (count of vertex): \n";
    cin >> n;
    cout << "Input s (start vertex): \n";
    cin >> s;
    cout << "Input t (finish vertex): \n";
    cin >> t;
    cout << "Input k: \n";
    cin >> k;
    --s;
    --t;
    int t_curr = t;
    vector<vector<int>> tmp(n, vector<int>(n)), g_curr;
    vector<int> ans, p_res;
    priority_queue<pair<int, int>> a;
    vector<pair<int, int>> p;
    vector<int> d(n, 1000000000);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> tmp[i][j];
        }
    }
    g_curr = tmp;
    dij(n, d, s, tmp, p, a);
    if (d[t] == 1000000000) {
        cout << "No paths from s to t!" << "\n";
        return 0;
    } else {
        cout << "The value of shortest path: " << d[t] << "\n";
        w = d[t];
        reverse(p.begin(), p.end());
        for (int i = 0; i < p.size(); ++i) {
            if (p[i].first == t_curr) {
                ans.push_back(t_curr);
                t_curr = p[i].second;
            }
        }
        ans.push_back(s);
        reverse(ans.begin(), ans.end());
        cout << "The shortest path: ";
        for (size_t i = 0; i < ans.size(); ++i) {
            cout << ans[i] + 1 << " ";
        }
        cout << "\n";
    }
    int w_res = -1;
    step(n, k, d, s, t, g_curr, w, w_res, ans, p_res);
    if (w_res == -1 || w_res == 1000000000) {
        cout << "No paths from s to t with blocked edges!" << "\n";
        return 0;
    } else {
        cout << "The value of shortest path with blocked edges: " << w_res << "\n";
        cout << "The shortest path with blocked edges: ";
        for (size_t i = 0; i < p_res.size(); ++i) {
            cout << p_res[i] + 1 << " ";
        }
        cout << "\n";
    }
    return 0;
}


