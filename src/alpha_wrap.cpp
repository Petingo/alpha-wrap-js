//
// Created by Petingo on 2024/2/2.
//

#include "alpha_wrap.h"

//char* AlphaWrap::wrap(char* serializedPoints) {
//    std::string serializedPointsStr(serializedPoints);
//    std::vector<Point_3> points = deserializePoints(serializedPointsStr);
//
//    CGAL::Bbox_3 bbox = CGAL::bbox_3(std::cbegin(points), std::cend(points));
//
//    const double diagLength = std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
//                                        CGAL::square(bbox.ymax() - bbox.ymin()) +
//                                        CGAL::square(bbox.zmax() - bbox.zmin()));
//    const double alpha = diagLength / m_relativeAlpha;
//    const double offset = diagLength / m_relativeOffset;
//
//    // Construct the wrap
//    Mesh wrappedMesh;
//    CGAL::alpha_wrap_3(points, alpha, offset, wrappedMesh);
//
//    m_serializedWrappedMesh = serializeMesh(wrappedMesh);
//
//    return m_serializedWrappedMesh.data();
//}

char* AlphaWrap::wrap() {
    CGAL::Bbox_3 bbox = CGAL::bbox_3(std::cbegin(m_vertices), std::cend(m_vertices));

    const double diagLength = std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
                                        CGAL::square(bbox.ymax() - bbox.ymin()) +
                                        CGAL::square(bbox.zmax() - bbox.zmin()));

    const double alpha = diagLength / m_relativeAlpha;
    const double offset = diagLength / m_relativeOffset;

    for(auto v : m_vertices){
        std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
    }

    // Construct the wrap
    Mesh wrappedMesh;
    CGAL::alpha_wrap_3(m_vertices, alpha, offset, wrappedMesh);

    m_serializedWrappedMesh = serializeMesh(wrappedMesh);

    return m_serializedWrappedMesh.data();
}

void AlphaWrap::addPoint(double x, double y, double z) {
    m_vertices.emplace_back(x, y, z);
}

std::vector<Point_3> AlphaWrap::deserializePoints(std::string serializedPoints) {
    std::vector<Point_3> points;
    std::istringstream iss(serializedPoints);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream iss2(line);
        double x, y, z;
        iss2 >> x >> y >> z;
        points.emplace_back(x, y, z);
    }
    return points;
}

std::string AlphaWrap::serializePoints(std::vector<Point_3> points) {
    std::ostringstream oss;
    for (auto &point : points) {
        oss << point.x() << " " << point.y() << " " << point.z() << std::endl;
    }
    return oss.str();
}

std::string AlphaWrap::serializeMesh(Mesh mesh) {
    std::ostringstream oss;
    oss << num_vertices(mesh) << std::endl;
    oss << num_faces(mesh) << std::endl;
    for (auto v : vertices(mesh)) {
        oss << mesh.point(v).x() << " " << mesh.point(v).y() << " " << mesh.point(v).z() << std::endl;
    }

    for (auto f : mesh.faces()) {
        CGAL::Vertex_around_face_circulator<Mesh> vcirc(mesh.halfedge(f), mesh), done(vcirc);
        do {
          oss << *vcirc++ << " ";
        } while (vcirc != done);
        oss << std::endl;
    }

    return oss.str();
}