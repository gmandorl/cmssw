#ifndef _DataFormats_PatCandidates_CovarianceParameterization_h_
#define _DataFormats_PatCandidates_CovarianceParameterization_h_
#include <TFile.h>
#include <TH3D.h>

class CovarianceParameterization {
    public:
        CovarianceParameterization() : loadedVersion_(0) {}
        void load(int version);
        float  meanValue(int i,int j,int sign,float pt, float eta, float phi, int nHits,int pixelHits, int firstValid,  float cii=1.,float cjj=1.) const ;

    private:
        int loadedVersion_;

        std::vector<TH3D *>  Cov_elements_positive_pixelHit;
        std::vector<TH3D *>  Cov_elements_positive_innerHit;
        std::vector<TH3D *>  Cov_elements_positive_outerHit;

        std::vector<TH3D *>  Cov_elements_negative_pixelHit;
        std::vector<TH3D *>  Cov_elements_negative_innerHit;
        std::vector<TH3D *>  Cov_elements_negative_outerHit;
 

};

#endif

