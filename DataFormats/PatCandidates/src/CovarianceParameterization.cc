#include "DataFormats/PatCandidates/interface/CovarianceParameterization.h"
//nclude "DataFormats/PatCandidates/interface/liblogintpack.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include <boost/format.hpp>
#include <iostream>
void CovarianceParameterization::load(int version)
{
 edm::FileInPath fip((boost::format("PhysicsTools/PatAlgos/data/CovarianceParameterization_version%d.root") % version).str());
 std::cout << "Hello there, I'm going to load " <<  fip.fullPath().c_str() << std::endl;
 TFile fileToRead(fip.fullPath().c_str());
//Read files from here fip.fullPath().c_str();
 if(fileToRead.IsOpen())  {
     loadedVersion_=version; 

 //Those can be loaded from the root file too in priciple
 //    for(int i=0;i<150;i++) bits_[i]=0;
     bits_[0][0][0]=2;
     bits_[1][1][0]=2;
     bits_[2][2][0]=2;
     bits_[3][3][0]=10;
     bits_[4][4][0]=10;
     bits_[3][4][0]=10;

     bits_[0][0][1]=0;
     bits_[1][1][2]=0;
     bits_[2][2][3]=0;
     bits_[3][3][4]=4;
     bits_[4][4][5]=4;
     bits_[3][4][6]=4;
 }

}

float CovarianceParameterization::meanValue(int i,int j,int sign,float pt, float eta, int nHits,int pixelHits,  float cii,float cjj) const {
   if(loadedVersion_==0) {
      if(i==0 and j==0) return 1./pt/pt;
      if(i==2 and j==2) return 1./pt/pt;
      return 1; 
    }
    
 return 1; //fixme

}


float CovarianceParameterization::packed(float value, int quality, int i,int j,float pt, float eta, int nHits,int pixelHits,  float cii,float cjj) const {
    if(i>j) std::swap(i,j);
    float ratio=value/meanValue(i,j,std::copysign(1.,value),pt,eta,nHits,pixelHits,cii,cjj);
    int bits=bits_[i][j][quality];
    if(bits==0) return 0;
    if(bits==1) return std::copysign(1.,value);
    return MiniFloatConverter::reduceMantissaToNbits(ratio,bits);

}

