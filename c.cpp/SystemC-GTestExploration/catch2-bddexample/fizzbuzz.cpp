#include <string>

std::string fizzbuzz(const int number)
{
  std::string result;
  if(number != 0)
  {
    auto m3 = number % 3;
    auto m5 = number % 5;
    if(!m3) { result = "fizz"; }
    if(!m5) { result += "buzz"; }
  }
  if(result == "") { result = std::to_string(number); }
  return result;
}
