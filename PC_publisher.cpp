#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
ros::Publisher pub;
void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
sensor_msgs::PointCloud2 output;
pcl::PCLPointCloud2* cloud =new pcl::PCLPointCloud2;
pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
pcl::PCLPointCloud2 cloud_filtered;
// Do data processing here…
pcl_conversions::toPCL(*input, *cloud);
pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
sor.setInputCloud (cloudPtr);
sor.setLeafSize(0.05,0.05,0.05);
sor.filter (cloud_filtered);
pcl_conversions::fromPCL(cloud_filtered, output);
// Publish the data.
pub.publish (output);
}
pcl::common::CloudGenerator<PointT, GeneratorT>::fill (int width, int height, pcl::PointCloud<PointT>& cloud)
 {
  if (width < 1)
  {
    PCL_ERROR ("[pcl::common::CloudGenerator] Cloud width must be >= 1!\n");
   return (-1);
 }

    if (height < 1)
   {
     PCL_ERROR ("[pcl::common::CloudGenerator] Cloud height must be >= 1!\n");
   return (-1);
   }

  if (!cloud.empty ())
  {
     PCL_WARN ("[pcl::common::CloudGenerator] Cloud data will be erased with new data!\n");
   }

   cloud.width = width;
  cloud.height = height;
 cloud.resize (cloud.width * cloud.height);
 cloud.is_dense = true;
 for (typename pcl::PointCloud<PointT>::iterator points_it = cloud.begin ();
      points_it != cloud.end ();
       ++points_it)
   {
     points_it->x = x_generator_.run ();
    points_it->y = y_generator_.run ();
   points_it->z = z_generator_.run ();
   }
  return (0);
 }

int main (int argc, char** argv)
{
// Initialize ROS
ros::init (argc, argv,"pub_pcl");
ros::NodeHandle nh;
// Create a ROS subscriber for the input point cloud
ros::Subscriber sub = nh.subscribe ("/camera/depth/points", 1, cloud_cb);
// Create a ROS publisher for the output point cloud
pub = nh.advertise<sensor_msgs::PointCloud2> ("KinectFilter", 1);
// Spin
ros::spin ();
return 0;
}
