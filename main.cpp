//@author MetalMario971
//@date Oct 16, 2020
//@input arg1 = The number of comparisons to run (integer) default 10.
//@input arg2 = The number of tests to run for each comparison (default 10000000).
//@brief Testing whether returning std::tuple causes a speed loss to using a struct.
// Winner: Tuple by small margin.  Struct is faster in debug (G++9)
//

#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <iomanip>
#include <chrono>
#include <cmath>
#ifdef _WIN32
#include <sstream>
#endif

#define hrc_now() std::chrono::high_resolution_clock::now()

std::string print(double d) {
  std::ostringstream out;
  out.precision(2);
  out << std::fixed << d;
  return out.str();
}

struct vec3 {
public:
  double x = 0;
  double y = 0;
  double z = 0;
  vec3() {}
  vec3(double dx, double dy, double dz) {
    x = (double)dx;
    y = (double)dy;
    z = (double)dz;
  }
  vec3(int dx, int dy, int dz) {
    x = (double)dx;
    y = (double)dy;
    z = (double)dz;
  }
  std::string toString() const {
    return std::string("") + "(" + print(x) + "," + print(y) + "," + print(z) + ")";
  }
  void add_and_avg(const vec3& other) {
    x = (x + other.x) / 2;
    y = (y + other.y) / 2;
    z = (z + other.z) / 2;
  }
};
auto rnd10 = [&]() { return -5 + rand() % 10; };
auto rvec = [&]() { return vec3{rnd10(), rnd10(), rnd10()}; };

