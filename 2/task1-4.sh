#!/bin/bash

begin=1
end=20

if [ $# -eq 2 ]; then
    begin=$1
    end=$2
fi

list=$(seq $begin $end)

for file in $list
do
    # 1. Cоздаем файлы
    touch "$file.txt"
    #  2. Записываем имя в файл
    echo "$file.txt" > "$file.txt"
    #  3. Удаляем файлы без 4 в имени
    if [[ ! $file =~ [4] ]]; then
        rm "$file.txt"
    #  4. Меняем содержимое файла 14.txt
    elif [ $file = "14" ]; then
        echo "Hello World" > "$file.txt"
    fi
done

exit 0