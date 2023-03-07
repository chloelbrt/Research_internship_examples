#! /bin/bash


if ! [[ -f /run/netns/blue ]]
then 
    sudo ip netns add blue
fi

if ! [[ -f /run/netns/red ]]
then 
    sudo ip netns add red
fi


if [[ "$1"==red ]]
then    
    color=31
elif [[ "$1"==blue ]]
then 
    color=34
fi

echo $1
sudo ip netns exec $1 bash 


