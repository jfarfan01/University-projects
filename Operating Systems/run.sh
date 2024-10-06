dir=$(pwd)

mkdir -p /tmp/sweb
cd /tmp/sweb;
cmake $dir
make -j
make qemu