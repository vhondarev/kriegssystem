# Kriegssystem

Opinionated combat engine for a game

## Install

- Clone repository
- Initialize dependencies `git submodule init`, `git submodule update`
- Build library in submodule `cd c-algs-and-ds-lib && make lib && cd ../`

## Development

- To update submodule: fetch latest commit and merge it

```shell
cd c-algs-and-ds-lib
git fetch
git merge origin/master
cd ../
```

- Adding new file or library require manual adding everything in **CmakeLists.txt**

### Dev build

- Build executable with debug flag `scripts/ccbuild.sh`
- Build and run executable with preconfigured data `scripts/debug.sh`

### Prod build

- Build prod executable `scripts/build.sh`

### Test prod build

- Build prod and run execution with preconfigured data
  - `scripts/test_failed.sh`
  - `scripts/test_success.sh`

## TODO

- refactor algds lib to cmake and refactor dependencies for Kriegssystem
- implement ground version of combat simulation
