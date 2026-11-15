# Local Docker build for Play! PS3 PKG
# Uses same hldtux/ps3dev image as GitHub Actions

$projectPath = "C:\Users\backu\Desktop\projetos_claude\play-ps3-port"

Write-Host "Starting local PS3 build in Docker..." -ForegroundColor Cyan

docker run --rm -it `
  -v "${projectPath}:/workspace" `
  -w /workspace `
  hldtux/ps3dev:latest `
  bash -c "
    set -ex

    # Clone Play! emulator source
    git clone --depth 1 https://github.com/jpd002/Play-.git /opt/play-emu 2>/dev/null || true

    # Clone Play! Framework for filesystem_def.h
    git clone --depth 1 https://github.com/jpd002/Play--Framework.git /opt/play-framework 2>/dev/null || true

    # Build
    export PSL1GHT=/usr/local/ps3dev
    export PATH=\"/usr/local/ps3dev/bin:\$PATH\"

    # Add all include paths
    EXTRA_INC=\"\"
    for d in \$(find /opt/play-emu/Source -type d 2>/dev/null); do
      EXTRA_INC=\"\$EXTRA_INC -I\$d\"
    done
    if [ -d \"/opt/play-framework/include\" ]; then
      EXTRA_INC=\"\$EXTRA_INC -I/opt/play-framework/include\"
    fi
    EXTRA_INC=\"\$EXTRA_INC -I/usr/local/ps3dev/ppu/include\"
    export EXTRA_INCLUDES=\"\$EXTRA_INC\"

    echo \"Building with includes: \$EXTRA_INCLUDES\"
    make clean
    make pkg

    # Copy result to host-accessible location
    cp play_ps3.pkg /workspace/play_ps3.pkg
    echo \"SUCCESS: PKG built at /workspace/play_ps3.pkg\"
  "

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n✅ Build successful! PKG at: $projectPath\play_ps3.pkg" -ForegroundColor Green
    Write-Host "File size: $(Get-Item "$projectPath\play_ps3.pkg" | Select-Object -ExpandProperty Length) bytes" -ForegroundColor Green
} else {
    Write-Host "`n❌ Build failed" -ForegroundColor Red
    exit 1
}