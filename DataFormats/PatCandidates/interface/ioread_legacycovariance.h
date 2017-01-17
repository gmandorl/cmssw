#include "DataFormats/PatCandidates/interface/libminifloat.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"


pat::PackedCandidate::PackedCovariance legacyCovarianceUnpacking(  uint16_t packedCovarianceDxyDxy_, uint16_t packedCovarianceDxyDz_, uint16_t packedCovarianceDzDz_,
                                 int8_t packedCovarianceDlambdaDz_, int8_t packedCovarianceDphiDxy_,
                                 int8_t packedCovarianceDptDpt_, int8_t packedCovarianceDetaDeta_, int8_t packedCovarianceDphiDphi_ ) {
    pat::PackedCandidate::PackedCovariance m;
    m_.dptdpt=unpack8log(packedCovarianceDptDpt_,-15,0)/pt()/pt(); 
    m_.detadeta=unpack8log(packedCovarianceDetaDeta_,-20,-5);
    m_.dphidphi=unpack8log(packedCovarianceDphiDphi_,-15,0)/pt()/pt(); 
    m_.dphidxy=unpack8log(packedCovarianceDphiDxy_,-17,-4);
    m_.dlamdadz=unpack8log(packedCovarianceDlambdaDz_,-17,-4);
    m_.dxydxy=MiniFloatConverter::float16to32(packedCovarianceDxyDxy_)/10000.;
    m_.dxydz=MiniFloatConverter::float16to32(packedCovarianceDxyDz_)/10000.;
    m_.dzdz=MiniFloatConverter::float16to32(packedCovarianceDzDz_)/10000.;
   return m;
}

