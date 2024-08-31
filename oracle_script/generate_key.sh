#!/bin/sh

for idx in `seq 65 120`;
do
  echo `../bazel-bin/tools/key_generator_tools "./cert/node_${idx}" "AES"`
done
