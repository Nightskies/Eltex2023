#!/bin/bash

if [ $# -lt 2 ]; then
    echo "You need to set a range of 2 numbers"
    exit 1
fi

file_dir="dir"

main_dir_path=$(pwd)

mkdir $file_dir
cd $file_dir

for (( i = $1; i <= $2; i++))
do
    for (( j = 0; j < i; j++))
    do
        mkdir "$i"
        cd "$i"
    done

    touch "$i.txt"
    # Сохраняем путь к файлу
    file_path=$(pwd)
    cd "$main_dir_path"
    # Создаем ссылку
    ln -s "$file_path/$i.txt" "$i.txt"
    cd $file_dir
done

exit 0