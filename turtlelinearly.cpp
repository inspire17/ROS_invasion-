/*@abhi*/
#include<unistd.h>
#include<termios.h>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>

using namespace std;
bool sent_disable_msg;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
int cnt=0;
int pc_count;
boost::thread my_thread1;
boost::thread my_thread;
geometry_msgs::Twist twist;

class TurtleOperator
{ 
public:
TurtleOperator();
ros::Publisher operatorPublisher;
ros::Subscriber subscriber;
virtual ~TurtleOperator() {}
void moveForward();
void forstopping();
void startmoving();
void startsensing();
void publisherfunction(geometry_msgs::Twist ptwist);
private:
ros::NodeHandle nodeHandle;
};
char getch(void)
{
char ch;
struct termios oldt;
struct termios newt;
tcgetattr(STDIN_FILENO, &oldt);
newt = oldt;
newt.c_lflag &= ~(ICANON | ECHO);
tcsetattr(STDIN_FILENO, TCSANOW, &newt);
ch = getchar();
tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
return ch;
}


void callback(const PointCloud::ConstPtr& msg)
{ pc_count=0;
  //cout<< msg->width << msg->height;
  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {pc_count++;
  // cout<< pt.x  <<pt.y  <<pt.z;
  }
  //std:: cout<<"Count="<<pc_count<<"\n\n";
}

int main(int argc, char **argv)
{
    //create two threads
ros::init(argc, argv, "move_turtle");
ros::NodeHandle nh;

TurtleOperator turtleOperator;
cout<<"Thread for starting the robot is called\n";
//boost::thread my_thread1(boost::bind(&TurtleOperator::startmoving,&turtleOperator));

turtleOperator.startmoving();
cout<<"Going to LOOP\n";
 ros::spin();
while(1)
{

    if(!my_thread1.joinable()&&!my_thread.joinable())
    {
        break;
    }
}
return EXIT_SUCCESS;
}

TurtleOperator::TurtleOperator()
{
    //cmd_vel_pub = nh->advertise<geometry_msgs::Twist>("cmd_vel", 1, true);
operatorPublisher = nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop",1,true);

}

void TurtleOperator:: startsensing()
{
    cout<<"Subscribing";
    subscriber = nodeHandle.subscribe<PointCloud>("KinectFilter", 1, callback);
    ros::spin();
}

void TurtleOperator::moveForward()
{
TurtleOperator turtleOperator;
//twist.linear.x = 0.50;
twist.linear.y = 0;
twist.linear.z = 0;
twist.angular.x = 0;
twist.angular.y = 0;
twist.angular.z = 0;
turtleOperator.publisherfunction(twist);
}

void TurtleOperator:: forstopping()
{
TurtleOperator turtleOperator;
cout<<"Thread for stoping is initiated\n";
cout<<"Press SpaceBar to stop the thread\n";
char ch;
Loop2:ch=getch();
if(ch==' ')
{   cnt=1;
    my_thread1.interrupt();
    if(!my_thread1.joinable())
    {
        cout<<"Thread one closed\n";
    }

}
else
{
    cout<<"wrong input-Press Space Bar";
    goto Loop2;

}
}
void TurtleOperator:: startmoving()
{
    TurtleOperator turtleOperator;
   // std::cout << "Starting to spin …" << std::endl;
    ros::Rate loopRate(15);
    char ch;
    cout<<"Press i to move turtle bot\n";
    Loop1: ch=getch();
    if(ch=='i'||ch=='I')
    {
    boost::thread my_thread1(boost::bind(&TurtleOperator::startsensing,&turtleOperator));
    boost::thread my_thread(boost::bind(&TurtleOperator::forstopping,&turtleOperator));
    sleep(3);
        TurtleOperator turtleOperator;
        if(pc_count<500)
        {
            cout<<"too close to obstacle";
            ros::shutdown();
               exit(0);
        }
         ros::Rate loopRate(15);
        for(int i=50;i>=0;i--)
        {
            twist.linear.x+=0.005;
            turtleOperator.publisherfunction(twist);
            loopRate.sleep();

        }
    while(ros::ok())
    {
     if(pc_count<1500)
     {
         cnt=2;
     }
     turtleOperator.moveForward();
    //ROS_INFO("Published twist");
     TurtleOperator turtleOperator;
    loopRate.sleep();
    if(cnt==1)
    {
        for(int i=100;i>=0;i--)
        {
            twist.linear.x-=0.005;
            turtleOperator.publisherfunction(twist);
            loopRate.sleep();

        }
        cout<<"Stopped .... Thanks\n";
     ros::shutdown();
        exit(0);
    }
    if(cnt==2)
    {
        for(int i=50;i>=0;i--)
        {
            twist.linear.x-=0.005;
            turtleOperator.publisherfunction(twist);
            loopRate.sleep();

        }
        cout<<"Stopped .... Thanks\n";
        ros::shutdown();
        exit(0);

    }
    }
    }
    else
    {
        cout<<"\nWrong Input-Press i \n";
        goto Loop1;
    }

}

void TurtleOperator::publisherfunction(geometry_msgs::Twist ptwist)
{
this->operatorPublisher.publish(ptwist);
}





