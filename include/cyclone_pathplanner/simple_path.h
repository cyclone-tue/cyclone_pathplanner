#ifndef CYCLONE_PATHPLANNER_SIMPLE_PATH_H
#define CYCLONE_PATHPLANNER_SIMPLE_PATH_H

#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_listener.h>

void hoopCallback(const geometry_msgs::PoseStamped::ConstPtr &hoopPos);

void updatePath();

#endif //CYCLONE_PATHPLANNER_SIMPLE_PATH_H
