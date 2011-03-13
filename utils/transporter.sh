#!/bin/bash

USER=cejkato2
HOST=star.fit.cvut.cz

SRCDIR=$(pwd)
TARGETDIR=pap

SSHPATH=${USER}@${HOST}

echo "Cleaning"
rm -f pack.tar.gz
make clean > /dev/null 2> /dev/null

echo "Packing"
tar -czf pack.tar.gz *.cpp *.h \
utils sekvencni paralelni \
obj data Makefile*

echo "Uploading"
scp pack.tar.gz ${SSHPATH}:${TARGETDIR}

echo "Remote unpacking"
ssh ${SSHPATH} "cd ${TARGETDIR}; tar -xf pack.tar.gz; rm -f pack.tar.gz"

echo "Finished"

