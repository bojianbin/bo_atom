#!/bin/bash

change()
{
    for file in $1/*; do
        if [ -f "$file" ] 
        then
            if [ "${file##*.}"x = "sh"x ] || [ "${file##*/}"x = "configure"x ]\
             || [ "${file##*-}"x = "sh"x ] || [ "${file##*/}"x = "INSTALL"x ]
            then
                echo "SH FILE "$file
                chmod 744 $file
            else
                echo "FILE "$file
                chmod 644 $file
            fi
        elif [ -d "$file" ] 
        then
            echo "DIR "$file
            chmod 755 $file
            change $file
        fi
    done
}

change $1
