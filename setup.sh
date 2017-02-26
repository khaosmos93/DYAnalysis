#!bin/bash

export KP_ANALYZER_PATH = $(pwd)
export ROOT_INCLUDE_PATH = ${KP_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}

echo "setup is finished"