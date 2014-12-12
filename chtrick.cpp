// "Convex hull trick": data structure that maintains a set of lines y = mx + b and allows querying the minimum value of mx_0 + b over all lines for some given x_0. Very useful in optimizing DP algorithms for partitioning problems.
// Tested against USACO MAR08 acquire. TODO: Test against IOI '02 Batch.
struct ConvexHullTrick {
	typedef long long LL;
	vector<LL> M;
	vector<LL> B;
	vector<double> left;
	ConvexHullTrick() {}
	bool bad(LL m1, LL b1, LL m2, LL b2, LL m3, LL b3) {
			// Careful, this may overflow
			return (b3-b1)*(m1-m2) < (b2-b1)*(m1-m3);
	}
	// Add a new line to the structure, y = mx + b.
	// Lines must be added in decreasing order of slope.
	void add(LL m, LL b) {
			while (M.size() >= 2 && bad(M[M.size()-2], B[B.size()-2], M.back(), B.back(), m, b)) {
					M.pop_back(); B.pop_back(); left.pop_back();
			}
			if (M.size() && M.back() == m) {
					if (B.back() > b) {
							M.pop_back(); B.pop_back(); left.pop_back();
					} else {
							return;
					}
			}
			if (M.size() == 0) {
					left.push_back(-numeric_limits<double>::infinity());
			} else {
					left.push_back((double)(b - B.back())/(M.back() - m));
			}
			M.push_back(m);
			B.push_back(b);
	}
	// Get the minimum value of mx + b among all lines in the structure.
	// There must be at least one line.
    LL query(LL x) {
                int i = upper_bound(left.begin(), left.end(), x) - left.begin();
                return M[i-1]*x + B[i-1];
        }
};
