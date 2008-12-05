#!/bin/sh
make dist

rmdir debtmp
mkdir -p debtmp
cd debtmp
tar -xvzf ../libglict-1.1.0.tar.gz
cd libglict-1.1.0
#dh_make -e ivucica@gmail.com -f ../../libglict-1.1.0.tar.gz

rm -rf debian/
cp -r ../../debian/ debian/

echo "--------------------"
echo "Modify now."
echo "Type exit when done"
bash

cp ../../libglict-1.1.0.tar.gz ../libglict_1.1.0.orig.tar.gz
debuild

cd ..
cd ..


