#!/bin/bash

DELAY=${1:-0}

tput clear

if ! [[ "$DELAY" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
	echo "Ошибка: параметр задержки должен быть положительным числом."
	exit 1
fi

mapfile -t lines </dev/stdin

chars=()
row=0
for line in "${lines[@]}"; do
	col=0
	while IFS= read -r -n1 char; do
		chars+=("$row,$col,$char")
		col=$((col + 1))
	done <<<"$line"
	row=$((row + 1))
done

shuffled_chars=($(printf "%s\n" "${chars[@]}" | shuf))

for item in "${shuffled_chars[@]}"; do
	IFS=',' read -r y x char <<<"$item"

	tput cup "$y" "$x"

	echo -n "$char"

	sleep "$DELAY"
done

tput cup "$(tput lines)" 0
