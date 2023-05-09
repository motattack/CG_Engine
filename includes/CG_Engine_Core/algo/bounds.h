#ifndef CG_ENGINE_BOUNDS_H
#define CG_ENGINE_BOUNDS_H

#include <CG_Engine_Core/math/vec3.h>

enum class BoundTypes {
    AABB = 0x00,    // 0x00 = 0	// Axis-aligned bounding box
    SPHERE = 0x01    // 0x01 = 1
};

class BoundingRegion {
public:
    BoundTypes type;

    // sphere values
    Vec3 center;
    float radius;

    // bounding box values
    Vec3 min;
    Vec3 max;

    /*
        Constructors
    */

    // initialize with type
    BoundingRegion(BoundTypes type) : type(type) {};

    // initialize as sphere
    BoundingRegion(Vec3 center, float radius) : type(BoundTypes::SPHERE), center(center), radius(radius) {};

    // initialize as AABB
    BoundingRegion(Vec3 min, Vec3 max) : type(BoundTypes::AABB), min(min), max(max) {};

    /*
        Calculating values for the region
    */

    // center
    Vec3 calculateCenter() {
        return (type == BoundTypes::AABB) ? (min + max) / 2.0f : center;
    };

    // calculate dimensions
    Vec3 calculateDimensions() {
        return (type == BoundTypes::AABB) ? (max - min) : Vec3(2.0f * radius);
    };

    /*
        testing methods
    */

    // determine if point inside
    bool containsPoint(Vec3 pt) {
        if (type == BoundTypes::AABB) {
            // box - point must be larger than man and smaller than max
            return (pt.x >= min.x) && (pt.x <= max.x) &&
                   (pt.y >= min.y) && (pt.y <= max.y) &&
                   (pt.z >= min.z) && (pt.z <= min.z);
        } else {
            // sphere - distance must be less than radius
            // x^2 + y^2 + z^2 <= r^2
            float distSquared = 0.0f;
            for (int i = 0; i < 3; i++) {
                distSquared += (center[i] - pt[i]) * (center[i] - pt[i]);
            }
            return distSquared <= (radius * radius);
        }
    };

    // determine if region completely inside
    bool containsRegion(BoundingRegion br) {
        if (br.type == BoundTypes::AABB) {
            // if br is a box, just has to contain min and max
            return containsPoint(br.min) && containsPoint(br.max);
        } else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
            // if both spheres, combination of distance from centers and br.radius is less than radius
            return (center - br.center).len() + br.radius < radius;
        } else {
            // if this is a box and br is a sphere

            if (!containsPoint(br.center)) {
                // center is outside of the box
                return false;
            }

            // center inside the box
            /*
                for each axis (x, y, z)
                - if distance to each side is smaller than the radius, return false
            */
            for (int i = 0; i < 3; i++) {
                if (abs(max[i] - br.center[i]) < br.radius ||
                    abs(br.center[i] - min[i]) < br.radius) {
                    return false;
                }
            }

            return true;
        }
    };

    // determine if region intersects (partial containment)
    bool intersectsWith(BoundingRegion br) {
        // overlap on all axes

        if (type == BoundTypes::AABB && br.type == BoundTypes::AABB) {
            // both boxes

            Vec3 rad = calculateDimensions() / 2.0f;                // "radius" of this box
            Vec3 radBr = br.calculateDimensions() / 2.0f;            // "radius" of br

            Vec3 center = calculateCenter();                        // center of this box
            Vec3 centerBr = br.calculateCenter();                    // center of br

            Vec3 dist = (center - centerBr).abs();

            for (int i = 0; i < 3; i++) {
                if (dist[i] > rad[i] + radBr[i]) {
                    // no overlap on this axis
                    return false;
                }
            }

            // failed to prove wrong on each axis
            return true;
        } else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
            // both spheres - distance between centers must be less than combined radius

            return (center - br.center).len() < (radius + br.radius);
        } else if (type == BoundTypes::SPHERE) {
            // this is a sphere, br is a box

            // determine if sphere is above top, below bottom, etc
            // find distance (squared) to the closest plane
            float distSquared = 0.0f;
            for (int i = 0; i < 3; i++) {
                if (center[i] < br.min[i]) {
                    // beyond min
                    distSquared += (br.min[i] - center[i]) * (br.min[i] * center[i]);
                } else if (center[i] > br.max[i]) {
                    // beyond max
                    distSquared += (center[i] - br.max[i]) * (center[i] - br.max[i]);
                }
                // else inside
            }

            return distSquared < (radius * radius);
        } else {
            // this is a box, br is a sphere
            // call algorithm for br (defined in preceding else if block)
            return br.intersectsWith(*this);
        }
    };
};

#endif //CG_ENGINE_BOUNDS_H
