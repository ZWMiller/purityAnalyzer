
#include "anaConst14.h"

void setRangeAndDraw(TGraphErrors*, int,TCanvas*);
void findRejectPoints(int, int, int);
double myFunc(double*, double*);
double myFuncPol(double*, double*);
double myFuncRatio(double*, double*);
double myFuncScaled(double*, double*);
int numFitPars = 0;
float rejectPointHigh[10], rejectPointLow[10];
int rejectPointLength;
void setNumFitPars();

int pl_FitParams(){
  // gStyle->SetOptFit(1111);
  // Set Flags for Code/Plots
  Bool_t DEBUG = !kTRUE;
  int isLogY = 1;

  // Access outside required info
  const Int_t numPtBins = anaConst::nPtBins;
  Float_t lowpt[numPtBins],highpt[numPtBins];
  for(Int_t c=0; c< numPtBins; c++){
    lowpt[c] = anaConst::lpt[c];
    highpt[c] = anaConst::hpt[c];
  }
  Float_t hptCut=anaConst::hptCut;
  const Int_t numTrigs = 4;
  Float_t hptMax=anaConst::hptMax; // Set max above range to allow overflow
  Float_t lowPhi=anaConst::lowPhi, highPhi=anaConst::highPhi;
  const Int_t numEtaBins = anaConst::nEtaBins;
  Float_t loweta[numEtaBins],higheta[numEtaBins];
  for(Int_t c=0; c< numEtaBins; c++){
    loweta[c] = anaConst::etaLow[c];
    higheta[c] = anaConst::etaHigh[c];
  }
  const Int_t numCentBins = anaConst::nCentBins;
  Float_t lowcent[numCentBins],highcent[numCentBins];
  for(Int_t c=0; c< numCentBins; c++){
    lowcent[c] = anaConst::centLow[c];
    highcent[c] = anaConst::centHigh[c];
  }
  const int numparams = 12;

  // create generic structures for use in all plots
  TPaveText* lbl[numCentBins][numEtaBins][numparams];
  TPaveText* lblE[numCentBins][numEtaBins];
  TPaveText* runInfo[numCentBins][numEtaBins];
  TPaveText* runInfo2[numCentBins][numEtaBins];
  char textLabel[100]; 
  TString parNames[15] = {" #pi C"," #pi #mu"," #pi #sigma"," K C"," K #mu"," K #sigma", " p C"," p #mu"," p #sigma",
    " e C"," e #mu"," e #sigma"," mer. #pi C"," mer. #pi #mu"," mer. #pi #sigma"};

  // Load all trigger type files
  const char* baseName = "outputs/purityHists_pA15";
  char BEMCName[4][100],SMDLName[4][100],SMDTName[4][100],BEMCHFTName[4][100],SMDLHFTName[4][100],SMDTHFTName[4][100];
  char trigName[4][100] = {"MB","BHT1","BHT2","BHT3"};
  TFile* BEMC[4];
  TFile* SMDL[4];
  TFile* SMDT[4];
  TFile* BEMCh[4];
  TFile* SMDLh[4];
  TFile* SMDTh[4];
  for(int q=0; q<3; q++)
  {
    //without HFT
    sprintf(BEMCName[q],"%s_Eta_%s_BEMC_processed.root",baseName,trigName[q]);
    BEMC[q] = new TFile(BEMCName[q],"READ");
    sprintf(SMDLName[q],"%s_Eta_%s_SMD_processed.root",baseName,trigName[q]);
    SMDL[q] = new TFile(SMDLName[q],"READ");
    sprintf(SMDTName[q],"%s_Eta_%s_SMD2_processed.root",baseName,trigName[q]);
    SMDT[q] = new TFile(SMDTName[q],"READ");
  }

  for(int i=0; i<3; i++)
  {
    if(BEMC[i]->IsOpen() && SMDL[i]->IsOpen() && SMDT[i]->IsOpen())
      cout << "Trig " << i << " Files Open." << endl;
    else
    {
      cout << "Files Not Opened Properly." << endl;
      exit(EXIT_FAILURE);
    }
  }

  // Access histograms
  Int_t nparTotal[3][numCentBins][numEtaBins][numparams];
  TGraphErrors* parTotal[3][numCentBins][numEtaBins][numparams];
  TF1* fitTotal[3][numCentBins][numEtaBins][numparams];
  TF1* fitTotalRatio[3][numCentBins][numEtaBins][numparams];

  TGraphErrors* parBEMC[4][numCentBins][numEtaBins][numparams];
  TGraphErrors* parSMDL[4][numCentBins][numEtaBins][numparams];
  TGraphErrors* parSMDT[4][numCentBins][numEtaBins][numparams];
  TGraphErrors* parBEMCRatio[4][numCentBins][numEtaBins][numparams];
  TGraphErrors* parSMDLRatio[4][numCentBins][numEtaBins][numparams];
  TGraphErrors* parSMDTRatio[4][numCentBins][numEtaBins][numparams];
  TF1* fitBEMC[4][numCentBins][numEtaBins][numparams];
  TF1* fitSMDL[4][numCentBins][numEtaBins][numparams];
  TF1* fitSMDT[4][numCentBins][numEtaBins][numparams];

  float fitRangeLow[4] = {3,2,3.5,4.5};
  float fitRangeHigh[4] = {7,18,18,18};

  setNumFitPars();
  const int nFitPars = numFitPars;
 float parStart[numparams][4] = {
    {0,0,0,0},
    {1.0,4.0,30.769,1.0},
    {-1.0,-8.02,17.31,12.04},
    {0,0,0,0},
    {43.0, 17.77,3.25,38.29},
    {43.0, 17.77,3.25,38.29},
    {0,0,0,0},
    {43.0, 17.77,3.25,38.29},
    {43.0, 17.77,3.25,38.29},
    {0,0,0,0},
    {0.065,4.74,-0.26,0.435},
    {0.058,4.02,1.31,-1.04},
  };

  for(int centbin=0; centbin<numCentBins; centbin++)
  {
    for(int etabin=0; etabin<numEtaBins; etabin++)
    {
      for(int parnum=0; parnum<numparams; parnum++)
      {
        for(int ier=0;ier<3;ier++) {
          parTotal[ier][centbin][etabin][parnum] = new TGraphErrors();
          nparTotal[ier][centbin][etabin][parnum] = 0;
          fitTotal[ier][centbin][etabin][parnum] = new TF1(Form("Fit_%i_%i_%i_%i",ier,centbin,etabin,parnum),myFunc,anaConst::lowPt,anaConst::highPt,numFitPars);
          if(ier!=1) fitTotal[ier][centbin][etabin][parnum]->SetLineStyle(2);
          fitTotalRatio[ier][centbin][etabin][parnum] = new TF1(Form("FitRatio_%i_%i_%i_%i",ier,centbin,etabin,parnum),myFuncScaled,anaConst::lowPt,anaConst::highPt,5);
          if(ier!=1) fitTotalRatio[ier][centbin][etabin][parnum]->SetLineStyle(2);

          for(int ll=0;ll<3;ll++)
          {
            fitTotal[ier][centbin][etabin][parnum]->SetParameter(ll,parStart[parnum][ll]);
          }
        }
      }
    }
  }
  Double_t x,y,xerr,yerr;
  for(int w=0; w<3; w++)
  {
    if(DEBUG) cout << "w: " << w << endl;
    for(int centbin=0; centbin<numCentBins; centbin++)
    {
      for(int etabin=0; etabin<numEtaBins; etabin++)
      {
        for(int parnum=0; parnum<numparams; parnum++)
        {
          parBEMC[w][centbin][etabin][parnum] = (TGraphErrors*)BEMC[w]->Get(Form("fitpar_%i_%i_%i",centbin,etabin,parnum));
          parSMDL[w][centbin][etabin][parnum] = (TGraphErrors*)SMDL[w]->Get(Form("fitpar_%i_%i_%i",centbin,etabin,parnum));
          parSMDT[w][centbin][etabin][parnum] = (TGraphErrors*)SMDT[w]->Get(Form("fitpar_%i_%i_%i",centbin,etabin,parnum));

          for(int np=0;np<parBEMC[w][centbin][etabin][parnum]->GetN();np++) {
            parBEMC[w][centbin][etabin][parnum]->GetPoint(np,x,y);
            xerr=parBEMC[w][centbin][etabin][parnum]->GetErrorX(np);
            yerr=parBEMC[w][centbin][etabin][parnum]->GetErrorY(np);
            if(fabs(x)>1e-6&&fabs(y)>1e-6&&(w==1||w==2)) {
              for(int ier=0;ier<3;ier++) {
                parTotal[ier][centbin][etabin][parnum]->SetPoint(nparTotal[ier][centbin][etabin][parnum],x,y+(ier-1)*yerr);
                parTotal[ier][centbin][etabin][parnum]->SetPointError(nparTotal[ier][centbin][etabin][parnum],xerr,yerr);
                nparTotal[ier][centbin][etabin][parnum]++;
              }
            }
          }
          for(int np=0;np<parSMDL[w][centbin][etabin][parnum]->GetN();np++) {
            parSMDL[w][centbin][etabin][parnum]->GetPoint(np,x,y);
            xerr=parSMDL[w][centbin][etabin][parnum]->GetErrorX(np);
            yerr=parSMDL[w][centbin][etabin][parnum]->GetErrorY(np);
            if(fabs(x)>1e-6&&fabs(y)>1e-6&&(w==1||w==2)) {
             /* for(int ier=0;ier<3;ier++) {
                parTotal[ier][centbin][etabin][parnum]->SetPoint(nparTotal[ier][centbin][etabin][parnum],x,y+(ier-1)*yerr);
                parTotal[ier][centbin][etabin][parnum]->SetPointError(nparTotal[ier][centbin][etabin][parnum],xerr,yerr);
                nparTotal[ier][centbin][etabin][parnum]++;
              }*/
            }
          }
          for(int np=0;np<parSMDT[w][centbin][etabin][parnum]->GetN();np++) {
            parSMDT[w][centbin][etabin][parnum]->GetPoint(np,x,y);
            xerr=parSMDT[w][centbin][etabin][parnum]->GetErrorX(np);
            yerr=parSMDT[w][centbin][etabin][parnum]->GetErrorY(np);
            if(fabs(x)>1e-6&&fabs(y)>1e-6&&(w==1||w==2)) {
              /*for(int ier=0;ier<3;ier++) {
                parTotal[ier][centbin][etabin][parnum]->SetPoint(nparTotal[ier][centbin][etabin][parnum],x,y+(ier-1)*yerr);
                parTotal[ier][centbin][etabin][parnum]->SetPointError(nparTotal[ier][centbin][etabin][parnum],xerr,yerr);
                nparTotal[ier][centbin][etabin][parnum]++;
              }*/
            }
          }

          fitBEMC[w][centbin][etabin][parnum] = new TF1(Form("BEMCFit_%i_%i_%i_%i",w,centbin,etabin,parnum),myFunc,fitRangeLow[w],fitRangeHigh[w],numFitPars);
          fitSMDL[w][centbin][etabin][parnum] = new TF1(Form("SMDLFit_%i_%i_%i_%i",w,centbin,etabin,parnum),myFunc,fitRangeLow[w],fitRangeHigh[w],numFitPars);
          fitSMDT[w][centbin][etabin][parnum] = new TF1(Form("SMDTFit_%i_%i_%i_%i",w,centbin,etabin,parnum),myFunc,fitRangeLow[w],fitRangeHigh[w],numFitPars);

          for(int ll=0;ll<3;ll++)
          {
            fitBEMC[w][centbin][etabin][parnum]->SetParameter(ll,parStart[parnum][ll]);
            fitSMDL[w][centbin][etabin][parnum]->SetParameter(ll,parStart[parnum][ll]);
            fitSMDT[w][centbin][etabin][parnum]->SetParameter(ll,parStart[parnum][ll]);
          }
          parBEMC[w][centbin][etabin][parnum]->SetMarkerSize(1.5);
          parSMDL[w][centbin][etabin][parnum]->SetMarkerSize(1.5);
          parSMDT[w][centbin][etabin][parnum]->SetMarkerSize(1.5);
        }
      }
    }
  }

  for(int centbin=0; centbin<numCentBins; centbin++)
  {
    for(int etabin=0; etabin<numEtaBins; etabin++)
    {
      for(int parnum=0; parnum<numparams; parnum++)
      {
        for(int ier=0;ier<3;ier++) {
          if(parnum%3!=0) {
            //parBEMC[ier][centbin][etabin][parnum]->Fit(fitBEMC[ier][centbin][etabin][parnum],"R0");
            //fitTotal[ier][centbin][etabin][parnum]->SetParameter(0,fitBEMC[ier][centbin][etabin][parnum]->GetParameter(0));
            //fitTotal[ier][centbin][etabin][parnum]->SetParameter(2,fitBEMC[ier][centbin][etabin][parnum]->GetParameter(2));

            if(DEBUG)cout << "+++Fit total Here+++" << endl;
            parTotal[1][centbin][etabin][parnum]->Fit(fitTotal[ier][centbin][etabin][parnum],"R0");
            fitTotal[ier][centbin][etabin][parnum]->SetParameter(0,fitTotal[ier][centbin][etabin][parnum]->GetParameter(0)*(0.92+0.08*ier));
            fitTotal[ier][centbin][etabin][parnum]->SetParameter(2,fitTotal[ier][centbin][etabin][parnum]->GetParameter(2)*(0.92+0.08*ier));
          }
        }
      }
    }
    for(int etabin=0; etabin<numEtaBins; etabin++)
    {
      for(int parnum=0; parnum<numparams; parnum++)
      {
        for(int ier=0;ier<3;ier++) {
          double pars[5];
          fitTotal[1][centbin][etabin][parnum]->GetParameters(pars);
          pars[4]=0.92+0.08*ier;
          fitTotalRatio[ier][centbin][etabin][parnum]->SetParameters(pars);
        }
      }
    }
  }

  if(DEBUG)
    cout << "Found Hists." << endl;

  // Make Canvas
  TCanvas* paramCanvTotal[numCentBins][numEtaBins];
  for(int centbin=0; centbin<numCentBins; centbin++)
  {
    for(int etabin=0;etabin<numEtaBins;etabin++)
    {
      if(DEBUG) cout << "plot settings eta: " << etabin << endl;
      paramCanvTotal[centbin][etabin]= new TCanvas(Form("paramCanvTotal_%i_%i",centbin,etabin),"Parameter Fits",50,50,1050,1050);
      paramCanvTotal[centbin][etabin]->Divide(3,4);
    }
  }
  TCanvas* paramCanvRatio[numCentBins][numEtaBins];
  for(int centbin=0; centbin<numCentBins; centbin++)
  {
    for(int etabin=0;etabin<numEtaBins;etabin++)
    {
      if(DEBUG) cout << "plot settings eta: " << etabin << endl;
      paramCanvRatio[centbin][etabin]= new TCanvas(Form("paramCanvRatio_%i_%i",centbin,etabin),"Parameter Fits",50,50,1050,1050);
      paramCanvRatio[centbin][etabin]->Divide(3,4);
    }
  }

  TCanvas* paramCanv[3][4][numCentBins][numEtaBins];
  if(DEBUG)
    cout << "Canvas Made." << endl;

  // Plot Settings
  for(int r=0; r<3; r++)
  {
    if(DEBUG) cout << "plot settings trig: " << r << endl;

    for(int centbin=0; centbin<numCentBins; centbin++)
    {
      for(int etabin=0;etabin<numEtaBins;etabin++)
      {
        if(DEBUG) cout << "plot settings eta: " << etabin << endl;
        for(int j=0; j<3; j++)
        {
          paramCanv[j][r][centbin][etabin]= new TCanvas(Form("paramCanv_%i_%i_%i_%i",j,r,centbin,etabin),"Parameter Fits",50,50,1050,1050);
          paramCanv[j][r][centbin][etabin]->Divide(3,4);
        }
      }
    }
  }

  if(DEBUG)
    cout << "Settings Assigned." << endl;
  // Regular constraints
  // float lowLimit[12] = {0.,-6.,0.8,0.,-5.,0.8,0.,-0.5,0.8,0.,2.,0.8};
  // float highLimit[12] = {100000,-4.,1.2,100000,-2.,1.2,10000,-0.12,1.2,100000,4.,1.5};
  // "No" constraints (just force electron mean to be right of pi and Kp
  //float lowLimit[12] = {-100000.,-6.,0.,-100000., -5.,0.0,-100000,-2,0.0,-100000,2.,0.0};
  //float highLimit[12] = {100000.,-4.,100.,100000.,-2.,100,100000,  2,100,100000, 4.,100};
  for(int t=0; t<3; t++)
  {
    if(DEBUG) cout << "Trig: " << t << endl;
    for(int centbin=0; centbin<numCentBins; centbin++)
    {
      for(int etabin=0;etabin<numEtaBins;etabin++)
      {
        for(int parnum=0; parnum<numparams; parnum++)
        {
          TLine* lineLow = new TLine(2.5,anaConst::lowLimit[parnum],14,anaConst::lowLimit[parnum]);
          lineLow->SetLineColor(kBlue);
          TLine* lineHigh = new TLine(2.5,anaConst::highLimit[parnum],14,anaConst::highLimit[parnum]);
          lineHigh->SetLineColor(kBlue);

          TString title = trigName[t] + parNames[parnum];
          if(DEBUG) cout << "BEMC Par Num: " << parnum << endl;
          parBEMC[t][centbin][etabin][parnum]->SetTitle("BEMC "+title);
          parBEMC[t][centbin][etabin][parnum]->SetMarkerStyle(20+t);
          parBEMC[t][centbin][etabin][parnum]->SetMarkerColor(2);
          parBEMC[t][centbin][etabin][parnum]->SetLineColor(2);
          setRangeAndDraw(parBEMC[t][centbin][etabin][parnum],parnum, paramCanv[0][t][centbin][etabin]);
          //findRejectPoints(t,1,parnum);
          fitBEMC[t][centbin][etabin][parnum]->SetLineColor(2);
          cout << "Fit: BEMC, Trig: " << t << ", Par: " << parnum << endl; 
          if(!((parnum%3) == 0))
            parBEMC[t][centbin][etabin][parnum]->Fit(fitBEMC[t][centbin][etabin][parnum],"R0");
          parBEMCRatio[t][centbin][etabin][parnum]=(TGraphErrors*)parBEMC[t][centbin][etabin][parnum]->Clone();
          Double_t x,y;

          for(int np=0;np<parBEMCRatio[t][centbin][etabin][parnum]->GetN();np++){
            parBEMCRatio[t][centbin][etabin][parnum]->GetPoint(np,x,y);
            parBEMCRatio[t][centbin][etabin][parnum]->SetPoint(np,x,y/fitTotal[1][centbin][etabin][parnum]->Eval(x));
            parBEMCRatio[t][centbin][etabin][parnum]->SetPointError(np,parBEMCRatio[t][centbin][etabin][parnum]->GetErrorX(np),parBEMCRatio[t][centbin][etabin][parnum]->GetErrorY(np)/fitTotal[1][centbin][etabin][parnum]->Eval(x));
          }
          parBEMCRatio[t][centbin][etabin][parnum]->GetXaxis()->SetRangeUser(anaConst::lowPt,anaConst::highPt);
          parBEMCRatio[t][centbin][etabin][parnum]->GetYaxis()->SetRangeUser(0.8,1.2);
          lineLow->Draw("same");
          lineHigh->Draw("same");
          if(DEBUG)cout<<"xxx"<<endl;
          if(DEBUG) cout << "SMDL Par Num: " << parnum << endl;
          parSMDL[t][centbin][etabin][parnum]->SetTitle("SMD "+title);
          parSMDL[t][centbin][etabin][parnum]->SetMarkerStyle(20+t);
          parSMDL[t][centbin][etabin][parnum]->SetMarkerColor(3);
          parSMDL[t][centbin][etabin][parnum]->SetLineColor(3);
          if(DEBUG)cout<<"xxx"<<endl;
          setRangeAndDraw(parSMDL[t][centbin][etabin][parnum],parnum,paramCanv[1][t][centbin][etabin]);
          if(DEBUG)cout<<"xxx"<<endl;
          //findRejectPoints(t,2,parnum);
          fitSMDL[t][centbin][etabin][parnum]->SetLineColor(3);
          cout << "Fit: SMDL, Trig: " << t << ", Par: " << parnum << endl; 
          if(!((parnum%3) == 0))
            parSMDL[t][centbin][etabin][parnum]->Fit(fitSMDL[t][centbin][etabin][parnum],"R0");
          parSMDLRatio[t][centbin][etabin][parnum]=(TGraphErrors*)parSMDL[t][centbin][etabin][parnum]->Clone();
          for(int np=0;np<parBEMCRatio[t][centbin][etabin][parnum]->GetN();np++){
            parSMDLRatio[t][centbin][etabin][parnum]->GetPoint(np,x,y);
            parSMDLRatio[t][centbin][etabin][parnum]->SetPoint(np,x,y/fitTotal[1][centbin][etabin][parnum]->Eval(x));
            parSMDLRatio[t][centbin][etabin][parnum]->SetPointError(np,parSMDLRatio[t][centbin][etabin][parnum]->GetErrorX(np),parSMDLRatio[t][centbin][etabin][parnum]->GetErrorY(np)/fitTotal[1][centbin][etabin][parnum]->Eval(x));
          }
          lineLow->Draw("same");
          lineHigh->Draw("same");
          if(DEBUG)cout<<"xxx"<<endl;
          if(DEBUG) cout << "SMDT Par Num: " << parnum << endl;
          parSMDT[t][centbin][etabin][parnum]->SetTitle("SMD2 "+title);
          parSMDT[t][centbin][etabin][parnum]->SetMarkerStyle(20+t);
          parSMDT[t][centbin][etabin][parnum]->SetMarkerColor(4);
          parSMDT[t][centbin][etabin][parnum]->SetLineColor(4);
          setRangeAndDraw(parSMDT[t][centbin][etabin][parnum],parnum,paramCanv[2][t][centbin][etabin]);
          //findRejectPoints(t,3,parnum);
          fitSMDT[t][centbin][etabin][parnum]->SetLineColor(4);
          cout << "Fit: SMDT, Trig: " << t << ", Par: " << parnum << endl; 
          if(!((parnum%3) == 0))
            parSMDT[t][centbin][etabin][parnum]->Fit(fitSMDT[t][centbin][etabin][parnum],"R0");
          parSMDTRatio[t][centbin][etabin][parnum]=(TGraphErrors*)parSMDT[t][centbin][etabin][parnum]->Clone();
          for(int np=0;np<parBEMCRatio[t][centbin][etabin][parnum]->GetN();np++){
            parSMDTRatio[t][centbin][etabin][parnum]->GetPoint(np,x,y);
            parSMDTRatio[t][centbin][etabin][parnum]->SetPoint(np,x,y/fitTotal[1][centbin][etabin][parnum]->Eval(x));
            parSMDTRatio[t][centbin][etabin][parnum]->SetPointError(np,parSMDTRatio[t][centbin][etabin][parnum]->GetErrorX(np),parSMDTRatio[t][centbin][etabin][parnum]->GetErrorY(np)/fitTotal[1][centbin][etabin][parnum]->Eval(x));
          }
          lineLow->Draw("same");
          lineHigh->Draw("same");

          if(t==1) {
            //parBEMC[t][centbin][etabin][parnum]->Draw("pae");
            setRangeAndDraw(parTotal[1][centbin][etabin][parnum],parnum,paramCanvTotal[centbin][etabin]);
          }
          if(t<3) {
            paramCanvTotal[centbin][etabin]->cd(parnum+1);
            parBEMC[t][centbin][etabin][parnum]->Draw("pesame");
            //parSMDL[t][centbin][etabin][parnum]->Draw("pesame");
            //parSMDT[t][centbin][etabin][parnum]->Draw("pesame");
          }
          if(t==2&&parnum%3!=0) {
            paramCanvTotal[centbin][etabin]->cd(parnum+1);
            for(int ier=0;ier<3;ier++) fitTotal[ier][centbin][etabin][parnum]->Draw("same");
          }
          lineLow->Draw("same");
          lineHigh->Draw("same");

          /*else parBEMC[t][centbin][etabin][parnum]->Draw("pesame");
            parSMDL[t][centbin][etabin][parnum]->Draw("pesame");
            parSMDT[t][centbin][etabin][parnum]->Draw("pesame");*/

          paramCanvRatio[centbin][etabin]->cd(parnum+1);
          if(parnum%3!=0) {
            if(t==0) {
              parBEMCRatio[t][centbin][etabin][parnum]->Draw("pae");
              if(parnum%3!=0) {
                for(int ier=0;ier<3;ier++) fitTotalRatio[ier][centbin][etabin][parnum]->Draw("same");
              }
            }
            if(t<3) {
              parBEMCRatio[t][centbin][etabin][parnum]->Draw("pesame");
              //parSMDLRatio[t][centbin][etabin][parnum]->Draw("pesame");
              //parSMDTRatio[t][centbin][etabin][parnum]->Draw("pesame");
            }
          }
        }
      }
    }
  }
  if(DEBUG) cout << "Drawing Finished. " << endl;

  // Place canvases in order on PDF
  //Set front page
  TCanvas* fp = new TCanvas("fp","Front Page",100,0,1000,900);
  fp->cd();
  TBox *bLabel = new TBox(0.01, 0.88, 0.99, 0.99);
  bLabel->SetFillColor(38);
  bLabel->Draw();
  TLatex tl;
  tl.SetNDC();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.033);
  char tlName[100];
  char tlName2[100];

  TString titlename = baseName;
  int found = titlename.Last('/');
  if(found >= 0){
    titlename.Replace(0, found+1, "");
  } 
  sprintf(tlName, "Run 15 p+Au 200 GeV NPE Purity");
  tl.SetTextSize(0.05);
  tl.SetTextColor(kWhite);
  tl.DrawLatex(0.05, 0.92,tlName);

  TBox *bFoot = new TBox(0.01, 0.01, 0.99, 0.12);
  bFoot->SetFillColor(38);
  bFoot->Draw();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.05);
  tl.DrawLatex(0.05, 0.05, (new TDatime())->AsString());
  tl.SetTextColor(kBlack);
  tl.SetTextSize(0.03);
  tl.DrawLatex(0.1, 0.14, titlename);
  sprintf(tlName,"Study of Various Cut Sets on Inclusive Electron Purity.");
  tl.DrawLatex(0.1, 0.75,tlName);
  sprintf(tlName,"Z.W. Miller - UIC");
  tl.DrawLatex(0.1, 0.67,tlName);


  TCanvas* temp = new TCanvas();
  char name[100];
  sprintf(name, "%s_paramFits_EtaBins.pdf[", baseName);
  temp->Print(name);
  sprintf(name, "%s_paramFits_EtaBins.pdf", baseName);
  temp = fp; // print front page
  temp->Print(name);

  for(int j=0; j<3; j++)
  {
    for(int q=0; q<3; q++)
    {
      if(DEBUG) cout << "Place Canvas in PDF trig: " << q << endl;
      for(int c=0; c<numCentBins; c++)
      {  
        for(int e=0; e<numEtaBins; e++)
        {  
          if(DEBUG) cout << "Place Canvas in PDF eta: " << e << endl;
          temp = paramCanv[j][q][c][e]; // print data canvases
          temp->Print(name);
        }
      }
    }
  }
  for(int c=0; c<numCentBins; c++)
  {  
    for(int e=0; e<numEtaBins; e++)
    {
      if(DEBUG) cout << "Place Canvas in PDF eta: " << e << endl;
      temp = paramCanvTotal[c][e]; // print data canvases
      temp->Print(name);
    }
    for(int e=0; e<numEtaBins; e++)
    {
      if(DEBUG) cout << "Place Canvas in PDF eta: " << e << endl;
      temp = paramCanvRatio[c][e]; // print data canvases
      temp->Print(name);
    }
  }

  sprintf(name, "%s_paramFits_EtaBins.pdf]", baseName);
  temp->Print(name);

  TFile *outputfile=new TFile("fitpar_output.root","recreate");
  outputfile->cd();
  for(int ier=0; ier<3; ier++) {
    for(int centbin=0;centbin<numCentBins;centbin++) {
      for(int etabin=0;etabin<numEtaBins;etabin++) {
        for(int parnum=0; parnum<numparams; parnum++) {
          if(parnum%3!=0) fitTotal[ier][centbin][etabin][parnum]->Write();
        }
      }
    }
  }
  outputfile->Close();
  return 1;
}


