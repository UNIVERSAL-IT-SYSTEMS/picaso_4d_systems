#include <picasoSerial.h>
#include <Picaso_Types4D.h>
#include <Picaso_const4D.h>

#include <ros/ros.h>
#include <std_msgs/Empty.h>

static int comspeed;

int errCallback(int ErrCode, unsigned char Errbyte)
{
    printf("Serial 4D Library reports error %s", Error4DText[ErrCode]);
    if (ErrCode == Err4D_NAK)
        printf(" returned data = 0x%02X\n", Errbyte) ;
    else
        printf("\n") ;

    //Abort on error?
    if (Error_Abort4D)
        exit(ErrCode);

    return ErrCode;
}

std::vector<WORD> COLORS = { RED, BLUE, GREEN, YELLOW };
unsigned int IDX = 0;
void switchColor()
{
    IDX = (IDX < COLORS.size() - 1) ? ++IDX : 0;
    gfx_Cls();
    gfx_BGcolour(COLORS[IDX]);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "picaso_4d_systems_trigger_node");
    ros::NodeHandle nh("~");

    ros::Publisher pub = nh.advertise<std_msgs::Empty>("trigger", 0);

    // Get param
    std::string serial_port;
    if (!nh.getParam("serialport", serial_port))
    {
        ROS_ERROR("Please specify the 'serialport' local parameter! For example /dev/ttyUSB0");
        exit(1);
    }

    int rc;
    int firstx ;
    int firsty ;
    int x ;
    int y ;
    int state ;

    TimeLimit4D = 2000;
    Callback4D = errCallback;
    // abort on detected 4D Serial error
    Error_Abort4D = 1 ;
    comspeed = 9600;
    //WORD *x = 0x001A;
    //char *s = sys_GetModel(&x);
    //x  = &0x001A;

    std::string str = "string";
    char* port = new char[serial_port.length() + 1];
    strcpy(port, serial_port.c_str());

    rc = OpenComm(port, comspeed);
    if (rc != 0)
    {
        ROS_ERROR("Error %d Opening: %s - %s\n", errno, port, strerror(errno));
        exit(1);
    }

    gfx_Cls();
    switchColor();
    touch_Set(TOUCH_ENABLE);

    ros::Duration d(1.0);
    while (nh.ok())
    {
        if (touch_Get(TOUCH_STATUS) == TOUCH_PRESSED)
        {
            switchColor();
            pub.publish(std_msgs::Empty());
            ROS_INFO("You touched the touchpad!");
        }
    }

    return 0;
}

