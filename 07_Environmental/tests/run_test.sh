#!/bin/bash

RHASHER="$1"
shift

HASH_ALGO="$1"
INPUT="$2"
EXPECTED_OUTPUT="$3"
EXPECTED_EXIT_CODE="${4:-0}"

OUTPUT=$(echo "$HASH_ALGO $INPUT" | "$RHASHER")
EXIT_CODE=$?

if [ "$EXIT_CODE" -ne "$EXPECTED_EXIT_CODE" ]; then
	echo "Ожидался код возврата $EXPECTED_EXIT_CODE, но получен $EXIT_CODE"
	exit 1
fi

if [ -n "$EXPECTED_OUTPUT" ]; then
	OUTPUT_CLEAN=$(echo -n "$OUTPUT" | tr -d ' \n')
	EXPECTED_OUTPUT_CLEAN=$(echo -n "$EXPECTED_OUTPUT" | tr -d ' \n')

	if [ "$OUTPUT_CLEAN" != "$EXPECTED_OUTPUT_CLEAN" ]; then
		echo "Ожидалось: $EXPECTED_OUTPUT_CLEAN"
		echo "Получено:  $OUTPUT_CLEAN"
		exit 1
	fi
else
	if [[ "$INPUT" != \"* ]]; then
		if [ "$HASH_ALGO" == "MD5" ] || [ "$HASH_ALGO" == "md5" ]; then
			EXPECTED_OUTPUT=$(md5sum "$INPUT" 2>/dev/null | awk '{print toupper($1)}')
		elif [ "$HASH_ALGO" == "SHA1" ] || [ "$HASH_ALGO" == "sha1" ]; then
			EXPECTED_OUTPUT=$(sha1sum "$INPUT" 2>/dev/null | awk '{print toupper($1)}')
		fi

		if [ -z "$EXPECTED_OUTPUT" ]; then
			echo "Не удалось получить ожидаемый хеш для файла '$INPUT' с алгоритмом '$HASH_ALGO'"
			exit 1
		fi

		OUTPUT_CLEAN=$(echo -n "$OUTPUT" | tr -d ' \n')
		EXPECTED_OUTPUT_CLEAN=$(echo -n "$EXPECTED_OUTPUT" | tr -d ' \n')

		if [ "$OUTPUT_CLEAN" != "$EXPECTED_OUTPUT_CLEAN" ]; then
			echo "Ожидалось: $EXPECTED_OUTPUT_CLEAN"
			echo "Получено:  $OUTPUT_CLEAN"
			exit 1
		fi
	fi
fi

exit 0