void setRangeAndDraw(TGraphErrors* gr, int parnum,TCanvas* can)
{
  can->cd(parnum+1);
  gr->GetXaxis()->SetRangeUser(anaConst::lowPt,anaConst::highPt);
  gr->Draw();
  switch (parnum)
  {
    case 1:
      gr->GetYaxis()->SetRangeUser(-10,0);
      break;
    case 2:
      gr->GetYaxis()->SetRangeUser(0.7,1.5);
      break;
    case 4:
      gr->GetYaxis()->SetRangeUser(-10,0);
      break;
    case 5:
      gr->GetYaxis()->SetRangeUser(0.7,1.5);
      break;
    case 7:
      gr->GetYaxis()->SetRangeUser(-10,0);
      break;
    case 8:
      gr->GetYaxis()->SetRangeUser(0.7,1.5);
      break;
    case 10:
      gr->GetYaxis()->SetRangeUser(-5,5);
      break;
    case 11:
      gr->GetYaxis()->SetRangeUser(0.7,1.5);
      break;
    case 13:
      gr->GetYaxis()->SetRangeUser(-5,7);
      break;
    case 14:
      gr->GetYaxis()->SetRangeUser(0.7,1.5);
      break;
    default:
      gr->GetYaxis()->SetRangeUser(-10,10);
      break;

  }
  gr->Draw("ape");
  if(parnum%3==0) gPad->SetLogy();
}

