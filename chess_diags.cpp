#include <algorithm>
#include <iostream>
#include <vector>

class Graph {
public:
	std::vector <std::vector <int> > p1, p2;
	int n, m;

	Graph(int nn, int  mm) {
		n = nn;
		m = mm;
		p1 = std::vector <std::vector <int> >(n + m - 1);
		p2 = std::vector <std::vector <int> >(n + m - 1);
	}

	void add_edge(int dg_1, int dg_2) {
		p1[dg_1].push_back(dg_2);
		p2[dg_2].push_back(dg_1);
	}

	void add_orient_edge(bool fr_1_flag, int dg_1, int dg_2) {
		if (fr_1_flag) {
			p1[dg_1].push_back(dg_2);
		} else {
			p2[dg_1].push_back(dg_2);
		}
	}

	int crd_to_1diag(int i, int j) {
		if (i == n - 1) {
			return m - j - 1;
		} else {
			return m + n - 2 - i;
		}
	}

	int crd_to_2diag(int i, int j) {
		if (j == 0) {
			return n - i - 1;
		} else {
			return n + j - 1;
		}
	}

	void print_1diag_to_str(int dg_num, bool def_flag) {
		char ww, bb;
		if (def_flag) {
			ww = 'W';
			bb = 'B';
		} else {
			ww = 'B';
			bb = 'W';
		}
		int n_i, n_j;
		if (dg_num < m) {
			n_i = n;
			n_j = m - dg_num;
		} else {
			n_i = n + m - dg_num - 1;
			n_j = 1;
		}
		std::cout << "1 " << n_i << " " << n_j << " "; 
		((n_i + n_j) % 2 == 0) ? std::cout << ww : std::cout << bb;
		std::cout << std::endl;
	}

	void print_2diag_to_str(int dg_num, bool def_flag) {
		char ww, bb;
		if (def_flag) {
			ww = 'W';
			bb = 'B';
		} else {
			ww = 'B';
			bb = 'W';
		}
		int n_i, n_j;
		if (dg_num < n) {
			n_i = n - dg_num;
			n_j = 1;
		} else {
			n_i = 1;
			n_j = dg_num - n + 2;
		}
		std::cout << "2 " << n_i << " " << n_j << " "; 
		((n_i + n_j) % 2 == 0) ? std::cout << ww : std::cout << bb;
		std::cout << std::endl;
	}
};


bool find_way(int &maxlen, std::vector <int> &way, Graph &g, int v, std::vector <bool> &us1, std::vector <int> &usd, std::vector <bool> &used) {
	if (used[v]) {
		return false;
	}
	used[v] = true;
	for (int j = 0; j != g.p1[v].size(); ++j) {
		int v_in_p2 = g.p1[v][j];
		if ((usd[v_in_p2] == -1) || (find_way(maxlen, way, g, usd[v_in_p2], us1, usd, used))) {
			us1[v] = true;
			usd[v_in_p2] = v;
			return true;
		}
	}
	return false;
}

void DFS(Graph &g, bool pflag, int v, std::vector <bool> &us1, std::vector <bool> &us2) {
	if (pflag) {
		if (us2[v]) {
			return;
		}
		us2[v] = true;
		for (int i : g.p2[v]) {
			DFS(g, false, i, us1, us2);
		}
	} else {
		if (us1[v]) {
			return;
		}
		us1[v] = true;
		for (int i : g.p1[v]) {
			DFS(g, true, i, us1, us2);
		}
	}
}

void convert_to_vertex_cover_out(int cnt_diags, int fsize, Graph &g, std::vector <bool> &usd1, std::vector <int> &usd2, bool def_flag) {
	Graph org(g.n, g.m);
	for (int i = 0; i != cnt_diags; ++i) {
		for (int j : g.p1[i]) {
			if (usd2[j] == i) {
				org.add_orient_edge(false, j, i);
			} else {
				org.add_orient_edge(true, i, j);
			}
		}
	}

	std::vector <bool> usd_p1(cnt_diags, false), usd_p2(cnt_diags, false);
	for (int i = 0; i != cnt_diags; ++i) {
		if (!usd1[i]) {
			DFS(org, false, i, usd_p1, usd_p2);
		}
	}
	std::cout << fsize << "\n";
	for (int i = 0; i != cnt_diags; ++i) {
		if (!usd_p1[i]) {
			org.print_1diag_to_str(i, def_flag);
		}
	}
	for (int i = 0; i != cnt_diags; ++i) {
		if (usd_p2[i]) {
			org.print_2diag_to_str(i, def_flag);
		}
	}
	return;
}

int main() {
	int n, m;
	std::cin >> n >> m;
	int cnt_diags = n + m - 1;
	Graph g1(n, m), g2(n, m);
	char c;
	for (int i = 0; i != n; ++i) {
		for (int j = 0; j != m; ++j) {
			std::cin >> c;
			int n_i = i, n_j = j, dg_crd = std::min(j, n - i - 1);
			n_i += dg_crd;
			n_j -= dg_crd;
			int dg1_num = g1.crd_to_1diag(n_i, n_j);
			n_i = i, n_j = j, dg_crd = std::min(j, i);
			n_i -= dg_crd;
			n_j -= dg_crd;
			int dg2_num = g1.crd_to_2diag(n_i, n_j);
			if ((((i + j) % 2 == 0) && (c != 'W')) || (((i + j) % 2 == 1) && (c != 'B'))) {
				g1.add_edge(dg1_num, dg2_num);
			}
			if ((((i + j) % 2 == 0) && (c != 'B')) || (((i + j) % 2 == 1) && (c != 'W'))) {
				g2.add_edge(dg1_num, dg2_num);
			}
		}
	}
	// searching maximum matching
	std::vector <bool> usd1_1(cnt_diags, false);
	std::vector <int> usd2_1(cnt_diags, -1);
	int maxlen = 0;
	std::vector <int> way;
	std::vector <bool> used(cnt_diags, false);
	// std::set <std::pair<int, int>> matching;
	// int cnt = 0;
	while (true) {
		bool check = false;
		for (int i = 0; i != cnt_diags; ++i) {
			if (!usd1_1[i]) {
				way = {};
				check = check || (find_way(maxlen, way, g1, i, usd1_1, usd2_1, used));
				for (int j = 0; j != cnt_diags; ++j) {
					used[j] = false;
				}
			}
		}
		if (!check) {
			break;
		}
	}
	for (int i = 0; i != cnt_diags; ++i) {
		used[i] = false;
	}
	maxlen = 0;
	std::vector <bool> usd1_2(cnt_diags, false);
	std::vector <int> usd2_2(cnt_diags, -1);
	while (true) {
		bool check = false;
		for (int i = 0; i != cnt_diags; ++i) {
			if (!usd1_2[i]) {
				way = {};
				check = check || (find_way(maxlen, way, g2, i, usd1_2, usd2_2, used));
				for (int j = 0; j != cnt_diags; ++j) {
					used[j] = false;
				}
			}
		}
		if (!check) {
			break;
		}
	}
	// checking what is minimum
	int fsize1 = 0, fsize2 = 0;
	for (int i = 0; i != cnt_diags; ++i) {
		if (usd1_1[i]) {
			++fsize1;
		}
		if (usd1_2[i]) {
			++fsize2;
		}
	}
	(fsize1 > fsize2) ? convert_to_vertex_cover_out(cnt_diags, fsize2, g2, usd1_2, usd2_2, false) : convert_to_vertex_cover_out(cnt_diags, fsize1, g1, usd1_1, usd2_1, true);
	return 0;
}
