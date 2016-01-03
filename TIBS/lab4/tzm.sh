#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo "ERR: Need root access."
	exit 1
fi

module="tzm"
node="/dev/tzm"

function start_tzm() {
	param=""
	if [ ! -z "$1" ]; then
		param="default_speed=$1"
	fi
	insmod ./$module.ko $param
	major=`awk "\\$2==\"$module\" {print \\$1}" /proc/devices`
	mknod $node c $major 0
	chgrp staff $node
	chmod a+rw $node
}

function stop_tzm() {
	rm -f $node
	rmmod "$module.ko"
}

case "$1" in
	start)
		echo -n "Loading $module ... "
		if [ ! -z "`lsmod | grep $module`" ]; then
			echo "[ERROR]"
			echo "Module already loaded!"
			echo "Abort."
			exit 1
		fi
		if [ -f ./$module.ko ]; then
			start_tzm $2
		else
			echo "[ERROR]"
			echo "No module '$module.ko' found."
			echo "Abort."
			exit 1
		fi
		echo "[DONE]"
		;;
	stop)
		echo -n "Unloading $module ... "
		stop_tzm
		echo "[DONE]"
		;;
	restart)
		echo -n "Reloading $module ... "
		stop_tzm
		start_tzm $2
		echo "[DONE]"
		;;
	status)
		if [ -z "`lsmod | grep $module`" ]; then
			echo "Module $module is NOT loaded."
		else
			echo "Module $Module is loaded."
		fi
		;;
	*)
		echo "Usage: $0 {start|stop|restart}"
		exit 1
esac

exit 0

