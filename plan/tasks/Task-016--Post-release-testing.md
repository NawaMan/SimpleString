# Task 7.5: Post-Release Testing

## Objective
Verify that all released packages can be successfully installed and used on their respective target platforms. Implement this testing as a GitHub workflow that runs automatically after releases.

Goal: 7.5: Post release testing

## Packages to Test

### Linux (x86_64)
1. Clang Builds
   - [ ] Test .deb package installation (SString-0.4.2-Linux-x86_64-clang.deb)
   - [ ] Test .rpm package installation (SString-0.4.2-Linux-x86_64-clang.rpm)
   - [ ] Test .tar.gz package usage (SString-0.4.2-Linux-x86_64-clang.tar.gz)

2. GCC Builds
   - [ ] Test .deb package installation (SString-0.4.2-Linux-x86_64-gcc.deb)
   - [ ] Test .rpm package installation (SString-0.4.2-Linux-x86_64-gcc.rpm)
   - [ ] Test .tar.gz package usage (SString-0.4.2-Linux-x86_64-gcc.tar.gz)

### macOS
- [ ] Test universal binary installation (SString-0.4.2-macOS-universal.tar.gz)

### Windows (x86_64)
- [ ] Test MSI installer (SString-0.4.2-Windows-x86_64-mingw.msi)
- [ ] Test MSVC zip package usage (SString-0.4.2-Windows-x86_64-msvc.zip)

## Test Steps for Each Package

1. Installation Testing
   - [ ] Clean system state (remove any previous installations)
   - [ ] Install package using appropriate package manager/method
   - [ ] Verify installation location and files
   - [ ] Check for any installation warnings or errors

2. Basic Usage Testing
   - [ ] Create a simple test program using the library
   - [ ] Verify include paths and linking
   - [ ] Compile the test program
   - [ ] Run basic functionality tests

3. Uninstallation Testing
   - [ ] Uninstall the package
   - [ ] Verify clean removal
   - [ ] Check for any leftover files

## Test Environments Required

1. Linux
   - [ ] Ubuntu (latest LTS) for .deb
   - [ ] Fedora (latest) for .rpm
   - [ ] Clean environment for .tar.gz

2. macOS
   - [ ] Latest macOS version
   - [ ] Test on both Intel and Apple Silicon

3. Windows
   - [ ] Windows 10/11
   - [ ] Clean environment without development tools

## GitHub Workflow Implementation
- [ ] Create `.github/workflows/post-release-testing.yml`
- [ ] Configure workflow to trigger after release creation
- [ ] Set up matrix for testing all package types
- [ ] Configure GitHub-hosted runners for each platform
- [ ] Implement package download from release assets
- [ ] Create test scripts for each package type
- [ ] Set up reporting of test results
- [ ] Add status badges to README

## Documentation
- [ ] Document installation process for each package type
- [ ] Create troubleshooting guide for common issues
- [ ] Update README with verified installation instructions

## Success Criteria
- All packages install without errors
- Test programs compile and run successfully
- Clean uninstallation possible
- Documentation updated with verified instructions

## Notes
- Test in clean virtual machines/containers when possible
- Document any system requirements or dependencies
- Keep track of any issues for future releases
