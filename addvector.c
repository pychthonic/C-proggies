/* See my_memcmp_add_vector_main.c */


void addvector1(int *r, const int *a, const int *b, unsigned int len)
{
  unsigned int i;
  for(i=0;i<len;i++) {
      *r = *a + *b;
      r++;
      a++;
      b++;
  }
}

void addvector2(int *r, const int *a, const int *b, unsigned int len)
{
  unsigned int i;
  for(i=0;i<len;i++) {
      r[i] = a[i] + b[i];
  }
}
