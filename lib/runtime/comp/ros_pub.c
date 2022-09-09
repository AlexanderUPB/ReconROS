/*
 * Copyright 2020 Christian Lienen <christian.lienen@upb.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "ros_pub.h"

#include "../utils.h"

int ros_publisher_init(struct ros_publisher_t *ros_pub, struct ros_node_t * ros_node, const rosidl_message_type_support_t * msg_type, char* topic_name)
{
  rcl_ret_t rc = 0;

  ros_pub->rcl_pub= rcl_get_zero_initialized_publisher();
  rcl_publisher_options_t pub_options = rcl_publisher_get_default_options();

      //QOS Signal
  pub_options.qos.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  pub_options.qos.depth = 5;
  pub_options.qos.reliability = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
  pub_options.qos.durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  //pub_options.qos.deadline = RMW_QOS_DEADLINE_DEFAULT;
  pub_options.qos.deadline.sec = 0;
  pub_options.qos.deadline.nsec = 0;
  //pub_options.qos.lifespan = RMW_QOS_LIFESPAN_DEFAULT;  
  pub_options.qos.lifespan.sec = 0;
  pub_options.qos.lifespan.nsec = 0;  
  pub_options.qos.liveliness = RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT;
  pub_options.qos.liveliness_lease_duration.sec = 0;
  pub_options.qos.liveliness_lease_duration.nsec = 0;
  pub_options.qos.avoid_ros_namespace_conventions = false;


  rc = rcl_publisher_init(
      &ros_pub->rcl_pub,
      &(ros_node->node),
      msg_type,
      topic_name,
      &pub_options);
  
  if (RCL_RET_OK != rc) 
  {
    panic("[ROS Publisher] Error in rcl_publisher_init %s; error code: %d \n", topic_name, rc);
    return -1;
  }
  else 
  {
    debug("[ROS Publisher] Created publisher: %s\n", topic_name);
  } 

  return 0;
}

int ros_publisher_destroy(struct ros_publisher_t *ros_pub)
{
  return rcl_publisher_fini(&ros_pub->rcl_pub, ros_pub->node);
}

int ros_publisher_publish(struct ros_publisher_t *ros_pub, void * msg)
{
  rcl_ret_t rc;

  rc = rcl_publish(&ros_pub->rcl_pub, msg, NULL);
  if (rc == RCL_RET_OK) {
    debug("[ROS Publisher] Published message!\n");
    return 0;
  } else {
    debug("[ROS Publisher] Error publishing message!\n");
    return rc;
  }
}
