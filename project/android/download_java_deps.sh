#!/usr/bin/sh

POM_FILE=$1
OUTPUT_DIR=$2

TMP_DIR=$(mktemp -d)

mvn \
    -f ${POM_FILE} \
    dependency:copy-dependencies \
    "-Doutput.directory=${TMP_DIR}"

for aar_file in ${TMP_DIR}/*.aar
do
    unzip -p $aar_file classes.jar > "${aar_file%.aar}.jar";
done

mkdir -p ${OUTPUT_DIR}

cp ${TMP_DIR}/*.jar ${OUTPUT_DIR}

rm -rf ${TMP_DIR}
