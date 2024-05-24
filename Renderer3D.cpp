#include "Renderer3D.h"
#include <SDL2/SDL.h>

Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* renderer, std::vector<Point3D>& _points, std::vector<Edge>& _edges)
{
	SDL_GetWindowSize(_window, &WindowSizeX, &WindowSizeY);
	SDL_render = renderer;
	points = _points;
	edges = _edges;
}

void Renderer3D::render()
{
	auto time1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration(0);

	SDL_SetRenderDrawColor(SDL_render, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(SDL_render);
	SDL_SetRenderDrawColor(SDL_render, 255, 192, 203, SDL_ALPHA_OPAQUE);

	rotation += 1 * DeltaTime;

	for (auto& edge : edges) {
		Point3D rotatedPointStart = rotateOnX(points[edge.start]);
		Point3D rotatedPointEnd = rotateOnX(points[edge.end]);
		Point2D start = projection(rotatedPointStart);
		Point2D end = projection(rotatedPointEnd);

		SDL_RenderDrawLine(SDL_render, start.x, start.y, end.x, end.y);
	}

	SDL_RenderPresent(SDL_render);

	auto time2 = std::chrono::high_resolution_clock::now();
	duration = time2 - time1;
	DeltaTime = duration.count();
	time1 = time2;
}

Point2D Renderer3D::projection(Point3D point)
{
	return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 + (point.y * FL) / (FL + point.z) * 100 };
}

Point3D Renderer3D::rotateOnX(Point3D point)
{
	Point3D rotatedPoint;
	rotatedPoint.x = point.x;
	rotatedPoint.y = cos(rotation) * point.y - sin(rotation) * point.z;
	rotatedPoint.z = sin(rotation) * point.y + cos(rotation) * point.z;
	return rotatedPoint;
}

Point3D Renderer3D::rotateOnY(Point3D point)
{
	Point3D rotatedPoint;
	rotatedPoint.x = cos(rotation) * point.x + sin(rotation) * point.z;
	rotatedPoint.y = point.y;
	rotatedPoint.z = - sin(rotation) * point.x + cos(rotation) * point.z;
	return rotatedPoint;
}
