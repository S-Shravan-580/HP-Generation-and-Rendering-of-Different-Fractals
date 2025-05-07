#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <time.h>

#define WIDTH 3840    
#define HEIGHT 2160
#define MAX_ITER 1000

unsigned char image[HEIGHT][WIDTH][3]; 

double minReal = -2.0, maxReal = 1.0;
double minImag = -1.0, maxImag = 1.0;

double mapToReal(int x, int width, double minR, double maxR) {
    return minR + x * (maxR - minR) / (width - 1);
}

double mapToImag(int y, int height, double minI, double maxI) {
    return minI + y * (maxI - minI) / (height - 1);
}

int computeEscapeTime(double cr, double ci, int maxIter) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;
    while ((zr * zr + zi * zi <= 4.0) && (iter < maxIter)) {
        double temp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = temp;
        iter++;
    }
    return iter;
}


void getColor(int iter, int maxIter, int *r, int *g, int *b) {
    if (iter == maxIter) {
        *r = *g = *b = 0; 
    } else {
        double t = (double)iter / maxIter;
        *r = (int)(9 * (1 - t) * t * t * t * 255);
        *g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
        *b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
}


void computeMandelbrotIncremental(int startY, int endY) {
    #pragma omp parallel for schedule(dynamic)
    for (int y = startY; y < endY; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double cr = mapToReal(x, WIDTH, minReal, maxReal);
            double ci = mapToImag(y, HEIGHT, minImag, maxImag);
            int iter = computeEscapeTime(cr, ci, MAX_ITER);

            int r, g, b;
            getColor(iter, MAX_ITER, &r, &g, &b);

            image[y][x][0] = (unsigned char)r;
            image[y][x][1] = (unsigned char)g;
            image[y][x][2] = (unsigned char)b;
        }
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);  
}


void display(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);
    glfwSwapBuffers(window);  
}

int main(void) {
    
    printf("Computing Mandelbrot set\n");

    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Set", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
    glRasterPos2i(0, 0);
    
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    double aspectRatio = (double)windowWidth/(double)windowHeight;
    double setWidth = maxReal - minReal;
    double setHeight = maxImag - minImag;
    
    if (aspectRatio > 1.0) {
        double newHeight = setWidth / aspectRatio;
        double offset = (newHeight - setHeight) / 2.0;
        minImag -= offset;
        maxImag += offset;
    } else {
        double newWidth = setHeight * aspectRatio;
        double offset = (newWidth - setWidth) / 2.0;
        minReal -= offset;
        maxReal += offset;
    }

    int numChunks = 10;
    int chunkHeight = HEIGHT / numChunks;
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    int currentChunk = 0;
    clock_t start_time=clock();
    while (!glfwWindowShouldClose(window)) {
        if (currentChunk < numChunks) {
            int startY = currentChunk * chunkHeight;
            int endY = (currentChunk + 1) * chunkHeight;
            computeMandelbrotIncremental(startY, endY);
            if (currentChunk == numChunks-1) {
                clock_t end_time = clock();  
                double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
                printf("Mandelbrot set computed in %.6f seconds\n", time_taken);
                currentChunk++;
            }
            display(window);
           
            currentChunk++;
        } else {
                       display(window);
        }

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
