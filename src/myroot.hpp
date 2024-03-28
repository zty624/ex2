#ifndef MYROOT_HPP
#define MYROOT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/rootheader.h"

template <class T = double>
struct Histogram {
  const T * data;
  int size;
  const char *filename;
  const char *xlabel;
  const char *ylabel;
  bool logy;
  bool output;
  int nbinsx;
  double xlow;
  double xup;

  TFile fnew;
  TCanvas *canvas;

  Histogram<T> (const T * data, int size, const char *filename,
    const char *xlabel, const char *ylabel, bool logy, bool output,
    int nbinsx, double xlow, double xup);
  ~Histogram<T> ();
  void save_png(TCanvas *&sharedcanvas);
  void output_data(const TH1F &);
  void draw_histogram(TCanvas *&sharedcanvas);
  void draw_poisson_fit(const TH1F &);
};

template <class T>
const std::vector<T> &read_file(const char *);

TRandom3 *grand = new TRandom3(0);

template <class T>
Histogram<T>::Histogram(const T * data, int size,
  const char *filename, const char *xlabel, const char *ylabel,
  bool logy, bool output, int nbinsx, double xlow, double xup) :
  data(data), size(size), filename(filename), xlabel(xlabel), ylabel(ylabel),
  logy(logy), output(output),
  nbinsx(nbinsx), xlow(xlow), xup(xup),
  fnew(filename, "recreate"),
  canvas(new TCanvas(filename, filename, 800, 600)) {
  canvas->SetGrid();
  if (logy) canvas->SetLogy();
};

template <class T>
Histogram<T>::~Histogram() {
  fnew.Write();
  fnew.Close();
  canvas->Close();
};

template <class T>
void Histogram<T>::save_png(TCanvas *&sharedcanvas) {
  std::string pngname = filename;
  pngname.append(".png");
  sharedcanvas->SaveAs(pngname.c_str());
};

template <class T>
void Histogram<T>::output_data(const TH1F &h1) {
  size_t size = h1.GetNbinsX();
  std::string dataname = h1.GetName();
  dataname.append(".txt");
  std::ofstream outfile(dataname, std::ios::out | std::ios::trunc);
  outfile << std::fixed << std::setprecision(0);
  for (size_t i = 1; i <= size + 1; i++) {
    outfile << h1.GetBinContent(i) << std::endl;
  }
  outfile.close();
  std::cout << "Data saved in " << dataname << std::endl;
};

// need to split the function !!!
template <class T>
void Histogram<T>::draw_histogram(TCanvas *&sharedcanvas) {
  std::cout << "Drawing histogram..." << std::endl;
  TH1F h1(filename, "", nbinsx, xlow, xup);
  h1.SetFillColorAlpha(kBlue, 0.4);
  h1.SetLineColor(kBlue+1); 
  h1.SetLineWidth(2); 
  h1.GetXaxis()->SetTitle(xlabel);
  h1.GetXaxis()->SetTitleColor(kBlack);
  h1.GetYaxis()->SetTitle(ylabel);
  h1.GetYaxis()->SetTitleColor(kBlack);
  h1.SetStats(kTRUE);
  for (int i = 0; i < size; i++) {
    h1.Fill(data[i]);
  }
  sharedcanvas->cd();
  h1.Draw();

  if (output) output_data(h1);
  save_png(sharedcanvas);
  canvas->cd();
};

template <class T>
void Histogram<T>::draw_poisson_fit(const TH1F &h1) {
  double mean = h1.GetMean();
  static TF1 *poissonFunc = new TF1("poisson", "[0]*TMath::Poisson(x,[1])", xlow, xup);
  poissonFunc->SetLineWidth(2);
  poissonFunc->SetParameters(h1.GetEntries() * (xup - xlow) / nbinsx, mean); 
  poissonFunc->SetLineColor(kRed);
  poissonFunc->Draw("SAME");
  // canvas->Update();
};

// Draw poisson on histogram
void draw_histogram_with_poisson(const int *data, int size, const char *filename, const char *xlabel, const char *ylabel, bool logy, bool output, int nbinsx, double xlow, double xup) {
    TFile fnew(filename, "RECREATE");
    TCanvas *canvas = new TCanvas("canvas", "Histogram with Poisson Distribution", 800, 600);
    canvas->SetGrid();
    TH1F h1(filename, "", nbinsx, xlow, xup);
    if (logy) {
        canvas->SetLogy();
    }
    h1.SetFillColorAlpha(kBlue, 0.4);
    h1.SetLineColor(kBlue+1); 
    h1.SetLineWidth(2); 
    h1.GetXaxis()->SetTitle(xlabel);
    h1.GetXaxis()->SetTitleColor(kBlack);
    h1.GetYaxis()->SetTitle(ylabel);
    h1.GetYaxis()->SetTitleColor(kBlack);
    h1.SetStats(kTRUE);
    for (int i = 0; i < size; i++) {
        h1.Fill(data[i]);
    }
    if (output) {
    }
    
    // Calculate the mean value for the Poisson distribution
    double mean = h1.GetMean();
    TF1 *poissonFunc = new TF1("poisson", "[0]*TMath::Poisson(x,[1])", xlow, xup);
    poissonFunc->SetLineWidth(2);
    poissonFunc->SetParameters(h1.GetEntries() * (xup - xlow) / nbinsx, mean); 
    poissonFunc->SetLineColor(kRed);
    h1.Draw();
    poissonFunc->Draw("SAME");

    std::string pngname = filename;
    pngname.append(".png");
    canvas->SaveAs(pngname.c_str());
    fnew.Write();
    fnew.Close();
    canvas->Close();
}

const std::vector<long long int> &read_file(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: cannot open file " << filename << std::endl;
    exit(1);
  }
  std::string line;
  static std::vector<long long int> array;
  while (std::getline(file, line, '\n')) {
    array.push_back(std::stoll(line));
  }
  return array;
};

#endif