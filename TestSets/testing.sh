#!/bin/bash
#
# Just write "sh testing.sh" in console 
#

sizes=( 20 40 60 80 100 1 2 3 )
sub_size=( 1 2 3 )
sets=( FirstSet SecondSet ThirdSet )
work_directories=( archive extract logs )

# Delete all garbage
echo "Delete all garbage";
for dir in "${work_directories[@]}"
do
    for set in "${sets[@]}"
    do
        echo "Delete all files in ${dir}/${set}"
        rm -r ${dir}/${set}/*
    done
done

# Huffman Archive
for set in "${sets[@]}"
do
    echo "Huffman Archive ${set}."

    for size in "${sizes[@]}"
    do
        for sub in "${sub_size[@]}"
        do
            echo "Starting ${size}_${sub} test..."
            ./ArchiveAlgorithm_HW -h sets/${set}/${size}_${sub}.txt >> logs/${set}/${size}_${sub}_h_log.txt

            echo "Move ${size}_${sub}.haff to archive/${set}/${size}_${sub}.haff"
            mv ${size}_${sub}.haff archive/${set}/
        done
    done
done

# Huffman Extract
for set in "${sets[@]}"
do
    echo "Huffman Extract ${set}:"

    for size in "${sizes[@]}"
    do
        for sub in "${sub_size[@]}"
        do
            echo "Starting ${size}_${sub} test..."
            ./ArchiveAlgorithm_HW -a archive/${set}/${size}_${sub}.haff >> logs/${set}/${size}_${sub}_unz_h_log.txt

            echo "Move ${size}_${sub}-unz-h.txt to extract/${set}/"
            mv ${size}_${sub}-unz-h.txt extract/${set}/
        done
    done
done

# Shannon-Fano Archive
for set in "${sets[@]}"
do
    echo "Shannon-Fano Archive ${set}."

    for size in "${sizes[@]}"
    do
        for sub in "${sub_size[@]}"
        do
            echo "Starting ${size}_${sub} test..."
            ./ArchiveAlgorithm_HW -s sets/${set}/${size}_${sub}.txt >> logs/${set}/${size}_${sub}_s_log.txt

            echo "Move ${size}_${sub}.shan to archive/${set}/${size}_${sub}.shan"
            mv ${size}_${sub}.shan archive/${set}/
        done
    done
done

# Shannon-Fano Extract
for set in "${sets[@]}"
do
    echo "Shannon-Fano Extract ${set}:"

    for size in "${sizes[@]}"
    do
        for sub in "${sub_size[@]}"
            do
            echo "Starting ${size}_${sub} test..."
            ./ArchiveAlgorithm_HW -u archive/${set}/${size}_${sub}.shan >> logs/${set}/${size}_${sub}_unz_s_log.txt

            echo "Move ${size}_${sub}-unz-s.txt to extract/${set}/"
            mv ${size}_${sub}-unz-s.txt extract/${set}/
        done
    done
done