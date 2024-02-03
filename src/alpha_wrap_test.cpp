#include <iostream>
#include <CGAL/IO/read_points.h>
#include "alpha_wrap.h"

int main() {
    AlphaWrap alphaWrap;

    // Read the input
    const std::string filename = CGAL::data_file_path("/home/wsl/alpha-wrap-js/misc/cube_sphere.ply");

    std::cout << "Reading " << filename << "..." << std::endl;
    std::vector<Point_3> points;
    if(!CGAL::IO::read_points(filename, std::back_inserter(points)) || points.empty())
    {
        std::cerr << "Invalid input." << std::endl;
        return EXIT_FAILURE;
    }

    auto serializedPoints = alphaWrap.serializePoints(points);

    // std::cout << serializedPoints << std::endl;

    for(auto p : points){
        alphaWrap.addPoint(p.x(), p.y(), p.z());
    }

    auto result = alphaWrap.wrap();

    //    std::cout << result << std::endl;

    return 0;
}