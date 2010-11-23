#!/bin/bash

EXEC_DIR=${1:-.}

TMP_DIR=`mktemp -d /tmp/find_match.XXXXXXXXXX`

trap "[[ -f \"$TMP_DIR\" ]] && rm -rf \"$TMP_DIR\"" EXIT

T1="libthis_is_one.so libanother_one.so"
T2="libthis_is_two.so libanother_two.so"
T3="libthis_is_three.so"
T_ALL="$T1 $T2 $T3"

for name in ${T_ALL}
do
  touch "${TMP_DIR}/${name}"
done

echo `pwd`

${EXEC_DIR}/find_matching_files_test "lib.*one.so" ${TMP_DIR}; test $? -eq 2 || exit 1
${EXEC_DIR}/find_matching_files_test "lib.*two.so" ${TMP_DIR}; test $? -eq 2 || exit 1
${EXEC_DIR}/find_matching_files_test "lib.*three.so" ${TMP_DIR}; test $? -eq 1 || exit 1
${EXEC_DIR}/find_matching_files_test "lib.*four.so" ${TMP_DIR}; test $? -eq 0 || exit 1

exit 0
