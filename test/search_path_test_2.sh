#!/bin/bash

EXEC_DIR=${1:-.}

export TMP_DIR=`mktemp -d /tmp/find_match.XXXXXXXXXX`

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

${EXEC_DIR}/search_path_test_2 "lib.*one.so"   TMP_DIR; test $? -eq 2 || exit 1
${EXEC_DIR}/search_path_test_2 "lib.*two.so"   TMP_DIR; test $? -eq 2 || exit 2
${EXEC_DIR}/search_path_test_2 "lib.*three.so" TMP_DIR; test $? -eq 1 || exit 3
${EXEC_DIR}/search_path_test_2 "lib.*four.so"  TMP_DIR; test $? -eq 0 || exit 4

rm -rf $TMP_DIR
exit 0
