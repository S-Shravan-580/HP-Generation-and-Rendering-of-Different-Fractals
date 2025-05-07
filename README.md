Heterogenous Parallelism Mini Project

Generation & Rendering of Different Fractals: Mandelbrot Set and Sierpinski Triangle

Libraries used: OpenMP, GLFW 3.4

Sierpinski Fractal Generator (C++ + OpenMP) 
 Features 
- Generates 1,000,000 points using the Chaos Game
- Utilizes OpenMP for parallel point generation
- Exports points to points.txt
- Python script provided to plot the result
  
 Prerequisites
- C++ compiler with OpenMP support (e.g., g++)
- Python 3 with matplotlib and numpy (for visualization)
-Install OpenGL dependencies (for future visual rendering, optional):
    bash
    sudo apt-get update
    sudo apt-get install freeglut3-dev

Compile with OpenMP
g++ sierpinski.cpp -o sierpinski -fopenmp

Run program
./sierpinski

Visualize with python
import numpy as np
import matplotlib.pyplot as plt
points = np.loadtxt('points.txt')
plt.figure(figsize=(10, 10))
plt.scatter(points[:, 0], points[:, 1], s=0.1, c='black', marker='.')
plt.axis('off')
plt.show()

