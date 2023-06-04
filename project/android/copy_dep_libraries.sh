#!/usr/bin/sh

LIBRARY_FILE=$1
SEARCH_PATH_FILE=$2
OUTPUT_DIR=$3

echo "Got library: ${LIBRARY_FILE}"
echo "Got search paths: ${SEARCH_PATHS}"
echo "Got output dir: ${OUTPUT_DIR}"

SEARCH_PATHS=$(cat ${SEARCH_PATH_FILE})

for _lib_file in $(readelf -d ${LIBRARY_FILE} | grep NEEDED | sed -n "s/^.*Shared library: \[\(.*\.so\)\]$/\1/p"); do
    echo "Lib file: ${_lib_file}"
    for _search_path in $(cat ${SEARCH_PATH_FILE}); do
        _file=$(find ${_search_path} -name "${_lib_file}" -print 2>/dev/null | head -n 1 | grep .)
        if [ $? -eq 0 ]; 
        then 
            cp ${_file} "${OUTPUT_DIR}/"
            break;
        fi
    done
done
