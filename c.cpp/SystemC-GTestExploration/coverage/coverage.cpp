#include <vector>
#include <string>

using namespace std;

struct A {
  vector<string> reference_tokens;
};

int main() {
  A a;

  vector <string> rt = {"a", "b", "c"};

  a.reference_tokens = {rt[0]};
  return 0;
}
