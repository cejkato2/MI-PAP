#!/bin/bash

SRCDIR=$(pwd)
. ${SRCDIR}/utils/credentials.sh

SSHPATH=${USER}@${HOST}

echo "Cleaning"
rm -f pack.tar.gz
make clean > /dev/null 2> /dev/null

echo "Packing"
tar -czf pack.tar.gz *.cpp utils \
  sekvencni paralelni \
  obj data Makefile* bin

echo "Uploading to ${SSHPATH}"
scp pack.tar.gz ${SSHPATH}:${TARGETDIR}

echo "Remote unpacking"
ssh ${SSHPATH} "cd ${TARGETDIR}; tar -xf pack.tar.gz; rm -f pack.tar.gz"

echo "Finished"

