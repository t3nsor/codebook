// 2-SAT solver based on Kosaraju's algorithm.
// Variables are 0-based. Positive variables are stored in vertices 2n, corresponding negative variables in 2n+1
// TODO: This is quite slow (3x-4x slower than Gabow's algorithm)
struct TwoSat {
	int n;
	vector<vector<int> > adj, radj, scc;
	vector<int> sid, vis, val;
	stack<int> stk;
	int scnt;
	
	// n: number of variables, including negations
	TwoSat(int n): n(n), adj(n), radj(n), sid(n), vis(n), val(n, -1) {}

	// adds an implication
	void impl(int x, int y) { adj[x].push_back(y); radj[y].push_back(x); }
	// adds a disjunction
	void vee(int x, int y) { impl(x^1, y); impl(y^1, x); }
	// forces variables to be equal
	void eq(int x, int y) { impl(x, y); impl(y, x); impl(x^1, y^1); impl(y^1, x^1); }
	// forces variable to be true
	void tru(int x) { impl(x^1, x); }
	
	void dfs1(int x) {
		if (vis[x]++) return;
		for (int i = 0; i < adj[x].size(); i++) {
			dfs1(adj[x][i]);
		}
		stk.push(x);
	}
	
	void dfs2(int x) {
		if (!vis[x]) return; vis[x] = 0;
		sid[x] = scnt; scc.back().push_back(x);
		for (int i = 0; i < radj[x].size(); i++) {
			dfs2(radj[x][i]);
		}
	}

	// returns true if satisfiable, false otherwise
	// on completion, val[x] is the assigned value of variable x
	// note, val[x] = 0 implies val[x^1] = 1
	bool two_sat() {
		scnt = 0;
		for (int i = 0; i < n; i++) {
			dfs1(i);
		}
		while (!stk.empty()) {
			int v = stk.top(); stk.pop();
			if (vis[v]) {
				scc.push_back(vector<int>());
				dfs2(v);
				scnt++;
			}
		}
		for (int i = 0; i < n; i += 2) {
			if (sid[i] == sid[i+1]) return false;
		}
		vector<int> must(scnt);
		for (int i = 0; i < scnt; i++) {
			for (int j = 0; j < scc[i].size(); j++) {
				val[scc[i][j]] = must[i];
				must[sid[scc[i][j]^1]] = !must[i];
			}
		}
		return true;
	}
};
