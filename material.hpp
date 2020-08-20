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

// approximation used to model the fact that real glass has reflectivity based on angle
double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

// creates glass-like objects that refract rays
// NOTE: you can create a hollow sphere by having a negative radius
class dielectric : public material {
    public:
        dielectric(double ri) : ref_idx(ri) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            //attenuation is always 1 since the glass surface is not absorbing anything
            attenuation = colour(1.0, 1.0, 1.0);
            // ratio of refractive indexed of air and material
            double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;

            // unit direction of incident ray
            vec3 unit_direction = unit_vector(r_in.direction());

            // if total internal reflection occurs, then we want to reflect instead of refract
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            if (etai_over_etat * sin_theta > 1.0 ) {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected);
                return true;
            }

            // create a reflection based on the schlick approximation
            double reflect_prob = schlick(cos_theta, etai_over_etat);
            if (random_double() < reflect_prob)
            {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected);
                return true;
            }

            // calculate the refracted scatter rays
            vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
            scattered = ray(rec.p, refracted);
            return true;
        }

        double ref_idx;
};

#endif