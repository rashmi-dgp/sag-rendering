#include "mesh_factory.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "nanosvg.h"
#include "config.h"
MTL::Buffer* MeshFactory::buildTriangle(MTL::Device* device) {
    // Declare the data to send
    Vertex vertices[3] = {
        {{-0.75, -0.75}, {1.0, 1.0, 0.0}},
        {{ 0.75, -0.75}, {1.0, 1.0, 0.0}},
        {{  0.0,  0.75}, {1.0, 1.0, 0.0}}
    };

    // Create a buffer to hold it
    MTL::Buffer* buffer = device->newBuffer(3 * sizeof(Vertex), MTL::ResourceStorageModeShared);

    // Upload to buffer
    memcpy(buffer->contents(), vertices, 3 * sizeof(Vertex));

    return buffer;
}

Mesh MeshFactory::buildQuad(MTL::Device* device) {
    Mesh mesh;

    // Declare the data to send
    Vertex vertices[4] = {
        {{-0.75, -0.75}, {0.0, 0.0, 0.0}},
        {{ 0.75, -0.75}, {0.0, 0.0, 0.0}},
        {{ 0.75,  0.75}, {0.0, 0.0, 0.0}},
        {{-0.75,  0.75}, {0.0, 0.0, 0.0}},
    };

    ushort indices[] = {
        0, 1,  // Bottom line
        1, 2,  // Right line
        2, 3,  // Top line
        3, 0   // Left line
    };

    // Vertex buffer
    mesh.vertexBuffer = device->newBuffer(4 * sizeof(Vertex), MTL::ResourceStorageModeShared);
    memcpy(mesh.vertexBuffer->contents(), vertices, 4 * sizeof(Vertex));

    // Index buffer
    mesh.indexBuffer = device->newBuffer(8 * sizeof(ushort), MTL::ResourceStorageModeShared);
    memcpy(mesh.indexBuffer->contents(), indices, 8 * sizeof(ushort));

    return mesh;
}

//Mesh MeshFactory::buildSVG(MTL::Device* device, const char* svgFilePath) {
//    Mesh mesh;
//    // Load SVG
//    NSVGimage* image = nsvgParseFromFile(svgFilePath, "px", 96);
//    if (!image) {
//        std::cerr << "Could not open SVG image." << std::endl;
//        return mesh;
//    }
//
//    std::vector<Vertex> vertices;
//    std::vector<ushort> indices;
//
//    ushort index = 0;
//    std::ofstream outFile("cubic_bezier_shapes.txt");
//    if (!outFile.is_open()) {
//        std::cerr << "Could not open output file." << std::endl;
//        nsvgDelete(image);
//        return mesh;
//    }
//
//    for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next) {
//        for (NSVGpath* path = shape->paths; path != nullptr; path = path->next) {
//            for (int i = 0; i < path->npts - 1; i += 3) {
//                float* p = &path->pts[i * 2];
//                outFile << "Shape ID: " << (shape->id ? shape->id : "none") << ", Path Points: ";
//                for (int j = 0; j < 4; ++j) {
//                    outFile << "[" << p[j * 2] << ", " << p[j * 2 + 1] << "]";
//                    if (j < 3) outFile << ", ";
//                }
//                outFile << std::endl;
//
//                for (int j = 0; j < 4; ++j) {
//                    vertices.push_back({{p[j * 2], p[j * 2 + 1]}, {1.0, 0.0, 0.0}});
//                    indices.push_back(index++);
//                }
//            }
//        }
//    }
//
//    outFile.close();
//
//    mesh.vertexBuffer = device->newBuffer(vertices.size() * sizeof(Vertex), MTL::ResourceStorageModeShared);
//    memcpy(mesh.vertexBuffer->contents(), vertices.data(), vertices.size() * sizeof(Vertex));
//
//    mesh.indexBuffer = device->newBuffer(indices.size() * sizeof(ushort), MTL::ResourceStorageModeShared);
//    memcpy(mesh.indexBuffer->contents(), indices.data(), indices.size() * sizeof(ushort));
//
//    nsvgDelete(image);
//
//    return mesh;
//}

Mesh MeshFactory::buildSVG(MTL::Device* device, const char* svgFilePath) {
    Mesh mesh;

    // Load SVG
    NSVGimage* image = nsvgParseFromFile(svgFilePath, "px", 96);
    if (!image) {
        std::cerr << "Could not open SVG image." << std::endl;
        return mesh;
    }
//diff from commented part, yaha vertex struct of simd is used
    std::vector<Vertex> vertices;
    std::vector<ushort> indices;

    ushort index = 0;
    for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next) {
        for (NSVGpath* path = shape->paths; path != nullptr; path = path->next) {
            for (int i = 0; i < path->npts - 1; i += 3) {
                float* p = &path->pts[i * 2];

                for (int j = 0; j < 4; ++j) {
                    vertices.push_back({{p[j * 2] / 500.0f - 1.0f, p[j * 2 + 1] / 500.0f - 1.0f}, {1.0, 0.0, 0.0}});
                    indices.push_back(index++);
                }
            }
        }
    }

    mesh.vertexBuffer = device->newBuffer(vertices.size() * sizeof(Vertex), MTL::ResourceStorageModeShared);
    memcpy(mesh.vertexBuffer->contents(), vertices.data(), vertices.size() * sizeof(Vertex));

    mesh.indexBuffer = device->newBuffer(indices.size() * sizeof(ushort), MTL::ResourceStorageModeShared);
    memcpy(mesh.indexBuffer->contents(), indices.data(), indices.size() * sizeof(ushort));

    nsvgDelete(image);

    return mesh;
}
Mesh MeshFactory::buildLine(MTL::Device* device) {
    Mesh mesh;

    Vertex vertices[2] = {
        {{-0.75, 0.0}, {0.0, 0.0, 0.0}}, // Start point
        {{0.75, 0.0}, {0.0, 0.0, 0.0}}  // End point
    };

    ushort indices[] = { 0, 1 };

    // Vertex buffer
    mesh.vertexBuffer = device->newBuffer(2 * sizeof(Vertex), MTL::ResourceStorageModeShared);
    memcpy(mesh.vertexBuffer->contents(), vertices, 2 * sizeof(Vertex));

    // Index buffer
    mesh.indexBuffer = device->newBuffer(2 * sizeof(ushort), MTL::ResourceStorageModeShared);
    memcpy(mesh.indexBuffer->contents(), indices, 2 * sizeof(ushort));

    return mesh;
}
