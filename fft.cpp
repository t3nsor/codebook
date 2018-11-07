typedef long double T_DOUBLE; // May be changed to double for more speed and less precision

struct cpx
{
  cpx(){}
  cpx(T_DOUBLE aa):a(aa),b(0){}
  cpx(T_DOUBLE aa, T_DOUBLE bb):a(aa),b(bb){}
  T_DOUBLE a;
  T_DOUBLE b;
  T_DOUBLE modsq(void) const
  {
    return a * a + b * b;
  }
  cpx bar(void) const
  {
    return cpx(a, -b);
  }
};

cpx operator +(cpx a, cpx b)
{
  return cpx(a.a + b.a, a.b + b.b);
}

cpx operator *(cpx a, cpx b)
{
  return cpx(a.a * b.a - a.b * b.b, a.a * b.b + a.b * b.a);
}

cpx operator /(cpx a, cpx b)
{
  cpx r = a * b.bar();
  return cpx(r.a / b.modsq(), r.b / b.modsq());
}

cpx EXP(T_DOUBLE theta)
{
  return cpx(cos(theta),sin(theta));
}

const T_DOUBLE two_pi = 4 * acos((T_DOUBLE)0);

// in:     input array
// out:    output array
// step:   {SET TO 1} (used internally)
// size:   length of the input/output {MUST BE A POWER OF 2}
// dir:    either plus or minus one (direction of the FFT)
// RESULT: out[k] = \sum_{j=0}^{size - 1} in[j] * exp(dir * 2pi * i * j * k / size)
void FFT(cpx *in, cpx *out, int step, int size, int dir)
{
  if(size < 1) return;
  if(size == 1)
  {
    out[0] = in[0];
    return;
  }
  int half = size >> 1;
  FFT(in, out, step * 2, half, dir);
  FFT(in + step, out + half, step << 1, half, dir);
  for(int i = 0 ; i < half ; i++)
  {
    cpx even = out[i];
    cpx odd = out[i + half];
    out[i] = even + EXP(dir * two_pi * i / size) * odd;
    out[i + half] = even + EXP(dir * two_pi * (i + half) / size) * odd;
  }
}

// f:      input array f
// g:      input array g
// h:      output array h
// size:   length of f/g/h {MUST BE A POWER OF 2}
// RESULT: h[i] = sum of f[k]*g[i-k] for k=0..(size-1)
// Note: f and g are cyclic (e.g. f[-1] = f[size-1]), so they may be padded
// the right with zeroes to instead compute an acyclic convolution.
void FFTConvolution(cpx *f, cpx *g, cpx *h, int size)
{
  cpx *F = new cpx[size], *G = new cpx[size], *H = new cpx[size];
  FFT(f, F, 1, size, 1);
  FFT(g, G, 1, size, 1);
  for (int i = 0; i < size; i++) H[i] = F[i] * G[i];
  FFT(H, h, 1, size, -1);
  for (int i = 0; i < size; i++) h[i] = h[i] / size;
  delete [] F;
  delete [] G;
  delete [] H;
}
