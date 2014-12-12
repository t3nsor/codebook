// Depending on your application it's pretty unlikely that you'll have to type out the entirety of this struct. For example, in most cases you won't need division, and you can leave out most of the operators too.
// NB: Thse are fairly terrible implementations. Multiplication is about twice as slow as Python, and division is about 20 times slower. Use only as a last resort when for some reason you can't use Java's native bignums.
struct bignum {
	typedef unsigned int uint;
	
	vector<uint> digits;
	static const uint RADIX = 1000000000;
	
	bignum(): digits(1, 0) {}
	
	bignum(const bignum& x): digits(x.digits) {}
	
	bignum(unsigned long long x) {
		*this = x;
	}
	
	bignum(const char* x) {
		*this = x;
	}
	
	bignum(const string& s) {
		*this = s;
	}
	
	bignum& operator=(const bignum& y) {
		digits = y.digits; return *this;
	}
	
	bignum& operator=(unsigned long long x) {
		digits.assign(1, x%RADIX);
		if (x >= RADIX) {
			digits.push_back(x/RADIX);
		}
		return *this;
	}

	bignum& operator=(const char* s) {
		int slen=strlen(s),i,l;
		digits.resize((slen+8)/9);
		for (l=0; slen>0; l++,slen-=9) {
			digits[l]=0;
			for (i=slen>9?slen-9:0; i<slen; i++) {
				digits[l]=10*digits[l]+s[i]-'0';
			}
		}
		while (digits.size() > 1 && !digits.back()) digits.pop_back();
		return *this;
	}
	
	bignum& operator=(const string& s) {
		return *this = s.c_str();
	}

	void add(const bignum& x) {
		int l = max(digits.size(), x.digits.size());
		digits.resize(l+1);
		for (int d=0, carry=0; d<=l; d++) {
			uint sum=carry;
			if (d<digits.size()) sum+=digits[d];
			if (d<x.digits.size()) sum+=x.digits[d];
			digits[d]=sum;
			if (digits[d]>=RADIX) {
				digits[d]-=RADIX; carry=1;
			} else {
				carry=0;
			}
		}
		if (!digits.back()) digits.pop_back();
	}

	void sub(const bignum& x) {
		// if ((*this)<x) throw; //negative numbers not yet supported
		for (int d=0, borrow=0; d<digits.size(); d++) {
			digits[d]-=borrow;
			if (d<x.digits.size()) digits[d]-=x.digits[d];
			if (digits[d]>>31) { digits[d]+=RADIX; borrow=1; } else borrow=0;
		}
		while (digits.size() > 1 && !digits.back()) digits.pop_back();
	}

	void mult(const bignum& x) {
		vector<uint> res(digits.size() + x.digits.size());
		unsigned long long y,z;
		for (int i=0; i<digits.size(); i++) {
			for (int j=0; j<x.digits.size(); j++) {
				unsigned long long y=digits[i]; y*=x.digits[j];
				unsigned long long z=y/RADIX;
				res[i+j+1]+=z; res[i+j]+=y-RADIX*z; //mod is slow
				if (res[i+j] >= RADIX) { res[i+j] -= RADIX; res[i+j+1]++; }
				for (int k = i+j+1; res[k] >= RADIX; res[k] -= RADIX, res[++k]++);
			}
		}
		digits = res;
		while (digits.size() > 1 && !digits.back()) digits.pop_back();
	}
	
	// returns the remainder
	bignum div(const bignum& x) {
		bignum dividend(*this);
		bignum divisor(x);
		fill(digits.begin(), digits.end(), 0);
		// shift divisor up
		int pwr = dividend.digits.size() - divisor.digits.size();
		if (pwr > 0) {
			divisor.digits.insert(divisor.digits.begin(), pwr, 0);
		}
		while (pwr >= 0) {
			if (dividend.digits.size() > divisor.digits.size()) {
				unsigned long long q = dividend.digits.back();
				q *= RADIX; q += dividend.digits[dividend.digits.size()-2];
				q /= 1+divisor.digits.back();
				dividend -= divisor*q; digits[pwr] = q;
				if (dividend >= divisor) { digits[pwr]++; dividend -= divisor; }
				assert(dividend.digits.size() <= divisor.digits.size()); continue;
			}
			while (dividend.digits.size() == divisor.digits.size()) {
				uint q = dividend.digits.back() / (1+divisor.digits.back());
				if (q == 0) break;
				digits[pwr] += q; dividend -= divisor*q;
			}
			if (dividend >= divisor) { dividend -= divisor; digits[pwr]++; }
			pwr--; divisor.digits.erase(divisor.digits.begin());
		}
		while (digits.size() > 1 && !digits.back()) digits.pop_back();
		return dividend;
	}
	
	string to_string() const {
		ostringstream oss;
		oss << digits.back();
		for (int i = digits.size() - 2; i >= 0; i--) {
			oss << setfill('0') << setw(9) << digits[i];
		}
		return oss.str();
	}

	bignum operator+(const bignum& y) const {
		bignum res(*this); res.add(y); return res;
	}
	
	bignum operator-(const bignum& y) const {
		bignum res(*this); res.sub(y); return res;
	}
	
	bignum operator*(const bignum& y) const {
		bignum res(*this); res.mult(y); return res;
	}
	
	bignum operator/(const bignum& y) const {
		bignum res(*this); res.div(y); return res;
	}
	
	bignum operator%(const bignum& y) const {
		bignum res(*this); return res.div(y);
	}

	bignum& operator+=(const bignum& y) {
		add(y); return *this;
	}

	bignum& operator-=(const bignum& y) {
		sub(y); return *this;
	}

	bignum& operator*=(const bignum& y) {
		mult(y); return *this;
	}
	
	bignum& operator/=(const bignum& y) {
		div(y); return *this;
	}
	
	bignum& operator%=(const bignum& y) {
		*this = div(y);
	}
	
	bool operator==(const bignum& y) const {
		return digits == y.digits;
	}

	bool operator<(const bignum& y) const {
		if (digits.size() < y.digits.size()) return true;
		if (digits.size() > y.digits.size()) return false;
		for (int i = digits.size()-1; i >= 0; i--) {
			if (digits[i] < y.digits[i]) {
				return true;
			} else if (digits[i] > y.digits[i]) {
				return false;
			}
		}
		return false;
	}

	bool operator>(const bignum& y) const {
		return y<*this;
	}

	bool operator<=(const bignum& y) const {
		return !(y<*this);
	}

	bool operator>=(const bignum& y) const {
		return !(*this<y);
	}
};