std::tuple<double, double, vec3> len2_len_norm_tuple(const vec3& a, const vec3& b) {
  //Returns length between vectors
  //normalized vector from a to b
  vec3 tmp{
      a.x - b.x,
      a.y - b.y,
      a.z - b.z,
  };

  double len2 = (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
  double len = sqrt(len2);
  if (len != 0) {
    tmp.x /= len;
    tmp.y /= len;
    tmp.z /= len;
  }
  else {
    tmp.x = tmp.y = tmp.z = 0;
  }

  return std::make_tuple(len2, len, tmp);
}
struct Annoying_Temp_Struct {
  double len2;
  double len;
  vec3 norm;
};
Annoying_Temp_Struct len2_len_norm_struct(const vec3& a, const vec3& b) {
  //Returns length between vectors
  //normalized vector from a to b
  vec3 tmp{
      a.x - b.x,
      a.y - b.y,
      a.z - b.z,
  };

  double len2 = (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
  double len = sqrt(len2);
  if (len != 0) {
    tmp.x /= len;
    tmp.y /= len;
    tmp.z /= len;
  }
  else {
    tmp.x = tmp.y = tmp.z = 0;
  }

  return Annoying_Temp_Struct{len2, len, tmp};
}
uint64_t parseIntArg(int argc, char** argv, int n, uint64_t def) {
  uint64_t ret = def;
  if (argc > n) {
    std::string arg = "";
    try {
      arg = std::string(argv[n]);
      ret = std::stoi(arg);
    }
    catch (std::invalid_argument& ex) {
      std::cout << "  Argument " << n << arg << " was not a valid integer." << std::endl;
    }
    catch (std::out_of_range& ex) {
      std::cout << "  Argument " << n << arg << " was out of range." << std::endl;
    }
  }
  return ret;
}
// int checkGetTests(int argc, char** argv){
// std::string st;
// uint32_t  ret = 10;
// if (argc>1) {
// std::string arg = "";
// try {
// arg = std::string(argv[1]);
// ret = std::stoi(arg);
// }
// catch(std::invalid_argument& ex) {
// std::cout<<"  Test # argument " << arg << " was not a valid integer."<<std::endl;
// }
// catch(std::out_of_range& ex) {
// std::cout<<"  Test # argument " << arg << " was out of range."<<std::endl;
// }
// }
// return ret;
// }
uint64_t test_tuple(uint64_t nTests, unsigned int seed) {
  srand(seed);
  vec3 v_avg = {0, 0, 0};
  double len_avg = 0;
  double len2_avg = 0;
  auto tp0 = hrc_now();
  for (uint64_t iTest = 0; iTest < nTests; ++iTest) {
    vec3 v_ret;
    double len2_ret;
    double len_ret;

    std::tie(len2_ret, len_ret, v_ret) = len2_len_norm_tuple(rvec(), rvec());

    //Sum the data to make sure we match.
    v_avg.add_and_avg(v_ret);
    len_avg = (len_avg + len_ret) / 2;
    len2_avg = (len2_avg + len2_ret) / 2;
  }
  uint64_t resultA = std::chrono::duration_cast<std::chrono::milliseconds>(hrc_now() - tp0).count();
  std::cout << "  time:" << resultA << "ms" << std::endl;
  std::cout << "    v_avg     = " << v_avg.toString() << std::endl;
  std::cout << "    len_avg   = " << print(len_avg) << std::endl;
  std::cout << "    len2_avg  = " << print(len2_avg) << std::endl;
  return resultA;
}
uint64_t test_struct(uint64_t nTests, unsigned int seed) {
  srand(seed);
  vec3 v_avg = {0, 0, 0};
  double len_avg = 0;
  double len2_avg = 0;
  auto tp0 = hrc_now();
  for (uint64_t iTest = 0; iTest < nTests; ++iTest) {
    Annoying_Temp_Struct s = len2_len_norm_struct(rvec(), rvec());

    //Sum the data to make sure we match.
    v_avg.add_and_avg(s.norm);
    len_avg = (len_avg + s.len) / 2;
    len2_avg = (len2_avg + s.len2) / 2;
  }
  uint64_t resultB = std::chrono::duration_cast<std::chrono::milliseconds>(hrc_now() - tp0).count();
  std::cout << "  time:" << resultB << "ms" << std::endl;
  std::cout << "    v_avg     = " << v_avg.toString() << std::endl;
  std::cout << "    len_avg   = " << print(len_avg) << std::endl;
  std::cout << "    len2_avg  = " << print(len2_avg) << std::endl;
  std::cout << std::endl;
  return resultB;
}
std::tuple<int, double> test_em(uint64_t nTests, bool reverse = false) {
  int winner = 0;
  uint64_t resultA;  //Tuple
  uint64_t resultB;  //Struct

  unsigned int seed = (unsigned int)time(NULL);
  //Test call order to prevent internal cacheing.
  if (reverse) {
    resultB = test_struct(nTests, seed);
    resultA = test_tuple(nTests, seed);
  }
  else {
    resultA = test_tuple(nTests, seed);
    resultB = test_struct(nTests, seed);
  }

  std::cout << std::endl;
  std::cout << " (The above test results should match) " << std::endl
            << std::endl;

  double dif = 0;

  if (resultA < resultB) {
    dif = (double)(resultB - resultA) / (double)resultB * 100;
    std::cout << "  WINNER: TUPLE (" << print(dif) << "%"
              << ")" << std::endl
              << std::endl;
    winner = 1;
  }
  else if (resultA == resultB) {
    std::cout << "Times matched" << std::endl;
  }
  else {
    dif = (double)(resultA - resultB) / (double)resultA * 100;
    std::cout << "  WINNER: STRUCT (" << print(dif) << "%"
              << ")" << std::endl
              << std::endl;
    winner = 2;
  }

  return std::make_pair(winner, dif);
}
int main(int argc, char** argv) {
  //Argument 1 is the number of tests to run.
  uint64_t nTests = parseIntArg(argc, argv, 1, 10);
  uint64_t nTestTime = parseIntArg(argc, argv, 2, 10000000);
  std::cout << std::endl;
  double avg_dif_tuple = 0;
  double avg_dif_struct = 0;
  int tuple_wins = 0;
  int struct_wins = 0;
  int ties = 0;

  std::cout << "Running " << nTests << " tuple vs struct tests, " << nTestTime << " times:" << std::endl;

  for (int i = 0; i < nTests; ++i) {
    std::cout << "--- Test " << i << "/" << nTests << " ---" << std::endl;
    double dif;
    int winner;
    std::tie(winner, dif) = test_em(nTestTime, i % 2 == 0);  //Flipflop the call order
    if (winner == 0) {
      ties++;
    }
    else if (winner == 1) {
      avg_dif_tuple = (avg_dif_tuple + dif) / 2;
      tuple_wins++;
    }
    else if (winner == 2) {
      avg_dif_struct = (avg_dif_struct + dif) / 2;
      struct_wins++;
    }
  }
  std::cout << "Struct: " << print(avg_dif_struct) << "% faster, on average and " << struct_wins << " wins." << std::endl;
  std::cout << "Tuple: " << print(avg_dif_tuple) << "% faster, on average and " << tuple_wins << " wins." << std::endl;

  return 0;
}