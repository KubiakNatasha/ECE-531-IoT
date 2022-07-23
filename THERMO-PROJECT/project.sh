#!/bin/bash

printf "Thermostat Simulation for ECE 531 - Intro to Iot\n"
printf "Natasha Kubiak\n"
printf "SUMMER 2022\n"
sleep 5


cat credentials.txt
printf "\n...\n"
sleep 5
printf "Log into VM using Credentials...\n"
sleep 5
printf "Starting Image VM...\n"
sleep 5
cd buildroot/
./qemu-versatile-shell.sh