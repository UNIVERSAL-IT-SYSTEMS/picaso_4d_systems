# picaso_4d_systems
Contains the Picaso driver for interfacing with the touchscreen (https://github.com/4dsystems/Picaso-Serial-Linux-Library) and a simple ros node that publishes an std_msgs/Empty on a topic.

!! Make sure you run the nodes with a user that is in the `dialout` group or as superuser !!

## Params
- ~/serialport (e.g. /dev/ttyUSB0)

## Topic
- ~/trigger [std_msgs/Empty]
