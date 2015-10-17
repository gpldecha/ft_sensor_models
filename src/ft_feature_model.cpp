#include "ft_sensor_model/ft_feature_model.h"
#include <statistics/decision_functions.h>

namespace ft_model {

Ft_feature_model::Ft_feature_model(const std::string& path_to_parameters, const float decision_threas, const float desc_beta):
desc_mu(desc_mu),desc_beta(desc_beta)
{

    //GMM_Load::load_gmms(path_to_parameters,gmms);
   // likelihood.resize(gmms.size());
    probability.resize(5);



    zero.resize(3);
    zero.zeros();

}

void Ft_feature_model::update(const arma::colvec& x){


    std::cout<< arma::norm(x,2) << std::endl;

    probability(air)  = Decision_functions::bound_mv_gaussian_step_function(x,zero,0.5,3.0,1.0);
    probability(up)   = Decision_functions::step_function(x(0),1.5);        //Decision_functions::step_function(-x(2),1.5);
    probability(down) = Decision_functions::step_function(-x(0),1.5);
    probability(left) = Decision_functions::step_function(x(1),1.5);
    probability(right) = Decision_functions::step_function(-x(1),1.5);



   /* if( arma::norm(x - zero,2) < 3){
        probability(0) = 1;
    }else{
        probability(0) = 0;
    }*/


    //for(std::size_t i = 1; i < gmms.size();i++){
    //    probability(i) = gmms[i].gmm.Probability(x);
      //  probability(i) = likelihood(i);// Decision_functions::gaussian_step_function<float>(likelihood(i),desc_mu,desc_beta);
   // }

}




}
