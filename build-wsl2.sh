#!/bin/bash
# Build Play! PS3 PKG inside WSL2 Ubuntu - no sudo needed
# Run with: wsl bash /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port/build-wsl2.sh

set -ex

cd /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port

echo "=== Installing PS3 toolchain (pre-built) ==="
# Try to download pre-built PS3 toolchain
cd /opt
# Create local toolchain directory in home
mkdir -p $HOME/ps3dev

# Check if PSL1GHT pre-built binaries exist
# The hldtux/ps3dev Docker image has them at /usr/local/ps3dev
# Let's try to extract or find a pre-built tarball

# Alternative: Use the ps3dev/ps3toolchain repo which has pre-built
git clone --depth 1 https://github.com/ps3dev/ps3toolchain.git 2>/dev/null || true
cd ps3toolchain

# Check what's available
ls -la

# The toolchain script builds from source which takes forever
# Let's check if there's a simpler way - maybe just compile with the existing container approach
# but run the container via WSL2's docker if we install it

echo "=== Installing Docker in WSL2 ==="
# Install Docker in WSL2
curl -fsSL https://get.docker.com | sh
sudo usermod -aG docker $USER

echo "Docker installed. Please restart WSL2 (wsl --shutdown in PowerShell) and run this script again."
echo "Then run: wsl docker run --rm -it -v /mnt/c/Users/backu/Desktop/projetos_claude/play-ps3-port:/workspace -w /workspace hldtux/ps3dev:latest bash build-inside-container.sh"