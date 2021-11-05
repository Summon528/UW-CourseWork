#! /bin/bash

if ! [[ -x pzip ]]; then
    echo "pzip executable does not exist"
    exit 1
fi

# Copy the tests once
if ! [[ -d tests ]]; then
	cp rmz/p3a/tests.tar.gz .
	tar -xvzf tests.tar.gz
	#cp -r rmz/p3a/tests .
fi

# Copy and extract tar locally once
if ! [[ -d /nobackup/p3a-data ]]; then
	cp rmz/p3a/p3a-data.tar.gz .
	tar -xvzf p3a-data.tar.gz -C /nobackup/
fi

rmz/tester/run-tests.sh $*

