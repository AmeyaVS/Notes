int z;

int
func (int sel_exp, int a, int b)
{

  switch (sel_exp)
  {
    case 5:
      z = a + b;
      break;
    default:
      z = a - b;
      break;
  }
  z = z * b;
}
