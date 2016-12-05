
#ifndef anaConst_H
#define anaConst_H

namespace anaConst
{

  //Binning
  /*const Int_t nPtBins = 9; 
    Float_t lpt[nPtBins] ={1.0,1.5,2.0,2.5,3.5,4.5,5.5,6.5,8.5};
    Float_t hpt[nPtBins] ={1.5,2.0,2.5,3.5,4.5,5.5,6.5,8.5,10.5};

    const Int_t nPtBins = 8; 
    Float_t lpt[nPtBins] ={1.5,2.5,3.5,4.5,5.5,6.5,7.5,9.5};
    Float_t hpt[nPtBins] ={2.5,3.5,4.5,5.5,6.5,7.5,9.5,13.5};


    const Int_t nPtBins = 10; 
    Float_t lpt[nPtBins] ={1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4};
    Float_t hpt[nPtBins] ={1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5};
    */
  /*const Int_t nPtBins = 40; 
    Float_t lpt[nPtBins] ={2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,
    3.5,3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,
    4.5,4.6,4.7,4.8,4.9,5.0,5.1,5.2,5.3,5.4,
    5.5,5.6,5.7,5.8,5.9,6.0,6.1,6.2,6.3,6.4,};
    Float_t hpt[nPtBins] ={2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,
    3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,4.5,
    4.6,4.7,4.8,4.9,5.0,5.1,5.2,5.3,5.4,5.5,
    5.6,5.7,5.8,5.9,6.0,6.1,6.2,6.3,6.4,6.5};

    const Int_t nPtBins = 26; 
    Float_t lpt[nPtBins] ={2.5,2.7,2.9,3.1,3.3,3.5,3.7,3.9,4.1,4.3,
    4.5,4.7,4.9,5.1,5.3,5.5,5.7,5.9,6.1,6.3,
    6.5,7.5,8.5 ,10.5,12.5,14.5 };
    Float_t hpt[nPtBins] ={2.7,2.9,3.1,3.3,3.5,3.7,3.9,4.1,4.3,4.5,
    4.7,4.9,5.1,5.3,5.5,5.7,5.9,6.1,6.3,6.5,
    7.5,8.5,10.5,12.5,14.5,20.5 };

  // Normal Sample
  const Int_t nPtBins = 20;
  Float_t lpt[nPtBins] ={1.50,1.60,1.70,1.80,1.90,2.00,
  2.10,2.20,2.30,2.40,2.60,2.80,3.00,3.50,4.00,
  5.00,6.00,7.00,9.00,13.00};
  Float_t hpt[nPtBins] ={1.60,1.70,1.80,1.90,2.00,2.10,
  2.20,2.30,2.40,2.60,2.80,3.00,3.50,4.00,5.00,
  6.00,7.00,9.00,13.00,20.00};
  Float_t hptCut=0.3; Float_t hptMax = 19;
  Float_t lowPhi=-3.25, highPhi=3.25;
  */

  float lowLimit[12] =  {       0,-8.,0.8,        0,-10.,0.8,      0,-0.25,0.8,       0,3.,0.8};
  float highLimit[12] = {10000000,-1.,1.4,    100000000, -2.,1.4,   1000000, -0.1, 1.05,   1000000,8.,2.5};
  //1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.5 
  // Normal Sample
  const Int_t nPtBins = 16;
  Float_t lpt[nPtBins] ={1.50,1.60,1.80,2.00,
    2.20,2.50,3.00,3.50,4.00,4.50,5.00,5.50,
    6.00,7.00,8.00,10.0};
  Float_t hpt[nPtBins] ={1.60,1.80,2.00,2.20,
    2.50,3.00,3.50,4.00,4.50,5.00,5.50,6.00,
    7.00,8.00,10.0,13.0};
  Float_t hptCut=0.3; Float_t hptMax = 19;
  Float_t lowPt=1.0, highPt=14;
  Float_t lowPhi=-3.25, highPhi=3.25;

  Float_t trigThreshold[4] = {1.5,2.0,3.0,4.0}; //Trig turn on pT

  /* const int nEtaBins = 20;
     float etaLow[20]  = {-1.0,-0.9,-0.8,-0.7,-0.6,
     -0.5,-0.4,-0.3,-0.2,-0.1,
     0.0, 0.1, 0.2, 0.3, 0.4,
     0.5, 0.6, 0.7, 0.8, 0.9};
     float etaHigh[20] = {-0.9,-0.8,-0.7,-0.6,-0.5,
     -0.4,-0.3,-0.2,-0.1, 0.0, 
     0.1, 0.2, 0.3, 0.4, 0.5, 
     0.6, 0.7, 0.8, 0.9, 1.0};*/


  /* // Small File Test Sample
     const Int_t nPtBins = 5;
     Float_t lpt[nPtBins] ={1.50,2.50,3.50,4.50,5.50};
     Float_t hpt[nPtBins] ={2.50,3.50,4.50,5.50,10};
     Float_t hptCut=0.3; Float_t hptMax = 19;
     Float_t lowPhi=-3.25, highPhi=3.25;

     Float_t trigThreshold[4] = {1.5,2.4,3.5,4.6}; //Trig turn on pT
     */
/*  const int nEtaBins = 10;
  float etaLow[20]  = {-0.7,-0.5,-0.3,-0.2,-0.1, 0.0, 0.1, 0.2, 0.3, 0.5};
  float etaHigh[20] = {-0.5,-0.3,-0.2,-0.1, 0.0, 0.1, 0.2, 0.3, 0.5, 0.7};
*/
  const int nEtaBins = 1;
  float etaLow[20]  = {-0.7};
  float etaHigh[20] = { 0.7};
  // Remember Cent bins go backward (15 most central) 0 = 80-75%, 1 = 75-70%, ... 15 = 5-0%
  // For pp, no cent bin, just set it to full range since Maker sets it to some value N
  const int nCentBins = 1;
  float centLow[20]  = {15};
  float centHigh[20] = {0};
}

#endif
