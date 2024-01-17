#include <cmath>
#include <fstream>

int main() {
  std::ofstream out;
  out.open("SineValues.h");
  out << "#pragma once\n";
  out << "#include <array>\n";
  out << "#include <cmath>\n\n";

  unsigned numResults = 500000;
  double step = M_PI / numResults;
  double value = 0;

  out << "constexpr std::array<double, " << std::to_string(numResults) << "> sineValues = {\n";
  out.precision(15);
  for (unsigned i = 0; i < numResults; i++) {
    double res = std::sin(value);
    value += step;
    out << "    " << res << ",\n";
  }
  out << "};\n\n";
  out << "constexpr double SINE_STEP = M_PI / sineValues.size();\n";
  out.close();
}
