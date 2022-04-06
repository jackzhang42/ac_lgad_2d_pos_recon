void viewComparison() {
	TFile *f = new TFile("ypred_vs_yreal_test_logA.root");
	TTree *t = (TTree *)f->Get("T");
	TH2F *h = new TH2F("h", "Comparison;yreal;ypred", 140, 300, 1000, 140, 300, 1000);
	TF1 *f1 = new TF1("f1", "x", 300., 1000.);
	double ypred;
	double yreal;
	t->SetBranchAddress("ypred", &ypred);
	t->SetBranchAddress("yreal", &yreal);
	Int_t n = (Int_t)t->GetEntries();
	for (Int_t i = 0; i < n; i++) {
		t->GetEntry(i);
		h->Fill(yreal, ypred);
	}
	TCanvas *c1 = new TCanvas("c1", "Comparison", 800, 600);
	c1->SetGrid();
	h->Draw("colzSame");
	f1->Draw("same");
}
