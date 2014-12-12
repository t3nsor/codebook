// Linear time all nearest smaller values, standard stack-based algorithm.
// ansv_left stores indices of nearest smaller values to the left in res. -1 means no smaller value was found.
// ansv_right likewise looks to the right. v.size() means no smaller value was found.
void ansv_left(vector<int>& v, vector<int>& res) {
	stack<pair<int, int> > stk; stk.push(make_pair(INT_MIN, v.size()));
	for (int i = v.size()-1; i >= 0; i--) {
		while (stk.top().first > v[i]) {
			res[stk.top().second] = i; stk.pop();
		}
		stk.push(make_pair(v[i], i));
	}
	while (stk.top().second < v.size()) {
		res[stk.top().second] = -1; stk.pop();
	}
}

void ansv_right(vector<int>& v, vector<int>& res) {
	stack<pair<int, int> > stk; stk.push(make_pair(INT_MIN, -1));
	for (int i = 0; i < v.size(); i++) {
		while (stk.top().first > v[i]) {
			res[stk.top().second] = i; stk.pop();
		}
		stk.push(make_pair(v[i], i));
	}
	while (stk.top().second > -1) {
		res[stk.top().second] = v.size(); stk.pop();
	}
}
