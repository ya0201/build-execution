#!/usr/bin/env zsh

pushd "$(cd $(dirname $0) >/dev/null 2>&1; pwd)/../sounds"
echo '' > sounds.h
for f in *.wav; do
  xxd -i "$f" >> sounds.h
done
