#include "myroot.hpp"

TRandom3 *grand = new TRandom3(0);
const double LAMBDA1 = 100; // Hz
const double LAMBDA2 = 200; // Hz
const double TAU = 10e-6; // us 
const double TOTAL_TIME = 3600 * 10; // s
const double TIME_EPS = 1e-7; // s

std::vector<double> data1;
std::vector<double> data2;
std::vector<double> time1;
std::vector<double> time2;
std::vector<double> event;


double get_random(const double &tau) {
  double x = grand->Uniform();
  double y = - log(1 - x) / tau;
  return y;
}

int main() {
  // question 1
  // generate 1 event
  double time = 0, tmp = 0;
  while (time < TOTAL_TIME) {
    tmp = get_random(LAMBDA1);
    data1.push_back(tmp);
    time1.push_back(time);
    time += tmp;
  };
  Histogram<double> hist1(data1, "data1");
  hist1.draw_histogram(100, "", "Time/s", "Count", 0.0, 3e-2);

  // generate 2 events
  time = 0, tmp = 0;
  while (time < TOTAL_TIME) {
    tmp = get_random(LAMBDA2);
    data2.push_back(tmp);
    time2.push_back(time);
    time += tmp;
  };
  Histogram<double> hist2(data2, "data2");
  hist2.draw_histogram(100, "", "Time/s", "Count", 0.0, 3e-2);

  // question 2
  unsigned long int i1 = 0, i2 = 0;
  double past = 0;
  while (i1 < time1.size() && i2 < time2.size()) {
    if (abs(time1[i1] - time2[i2]) < TAU) {
      event.push_back(TMath::Min(time1[i1], time2[i2]) - past);
      past = TMath::Min(time1[i1], time2[i2]);
      i1++;
      i2++;
      continue;
    }
    if (time1[i1] < time2[i2]) {
      i1++;
    } else {
      i2++;
    }
  };
  Histogram<double> ent(event, "q1EventA");
  ent.draw_histogram(100, "", "Time/s", "Count", 0.0, 10);
  
  delete grand;
  return 0;
}