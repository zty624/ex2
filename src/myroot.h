#ifndef MYROOT_H
#define MYROOT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/rootheader.h"
#include "TCanvas.h"

template <class T>
struct Histogram {
  const T * data;
  int size;
  char *filename;
  char *xlabel;
  char *ylabel;
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
  void save_png() const;
  void output_data(const TH1F &);
  void draw_histogram();
  void draw_poisson_fit(const TH1F &);
};

template <class T>
const std::vector<T> &read_file(const char *);
#endif