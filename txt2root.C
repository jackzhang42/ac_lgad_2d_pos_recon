#include <iostream>
#include <fstream>

using namespace std;

void txt2root() {
	TFile *f1 = new TFile("std_epoch_error.root", "RECREATE");
	TTree *t1 = new TTree("error hist", "epoch error");
	t1->ReadFile("std_epoch_error.txt", "x");
	t1->Write();
	TCanvas *c1 = new TCanvas("c1", "c1");
	TH1F *h1 = new TH1F("h1", "h1", 10, 0, 20000);
	t1->Draw("x>>h1");
	double error_sigma = h1->GetStdDev();
	double error_mean = h1->GetMean();

	ofstream myfile1;
	myfile1.open("error.txt", ios::app);
	myfile1 << error_mean << "  " << error_sigma << "\n";

	TFile *f2 = new TFile("std_epoch_valid_error.root", "RECREATE");
	TTree *t2 = new TTree("valid error hist", "epoch valid error");
	t2->ReadFile("std_epoch_valid_error.txt", "x");
	t2->Write();
	TH1F *h2 = new TH1F("h2", "h2", 10, 0, 20000);
	t2->Draw("x>>h2");
	double valid_error_sigma = h2->GetStdDev();
	double valid_error_mean = h2->GetMean();

	ofstream myfile2;
	myfile2.open("valid_error.txt", ios::app);
	myfile2 << valid_error_mean << "  " << valid_error_sigma << "\n";
}
