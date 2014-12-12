// O(N log N) Monotone Chains algorithm for 2d convex hull.
// Gives the hull in counterclockwise order from the leftmost point, which is repeated at the end. Minimizes the number of points on the hull when collinear points exist.
long long cross(pair<int, int> A, pair<int, int> B, pair<int, int> C) {
	return (B.first - A.first)*(C.second - A.second)
	     - (B.second - A.second)*(C.first - A.first);
}
// The hull is returned in param "hull"
void convex_hull(vector<pair<int, int> > pts, vector<pair<int, int> >& hull) {
	hull.clear(); sort(pts.begin(), pts.end());
	for (int i = 0; i < pts.size(); i++) {
		while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(pts[i]);
	}
	int s = hull.size();
	for (int i = pts.size()-2; i >= 0; i--) {
		while (hull.size() >= s+1 && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0) {
			hull.pop_back();
		}
		hull.push_back(pts[i]);
	}
}

