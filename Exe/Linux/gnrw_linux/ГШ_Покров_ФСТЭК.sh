#!/bin/bash
unset QT_PLUGIN_PATH   
appname=$(basename "$0" .sh)
dirname=$(dirname "$0")
cd "$dirname/bin"
export LD_LIBRARY_PATH=`pwd`
./$appname "$@"
