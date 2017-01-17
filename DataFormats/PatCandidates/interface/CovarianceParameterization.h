#ifndef _DataFormats_PatCandidates_CovarianceParameterization_h_
#define _DataFormats_PatCandidates_CovarianceParameterization_h_
#include <TFile.h>
#include <TH3D.h>

class CovarianceParameterization {
    public:
        CovarianceParameterization() : loadedVersion_(-1) {}
        bool isValid() {return loadedVersion_!=-1; }
        void load(int version);
        float  meanValue(int i,int j,int sign,float pt, float eta, int nHits,int pixelHits,  float cii=1.,float cjj=1.) const ;
        float  packed(float value, int i,int j,int sign,float pt, float eta, int nHits,int pixelHits,  float cii=1.,float cjj=1.) const;
    private:
        int loadedVersion_;
        std::vector<TH3D *>  cov_elements_pixelHit;
        std::vector<TH3D *>  cov_elements_noPixelHit;
};

#endif

