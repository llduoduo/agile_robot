/*
 * control_wrapper.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: bibei
 */

#include "apps/control_wrapper.h"

#include "foundation/cfg_reader.h"
#include "foundation/auto_instanceor.h"
#include "foundation/thread/threadpool.h"

SINGLETON_IMPL_NO_CREATE(ControlWrapper)

ControlWrapper* ControlWrapper::create_instance(const std::string& __tag) {
  if (nullptr != instance_) {
    LOG_WARNING << "This method 'create_instance()' is called twice.";
  } else {
    instance_ = new ControlWrapper(Label::make_label(__tag, "wrapper"));
  }
  return instance_;
}

ControlWrapper::ControlWrapper(const std::string& _prefix)
  : MiiControl(Label::make_label(_prefix, "control")), root_tag_(_prefix),
    alive_(true) {
  ;
}

ControlWrapper::~ControlWrapper() {
  halt();
}

bool ControlWrapper::start() {
  bool debug = false;
  ros::param::get("~debug", debug);
  google::SetStderrLogging(debug ?
      google::GLOG_INFO : google::GLOG_WARNING);

  if (!MiiControl::init())
    LOG_FATAL << "Initialization the 'MiiControl' has failed.";

  ThreadPool::instance()->add("mii-control", &ControlWrapper::tick, this);

  std::string str;
  if (!ros::param::get("~gait_topic", str)) {
    LOG_INFO << "No 'gait_topic' parameter, using the default name of topic"
        << " -- gait_control";
    str = "gait_control";
  }
  gait_ctrl_sub_ = nh_.subscribe<std_msgs::String>(str, 1,
      &ControlWrapper::gaitControlCb, this);

  // For debug
  #ifdef DEBUG_TOPIC
  cmd_sub_ = nh_.subscribe<std_msgs::Float32>("/control/debug", 100,
      &ControlWrapper::cbForDebug, this);
  #endif

  return true;
}

void ControlWrapper::create_system_instance() {
  std::string str;
  // if (nh_.getParam("configure", cfg)) {
  if (!ros::param::get("~gait_cfg", str)) {
    LOG_FATAL << "RosWapper can't find the 'configure' parameter "
        << "in the parameter server. Did you forget define this parameter.";
  }
  if (nullptr == MiiCfgReader::create_instance(str))
    LOG_FATAL << "Create the singleton 'MiiCfgReader' has failed.";

  if (!ros::param::get("~gait_lib", str)) {
    LOG_FATAL << "RosWapper can't find the 'library' parameter "
        << "in the parameter server. Did you forget define this parameter.";
  }
  LOG_DEBUG << str;
  if (nullptr == AutoInstanceor::create_instance(str))
    LOG_FATAL << "Create the singleton 'AutoInstanceor' has failed.";

  MiiControl::create_system_instance();
}

void ControlWrapper::halt() {
  alive_ = false;
  // agile_control::MiiControl::instance()->destroy_instance();
  // AutoInstanceor::destroy_instance();
  MiiCfgReader::destroy_instance();
}

void ControlWrapper::gaitControlCb(const std_msgs::String::ConstPtr& msg) {
  if (msg->data.compare("p")) {
    activate("null");
  }

  activate(msg->data);
}

#ifdef DEBUG_TOPIC
void ControlWrapper::cbForDebug(const std_msgs::Float32ConstPtr& msg) {
  LOG_INFO << "Debug Callback Completed!";
}
#endif