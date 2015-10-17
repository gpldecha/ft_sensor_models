#ifndef FT_FEATURE_MODEL_H_
#define FT_FEATURE_MODEL_H_

#include <string>
#include <armadillo>
#include <statistics/distributions/gmm.h>
#include <boost/filesystem.hpp>

namespace ft_model {

class Ft_feature_model{

public:

    enum{air=0,left=1,right=2,up=3,down=4} contact_direction;

public:

    Ft_feature_model(const std::string& path_to_parameters,const float desc_mu,const float desc_beta);

    void update(const arma::colvec& x);

    arma::colvec                         probability;
    std::vector<GMM>                     gmms;
    arma::colvec                         likelihood;
    arma::colvec                         zero;
    float                                desc_mu;
    float                                desc_beta;


};

}



#endif
