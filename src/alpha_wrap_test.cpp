#include <iostream>
#include <fstream>
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

    std::cout << "Loaded " << points.size() << " points." << std::endl;

    for(auto p : points){
        alphaWrap.addPoint(p.x(), p.y(), p.z());
    }

    std::cout << "Start wrapping..." << std::endl;
    bool succeed = alphaWrap.wrap();

    if (!succeed) {
        std::cout << "Alpha wrap failed." << std::endl;
        return EXIT_FAILURE;
    }

    std::string outputFilename = "output.ply";
    std::cout << "Alpha wrap succeed. Writing result to " << outputFilename << std::endl;

    // Mesh wrappedMesh = alphaWrap.getWrappedMesh();
    // CGAL::IO::write_polygon_mesh(output_name, wrappedMesh, CGAL::parameters::stream_precision(17));

    // write serializedMeshPly to output file
    std::string serializedMeshPly = alphaWrap.getWrappedMeshPly();

    std::ofstream outputFile;
    outputFile.open(outputFilename);
    outputFile << serializedMeshPly;
    outputFile.close();

    return EXIT_SUCCESS;
}