#ifndef MYROOT_HPP
#define MYROOT_HPP

#include <iostream>
#include <fstream>
#include "../include/rootheader.h"


class Graph {
protected:
  TFile *fnew;
  TCanvas *canvas;
  std::string filename;

public:
  explicit Graph(std::string filename, int ww = 800, int wh = 600):
  filename(filename) {
    fnew = new TFile((filename + ".root").c_str()
    , "recreate");
    canvas = new TCanvas(filename.c_str(), filename.c_str(), ww, wh);
    canvas->SetGrid();
  };

  Graph(const Graph &graph): fnew(graph.fnew), canvas(graph.canvas),
    filename(graph.filename) {
  }

  ~Graph(){
    if (fnew->IsOpen()) {
      fnew->Close();
      canvas->Close();
      std::cout << "Graph object \"" << filename << "\" has been deleted"
      << std::endl;
    }
  };

  virtual void output_data(std::string dataname = ""){};

  virtual void draw_canvas(std::string figname = "") {
    if (figname == "") {
      this->canvas->SaveAs((filename + ".png").c_str(), "png");
    } else {
      if (figname.rfind(".png") == std::string::npos) {
        this->canvas->SaveAs((filename + "_" + figname + ".png").c_str(), 
        "png");
      } else {
        this->canvas->SaveAs((filename + "_" + figname).c_str(),
        "png");
      }
    }
  }
};

template <class T>
class Histogram : public Graph {
private:
  std::vector<T> data;
  TH1D hist;

public:
  explicit Histogram<T>(const std::vector<T> &data, std::string filename = "histogram",
    int ww = 800, int wh = 600): Graph(filename, ww, wh), data(data) {
  };
  explicit Histogram<T>(const Graph &graph, const std::vector<T> &data):
    Graph(graph), data(data) {
  };
  ~Histogram<T>(){
  };

  void output_data(std::string dataname = "") override {
    if (dataname == "") {
      dataname = filename;
    } else if (dataname.rfind(".txt") == std::string::npos) {
      dataname += ".txt";
    }
    std::ofstream file(dataname);
    for (auto i = 0; i < data.size(); i++) {
      file << data[i] << std::endl;
    }
    file.close();
  };

  void draw_histogram(int nbins = 10,
    std::string figname = "histogram",
    std::string xtitle = "x", std::string ytitle = "y",
    double xlow = 0.0, double xup = 0.0,
    bool logy = false, bool logx = false) {
    if (data.size() == 0) {
      data = this->data;
    }
    if (xlow == 0.0 && xup == 0.0) {
      xlow = *std::min_element(data.begin(), data.end());
      xup = *std::max_element(data.begin(), data.end());
    }

    fnew->cd();
    canvas->cd();
    if (logx) {
      canvas->SetLogx();
    }
    if (logy) {
      canvas->SetLogy();
    }

    hist = *new TH1D(filename.c_str(), filename.c_str(), nbins, xlow, xup);
    hist.SetFillColorAlpha(kBlue, 0.4);
    hist.SetLineColor(kBlue+1); 
    hist.SetLineWidth(2); 
    hist.GetXaxis()->SetTitleColor(kBlack);
    hist.GetYaxis()->SetTitleColor(kBlack);
    hist.GetXaxis()->SetTitle(xtitle.c_str());
    hist.GetYaxis()->SetTitle(ytitle.c_str());
    hist.Draw();

    for (auto i = 0; i < data.size(); i++) {
      hist.Fill(data[i]);
    }
    
    hist.Write();
    draw_canvas(figname);
  }
};

template <class T>
class Plot : public Graph {
private:
  TGraph graph;
  std::vector<T> datax, datay;

public:
  explicit Plot<T>(const std::vector<T> &datax, std::vector<T> &datay, 
    std::string filename = "plot", int ww = 800, int wh = 600):
    Graph(filename, ww, wh), datax(datax), datay(datay){
    if (datax.size() != datay.size()) {
      std::cerr << "Error: datax and datay must have the same size" << std::endl;
      exit(1);
    }
  };
  explicit Plot<T>(const Graph &graph, const std::vector<T> &datax,
    const std::vector<T> &datay): 
    Graph(graph), datax(datax), datay(datay) {
    if (datax.size() != datay.size()) {
      std::cerr << "Error: datax and datay must have the same size" << std::endl;
      exit(1);
    }
  };
  ~Plot<T>() {
  };

  void output_data(std::string dataname = "") {
    if (dataname == "") {
      dataname = filename;
    } else if (dataname.rfind(".txt") == std::string::npos) {
      dataname += ".txt";
    }
    std::ofstream file(dataname);
    for (auto i = 0; i < datax.size(); i++) {
      file << datax[i] << " " << datay[i] << std::endl;
    }
    file.close();
  };

