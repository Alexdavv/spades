#!/bin/bash

#renaming templates to originals
cp -n .project.template .project
cp -n .cproject.template .cproject

cd src/debruijn
#cp -n .project.template .project
#cp -n .cproject.template .cproject
cp -n "debruijn release.launch.template" "debruijn release.launch"
cp -n "debruijn debug.launch.template" "debruijn debug.launch"

cd ../abruijn
#cp -n .project.template .project
#cp -n .cproject.template .cproject
cp -n "abruijn release.launch.template" "abruijn release.launch"
cp -n "abruijn debug.launch.template" "abruijn debug.launch"

cd ../hammer
#cp -n .project.template .project
#cp -n .cproject.template .cproject
cp -n "hammer release.launch.template" "hammer release.launch"
cp -n "hammer debug.launch.template" "hammer debug.launch"

cd ../common_test
#cp -n .project.template .project
#cp -n .cproject.template .cproject
cp -n "common_test release.launch.template" "common_test release.launch"
cp -n "common_test debug.launch.template" "common_test debug.launch"

# going to root folder (assembler)
cd ../..

# preparing makefile for all projects and both configurations
rm -rf build/*
./prepare_cfg.sh
