#ifndef MATERIAL_H
#define MATERIAL_H

#include "constraints.hpp"
#include "hittable.hpp"

//struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const = 0;
};

// created dull materials with lambertian scattering (random dispersion)
class lambertian : public material {
    public:
        lambertian(const colour& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        colour albedo;
};

// creates shiny objects that reflect rays symmetrically
class metal : public material {
    public:
        metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        colour albedo;
        double fuzz;
};

#endif