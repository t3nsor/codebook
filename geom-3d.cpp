#define LINE 0
#define SEGMENT 1
#define RAY 2

struct point{
    double x, y, z;
    point(){};
    point(double _x, double _y, double _z){ x=_x; y=_y; z=_z; }
    point operator+ (point p) { return point(x+p.x, y+p.y, z+p.z); }
    point operator- (point p) { return point(x-p.x, y-p.y, z-p.z); }
    point operator* (double c) { return point(x*c, y*c, z*c); }
};

double dot(point a, point b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

point cross(point a, point b) {
    return point(a.y*b.z-a.z*b.y,
                 a.z*b.x-a.x*b.z,
                 a.x*b.y-a.y*b.x);
}

double distSq(point a, point b){
    return dot(a-b, a-b);
}

// compute a, b, c, d such that all points lie on ax + by + cz = d. TODO: test this
double planeFromPts(point p1, point p2, point p3, double& a, double& b, double& c, double& d) {
	point normal = cross(p2-p1, p3-p1);
	a = normal.x; b = normal.y; c = normal.z;
	d = -a*p1.x-b*p1.y-c*p1.z;
}

// project point onto plane. TODO: test this
point ptPlaneProj(point p, double a, double b, double c, double d) {
	double l = (a*p.x+b*p.y+c*p.z+d)/(a*a+b*b+c*c);
	return point(p.x-a*l, p.y-b*l, p.z-c*l);
}

// distance from point p to plane aX + bY + cZ + d = 0
double ptPlaneDist(point p, double a, double b, double c, double d){
    return fabs(a*p.x + b*p.y + c*p.z + d) / sqrt(a*a + b*b + c*c);
}

// distance between parallel planes aX + bY + cZ + d1 = 0 and
// aX + bY + cZ + d2 = 0
double planePlaneDist(double a, double b, double c, double d1, double d2){
    return fabs(d1 - d2) / sqrt(a*a + b*b + c*c);
}

// square distance between point and line, ray or segment
double ptLineDistSq(point s1, point s2, point p, int type){
    double pd2 = distSq(s1, s2);
    point r;
    if(pd2 == 0)
	r = s1;
    else{
	double u = dot(p-s1, s2-s1) / pd2;
	r = s1 + (s2 - s1)*u;
	if(type != LINE && u < 0.0)
	    r = s1;
	if(type == SEGMENT && u > 1.0)
	    r = s2;
    }
    return distSq(r, p);
}

// Distance between lines ab and cd. TODO: Test this
double lineLineDistance(point a, point b, point c, point d) {
	point v1 = b-a;
	point v2 = d-c;
	point cr = cross(v1, v2);
	if (dot(cr, cr) < EPS) {
		point proj = v1*(dot(v1, c-a)/dot(v1, v1));
		return sqrt(dot(c-a-proj, c-a-proj));
	} else {
		point n = cr/sqrt(dot(cr, cr));
		point p = dot(n, c - a);
		return sqrt(dot(p, p));
	}
}

// Distance between line segments ab and cd (translated from Java)
double segmentSegmentDistance(point a, point b, point c, point d) {
	point u = b - a, v = d - c, w = a - c;
	double a = dot(u, u), b = dot(u, v), c = dot(v, v), d = dot(u, w), e = dot(v, w);
	double D = a*c-b*b;
	double sc, sN, sD = D;
	double tc, tN, tD = D;

    // compute the line parameters of the two closest points
    if (D < EPS) { // the lines are almost parallel
        sN = 0.0;        // force using point P0 on segment S1
        sD = 1.0;        // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else {                // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {           // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1 => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < EPS ? 0.0 : sN / sD);
    tc = (abs(tN) < EPS ? 0.0 : tN / tD);

    // get the difference of the two closest points
    point dP = w + (sc * u) - (tc * v);  // = S1(sc) - S2(tc)
    return sqrt(dot(dP, dP));   // return the closest distance
}

double signedTetrahedronVol(point A, point B, point C, point D) {
	double A11 = A.x - B.x;
	double A12 = A.x - C.x;
	double A13 = A.x - D.x;
	double A21 = A.y - B.y;
	double A22 = A.y - C.y;
	double A23 = A.y - D.y;
	double A31 = A.z - B.z;
	double A32 = A.z - C.z;
	double A33 = A.z - D.z;
	double det =
		A11*A22*A33 + A12*A23*A31 +
		A13*A21*A32 - A11*A23*A32 -
		A12*A21*A33 - A13*A22*A31;
	return det / 6;
}

// Parameter is a vector of vectors of points - each interior vector
// represents the 3 points that make up 1 face, in any order.
// Note: The polyhedron must be convex, with all faces given as triangles.
double polyhedronVol(vector<vector<point> > poly) {
	int i,j;
	point cent(0,0,0);
	for (i=0; i<poly.size(); i++)
		for (j=0; j<3; j++)
			cent=cent+poly[i][j];
	cent=cent*(1.0/(poly.size()*3));
	double v=0;
	for (i=0; i<poly.size(); i++)
		v+=fabs(signedTetrahedronVol(cent,poly[i][0],poly[i][1],poly[i][2]));
	return v;
}

