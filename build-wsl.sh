#!/bin/bash
# Build Play! PS3 PKG inside WSL2 Ubuntu
# Run with: wsl bash /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port/build-wsl.sh

set -ex

cd /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port

echo "=== Installing dependencies ==="
sudo apt-get update
sudo apt-get install -y build-essential git python3 python3-pip wget curl \
  autoconf automake libtool pkg-config texinfo bison flex \
  libgmp3-dev libmpfr-dev libmpc-dev libelf-dev \
  zlib1g-dev libssl-dev

echo "=== Cloning PSL1GHT ==="
cd /opt
sudo git clone --depth 1 https://github.com/ps3dev/PSL1GHT.git 2>/dev/null || true
sudo chown -R $USER:$USER PSL1GHT

echo "=== Building PS3 toolchain ==="
cd /opt/PSL1GHT
# PSL1GHT uses a build script
if [ -f "toolchain.sh" ]; then
    ./toolchain.sh
else
    echo "toolchain.sh not found, checking structure..."
    find . -name "*.sh" -type f | head -20
fi

echo "=== Setting up environment ==="
export PSL1GHT=/usr/local/ps3dev
export PATH="/usr/local/ps3dev/bin:$PATH"

echo "=== Cloning Play! emulator source ==="
git clone --depth 1 https://github.com/jpd002/Play-.git /opt/play-emu 2>/dev/null || true

echo "=== Cloning Play! Framework ==="
git clone --depth 1 https://github.com/jpd002/Play--Framework.git /opt/play-framework 2>/dev/null || true

echo "=== Building project ==="
cd /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port

# Add all include paths
EXTRA_INC=""
for d in $(find /opt/play-emu/Source -type d 2>/dev/null); do
  EXTRA_INC="$EXTRA_INC -I$d"
done
if [ -d "/opt/play-framework/include" ]; then
  EXTRA_INC="$EXTRA_INC -I/opt/play-framework/include"
fi
EXTRA_INC="$EXTRA_INC -I/usr/local/ps3dev/ppu/include"
export EXTRA_INCLUDES="$EXTRA_INC"

echo "Building with includes: $EXTRA_INCLUDES"
make clean
make pkg

echo "=== SUCCESS! PKG built ==="
ls -la play_ps3.pkg
cp play_ps3.pkg /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port/play_ps3.pkg