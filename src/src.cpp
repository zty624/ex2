#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
TRandom3 *grand = new TRandom3(0);

// generate a data txt
void output_data(const TH1F &h1) {
  long long int size = h1.GetNbinsX();
  std::string dataname = h1.GetName();
  dataname.append(".txt");
  std::ofstream outfile(dataname, std::ios::out | std::ios::trunc);
  outfile << std::fixed << std::setprecision(0); // 设置精度，0 表示不显示小数部分
  for (long long int i = 1; i <= size + 1; i++) { // 包括最后的溢出bin
    outfile << h1.GetBinContent(i) << std::endl;
  }
  outfile.close();
  std::cout << "Data saved in " << dataname << std::endl;
}

void draw_histogram(const int *data, int size, const char *filename, const char *xlabel, const char *ylabel, bool logy, bool output, int nbinsx = 100, double xlow = 0, double xup = 10) {
  TFile fnew(filename, "recreate");
  TCanvas *canvas = new TCanvas("canvas", "Semilog Plot", 800, 600);
  canvas->SetGrid();
  if (logy) {
    canvas->SetLogy();
  }

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
  h1.Draw();
  fnew.Write();
  fnew.Close();
  if (output) {
    output_data(h1);
  }
  std::string pngname = filename;
  pngname.append(".png");
  canvas->SaveAs(pngname.c_str());
  canvas->Close();
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
      output_data(h1);
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

// ex2
const std::vector<long long int> &read_file(std::string filename) {
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