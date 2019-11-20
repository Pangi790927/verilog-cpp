#!/bin/sh

# Prerequisites:
#sudo apt-get install git make autoconf g++ flex bison
sudo apt-get install libfl2  # Ubuntu only (ignore if gives error)
sudo apt-get install libfl-dev  # Ubuntu only (ignore if gives error)

git clone https://git.veripool.org/git/verilator   # Only first time
## Note the URL above is not a page you can see with a browser, it's for git only

# Every time you need to build:
unsetenv VERILATOR_ROOT  # For csh; ignore error if on bash
unset VERILATOR_ROOT  # For bash
cd verilator
git pull        # Make sure git repository is up-to-date
git tag         # See what versions exist
#git checkout master      # Use development branch (e.g. recent bug fixes)
#git checkout stable      # Use most recent stable release
#git checkout v{version}  # Switch to specified release version

autoconf        # Create ./configure script
./configure
make
sudo make install

sudo mv verilator /usr/share/verilator
# Now see "man verilator" or online verilator.pdf's for the example tutorials