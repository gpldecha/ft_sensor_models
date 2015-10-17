#include "ft_sensor_model/ft_feature_model.h"
#include <string>
#include <map>

#include <optitrack_rviz/input.h>
#include <optitrack_rviz/listener.h>
#include <netft_rdt_driver/ft_listener.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/String.h>

#include <ros/ros.h>

int main(int argc, char** argv){



    std::map<std::string,std::string> input;
    input["-parameters"]       = "";
    input["-topic_listen"]     = "";
    input["-mu"]               = "0.02";
    input["-beta"]             = "1.0";
    input["-topic_pub"]        = "/ft_feature_sensor";
    input["-rate"]             = "100";

    if(!opti_rviz::Input::process_input(argc,argv,input)){
         ROS_ERROR("failed to load input");
         return 0;
     }
     opti_rviz::Input::print_input_options(input);

     ros::init(argc, argv, "ft_featur_sensor_node");
     ros::NodeHandle node;
     ros::Rate rate(boost::lexical_cast<float>(input["-rate"]));


     netft::Ft_listener ft_listener(node,input["-topic_listen"]);

     float mu   = boost::lexical_cast<float>(input["-mu"]);
     float beta = boost::lexical_cast<float>(input["-beta"]);

     ft_model::Ft_feature_model ft_feature_model(input["-parameters"],mu,beta);

     arma::colvec3 force;

     ros::spinOnce();

     ros::Publisher pub_feature       =  node.advertise<std_msgs::Float64MultiArray>(input["-topic_pub"] + "_probability",100);
     ros::Publisher pub_feature_names =  node.advertise<std_msgs::String>(input["-topic_pub"] + "_names",100);



     std_msgs::Float64MultiArray feature_msg;
     feature_msg.data.resize(ft_feature_model.probability.n_elem);

     /*std_msgs::String string_msg;
     for(std::size_t i = 0; i < ft_feature_model.gmms.size()-1;i++){
            string_msg.data = string_msg.data + ft_feature_model.gmms[i].name + "|";
     }
     string_msg.data = string_msg.data + ft_feature_model.gmms[ft_feature_model.gmms.size()-1].name + "|";*/

     ROS_INFO("==== start sending features ==== ");
     while(node.ok()){

         force(0) = ft_listener.current_msg.wrench.force.x;
         force(1) = ft_listener.current_msg.wrench.force.y;
         force(2) = ft_listener.current_msg.wrench.force.z;

         ft_feature_model.update(force);

         for(std::size_t i = 0; i < ft_feature_model.probability.n_elem;i++){
            feature_msg.data[i] = ft_feature_model.probability(i);
         }

         pub_feature.publish(feature_msg);
        // pub_feature_names.publish(string_msg);

         ros::spinOnce();
         rate.sleep();
     }

    return 0;
}
