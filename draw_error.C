void draw_error() {
   auto c1 = new TCanvas("c1","Error vs Epoch",200,10,700,500);
//   c1->SetFillColor(42);
   c1->SetGrid();
   c1->GetFrame()->SetBorderSize(12);
	 auto g1_batch_10 = new TGraphErrors("./error_batch_10.txt", "%lg %lg %lg");
	 auto g2_batch_10 = new TGraphErrors("./valid_error_batch_10.txt", "%lg %lg %lg");
	 auto g1_batch_20 = new TGraphErrors("./error_batch_20.txt", "%lg %lg %lg");
	 auto g2_batch_20 = new TGraphErrors("./valid_error_batch_20.txt", "%lg %lg %lg");
	 auto g1_batch_40 = new TGraphErrors("./error_batch_40.txt", "%lg %lg %lg");
	 auto g2_batch_40 = new TGraphErrors("./valid_error_batch_40.txt", "%lg %lg %lg");
	 auto g1_batch_80 = new TGraphErrors("./error_batch_80.txt", "%lg %lg %lg");
	 auto g2_batch_80 = new TGraphErrors("./valid_error_batch_80.txt", "%lg %lg %lg");

	 g1_batch_10->SetTitle("Error vs. Epoch");
	 g1_batch_10->GetXaxis()->SetTitle("Epoch");
	 g1_batch_10->GetYaxis()->SetTitle("Position Reconstruction Error");
	 g1_batch_10->GetXaxis()->SetLimits(0.,120);
	 g1_batch_10->GetYaxis()->SetRangeUser(0., 220);
	 
	 g1_batch_10->SetMarkerColor(4);
   g1_batch_10->SetMarkerStyle(21);
	 g1_batch_10->SetMarkerSize(0.5);

	 g1_batch_20->SetMarkerColor(kGreen);
	 g1_batch_20->SetMarkerStyle(21);
	 g1_batch_20->SetMarkerSize(0.5);

	 g1_batch_40->SetMarkerColor(kBlack);
   g1_batch_40->SetMarkerStyle(21);
	 g1_batch_40->SetMarkerSize(0.5);

	 g1_batch_80->SetMarkerColor(kRed);
	 g1_batch_80->SetMarkerStyle(21);
	 g1_batch_80->SetMarkerSize(0.5);


	 g1_batch_10->Draw("ALP");
	 g2_batch_10->Draw("LP*");
	 g1_batch_20->Draw("LP");
	 g2_batch_20->Draw("LP*");
	 g1_batch_40->Draw("LP");
	 g2_batch_40->Draw("LP*");
	 g1_batch_80->Draw("LP");
	 g2_batch_80->Draw("LP*");
	 auto leg1 = new TLegend(.7,.4,.9,.9);
	 leg1->SetFillColor(0);
	 leg1->AddEntry(g1_batch_10, "test error for batch 10");
	 leg1->AddEntry(g2_batch_10, "train error for batch 10");
	 leg1->AddEntry(g1_batch_20, "test error for batch 20");
	 leg1->AddEntry(g2_batch_20, "train error for batch 20");
	 leg1->AddEntry(g1_batch_40, "test error for batch 40");
	 leg1->AddEntry(g2_batch_40, "train error for batch 40");
	 leg1->AddEntry(g1_batch_80, "test error for batch 80");
	 leg1->AddEntry(g2_batch_80, "train error for batch 80");
	 leg1->Draw();
	 g1_batch_10->Print();
//	 g2_batch_10->Print();
	 g1_batch_20->Print();
//	 g2_batch_20->Print();
	 g1_batch_40->Print();
//	 g2_batch_40->Print();
	 g1_batch_80->Print();
//	 g2_batch_80->Print();
	 c1->Print("error_vs_epoch_batch.eps");

	 auto c2 = new TCanvas("c2", "error vs batchsize");
   c2->SetGrid();
   c2->GetFrame()->SetBorderSize(12);
	 auto g_test = new TGraphErrors("./error_epoch_100_test.txt", "%lg %lg %lg");
	 auto g_train = new TGraphErrors("./error_epoch_100_train.txt", "%lg %lg %lg");
	 g_test->SetTitle("Error vs. Batchsize");
	 g_test->GetXaxis()->SetTitle("Batchsize");
	 g_test->GetYaxis()->SetTitle("Position Reconstruction Error");
	 g_test->GetXaxis()->SetLimits(0.,100);
	 g_test->GetYaxis()->SetRangeUser(0., 25);
	 
	 g_test->SetMarkerColor(4);
   g_test->SetMarkerStyle(21);
	 g_test->SetMarkerSize(0.5);

	 g_train->SetMarkerColor(kGreen);
	 g_train->SetMarkerStyle(21);
	 g_train->SetMarkerSize(0.5);

	 g_test->Draw("ALP");
	 g_train->Draw("LP*");

	 auto leg2 = new TLegend(.7,.8,.9,.9);
	 leg2->SetFillColor(0);
	 leg2->AddEntry(g_test, "test error for epoch 100");
	 leg2->AddEntry(g_train, "train error for epoch 100");
	 leg2->Draw();
	
	 c2->Print("error_epoch_100_vs_batch.eps");
}