  void draw_plot(std::string figname = "plot", std::string xtitle = "x",
    std::string ytitle = "y", bool logy = false, bool logx = false) {
    fnew->cd();
    canvas->cd();
    if (logx) {
      canvas->SetLogx();
    }
    if (logy) {
      canvas->SetLogy();
    }

    graph = *new TGraph(datax.size(), &datax[0], &datay[0]);
    graph.SetLineColor(kBlue+1); 
    graph.SetLineWidth(2); 
    graph.GetXaxis()->SetTitleColor(kBlack);
    graph.GetYaxis()->SetTitleColor(kBlack);
    graph.GetXaxis()->SetTitle(xtitle.c_str());
    graph.GetYaxis()->SetTitle(ytitle.c_str());
    graph.Draw("AL");

    graph.Write();
    draw_canvas(figname);
  }
};

template <class T>
class Scatter : public Graph {
private:
  TGraph graph;
  std::vector<T> datax, datay;

public:
  explicit Scatter<T>(const std::vector<T> &datax, std::vector<T> &datay, 
    std::string filename = "scatter", int ww = 800, int wh = 600):
    Graph(filename, ww, wh), datax(datax), datay(datay){
    if (datax.size() != datay.size()) {
      std::cerr << "Error: datax and datay must have the same size" << std::endl;
      exit(1);
    }
  };
  explicit Scatter<T>(const Graph &graph, const std::vector<T> &datax,
    const std::vector<T> &datay): 
    Graph(graph), datax(datax), datay(datay) {
    if (datax.size() != datay.size()) {
      std::cerr << "Error: datax and datay must have the same size" << std::endl;
      exit(1);
    }
  };
  ~Scatter<T>(){
  };
  
  void output_data(std::string dataname = "") override {
    if (dataname == "") {
      dataname = filename;
    } else if (dataname.rfind(".txt") == std::string::npos) {
      dataname += ".txt";
    }
    std::ofstream file(dataname);
    for (auto i = 0; i < datax.size(); i++) {
      file << datax[i] << " " << datay[i] << std::endl;
    }
    file.close();
  }

  void draw_scatter(std::string figname = "scatter", std::string xtitle = "x", 
    std::string ytitle = "y", bool logy = false, bool logx = false) {
    fnew->cd();
    canvas->cd();
    if (logx) {
      canvas->SetLogx();
    }
    if (logy) {
      canvas->SetLogy();
    }
    
    graph = *new TGraph(datax.size(), &datax[0], &datay[0]);
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(1);
    graph.SetMarkerColor(kBlue+1);
    graph.SetLineWidth(0); 
    graph.GetXaxis()->SetTitleColor(kBlack);
    graph.GetYaxis()->SetTitleColor(kBlack);
    graph.GetXaxis()->SetTitle(xtitle.c_str());
    graph.GetYaxis()->SetTitle(ytitle.c_str());
    graph.Draw("AP");

    graph.Write();
    draw_canvas(figname);
  }

};

template <class T>
class ScatterErr : public Graph {
private:
  TGraphErrors graph;
  std::vector<T> datax, datay, errx, erry;

public:
  explicit ScatterErr<T>(const std::vector<T> &datax, std::vector<T> &datay, 
    std::vector<T> &errx, std::vector<T> &erry,
    std::string filename = "grapherr", int ww = 800, int wh = 600):
    Graph(filename, ww, wh), datax(datax), datay(datay),
    errx(errx), erry(erry) {
    if (datax.size() == datay.size() == erry.size() == errx.size()) {

    } else {
      std::cerr << "Error: datax, datay, errx, and erry must have the same size" << std::endl;
      exit(1);
    }
  };
  explicit ScatterErr<T>(const Graph &graph, const std::vector<T> &datax,
    const std::vector<T> &datay, const std::vector<T> &errx,
    const std::vector<T> &erry): 
    Graph(graph), datax(datax), datay(datay), errx(errx), erry(erry) {
    if (datax.size() == datay.size() && datax.size() == erry.size() 
    && datax.size() == errx.size()) {

    } else {
      std::cerr << "Error: datax, datay, errx, and erry must have the same size" << std::endl;
      exit(1);
    }
  };
  ~ScatterErr<T>(){
  };

  void output_data(std::string dataname = "") override {
    if (dataname == "") {
      dataname == filename;
    } else if (dataname.rfind(".txt") == std::string::npos) {
      dataname += ".txt";
    }

    std::ofstream file(dataname);
    for (auto i = 0; i < datax.size(); i++) {
      file << datax[i] << " " << datay[i] << " "
           << errx[i] << " " << erry[i] << std::endl;
    }
    file.close();
  }

  void draw_scatterr(std::string figname = "", std::string xtitle = "x",
    std::string ytitle = "y", bool logy = false, bool logx = false) {
    fnew->cd();
    canvas->cd();
    if (logx) {
      canvas->SetLogx();
    }
    if (logy) {
      canvas->SetLogy();
    }
    
    graph = *new TGraphErrors(datax.size(), &datax[0], &datay[0], &errx[0], &erry[0]);
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.5);
    graph.SetMarkerColor(kBlue+1);
    graph.GetXaxis()->SetTitleColor(kBlack);
    graph.GetYaxis()->SetTitleColor(kBlack);
    graph.GetXaxis()->SetTitle(xtitle.c_str());
    graph.GetYaxis()->SetTitle(ytitle.c_str());
    graph.Draw("AP");

    graph.Write();
    draw_canvas(figname);
  }


};


#endif