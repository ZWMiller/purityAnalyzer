void extractUsefulHists(const char* fileName){
  char name[1000];
  sprintf(name,"%s.root",fileName);
  TFile *f = new TFile(name,"READ");
  if (f->IsOpen()==kFALSE)
  { std::cout << "!!! File Not Found !!!" << std::endl;
    exit(1); }
  if (f->IsOpen())
  {
    cout << "Opened " << name << endl;
  }


  char fname[100];
  TFile* file;
  sprintf(fname,"%s_small.root",fileName);
  file = new TFile(fname,"RECREATE");
  if (file->IsOpen()==kFALSE)
  {
    std::cout << "!!! Outfile Not Opened !!!" << std::endl;
  }
  
  THnSparse* nSigmaESparse;
  THnSparse* nSigmaESparseHFT;
  const char* cutType[3] = {"BEMC","SMD","SMD2"};
  for(int q=0; q<3; q++)
  {
    for(int trig=0; trig<4; trig++)
    {
      nSigmaESparse = (THnSparse*)f->Get(Form("nSigmaE_%s_%i",cutType[q],trig));
      nSigmaESparseHFT = (THnSparse*)f->Get(Form("nSigmaE_%s_HFT_%i",cutType[q],trig));
      nSigmaESparse->Write(Form("nSigmaE_%s_%i",cutType[q],trig));
      nSigmaESparseHFT->Write(Form("nSigmaE_%s_HFT_%i",cutType[q],trig));
    }
  }
  cout << "Done." << endl;
}
