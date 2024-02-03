//
// Created by Petingo on 2024/2/3.
//

#ifndef ALPHA_WRAP_JS_SERIALIZER_H
#define ALPHA_WRAP_JS_SERIALIZER_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point_3>;

class Serializer {
public:
    static std::string MeshToPly(Mesh mesh);
    static std::string SerializePoints(std::vector<Point_3> points);
    static std::vector<Point_3> DeserializePoints(std::string serializedPoints);
};


#endif //ALPHA_WRAP_JS_SERIALIZER_H
