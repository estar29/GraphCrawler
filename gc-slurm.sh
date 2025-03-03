#!/bin/bash

# Evan Stark - March 2nd 2025 - ITCS 4145 001
# This script will print out the results and computation times
# of several runs of the Tom Hanks node.

#SBATCH --job-name=TomHanksGraphCrawler
#SBATCH --partition=Centaurus
#SBATCH --time=02:00:00
#SBATCH --mem=16G

make graph-crawler
./graph-crawler "Tom Hanks" 0
./graph-crawler "Tom Hanks" 1
./graph-crawler "Tom Hanks" 2
./graph-crawler "Tom Hanks" 3
