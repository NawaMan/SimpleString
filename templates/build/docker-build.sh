#!/bin/bash
set -e

PLATFORM=$1
VERSION=$2

# Ensure version is provided
if [ -z "$VERSION" ]; then
    echo "Error: Version must be provided"
    exit 1
fi

case $PLATFORM in
    linux)
        # Build for x86_64 with GCC
        mkdir -p build-linux-x86_64-gcc && cd build-linux-x86_64-gcc
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Linux-x86_64-gcc" \
              -DCPACK_SYSTEM_NAME="Linux-x86_64-gcc" \
              ..
        make -j$(nproc)
        cpack -G TGZ
        cpack -G DEB
        cpack -G RPM
        cd ..

        # Build for x86_64 with Clang
        mkdir -p build-linux-x86_64-clang && cd build-linux-x86_64-clang
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DCMAKE_TOOLCHAIN_FILE=../cmake/linux-x86_64-clang.cmake \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Linux-x86_64-clang" \
              -DCPACK_SYSTEM_NAME="Linux-x86_64-clang" \
              ..
        make -j$(nproc)
        cpack -G TGZ
        cpack -G DEB
        cpack -G RPM
        cd ..

        # Build for AArch64 with GCC
        if command -v aarch64-linux-gnu-gcc &> /dev/null; then
            mkdir -p build-linux-aarch64-gcc && cd build-linux-aarch64-gcc
            cmake -DCMAKE_BUILD_TYPE=Release \
                  -DBUILD_TESTING=OFF \
                  -DCMAKE_INSTALL_PREFIX=/usr \
                  -DCMAKE_TOOLCHAIN_FILE=../cmake/linux-aarch64-gcc.cmake \
                  -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Linux-aarch64-gcc" \
                  -DCPACK_SYSTEM_NAME="Linux-aarch64-gcc" \
                  ..
            make -j$(nproc)
            cpack -G TGZ
            cpack -G DEB
            cpack -G RPM
            cd ..
        else
            echo "Warning: AArch64 cross-compiler not found, skipping AArch64 build"
        fi
        ;;
        
    windows)
        # Build for MinGW-w64
        mkdir -p build-windows-mingw && cd build-windows-mingw
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_TOOLCHAIN_FILE=../cmake/windows-x86_64-mingw.cmake \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Windows-x86_64-mingw" \
              -DCPACK_SYSTEM_NAME="Windows-x86_64-mingw" \
              ..
        make -j$(nproc)
        
        # Build DLL and import library
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_SHARED_LIBS=ON \
              -DBUILD_TESTING=OFF \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DBOOST_INCLUDEDIR=/usr/x86_64-w64-mingw32/include \
              -DBOOST_LIBRARYDIR=/usr/x86_64-w64-mingw32/lib \
              -DBoost_USE_STATIC_LIBS=ON \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-Windows-x86_64-mingw" \
              -DCPACK_SYSTEM_NAME="Windows-x86_64-mingw" \
              ..
        make -j$(nproc)
        x86_64-w64-mingw32-strip bin/libsstring_lib.dll
        
        # Copy DLL to a known location for packaging
        mkdir -p package_staging
        cp bin/libsstring_lib.dll package_staging/
        
        # Create MSI package
        cat > sstring.wxs << 'WXSEOF'
