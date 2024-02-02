//
// Created by Petingo on 2024/2/2.
//

#include "alpha_wrap.h"

char* AlphaWrap::wrap(char* serializedPoints) {
    std::string serializedPointsStr(serializedPoints);
    std::vector<Point_3> points = deserializePoints(serializedPointsStr);
    return serializedPoints;
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

std::string serializePoints(std::vector<Point_3> points) {
    std::ostringstream oss;
    for (auto &point : points) {
        oss << point.x() << "," << point.y() << "," << point.z() << std::endl;
    }
    return oss.str();
}
