/*
 * ros_wrapper.h
 *
 *  Created on: Dec 5, 2016
 *      Author: silence
 */

#ifndef INCLUDE_QR_ROS_WRAPPER_H_
#define INCLUDE_QR_ROS_WRAPPER_H_

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64MultiArray.h>

#include <chrono>
#include <boost/scoped_ptr.hpp>

#include "mii_robot.h"
///! qr-next-control
#include "mii_control.h"

#define DEBUG_TOPIC
#ifdef DEBUG_TOPIC
#include <repository/joint.h>
#endif

using namespace agile_robot;

using agile_control::MiiControl;

class RosWrapper : public MiiRobot, public MiiControl {
SINGLETON_DECLARE(RosWrapper)

protected:
  virtual bool init() override;
  virtual void create_system_singleton() override;

private:
  ///! This method publish the real-time message, e.g. "/joint_states", "imu", "foot_force"
  void pub_rt_msg();
  /*!
   * @brief The callback for gait_topic.
   */
  void gaitControlCb(const std_msgs::String::ConstPtr&);

  // Just for test
#ifdef DEBUG_TOPIC
  void cbForDebug(const std_msgs::Float32ConstPtr&);
  ros::Subscriber cmd_sub_;
#endif

private:
  // The ROS handle
  ros::NodeHandle nh_;
  typedef enum {
    APP_ROBOT = 0,
    APP_CONTROL = 1,
    N_APPTYPE
  } AppType;

  std::string     param_nss_[N_APPTYPE];
  std::string     root_wrapper_[N_APPTYPE];

  bool            alive_;
  ros::Subscriber gait_ctrl_sub_;

  // About ROS control
  std::chrono::milliseconds  rt_duration_;
};

#endif /* INCLUDE_QR_ROS_WRAPPER_H_ */
