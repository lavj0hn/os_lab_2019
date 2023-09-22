#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Нет аргументов"
  exit 1
fi

count=$#
sum=0

for arg in "$@"; do
  sum=$(echo "scale=2; $sum + $arg" | bc)
done

average=$(echo "scale=2; $sum / $count" | bc)

echo "Количество аргументов: $count"
echo "Среднее арифметическое: $average"

