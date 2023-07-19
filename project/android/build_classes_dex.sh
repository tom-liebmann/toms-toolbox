#!/usr/bin/sh

ANDROID_DX=$1
ANDROID_JAR=$2
BUILD_DIR=$3
OUTPUT_FILE=$4
SRC_FILES=${@:5}

SCRIPT_DIR=$(dirname "$0")

JAVA_COMPILER=javac

function download_dependencies()
{
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
}

download_dependencies \
    "${SCRIPT_DIR}/pom.xml" \
    "${BUILD_DIR}/dependency_jars"

DEPENDENCY_JARS=(`ls ${BUILD_DIR}/dependency_jars/*.jar`)
DEPENDENCY_CLASSPATH=$(printf ":%s" "${DEPENDENCY_JARS[@]}")
DEPENDENCY_CLASSPATH=${DEPENDENCY_CLASSPATH:1}

CLASS_DIR=${BUILD_DIR}/classes

mkdir -p ${CLASS_DIR}

${JAVA_COMPILER} \
    -classpath "${ANDROID_JAR}:${DEPENDENCY_CLASSPATH}" \
    -Xlint:deprecation \
    -source 1.8 \
    -target 1.8 \
    -d ${CLASS_DIR} \
    ${SRC_FILES[@]}

${ANDROID_DX} \
    --dex \
    --output=${OUTPUT_FILE} \
    --min-sdk-version=26 \
    ${CLASS_DIR} \
    ${DEPENDENCY_JARS[@]}