double myFunc(double* x, double* par)
{
  /*for(int ii=0; ii<rejectPointLength; ii++)
  {
    if(x[0] > rejectPointLow[ii] && x[0] < rejectPointHigh[ii])
    {
      TF1::RejectPoint();
      return 0;
    }
  }*/
  double ff = par[0]*TMath::Erf((x[0]-par[1])/par[2])-par[3];
  //ff = par[0] + par[1]*x[0];// + par[2]*x[0]*x[0];
  //  double ff = par[0]*x[0]*x[0]+par[1]*x[0]+par[2];//TMath::Erf((x[0]-par[1])/par[2])-par[3];

  return ff;
}

double myFuncRatio(double* x, double* par)
{
  double ff1 = par[0]*TMath::Erf((x[0]-par[1])/par[2])-par[3];
  double ff2 = par[4]*TMath::Erf((x[0]-par[5])/par[6])-par[7];

  return ff1/ff2;
}

double myFuncScaled(double* x, double* par)
{
  //double ff1 = par[0]*TMath::Erf((x[0]-par[1])/par[2])-par[3];
  double ff2 = par[4];

  return ff2;
}


double myFuncPol(double* x, double* par)
{
  /*for(int ii=0; ii<rejectPointLength; ii++)
  {
    if(x[0] > rejectPointLow[ii] && x[0] < rejectPointHigh[ii])
    {
      TF1::RejectPoint();
      return 0;
    }
  }*/
  //double ff = par[0]*TMath::Erf((x[0]-par[1])/par[2])-par[3];
  ff = par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0];
  //  double ff = par[0]*x[0]*x[0]+par[1]*x[0]+par[2];//TMath::Erf((x[0]-par[1])/par[2])-par[3];

  return ff;
}


