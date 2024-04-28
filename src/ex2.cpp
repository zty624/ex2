#include "myroot.hpp"

extern TRandom3 *grand;
const long int MAX = 1e8;
const long int MAX2 = 1e6;
const double N0 = 100;
std::vector<int> n_array = {1, 10, 30, 60};
std::vector<int> n_array2 = {1, 10, 30, 60, 90, 180, 250, 360};
std::vector<long int> tmp = {0,0,0,0};
std::vector<int> tmp2 = {0,0,0,0,0,0,0};
std::vector<long long> event;

double stock_random() {
    double x = grand->Uniform();
    if (x < 0.5) {
        return 1.7;
    } else {
        return 0.5;
    }
}

int main() {
    // question 1
    for (int j = 0; j < MAX; j++) {
        long long int n = N0;
        for (int k = 0; k < n_array.size(); k++) {
            for (int l = (k == 0)? 0 : n_array[k-1]; l < n_array[k]; l++) {
                n *= stock_random();
            }
            tmp[k] += n;
        }
    }
    for (int i = 0; i < 4; i++) {
        std::cout << "n = " << n_array[i] << " : " << (double)tmp[i]/MAX << std::endl;
    }

    // question 2
    for (int j = 0; j < MAX2; j++) {
        double n = N0;
        for (int k = 0; k < n_array2.size(); k++) {
            for (int l = (k == 0)? 0 : n_array2[k-1]; l < n_array2[k]; l++) {
                n *= stock_random();
            }
            if (n < 1) {
                tmp2[k]++;
                event.push_back(n_array2[k]);
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        std::cout << "n = " << n_array2[i] << " : " << (double)tmp2[i]/MAX2 << std::endl;
    }
    Histogram<long long> hist(event.data(), event.size(), "stock", "n", "Count", false, false, 380, 0, 380);
    hist.draw_histogram(hist.canvas);
    return 0;
}