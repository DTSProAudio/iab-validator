find ../bitstreams -type f -name '*.zip' -exec unzip -- '{}' -d ../bitstreams \;

mkdir ../out
mkdir ../out/cA

sh validate_cA_r1.sh

if diff -qr ../reference ../out ; then
	echo "Pass"
	exit 0
else
	echo "Fail"
	exit 1
fi