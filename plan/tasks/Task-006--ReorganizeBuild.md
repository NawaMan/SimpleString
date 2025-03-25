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
- [x] Add build-release.sh
    - [x] Docker-based build for host independence
    - [x] Build all targets
    - [x] Package all artifacts
        - [x] Linux: .tar.gz, .deb, .rpm
        - [x] Windows: .zip
        - [x] macOS: .tar.gz
        - [x] fpm, cpack, cmake
    - [x] Add version information to packages
- [x] Supported targets
    - [x] Linux
        - [x] g++ (13.3.0)
        - [x] clang++ (available via Docker)
    - [x] Windows
        - [ ] Visual Studio (specify versions)
        - [x] MinGW (via mingw-w64)
    - [x] macOS
        - [x] clang++ (via Docker)
- [x] Build organization
    - [x] All temporary files in build/
    - [x] All artifacts in dist/
    - [x] All configs in cmake/
    - [x] Consistent artifact naming scheme
- [x] Add a standard way to specify versions.
- [ ] Add GitHub action for release. TODO - Make more detail plan before executing
    - [x] Add version.txt
    - [x] Create release branch
    - [x] Modify `build-release.sh` to use version.txt if no version is given
    - [x] Git action only triggered manually
    - [ ] When the build is done (all test passed) and the commit is on the release branch
        - [x] Publish artifacts to GitHub artifacts
        - [x] Add Git tag for that version
        - [x] Create a release and Publish artifacts as a release package.
        - [x] Only runnable in main and release branch
        - [x] If the branch is not release, append the version with "-snapshot"
        - [x] If the branch is not release, do not publish the release.

### Phase 3: Testing & Verification
- [ ] Verify builds
    - [ ] Smoke tests for each platform
    - [ ] Version information check
- [x] CI/CD Integration
    - [x] GitHub Actions workflow
    - [x] Build matrix for all targets
    - [x] Artifact publishing

### Phase 4: Organization
- [x] Root level
    - [x] Keep `README.md`, `LICENSE`, `CMakeLists.txt`, `compile_commands.json`
    - [x] Keep `build-quick.sh` and `build-release.sh`
- [x] Move to appropriate folders
    - [x] Source code to `src/`
    - [x] Tests to `tests/`
    - [x] Documentation to `docs/` -- EXCEPT the README.md and LICENSE
    - [x] Planning files to `plan/` -- include `Goals.md`, `Tasks/*`, `TODO.md`
    - [x] Build configs to `config/` -- EXCEPT for `CMakeLists.txt` and `compile_commands.json`

NOTE: Unless there are many more, leave `build-quick.sh` (from phase 1) and `build-release.sh` (from phase 2) on the root. -- No need to create scripts folder just yet.
