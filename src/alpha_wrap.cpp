//
// Created by Petingo on 2024/2/2.
//

#include "alpha_wrap.h"

bool AlphaWrap::wrap() {
    CGAL::Bbox_3 bbox = CGAL::bbox_3(std::cbegin(m_vertices), std::cend(m_vertices));

    const double diagLength = std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
                                        CGAL::square(bbox.ymax() - bbox.ymin()) +
                                        CGAL::square(bbox.zmax() - bbox.zmin()));

    const double alpha = diagLength / m_relativeAlpha;
    const double offset = diagLength / m_relativeOffset;

    // Construct the wrap
    CGAL::alpha_wrap_3(m_vertices, alpha, offset, m_wrappedMesh);

    return true;
}

char* AlphaWrap::getWrappedMeshPly(){
    m_serializedWrappedMesh = Serializer::MeshToPly(m_wrappedMesh);
    return m_serializedWrappedMesh.data();
}