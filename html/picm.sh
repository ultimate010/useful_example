#!/bin/sh
mkdir ./picture_m;
for img in `ls *.JPG`;
do convert -resize 35%x35% -font ElegantScript.ttf -fill white -pointsize 56  -draw 'text 25,60 "Photo By Zaks Wang"' $img ./picture_m/m-$img;
done
