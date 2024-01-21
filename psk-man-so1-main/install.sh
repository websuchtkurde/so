#!/bin/bash

set -eu

man_path="/usr/local/man/man1"

cd "$(dirname "$0")"
pwd
sudo mkdir -p "$man_path"
sudo cp -f *.1.gz "$man_path"
sudo mandb -q
