#include "../include/cyclone_pathplanner/simple_path.h"
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Path.h>

ros::Publisher pathPublisher;

tf::Stamped<tf::Pose> hoopPose;
bool newHoop = false;

int main(int argc, char **argv){
    ros::init(argc, argv, "simple_pathplanner");

    ros::NodeHandle n;

    ros::Subscriber hoopSubscriber = n.subscribe("hoop_pos", 1, hoopCallback);

    tf::TransformListener listener;
    pathPublisher = n.advertise<nav_msgs::Path>("path", 1);

    ros::Rate rate(10);
    while(ros::ok()){
        if(newHoop){
            tf::Vector3 beforePoint(-0.5, 0, 0);
            tf::Vector3 afterPoint(0.5, 0, 0);

            ros::Time now = hoopPose.stamp_;
            tf::StampedTransform transform;
            listener.waitForTransform("/map", "/hoop", now, ros::Duration(3.0));
            try {
                listener.lookupTransform("/map", "/hoop", now, transform);
                ROS_INFO_STREAM("hoop to map transform: " << transform.getOrigin().x() << ", "
                << transform.getOrigin().y() << ", " << transform.getOrigin().z());
            }
            catch (tf::TransformException ex){
                ROS_ERROR("%s", ex.what());
                ros::Duration(1.0).sleep();
            }

            tf::Vector3 beforeMapPoint, afterMapPoint;

            beforeMapPoint = transform.getOrigin() + transform.getBasis() * beforePoint;
            afterMapPoint = transform.getOrigin() + transform.getBasis() * afterPoint;

            tf::Stamped<tf::Pose> firstPose, secondPose;

            firstPose.stamp_ = now;
            firstPose.frame_id_ = "map";
            secondPose.stamp_ = now;
            secondPose.frame_id_ = "map";
            tf::Quaternion q;
            q.setRPY(0, 0, 0);
            firstPose.setOrigin(beforeMapPoint);
            firstPose.setRotation(q);
            secondPose.setOrigin(afterMapPoint);
            secondPose.setRotation(q);

            geometry_msgs::PoseStamped point;

            nav_msgs::Path path;
            path.header.frame_id = "map";
            path.header.stamp = now;

            tf::poseStampedTFToMsg(firstPose, point);
            path.poses.push_back(point);
            tf::poseStampedTFToMsg(secondPose, point);
            path.poses.push_back(point);

            pathPublisher.publish(path);
            newHoop = false;
        }

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

void updatePath(tf::TransformListener listener){

}

void hoopCallback(const geometry_msgs::PoseStamped::ConstPtr &hoopPos) {
    tf::poseStampedMsgToTF(*hoopPos, hoopPose);
    newHoop = true;
}
