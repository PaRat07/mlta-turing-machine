#! /usr/bin/env bash

install_path="${1:-./install}"

docker build . --output type=local,dest="$install_path"
