# Kriegssystem

Opinionated combat engine for a game

## Install

- Clone repository
- Initialize dependencies `git submodule init`, `git submodule update`
- Build library in submodule `cd c-algs-and-ds-lib && make lib && cd ../`
- Initialize cmake `sh ccbuild.txt`

## Development

- To update submodule: fetch latest commit and merge it

```shell
cd c-algs-and-ds-lib
git fetch
git merge origin/master
cd ../
```

- Adding new file or library require manual adding everything in CmakeLists.txt
- To execute build run `sh run.txt`

## TODO

- refactor algds lib to cmake and refactor dependencies for Kriegssystem
- implement ground version of combat simulation
