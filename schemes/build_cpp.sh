
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)

${SHELL_FOLDER}/flatc --cpp ${SHELL_FOLDER}/schema.fbs
mv ./*.h ${SHELL_FOLDER}/../cpp/include
