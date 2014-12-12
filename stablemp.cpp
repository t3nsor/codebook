// Gale-Shapley algorithm for the stable marriage problem.
// madj[i][j] is the jth highest ranked woman for man i.
// fpref[i][j] is the rank woman i assigns to man j.
// Returns a pair of vectors (mpart, fpart), where mpart[i] gives the partner of man i, and fpart is analogous
pair<vector<int>, vector<int> > stable_marriage(vector<vector<int> >& madj, vector<vector<int> >& fpref) {
	int n = madj.size();
	vector<int> mpart(n, -1), fpart(n, -1);
	vector<int> midx(n);
	queue<int> mfree;
	for (int i = 0; i < n; i++) {
		mfree.push(i);
	}
	while (!mfree.empty()) {
		int m = mfree.front(); mfree.pop();
		int f = madj[m][midx[m]++];
		if (fpart[f] == -1) {
			mpart[m] = f; fpart[f] = m;
		} else if (fpref[f][m] < fpref[f][fpart[f]]) {
			mpart[fpart[f]] = -1; mfree.push(fpart[f]);
			mpart[m] = f; fpart[f] = m;
		} else {
			mfree.push(m);
		}
	}
	return make_pair(mpart, fpart);
}
