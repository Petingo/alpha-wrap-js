//
// Created by Petingo on 2024/2/3.
//

#include "serializer.h"

std::vector<Point_3> Serializer::DeserializePoints(std::string serializedPoints) {
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

std::string Serializer::SerializePoints(std::vector<Point_3> points) {
    std::ostringstream oss;
    for (auto &point : points) {
        oss << point.x() << " " << point.y() << " " << point.z() << std::endl;
    }
    return oss.str();
}

std::string Serializer::MeshToPly(Mesh mesh) {
    std::ostringstream oss;
    oss << "ply" << std::endl;
    oss << "format ascii 1.0" << std::endl;
    oss << "comment author: Petingo" << std::endl;

    oss << "element vertex " << num_vertices(mesh) << std::endl;
    oss << "property float x\n" <<
        "property float y\n" <<
        "property float z"   << std::endl;
    oss << "element face " << num_faces(mesh) << std::endl;
    oss << "property list uchar int vertex_index" << std::endl;
    oss << "end_header" << std::endl;

    for (auto v : vertices(mesh)) {
        oss << mesh.point(v).x() << " " << mesh.point(v).y() << " " << mesh.point(v).z() << std::endl;
    }

    for (auto f : mesh.faces()) {
        oss << "3 ";
        CGAL::Vertex_around_face_circulator<Mesh> vcirc(mesh.halfedge(f), mesh), done(vcirc);
        do {
            oss << (*vcirc).id() << " ";
            vcirc++;
        } while (vcirc != done);
        oss << std::endl;
    }

    return oss.str();
}