#!/bin/sh

#pull up data_exchange service 
#check pid file and /proc/pid related
#no need this when it is controled by systemd.service

PID_FILE="/var/run/data_exchange.pid"
n=0
trap "rm -f /tmp/exchange_guard.lock;exit 0" TERM

in_error()
{
    if [ -f  ${PID_FILE} ]
    then
        pid=$(cat ${PID_FILE})
        proc_dir="/proc/"${pid}
        if [ ! -d  ${proc_dir} ]
        then
            return 1
        fi
    else
        return 1
    fi

    return 0
}
in_cpu_error()
{
	cpu=$(ps -aux | grep out | awk '{print $3}' | sort -rn | head -1)
	outcome=$(echo "$cpu > 97"|bc)
	if [ $outcome -eq 1 ]
	then
		return 1
	else
		return 0
	fi
}

restart()
{
    $(service data_exchange restart)
}

main()
{
    while true
    do
        while [ ${n} -lt 6 ]
        do
            $(in_error)
            if [ $? -eq 1 ]
            then
                n=$(( $n + 1  ))
                sleep 1
            else
                n=0
                sleep 15
            fi
        done

        n=0

        restart
    done
}

if [ $1 = "father" ]
then
        $0 son &
        exit 0
fi

[ -f /tmp/exchange_guard.lock  ] && exit 0
echo $$ > /tmp/exchange_guard.lock
sleep 1
#double check
[ "x$(cat /tmp/exchange_guard.lock)" = "x"$$ ] || exit 0

#loop
main

rm -f /tmp/exchange_guard.lock
