#ifndef CG_ENGINE_BOUNDS_H
#define CG_ENGINE_BOUNDS_H
// T

#include <CG_Engine_Core/math/vec3.h>
#include "CG_Engine_Core/physics/rigidbody.h"

namespace Octree {
    class node;
}

/*
    enum for possible Bound Types
*/

enum class BoundTypes : unsigned char {
    AABB = 0x00,    // 0x00 = 0	// Axis-aligned bounding box
    SPHERE = 0x01    // 0x01 = 1
};

/*
    class to represent bounding region
*/

class BoundingRegion {
public:
    // type of region
    BoundTypes type;

    // pointer for quick access to instance
    RigidBody *instance;

    // pointer for quick access to current octree node
    Octree::node *cell;

    // sphere values
    Vec3 center;
    float radius;

    Vec3 ogCenter;
    float ogRadius;

    // bounding box values
    Vec3 min;
    Vec3 max;

    Vec3 ogMin;
    Vec3 ogMax;

    /*
        Constructors
    */

    // initialize with type
    BoundingRegion(BoundTypes type = BoundTypes::AABB) : type(type) {};

    // initialize as sphere
    BoundingRegion(Vec3 center, float radius) : type(BoundTypes::SPHERE), center(center), ogCenter(center),
                                                radius(radius), ogRadius(radius) {};

    // initialize as AABB
    BoundingRegion(Vec3 min, Vec3 max) : type(BoundTypes::AABB), min(min), ogMin(min), max(max), ogMax(max) {};

    /*
        Calculating values for the region
    */

    // transform for instance
    void transform() {
        if (instance) {
            if (type == BoundTypes::AABB) {
                min = ogMin * instance->size + instance->pos;
                max = ogMax * instance->size + instance->pos;
            } else {
                center = ogCenter * instance->size + instance->pos;
                radius = ogRadius * instance->size.x;
            }
        }
    };

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
                   (pt.z >= min.z) && (pt.z <= max.z);
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
            float distSquared = 0.0f;
            for (int i = 0; i < 3; i++) {
                // determine closest side
                float closestPt = std::max(br.min[i], std::min(center[i], br.max[i]));
                // add squared distance
                distSquared += (closestPt - center[i]) * (closestPt - center[i]);
            }

            return distSquared < (radius * radius);
        } else {
            // this is a box, br is a sphere
            // call algorithm for br (defined in preceding else if block)
            return br.intersectsWith(*this);
        }
    };

    // operator overload
    bool operator==(BoundingRegion br) {
        if (type != br.type) {
            return false;
        }

        if (type == BoundTypes::AABB) {
            return min == br.min && max == br.max;
        } else {
            return center == br.center && radius == br.radius;
        }
    };
};

#endif //CG_ENGINE_BOUNDS_H
