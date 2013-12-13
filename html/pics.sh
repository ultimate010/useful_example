#!/bin/sh
mkdir ./picture_s;
for img in `ls *.JPG`;
do convert -resize 240 $img ./picture_s/s-$img;
done
