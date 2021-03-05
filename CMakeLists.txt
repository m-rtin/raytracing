cmake_minimum_required(VERSION 3.17)
project(helloWorld)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


add_executable(helloWorld main.cpp stb_image_write.h stb_image.h Models/Vector.cpp Models/Vector.h Models/Ray.cpp Models/Ray.h Models/Sphere.cpp Models/Sphere.h Models/Scene.cpp Models/Scene.h Models/TriangleIndices.h Models/Object.cpp Models/Object.h Models/BoundingBox.cpp Models/BoundingBox.h Models/TriangleMesh.cpp Models/TriangleMesh.h Models/Node.cpp Models/Node.h)