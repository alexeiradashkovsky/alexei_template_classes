BIT=$(getconf LONG_BIT)
DIRECTORY=/usr/include/aps
TEMPLATED=$DIRECTORY/templates


if [ ! -d "$DIRECTORY" ]; then
  mkdir $DIRECTORY
fi

if [ ! -d "$TEMPLATED" ]; then
  mkdir $TEMPLATED
fi

cp ./templates/* $TEMPLATED/

cd uni_tests
make clean
make
./uni_test
