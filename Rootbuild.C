TFile *Rootbuild(Int_t getFile=0, Int_t print=1) {

   Double_t           ypred;
   Double_t           yreal;

   TString filename = "ypred_vs_yreal_test_logA.root";
   FILE *fp = fopen("ypred_vs_yreal_test_logA.txt","r");

   TFile *hfile = 0;

   if (gSystem->AccessPathName(".",kWritePermission)) {
      printf("you must run the script in a directory with write access\n");
      return 0;
   }
   hfile = TFile::Open(filename,"RECREATE");
   TTree *tree = new TTree("T","position data");
   tree->Branch("ypred",&ypred,"ypred/D");
   tree->Branch("yreal",&yreal,"yreal/D");
   char line[80];
   while (fgets(line,80,fp)) {
      sscanf(&line[0],"%lf	%lf",&ypred,&yreal);
      tree->Fill();
   }
   if (print) tree->Print();
   tree->Write();

   fclose(fp);
   delete hfile;
   if (getFile) {
      //we come here when the script is executed outside $ROOTSYS/tutorials/tree
      hfile = TFile::Open(filename);
      return hfile;
   }
   return 0;
}
