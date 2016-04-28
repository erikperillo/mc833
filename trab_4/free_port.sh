#!/bin/sh

port=31472

iptables -I INPUT -p tcp --dport $port -j ACCEPT
