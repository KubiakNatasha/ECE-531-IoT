#!/bin/bash

printf "Thermostat Simulation for ECE 531 - Intro to Iot\n"
printf "Natasha Kubiak\n"
printf "SUMMER 2022\n"
sleep 5


cat credentials.txt
printf "\n...\n"
sleep 5
printf "Log into VM using Credentials...\n"

printf "After login to VM, type \"thermostat --help\" to show help info\n"
printf "...\n"
sleep 8
printf "Thank you!\n"
printf "...\n"
sleep 2

printf "Booting VM\n"
printf "...\n"
sleep 2
cd buildroot/
./qemu-versatile-shell.sh