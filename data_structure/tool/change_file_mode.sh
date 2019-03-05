#!/bin/bash

change()
{
    echo -e "\033[47;30m Enter dir $1 \033[0m"
    
    for file in $1/*; do
        if [ ! -e "$file" ]
        then
            echo "NOT find "$file
            continue
        fi

        if [ -f "$file" ] 
        then
            if [ "${file##*.}"x = "sh"x ] || [ "${file##*/}"x = "configure"x ]\
             || [ "${file##*-}"x = "sh"x ] || [ "${file##*/}"x = "INSTALL"x ]
            then
                echo "SH FILE "$file
                chmod 744 "$file"
            else
                echo "FILE "$file
                chmod 644 "$file"
            fi
        elif [ -d "$file" ] 
        then
            echo "DIR "$file
            chmod 755 "$file"
            change "$file"
        fi

    done
}

change $1
