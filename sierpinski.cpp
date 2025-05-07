
// sierpinski.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <omp.h>
#include <ctime>

struct Point {
    float x, y;
};

const int N = 1000000;  // Total number of points
std::vector<Point> points;

// Chaos Game-based point generator (parallelized using OpenMP)
void generatePoints() {
    points.resize(N);
    Point vertices[3] = {
        {-1.0f, -1.0f},   // Vertex A
        {1.0f, -1.0f},    // Vertex B
        {0.0f, 1.0f}      // Vertex C
    };

    // Parallel region starts
    #pragma omp parallel
    {
        std::mt19937 rng(time(NULL) + omp_get_thread_num());
        std::uniform_int_distribution<int> dist(0, 2);

        #pragma omp for
        for (int i = 0; i < N; i++) {
            float x = 0.0f, y = 0.0f;

            // Warm-up iterations (brings the point into the fractal structure)
            for (int j = 0; j < 10; j++) {
                int r = dist(rng);
                x = (x + vertices[r].x) / 2.0f;
                y = (y + vertices[r].y) / 2.0f;
            }

            // Store the final point
            int r = dist(rng);
            x = (x + vertices[r].x) / 2.0f;
            y = (y + vertices[r].y) / 2.0f;
            points[i] = {x, y};
        }
    }
}

int main() {
    generatePoints();
    std::ofstream out("points.txt");
    for (const auto& p : points) {
        out << p.x << " " << p.y << "\n";
    }
    out.close();
    return 0;
}
