# Purpose
Having fun, really. In more pratical term, the goal of this projet is to make something like the good old Wolfenstein 3D engine, unsing raycasting.

# Dependencies
- compile using clang++, link using lld, so they are required
  - but you can use `g++` and `ld`, but you need to edit the Makefile
- SFML, for window management, textures, etc...

## On Ubuntu
```bash
# apt-get install libsfml-dev clang++ lld make
```

## On Fedora
```bash
# dnf install SFML-dev clang++ lld make
```

## On Arch
```bash
# pacman -S sfml clang++ lld make
```

# Building
```bash
make
```

# Running
```bash
./doon [-h]
```
