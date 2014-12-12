// This is the famous "Yarin sieve", for use when memory is tight.
#define MAXSIEVE 100000000 // All prime numbers up to this
#define MAXSIEVEHALF (MAXSIEVE/2)
#define MAXSQRT 5000 // sqrt(MAXSIEVE)/2
char a[MAXSIEVE/16+2];
#define isprime(n) (a[(n)>>4]&(1<<(((n)>>1)&7))) // Works when n is odd

int i,j;
memset(a,255,sizeof(a));
a[0]=0xFE;
for(i=1;i<MAXSQRT;i++)
if (a[i>>3]&(1<<(i&7)))
for(j=i+i+i+1;j<MAXSIEVEHALF;j+=i+i+1)
a[j>>3]&=~(1<<(j&7));
