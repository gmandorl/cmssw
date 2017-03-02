#include "DataFormats/PatCandidates/interface/CovarianceParameterization.h"
#include "DataFormats/PatCandidates/interface/liblogintpack.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include <boost/format.hpp>
#include <iostream>
#include <TParameter.h>
#include <TVector.h>
#include <TFolder.h>

uint16_t CompressionElement::pack(float value, float ref) const
{
    float toCompress=0;
    switch(target) {
        case(realValue):
          toCompress=value;
          break;
        case(ratioToRef):
          toCompress=value/ref;
          break;
        case(differenceToRef):
          toCompress=value-ref;
          break;
    }
    switch(method) {
        case(float16):
          return MiniFloatConverter::float32to16(toCompress);
          break;
        case(reduceMantissa):
          return MiniFloatConverter::reduceMantissaToNbits(toCompress,params[0]);
          break;
        case(zero):
          return 0;
          break;
        case(one):
          return 1.0;
          break;
          //return  pack16log(toCompress,params[0],params[1],params[2]);
        case(tanLogPack):
          return 0;
          break;
        case(logPack):
          int16_t r=logintpack::pack16log(toCompress,params[0],params[1],bits);
          return * reinterpret_cast<uint16_t *>(&r); //logintpack::pack16log(toCompress,params[0],params[1],params[2]));
          break;
      
    }
  return 0;
}
float CompressionElement::unpack(uint16_t packed, float ref) const
{
    float unpacked=0;
    switch(method) {
        case(float16):
          unpacked= MiniFloatConverter::float16to32(packed);
          break;
        case(reduceMantissa):
          unpacked=packed;
          break;
        case(logPack):
          unpacked=logintpack::unpack16log(* reinterpret_cast<int16_t *>(&packed),params[0],params[1],bits);
          break;
        case(zero):
          unpacked=0;
          break;
        case(one):
        case(tanLogPack):
          unpacked=1;
    }
    switch(target) {
        case(realValue):
          return unpacked;
        case(ratioToRef):
          return unpacked*ref;
        case(differenceToRef):
          return unpacked+ref;
    }

    return ref;
  
}






void CovarianceParameterization::load(int version)
{
 edm::FileInPath fip("/scratch/mandorli/CMSSW_8_0_21/src/plot/parametrizzazioni_pixel_noPixel_mantissaVariabile/ROOT/FinalFileWithParametrization.root");
//  edm::FileInPath fip((boost::format("DataFormats/PatCandidates/data/CovarianceParameterization_version%d.root") % version).str());
 std::cerr << "Hello there, I'm going to load " <<  fip.fullPath().c_str() << std::endl;
 TFile fileToRead(fip.fullPath().c_str()); 
//Read files from here fip.fullPath().c_str();
 if(fileToRead.IsOpen())  {
     readFile(fileToRead);

     //this can be read from file
//      CompressionSchema schema0;
//      schema0(0,0)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,4,{-2,1});
//      schema0(1,1)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,4,{-1,1});
//      schema0(2,2)=schema0(1,1);
//      schema0(3,3)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,1024,{-3,4});
//      schema0(3,4)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,1024,{-8,4});
//      schema0(4,4)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,1024,{-3,4});
//      schema0(2,3)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,32,{-1,1.5});
//      schema0(1,4)=schema0(2,3);
     
     CompressionSchema schema1;
     schema1(3,3)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,16,{-3,4});
     schema1(4,4)=schema1(3,3);
     schema1(3,4)=schema1(3,3);
     schema1(2,3)=CompressionElement(CompressionElement::logPack,CompressionElement::ratioToRef,4,{-3,4});
     schema1(1,4)=schema1(2,3);
     

// { (*((TVector)fileToRead.Get("schemas/1/00/param"))) [0], (*((TVector)fileToRead.Get("schemas/1/00/param"))) [1] }
     
     
//      TFolder * fold =(TFolder *) _file0->Get("schemas")
//      GetListOfFolders()
     
     std::string ListOfFolderName [10] = {"0","1","2","3","4","5","6","7","8","9"} ;
     std::string s [10] = {"0","1","2","3","4"} ;

     for (int folderNumber = 0; folderNumber < 6 ; folderNumber++) {
        CompressionSchema schema; 
         
        std::string folder = "schemas/";
        folder = folder + ListOfFolderName[folderNumber] + "/";
        
        for (int i = 0; i < 5; i++) {
            for (int j = i; j < 5; j++) {        //FILLING ONLY THE SCHEMA OF SOME ELEMENTS
                folder = folder + s[i] + s[j];
                std::string methodString = folder + "/method";
                std::string targetString = folder + "/target";
                std::string bitString = folder + "/bit";

                schema(i,j)=CompressionElement((CompressionElement::Method) ((TParameter<int>*) fileToRead.Get(methodString.c_str()))->GetVal(),(CompressionElement::Target) ((TParameter<int>*) fileToRead.Get(targetString.c_str()))->GetVal(), (int) ((TParameter<int>*) fileToRead.Get(bitString.c_str()))->GetVal(), {-2,1});
        
                
            }
        }
        
     schemas.push_back(schema); 
     }
//      schemas.push_back(schema0); 
     schemas.push_back(schema1);
     
     
    fileToRead.Close();

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
/*   if(loadedVersion_==0) {
      if(i==0 and j==0) return 1./pt/pt;
      if(i==2 and j==2) return 1./pt/pt;
      return 1; 
    }*/
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
    std::cout << "bins " << ptBin << " " << etaBin << " "<< hitBin << " " << indexOfTheHitogramInTheList << std::endl;
    double meanValue = 0.;
    if (pixelHits > 0) {
            meanValue =sign* cov_elements_pixelHit[indexOfTheHitogramInTheList]->GetBinContent(ptBin, etaBin, hitBin);
        }
        else {
            meanValue = sign*cov_elements_noPixelHit[indexOfTheHitogramInTheList]->GetBinContent(ptBin, etaBin, hitBin);
        }
    return meanValue;

}

float CovarianceParameterization::pack(float value, int schema, int i,int j,float pt, float eta, int nHits,int pixelHits,  float cii,float cjj) const {
    if(i>j) std::swap(i,j);
    float ref=meanValue(i,j,1.,pt,eta,nHits,pixelHits,cii,cjj);
    std::cout << "pack: " << pt << " " << eta << " " << nHits << " "  << i << " , " << j << " v: " << value << " r: " << ref << " " << schemas[schema](i,j).pack(value,ref)<< std::endl;
    return schemas[schema](i,j).pack(value,ref);
}
float CovarianceParameterization::unpack(uint16_t packed, int schema, int i,int j,float pt, float eta, int nHits,int pixelHits,  float cii,float cjj) const {
    if(i>j) std::swap(i,j);
    float ref=meanValue(i,j,1.,pt,eta,nHits,pixelHits,cii,cjj);
    std::cout<< "unPack: " << pt << " " << eta << " " << nHits << " "  << i << " , " << j << " v: " << schemas[schema](i,j).unpack(packed,ref) << " r: " << ref << " " << packed << std::endl;
    return schemas[schema](i,j).unpack(packed,ref);
 
}
