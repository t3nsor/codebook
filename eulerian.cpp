// Eulerian path/circuit in an undirected graph. TODO: Does this handle self-edges?
// NOTE(Brian): This looks like it could theoretically degrade to quadratic time in, say, a graph where we keep going back and forth between two vertices; in this case a lot of time could be wasted searching for an unused edge.
struct EulerianPath {
	int n;
	vector<vector<int> > adj;
	vector<pair<int, int> > edges;
	vector<int> valid;
	vector<int> circuit;

	EulerianPath(int n): n(n), adj(n) {}

	// Call this to construct the graph.
	// Edges are zero-based and undirected (only add each edge once!)
	void add_edge(int x, int y) {
		adj[x].push_back(edges.size());
		adj[y].push_back(edges.size());
		edges.push_back(make_pair(x, y));
		valid.push_back(1);
	}
	
	void find_path(int x){
	  for(int i = 0; i < adj[x].size(); i++){
		int e = adj[x][i];
		if(!valid[e]) continue;
		int v = edges[e].first;
		if(v == x) v = edges[e].second;
		valid[e] = 0;
		find_path(v);
	  }
	  circuit.push_back(x);
	}

	// Call this to find the path/circuit (autodetects)
	// Returns the path/circuit itself in "circuit" variable
	// Initial node is repeated at end if it's a circuit.
	void find_euler_path() {
	  circuit.clear();
	  //supposes graph is connected and has correct degree
	  for(int i = 0; i < n; i++)
		if(adj[i].size()%2){
		  find_path(i);
		  return;
		}
	  find_path(0);
	}
};
