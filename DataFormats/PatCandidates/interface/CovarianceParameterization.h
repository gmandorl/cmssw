#ifndef _DataFormats_PatCandidates_CovarianceParameterization_h_
#define _DataFormats_PatCandidates_CovarianceParameterization_h_
#include <TFile.h>
#include <TH3D.h>

class CovarianceParameterization {
    public:
        CovarianceParameterization() : loadedVersion_(-1),bits_{{{0}}}  {}
        bool isValid() const {return loadedVersion_!=-1; }
        bool loadedVersion() const {return loadedVersion_; }
        void load(int version);
        float  meanValue(int i,int j,int sign,float pt, float eta, int nHits,int pixelHits,  float cii=1.,float cjj=1.) const ;
        float  packed(float value,int quality, int i,int j,float pt, float eta, int nHits,int pixelHits,  float cii=1.,float cjj=1.) const;
    private:
        void readFile( TFile &);
        void  addTheHistogram(std::vector<TH3D *> * HistoVector, std::string StringToAddInTheName, int i, int j, TFile & fileToRead);
        int loadedVersion_;
        int bits_[5][5][10];
        std::vector<TH3D *>  cov_elements_pixelHit;
        std::vector<TH3D *>  cov_elements_noPixelHit;
};

#endif

