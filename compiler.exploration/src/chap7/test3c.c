
int fun (int x, int y)
{
  int *p;
  int a, b;

  /* NOT dead store */
  a = x+y;

  p = &a;
  b = *p;

  return(b);

}
