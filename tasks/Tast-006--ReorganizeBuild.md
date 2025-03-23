## Tast-006--ReorganizeBuild.md

### Motivation
I want to make files structure more clear and organized.
Let's start with organizing build files.

## Acceptance Criteria

### Phase 1: Quick build
- [x] Add build-quick.sh 
    - [x] Build just enough to run tests
    - [x] Add Debug build types
    - [x] Add build status output
    - [x] Add option to build with/without tests
- [x] Document required dependencies
    - [x] List all dependencies with versions
    - [x] Add installation instructions for Ubuntu 24.04
    - [x] Document minimum CMake version
- [x] Add GitHub action for quick build -- for all PRs and push to main (except PR merge)
- [x] Add clean option
    - [x] Update .gitignore accordingly


NOTE: 
- Make sure all temporary files are localized to build folder

### Phase 2: Full build
- [ ] Add build-release.sh
    - [ ] Docker-based build for host independence
    - [ ] Build all targets
    - [ ] Package all artifacts
        - [ ] Linux: .tar.gz, .deb, .rpm
        - [ ] Windows: .zip, .msi
        - [ ] macOS: .tar.gz, .pkg
        - [ ] fpm, cpack, cmake
    - [ ] Add version information to packages
- [ ] Supported targets
    - [ ] Linux
        - [ ] g++ (specify min version)
        - [ ] clang++ (specify min version)
    - [ ] Windows
        - [ ] Visual Studio (specify versions)
        - [ ] MinGW (specify version)
    - [ ] macOS
        - [ ] clang++ (specify min version)
- [ ] Build organization
    - [ ] All temporary files in build/
    - [ ] All artifacts in dist/
    - [ ] All configs in config/
    - [ ] Consistent artifact naming scheme
- [ ] Add a standard way to specify versions.
- [ ] Add GitHub action for release. TODO - Make more detail plan before executing

### Phase 3: Testing & Verification
- [ ] Verify builds
    - [ ] Smoke tests for each platform
    - [ ] Library load tests
    - [ ] Version information check
- [ ] CI/CD Integration
    - [ ] GitHub Actions workflow
    - [ ] Build matrix for all targets
    - [ ] Artifact publishing

### Phase 4: Organization
- [ ] Root level
    - [ ] Keep `README.md`, `LICENSE`, `CMakeLists.txt`, `compile_commands.json`
    - [ ] Keep `build-quick.sh` and `build-release.sh`
- [ ] Move to appropriate folders
    - [ ] Source code to `src/`
    - [ ] Tests to `tests/`
    - [ ] Documentation to `docs/` -- EXCEPT the README.md and LICENSE
    - [ ] Planning files to `plan/` -- include `Goals.md`, `Tasks/*`, `TODO.md`
    - [ ] Build configs to `config/` -- EXCEPT for `CMakeLists.txt` and `compile_commands.json`

NOTE: Unless there are many more, leave `build-quick.sh` (from phase 1) and `build-release.sh` (from phase 2) on the root. -- No need to create scripts folder just yet.
