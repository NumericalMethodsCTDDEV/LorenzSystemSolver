# Lorenz System Solver

## How to build:

Build and install gnuplot (dowload here https://sourceforge.net/projects/gnuplot/files/gnuplot/)

Run from the root of project


```bash

mkdir build  
cd build  
cmake -DUSE_QT=OFF ..  
make  

```

Change 3rd line to `cmake .. ` to use qt libraries


## How to run:

Run from build folder:

```bash
cd bin  
./LorenzSystemSolver  
```
