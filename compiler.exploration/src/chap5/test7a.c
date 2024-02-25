int z;

int
func (int sel_exp, int a, int b)
{

  switch (sel_exp)
  {
    case 5:
      z = a + b;
      break;
    case 6:
      z = a + 2*b;
      break;
    case 7:
      z = a + 3*b;
      break;
    default:
      z = a - b;
      break;
  }
  z = z * b;
}
