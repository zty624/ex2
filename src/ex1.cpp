#include "myroot.hpp"

extern TRandom3 *grand;
const double LAMBDA1 = 100; // Hz
const double LAMBDA2 = 200; // Hz
const double TAU = 10e-6; // us 
const double TOTAL_TIME = 100; // s
const double TIME_EPS = 1e-8; // s

std::vector<double> data1;
std::vector<double> data2;

// use poisson to generate random numbers
auto get_poisson(const double &lamda) {
  double x = grand->Uniform();
  if (x < (lamda * TIME_EPS)) {
    return true;
  } else {
    return false;
  }
}

auto get_random(const double &tau) {
  double x = grand->Uniform();
  double y = - log(1 - x) * tau;
  return y;
}

int main() {
  // question 1
  // generate 1 event
  double time = 0, tmp = 0;
  while (time < TOTAL_TIME) {
    if (get_poisson(LAMBDA1)) {
      data1.push_back(time - tmp);
      tmp = time;
    }
    time += TIME_EPS;
  };
  double *data1_c = data1.data();
  Histogram<double> hist1(data1_c, data1.size(), "Exptest.root", "Time", "Count", false, false, 100, 0.0, 3e-2);
  hist1.draw_histogram(hist1.canvas);

  // generate 2 events
  time = 0, tmp = 0;
  while (time < TOTAL_TIME) {
    if (get_poisson(LAMBDA2)) {
      data2.push_back(time - tmp);
      tmp = time;
    }
    time += TIME_EPS;
  };
  double *data2_c = data2.data();
  Histogram<double> hist2(data1_c, data1.size(), "Exptest2.root", "Time", "Count", false, false, 100, 0.0, 1e-2);
  hist2.draw_histogram(hist2.canvas);
  
  delete[] data1_c;
  delete[] data2_c;
  return 0;
}