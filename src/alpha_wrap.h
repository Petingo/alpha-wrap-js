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
    AlphaWrap(double relativeAlpha, double relativeOffset):
        m_relativeAlpha(relativeAlpha), m_relativeOffset(relativeOffset) {};

    void setAlpha(const double alpha) { m_relativeAlpha = alpha; };
    void setOffset(const double offset) { m_relativeOffset = offset; }
    void addPoint(double x, double y, double z);
    void addSerializedPoints(char* serializedPoints);

    char* wrap();
    // char* wrap(char* serializedPoints);
    std::string serializeMesh(Mesh mesh);
    std::string serializePoints(std::vector<Point_3> points);
    std::vector<Point_3> deserializePoints(std::string serializedPoints);

private:
    std::vector<Point_3> m_vertices;
    double m_relativeAlpha {10.};
    double m_relativeOffset {300.};
    std::string m_serializedWrappedMesh;
};


#endif //ALPHA_WRAP_JS_ALPHA_WRAP_H
