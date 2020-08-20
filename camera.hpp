#ifndef CAMERA_H
#define CAMERA_H

#include "constraints.hpp"

class camera {
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio
            ) 
        {
            // field of view calculation to get the height and width of the viewport
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            // creates the uv plane with the w vector being the normal to the plan in the direction we are looking
            auto w = unit_vector(lookfrom - lookat);
            // note vup is in the same plane as uv
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            // orients the camera to view the frame from different angles
            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;
        }

        // the rays are still coming from the origin
        ray get_ray(double s, double t) const {
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};
#endif