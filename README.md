# Lorenz System Solver

## How to build:

Run from the root of project


```bash

mkdir build  
cd build  
cmake -DUSE_QT=OFF ../src/  
make  

```

Change 3rd line to `cmake ../src/ ` to use qt libraries


## How to run:

Run from build folder:

`./LorenzSystemSolver`