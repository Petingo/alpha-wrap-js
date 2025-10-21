//
// Created by Petingo on 2024/2/2.
//

#ifndef ALPHA_WRAP_JS_ALPHA_WRAP_H
#define ALPHA_WRAP_JS_ALPHA_WRAP_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>

#include "serializer.h"

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

    void clearPoints() { m_vertices.clear(); }
    void addPoint(double x, double y, double z) { m_vertices.emplace_back(x, y, z); };

    bool wrap();

    Mesh getWrappedMesh() { return m_wrappedMesh; };
    char* getWrappedMeshPly();

private:
    std::vector<Point_3> m_vertices;

    double m_relativeAlpha {10.};
    double m_relativeOffset {300.};

    Mesh m_wrappedMesh;
    std::string m_serializedWrappedMesh;
};


#endif //ALPHA_WRAP_JS_ALPHA_WRAP_H
