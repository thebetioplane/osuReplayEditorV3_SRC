#!/bin/sh
echo 'This is a dangerous script that deletes the entire dev branch, please be careful'
read
git checkout master \
 && git branch -D dev \
 && git checkout -b dev \
 && git push -uf devorigin dev
