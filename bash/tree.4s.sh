#!/usr/bin/env bash
TREE_ICON=$(cat "/home/vasia/projects/ChristmasTreeLights/bash/tree.png" | base64 -w 0)
DISCONNECTED=$(cat "/home/vasia/projects/ChristmasTreeLights/bash/disconnected.png" | base64 -w 0)
FAST=$(cat "/home/vasia/projects/ChristmasTreeLights/bash/fast.png" | base64 -w 0)
SLOW=$(cat "/home/vasia/projects/ChristmasTreeLights/bash/slow.png" | base64 -w 0)
echo "Tree | image='$TREE_ICON' imageWidth=20"
echo "---"
found=false
for sysdevpath in $(find /sys/bus/usb/devices/usb*/ -name dev); do
    syspath="${sysdevpath%/dev}"
    devname="$(udevadm info -q name -p $syspath)"
    [[ "$devname" == "bus/"* ]] && continue
    eval "$(udevadm info -q property --export -p $syspath)"
    [[ -z "$ID_SERIAL" ]] && continue
    if [ $ID_SERIAL =  "1a86_USB2.0-Serial" ]; then 
        found=true
        name=$devname
	fi
done

if [ "$found" = true ] ; then
    echo "Adress /dev/$name"
    echo "Fast | bash='stty 9600 -F /dev/$name raw -echo -hupcl && echo s0>>/dev/$name' image='$FAST' imageWidth=23 terminal=false"
    echo "Slow | bash='stty 9600 -F /dev/$name raw -echo -hupcl && echo s1>>/dev/$name' image='$SLOW' imageWidth=23 terminal=false"
else
    echo "Off | image='$DISCONNECTED' imageWidth=23"
fi

#
#if [ $state == '1' ];then
#  echo "Off | bash='xinput --disable $device' iconName=input-keyboard terminal=false"
#else
#  echo "On | bash='xinput --enable $device' iconName=input-keyboard terminal=false"
#fi