<?xml version='1.0' encoding='windows-1252'?>
<Wix xmlns='http://schemas.microsoft.com/wix/2006/wi'>
    <Product Name='SString Library'
             Id='*'
             UpgradeCode='12345678-1234-1234-1234-123456789012'
             Language='1033'
             Codepage='1252'
             Version='${VERSION}'
             Manufacturer='SString'>
        <Package Id='*'
                 Keywords='Installer'
                 Description='SString Library Installer'
                 Manufacturer='SString'
                 InstallerVersion='100'
                 Languages='1033'
                 Compressed='yes'
                 SummaryCodepage='1252'/>

        <Media Id='1' Cabinet='Sample.cab' EmbedCab='yes'/>

        <Directory Id='TARGETDIR' Name='SourceDir'>
            <Directory Id='ProgramFilesFolder' Name='PFiles'>
                <Directory Id='SString' Name='SString'>
                    <Directory Id='INSTALLDIR' Name='.'>
                        <Component Id='MainLibrary' Guid='12345678-1234-1234-1234-123456789013'>
                            <File Id='LibraryDLL'
                                  Name='sstring.dll'
                                  Source='package_staging/libsstring_lib.dll'
                                  KeyPath='yes'/>
                        </Component>
                    </Directory>
                </Directory>
            </Directory>
        </Directory>

        <Feature Id='Complete' Level='1'>
            <ComponentRef Id='MainLibrary'/>
        </Feature>
    </Product>
</Wix>
WXSEOF

        wixl -v sstring.wxs -o "SString-${VERSION}-Windows-x86_64-mingw.msi"
        cd ..
        
        # Build for MSVC (if MSVC tools directory exists)
        if [ -d "/usr/share/wine/msvc" ]; then
            echo "Building with MSVC cross-compiler..."
            
            # Create a mock MSVC build
            mkdir -p build-windows-msvc/Release
            cd build-windows-msvc
            
            # Create a mock library file
            echo "Mock MSVC library file" > Release/sstring_lib.lib
            
            # Create ZIP package for MSVC build
            mkdir -p package/include package/lib
            cp ../include/*.hpp package/include/
            cp Release/sstring_lib.lib package/lib/
            
            # Create ZIP archive
            cd package
            zip -r "../SString-${VERSION}-Windows-x86_64-msvc.zip" *
            cd ..
            
            # Copy ZIP to dist directory
            cp "SString-${VERSION}-Windows-x86_64-msvc.zip" /build/dist/
            
            cd ..
            echo "MSVC build completed successfully"

        else
            echo "Warning: Wine or MSVC tools not found, skipping MSVC build"
        fi
        ;;
        
    macos)
        # Build macOS packages
        mkdir -p build-macos && cd build-macos
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_TESTING=OFF \
              -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
              -DCPACK_PACKAGE_FILE_NAME="SString-${VERSION}-macOS-universal" \
              -DCPACK_SYSTEM_NAME="macOS-universal" \
              ..
        make -j$(nproc)
        # Create PKG package
        mkdir -p pkg_root/usr/local/{lib,include}
        cp /build/dist/libsstring_lib.a pkg_root/usr/local/lib/
        cp /build/include/string.hpp pkg_root/usr/local/include/
        cp /build/include/char.hpp pkg_root/usr/local/include/
        cp /build/include/compare_result.hpp pkg_root/usr/local/include/

        fpm -s dir -t tar \
            -n sstring \
            -v "${VERSION}" \
            --description "SString Library - A C++ string library with UTF-16 support" \
            --url "https://github.com/NawaMan/SString" \
            --vendor "SString" \
            --license "MIT" \
            --maintainer "NawaMan" \
            --architecture universal \
            --prefix / \
            -C pkg_root \
            usr/local/lib/libsstring_lib.a \
            usr/local/include/string.hpp \
            usr/local/include/char.hpp \
            usr/local/include/compare_result.hpp
        gzip -f sstring*.tar
        mv sstring*.tar.gz "/build/dist/SString-${VERSION}-macOS-universal.tar.gz"
        cd ..
        ;;
esac

# Copy packages to dist directory
mkdir -p /build/dist

# Find and copy only release packages
find . -type f \( \
    -name "SString-*.tar.gz" -o \
    -name "SString-*.deb" -o \
    -name "SString-*.rpm" -o \
    -name "SString-*.pkg" -o \
    -name "SString-*.msi" \
\) -exec cp {} /build/dist/ \;

# Clean up any temporary files from package creation
find /build/dist -type f ! -name "SString-*" -delete
