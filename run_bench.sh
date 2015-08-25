#!/usr/bin/env bash

curr_dir=`pwd`

for build_dir in `ls -d build*`; do
	if [ -d "$curr_dir/$build_dir" ]; then
		echo "$build_dir";
		cd "$curr_dir/$build_dir";
		make -B -j9
		./eigen_slow_sparse 2>&1 | tee "$curr_dir/$build_dir.log"
	fi
done

cd $curr_dir
