#include "cubeNoOOP.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <SDL2/SDL.h>

struct Point2D { float x; float y; };
struct Point3D { float x; float y; float z; };
struct Edge { int start; int end; };

int WindowSizeX;
int WindowSizeY;
SDL_Renderer* renderer;
SDL_Window* window;
float FL = 5;
float rotation = 0;
float DeltaTime;
float scaledValue = 2;

int iterations = 0;


//Creates an array of points
std::vector<Point3D> points2{
         Point3D{ -1, -1, -1 }, Point3D{ -1, -1, 1 },
         Point3D{ 1,  -1, -1 }, Point3D{ -1,  1, -1 },
         Point3D{ -1, 1, 1 }, Point3D{ 1,  -1, 1 },
         Point3D{ 1,  1, -1 }, Point3D{ 1,  1, 1 }
};

//creates an array of edges (This hold indices referencing to the points array)
std::vector<Edge> edges2{
         Edge{ 0, 1 },
         Edge{ 0, 2 },
         Edge{ 0, 3 },
         Edge{ 2, 5 },
         Edge{ 3, 6 },
         Edge{ 3, 4 },
         Edge{ 4, 7 },
         Edge{ 6, 7 },
         Edge{ 7, 5 },
         Edge{ 5, 1 },
         Edge{ 4, 1 },
         Edge{ 2, 6 },
};


Point2D projection2(Point3D point)
{
    return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 + (point.y * FL) / (FL + point.z) * 100 };
}

Point3D rotateOnX(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = point.x + 0 * point.y + 0 * point.z;
    rotatedPoint.y = 0 * point.x + cos(rotation) * point.y - sin(rotation) * point.z;
    rotatedPoint.z = 0 * point.x + sin(rotation) * point.y + cos(rotation) * point.z;
    return rotatedPoint;
}

Point3D rotateOnY(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = cos(rotation) * point.x + sin(rotation) * point.z;
    rotatedPoint.y = point.y;
    rotatedPoint.z = -sin(rotation) * point.x + cos(rotation) * point.z;
    return rotatedPoint;
}

Point3D scaleOut(Point3D point)
{
    Point3D scaledPoint;
    scaledPoint.x = point.x * scaledValue;
    scaledPoint.y = point.y * scaledValue;
    scaledPoint.z = point.z * scaledValue;
    return scaledPoint;
}

void render2(std::vector<Point3D> points, std::vector<Edge> edges)
{
    iterations++;
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    rotation = rotation + 1 * DeltaTime;

    for (auto& edge : edges) {
        Point3D rotatedPointStart = rotateOnX(points[edge.start]);
        Point3D rotatedPointEnd = rotateOnX(points[edge.end]);
        Point2D start = projection2(rotatedPointStart);
        Point2D end = projection2(rotatedPointEnd);

        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }

    SDL_RenderPresent(renderer);
    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}


//default C++ function
int main(int argc, char* argv[])
{
    iterations = 0;
    //creates a centered window with 960 width and 540 height
    window = SDL_CreateWindow("Cube Renderer No OOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);

    bool running = true;

    while (running)
    {
        //if the quit button is pressed, the loop breaks
        if (SDL_QuitRequested()) { running = false; break; }

        //Calls the function created in the Renderer3D class (render 3D is an instance of the class defined above)
        render2(points2, edges2);
    }

    return 0;
}