void setNumFitPars()
{
  numFitPars = 4;
}

void findRejectPoints(int tr, int cut, int par)
{
  // cut 1=BEMC,2=SMD,3=SMD2
  if(cut < 1 or cut > 3)
  { 
    cout << "Cuts not correct for Reject Points" << endl; 
    return;
  }

  if((par % 3) == 0)
    return;
  for(int ii = 0; ii < rejectPointLength; ii++)
  {
    rejectPointLow[ii] = rejectPointHigh[ii] = 0.;
  }
  rejectPointLength = 0;

  /// Reject point length tells how many regions are being rejected.
  /// Must be set to 1 if any regions are being rejected.
  if(cut == 1 && tr == 1)
  {   
    if(par == 1 || par == 7)
    {
      rejectPointLow[0]  = 10.;
      rejectPointHigh[0]  = 13.;
      rejectPointLength  = 1;
    }
    if(par == 8)
    {
      rejectPointLow[0]   = 0.;
      rejectPointHigh[0]  = 4.;
      rejectPointLength  = 1;
    }  
  }
  if(cut == 1 && tr == 2)
    if(par == 7)
    {
      rejectPointLow[0]   = 12.;
      rejectPointHigh[0]  = 20.;
      rejectPointLength = 1;
    }


  if(cut == 2 && tr == 1)
  {
    if(par == 1 || par == 7)
    {
      rejectPointLow[0]   = 10.;
      rejectPointHigh[0]  = 13.;
      rejectPointLow[1]   = 0.;
      rejectPointHigh[1]  = 2.5;
      rejectPointLength = 2;
    }
  }
  if(cut == 3 && tr == 1)
  {   
    if(par == 1 || par == 7)
    {
      rejectPointLow[0]   = 10.;
      rejectPointHigh[0]  = 13.;
      rejectPointLow[1]   = 0.;
      rejectPointHigh[1]  = 3.5;
      rejectPointLength = 2;
    }
    if(par == 2 || par == 5)
    {
      rejectPointLow[0]   = 10.;
      rejectPointHigh[0]  = 20.;
      rejectPointLow[1]   = 0.;
      rejectPointHigh[1]  = 2.5;
      rejectPointLength = 2;
    }
    if(par == 4)
    {
      rejectPointLow[0]   = 0.;
      rejectPointHigh[0]  = 2.5;
      rejectPointLength = 1;
    }
  }
  if(cut == 3 && tr == 2)
  {
    if(par == 5)
    {
      rejectPointLow[0]   = 6.;
      rejectPointHigh[0]  = 7.;
      rejectPointLength = 1;
    }
  }

  return;
}
