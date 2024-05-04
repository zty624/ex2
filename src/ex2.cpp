#include "myroot.hpp"

TRandom3 *grand = new TRandom3(0);
const long int MAX = 1e8;
const long int MAX2 = 1e6;
const double N0 = 100;
std::vector<int> n_array = {1, 10, 30, 60};
std::vector<int> n_array2 = {1, 10, 30, 60, 90, 180, 360};
std::vector<double> n_array3 = {1, 10, 30, 60, 90, 180, 360};
std::vector<long int> tmp = {0,0,0,0};
std::vector<int> tmp2 = {0,0,0,0,0,0,0};
std::vector<long long> event;
std::vector<double> prob = {0, 0.024996, 0.258909, 0.522741, 
0.679577,0.888904, 0.983124}, prob_real(7, 0), err1(7,0);

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
    /*
    for (int j = 0; j < MAX; j++) {
        double n = (double)N0;
        for (int k = 0; k < n_array.size(); k++) {
            for (int l = (k == 0)? 0 : n_array[k-1]; l < n_array[k]; l++) {
                n *= stock_random();
            }
            tmp[k] += n;
        }
    }
    for (int i = 0; i < 4; i++) {
        std::cout << "n = " << n_array[i] << " : " << (double)tmp[i]/MAX << std::endl;
    }*/

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
    for (int i = 0; i < 7; i++) {
        std::cout << "n = " << n_array2[i] << " : " <<
        (double)tmp2[i]/MAX2 << std::endl;
        prob_real[i] = (double)tmp2[i]/MAX2;
        err1[i] = abs(prob_real[i] - prob[i]);
    }
    Graph ques2("stock");
    ScatterErr<double> err(ques2, n_array3, prob, std::vector<double>(7, 0), err1);
    err.draw_scatterr("", "n", "P");

    delete grand;
    return 0;
}