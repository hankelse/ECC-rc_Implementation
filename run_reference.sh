#!/bin/bash

# Directory containing the text files
directory="nde_datasets"

# ECC8.jar directory
ref_directory="/Users/hankelse/Desktop/Strash Research/Conte et al./Reference Implementation/ECC-master-testing/ECC8.jar"

# Check if the directory exists
# if [ ! -f "$ref_directory" ]; then
#     echo "JAR file not found at $ref_directory"
#     exit 1
# fi
if [ -d "$directory" ]; then
    # Loop through all .txt files in the specified directory
    for file in "$directory"/*.txt
    do
        # Check if the file exists (in case there are no .txt files)
        if [ -f "$file" ]; then
            # Print the full path of the file
            full_path=$(realpath "$file")
            # cd "/Users/hankelse/Desktop/Strash\ Research/Conte\ et\ al./Reference\ Implementation/ECC-master-testing/"
            java -jar "$ref_directory" -g "$full_path" -f nde -o output.txt
        fi
    done
else
    echo "Directory '$directory' does not exist."
fi