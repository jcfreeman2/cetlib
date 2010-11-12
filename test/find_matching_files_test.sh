#!/bin/bash

TMP_DIR=/tmp/find_match.$$
T1="libthis_is_one.so libanother_one.so"
T2="libthis_is_two.so libanother_two.so"
T3="libthis_is_three.so"
T_ALL="$T1 $T2 $T3"

rm -rf ${TMP_DIR}
mkdir ${TMP_DIR}

for name in ${T_ALL}
do
  touch ${TMP_DIR}/${name}
done

echo `pwd`

find_matching_files_test "lib.*one.so" ${TMP_DIR}; test $$ -eq 2 || exit 1
find_matching_files_test "lib.*two.so" ${TMP_DIR}; test $$ -eq 2 || exit 1
find_matching_files_test "lib.*three.so" ${TMP_DIR}; test $$ -eq 1 || exit 1
find_matching_files_test "lib.*four.so" ${TMP_DIR}; test $$ -eq 0 || exit 1

exit 0
