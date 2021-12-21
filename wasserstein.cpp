#define _CRT_SECURE_NO_WARNINGS 1
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

template <typename T>
class Vector {
   public:
    T x, y, z;
    Vector() = default;
    Vector(T x, T y, T z) : x(x), y(y), z(z) {}
    Vector(const Vector<T> &v) : x(v.x), y(v.y), z(v.z) {}

    Vector<T> &operator=(const Vector<T> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    // Dot product
    auto dot(const Vector<T> &v) { return x * v.x + y * v.y + z * v.z; }

    void normalize() {
        auto norm = std::sqrt(dot(*this));
        x /= norm;
        y /= norm;
        z /= norm;
    }

    auto operator+(const Vector<T> &v) {
        return Vector<T>(x + v.x, y + v.y, z + v.z);
    }

    auto operator*(double r) {
        return Vector<T>(r * x, r * y, r * z);
    }

    auto &operator+=(const Vector<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
};

int main(int argc, char const *argv[]) {
    const char *filename1 = "../data/imgA.jpg";
    const char *filename2 = "../data/imgB.jpg";
    int iterations = 10;
    if (argc > 2) {
        filename1 = argv[1];
        filename2 = argv[2];
    }
    if (argc > 3)
        iterations = std::stoi(argv[3]);

    int x1, x2, y1, y2, n1, n2;
    unsigned char *data1 = stbi_load(filename1, &x1, &y1, &n1, 0);
    unsigned char *data2 = stbi_load(filename2, &x2, &y2, &n2, 0);
    if (data1 == NULL || data2 == NULL) {
        std::cerr << "Error loading the images\n";
        return 1;
    }

    std::cout << "number of channels f: " << n1 << "\n";
    std::cout << "number of channels g: " << n2 << "\n";

    auto imageF = std::vector<Vector<double>>(y1 * x1);
    for (int i = 0; i < imageF.size(); ++i) {
        auto imIdx = i * 3;
        imageF.at(i) = Vector(static_cast<double>(data1[imIdx]),
                              static_cast<double>(data1[imIdx + 1]),
                              static_cast<double>(data1[imIdx + 2]));
    }

    auto imageG = std::vector<Vector<double>>(y2 * x2);
    for (int i = 0; i < imageG.size(); ++i) {
        auto imIdx = i * 3;
        imageG.at(i) = Vector(static_cast<double>(data2[imIdx]),
                              static_cast<double>(data2[imIdx + 1]),
                              static_cast<double>(data2[imIdx + 2]));
    }

    // Algorithm
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{0, 1};

    for (int i = 0; i < iterations; i++) {
        // pick a line with random direction
        auto dir = Vector(d(gen), d(gen), d(gen));
        dir.normalize();

        // for each pixel of both images calculate it's projection on the line
        std::vector<std::pair<double, int>> proj1(imageF.size()), proj2(imageG.size());
        for (int j = 0; j < imageF.size(); ++j) {
            proj1[j] = {dir.dot(imageF[j]), j};
        }
        for (int j = 0; j < imageG.size(); ++j) {
            proj2[j] = {dir.dot(imageG[j]), j};
        }

        // sort projection arrays
        std::sort(proj1.begin(), proj1.end());
        std::sort(proj2.begin(), proj2.end());
        // update images
        for (int j = 0; j < imageF.size(); ++j) {
            imageF[proj1[j].second] += dir * (proj2[j].first - proj1[j].first);
        }
    }

    std::vector<unsigned char> out(imageF.size() * 3);
    for (int i = 0; i < imageF.size(); i++) {
        int imIdx = i * 3;
        out[imIdx] = std::clamp<unsigned char>(imageF[i].x, 0, 255);
        out[imIdx + 1] = std::clamp<unsigned char>(imageF[i].y, 0, 255);
        out[imIdx + 2] = std::clamp<unsigned char>(imageF[i].z, 0, 255);
    }

    stbi_write_png("out.png", x1, y1, 3, out.data(), 0);
    std::cout << "Finished. Check out out.png\n";

    stbi_image_free(data1);
    stbi_image_free(data2);

    return 0;
}