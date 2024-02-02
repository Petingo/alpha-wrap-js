//
// Created by Petingo on 2024/2/2.
//

#ifndef ALPHA_WRAP_JS_ALPHA_WRAP_H
#define ALPHA_WRAP_JS_ALPHA_WRAP_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point_3>;

class AlphaWrap {
public:
    AlphaWrap() = default;
    char* wrap(char* serializedPoints);

private:
    std::vector<Point_3> deserializePoints(std::string serializedPoints);

    Mesh wrapMesh(std::vector<Point_3> points);

    std::string serializeMesh(Mesh wrap);

    std::string serializePoints(std::vector<Point_3> points);

private:
    double relativeAlpha {10.};
    double relativeOffset {300.};
//    std::vector<Point_3> m_points;
//    Mesh m_wrap;
};


#endif //ALPHA_WRAP_JS_ALPHA_WRAP_H
