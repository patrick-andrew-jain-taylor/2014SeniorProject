

#!/bin/bash

rsync -ruv Motion/ | grep -oP '01-\d\d\d\d\d\d\d\d\d\d\d\d\d\d-snapshot.jpg' > tmp.txt
while :; do
rsync -ruv Motion/ | grep -oP '01-\d\d\d\d\d\d\d\d\d\d\d\d\d\d-snapshot.jpg' > tmp2.txt
sudo ./motionsend $(diff tmp2.txt tmp.txt | grep -oP '01-\d\d\d\d\d\d\d\d\d\d\d\d\d\d-snapshot.jpg') 
mv tmp2.txt tmp.txt
done
