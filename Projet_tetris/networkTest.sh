#! /bin/bash


if ! [[ -f /run/netns/blue ]]
then 
    sudo ip netns add blue
fi

if ! [[ -f /run/netns/red ]]
then 
    sudo ip netns add red
fi

sudo ip link add veth-red type veth peer name veth-blue
sudo ip link set veth-red netns red 
sudo ip link set veth-blue netns blue 
sudo ip -n blue link set veth-blue up 
sudo ip -n red link set veth-red up 
sudo ip -n red addr add 192.168.15.1/24 dev veth-red  
sudo ip -n blue addr add 192.168.15.2/24 dev veth-blue



