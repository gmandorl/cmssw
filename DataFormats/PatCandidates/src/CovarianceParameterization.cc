#include "DataFormats/PatCandidates/interface/CovarianceParameterization.h"
//nclude "DataFormats/PatCandidates/interface/liblogintpack.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include <boost/format.hpp>
#include <iostream>
void CovarianceParameterization::load(int version)
{
 edm::FileInPath fip((boost::format("DataFormats/PatCandidates/data/CovarianceParameterization_version%d.root") % version).str());
 std::cerr << "Hello there, I'm going to load " <<  fip.fullPath().c_str() << std::endl;
 TFile fileToRead(fip.fullPath().c_str());
//Read files from here fip.fullPath().c_str();
 if(fileToRead.IsOpen())  {
     readFile(fileToRead);
     fileToRead.Close();
 //Those can be loaded from the root file too in priciple
 //    for(int i=0;i<150;i++) bits_[i]=0;


     bits_[0][0][0]=3;
     bits_[1][1][0]=3;
     bits_[2][2][0]=3;
     bits_[3][3][0]=12;
     bits_[4][4][0]=12;
     bits_[3][4][0]=12;
     bits_[1][4][0]=3;
     bits_[2][3][0]=3;



     bits_[0][0][1]=2;
     bits_[1][1][1]=2;
     bits_[2][2][1]=2;
     bits_[3][3][1]=12;
     bits_[4][4][1]=12;
     bits_[3][4][1]=12;
     bits_[1][4][1]=3;
     bits_[2][3][1]=3;



     bits_[0][0][2]=2;
     bits_[1][1][2]=2;
     bits_[2][2][2]=2;
     bits_[3][3][2]=12;
     bits_[4][4][2]=12;
     bits_[3][4][2]=12;
     bits_[1][4][2]=2;
     bits_[2][3][2]=2;



     bits_[0][0][3]=2;
     bits_[1][1][3]=2;
     bits_[2][2][3]=2;
     bits_[3][3][3]=10;
     bits_[4][4][3]=10;
     bits_[3][4][3]=10;
     bits_[1][4][3]=5;
     bits_[2][3][3]=5;



     bits_[0][0][4]=2;
     bits_[1][1][4]=2;
     bits_[2][2][4]=2;
     bits_[3][3][4]=10;
     bits_[4][4][4]=10;
     bits_[3][4][4]=10;
     bits_[1][4][4]=4;
     bits_[2][3][4]=4;




     bits_[3][3][5]=6;
     bits_[4][4][5]=6;
     bits_[3][4][5]=6;
     bits_[1][4][5]=2;
     bits_[2][3][5]=2;


     bits_[3][3][6]=4;
     bits_[4][4][6]=4;
     bits_[3][4][6]=4;
     bits_[1][4][6]=2;
     bits_[2][3][6]=2;


     bits_[3][3][7]=6;
     bits_[4][4][7]=6;
     bits_[3][4][7]=6;
     bits_[1][4][7]=0;
     bits_[2][3][7]=0;


/*
 *
0 12  3 3
1 12  2 3
2 12  2 2
3 10  2 5
4 10  2 4
5 6 0 2
6 4 0 2
7 6 0 0
 */ 
    loadedVersion_=version; 
     std::cerr << "Loaded version " << loadedVersion_ << " " << version << " " << loadedVersion() << std::endl;
 } else {loadedVersion_=-1;}

}



void CovarianceParameterization::readFile( TFile & f) {

    for (int i = 0; i < 5; i++) {
        for (int j = i; j < 5; j++) {

            std::string String_first_positive = "_pixel_positive";
            std::string String_second_positive = "_inner_positive";

            addTheHistogram(&cov_elements_pixelHit, String_first_positive, i, j,f);
            addTheHistogram(&cov_elements_noPixelHit, String_second_positive, i, j,f);

            std::cout <<std::endl;

        }
    }
}


void CovarianceParameterization::addTheHistogram(std::vector<TH3D *> * HistoVector, std::string StringToAddInTheName, int i, int j,  TFile & fileToRead) {

    std::string  List_covName[5] = {"qoverp", "lambda", "phi", "dxy", "dsz"};

    std::string histoNameString = "histo_" + List_covName[i] + "_" + List_covName[j] + StringToAddInTheName;// + "_entries";
    TH3D * matrixElememtHistogramm = (TH3D*) fileToRead.Get(histoNameString.c_str());
    HistoVector->push_back(matrixElememtHistogramm);
    std::cout << "un istogrammma:\t" << matrixElememtHistogramm << " \t\t"+histoNameString << "      \t\tle entrate sono:\t" << matrixElememtHistogramm->GetEntries() << std::endl;
}



float CovarianceParameterization::meanValue(int i,int j,int sign,float pt, float eta, int nHits,int pixelHits,  float cii,float cjj) const {
   if(loadedVersion_==0) {
      if(i==0 and j==0) return 1./pt/pt;
      if(i==2 and j==2) return 1./pt/pt;
      return 1; 
    }
    int hitNumberToUse = nHits;
    if (hitNumberToUse < 2 ) hitNumberToUse = 2;
    if (hitNumberToUse > 32 ) hitNumberToUse = 32;
    int  ptBin = cov_elements_pixelHit[0]->GetXaxis()->FindBin(pt);
    int etaBin = cov_elements_pixelHit[0]->GetYaxis()->FindBin(std::abs(eta));
    int hitBin = cov_elements_pixelHit[0]->GetZaxis()->FindBin(hitNumberToUse);
    int min_idx = i;
    int max_idx = j;

    if (i>j) {
        min_idx = j;
        max_idx = i;
    }

    int indexOfTheHitogramInTheList = ((9 - min_idx)*min_idx)/2 + max_idx;


//uble TrackCovarianceMatrixParametrization::assignTheElement(double oldElement, int pixelValid, int innerStripsValid, int innerStripsLost, int indexOfTheHitogramInTheList, int ptBin, int etaBin, int hitBin) {
//    std::cout << "bins " << ptBin << " " << etaBin << " "<< hitBin << " " << indexOfTheHitogramInTheList;
    double meanValue = 0.;
    if (pixelHits > 0) {
            meanValue =sign* cov_elements_pixelHit[indexOfTheHitogramInTheList]->GetBinContent(ptBin, etaBin, hitBin);
        }
        else {
            meanValue = sign*cov_elements_noPixelHit[indexOfTheHitogramInTheList]->GetBinContent(ptBin, etaBin, hitBin);
        }
    return meanValue;

}

float CovarianceParameterization::packed(float value, int quality, int i,int j,float pt, float eta, int nHits,int pixelHits,  float cii,float cjj) const {
    if(i>j) std::swap(i,j);
    float ratio=value/meanValue(i,j,std::copysign(1.,value),pt,eta,nHits,pixelHits,cii,cjj);
    int bits=bits_[i][j][quality];
//    std::cout << "bits " << bits << " " << i << " " << j << " " << quality << std::endl;
    if(bits==0) return 0;
    if(bits==1) return std::copysign(1.,value);
    return MiniFloatConverter::reduceMantissaToNbits(ratio,bits);

}

