#ifndef CG_ENGINE_MATERIAL_H
#define CG_ENGINE_MATERIAL_H

#include <CG_Engine/math/common.h>

struct Material {
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    float shininess;

    static Material emerald;
    static Material jade;
    static Material obsidian;
    static Material pearl;
    static Material ruby;
    static Material turquoise;
    static Material brass;
    static Material bronze;
    static Material chrome;
    static Material copper;
    static Material gold;
    static Material silver;
    static Material black_plastic;
    static Material cyan_plastic;
    static Material green_plastic;
    static Material red_plastic;
    static Material white_plastic;
    static Material yellow_plastic;
    static Material black_rubber;
    static Material cyan_rubber;
    static Material green_rubber;
    static Material red_rubber;
    static Material white_rubber;
    static Material yellow_rubber;

    static Material mix(Material m1, Material m2, float mix = 0.5f);
};

Material Material::emerald = {Vec3(0.0215, 0.1745, 0.0215), Vec3(0.07568, 0.61424, 0.07568),
                              Vec3(0.633, 0.727811, 0.633), 0.6};
Material Material::jade = {Vec3(0.135, 0.2225, 0.1575), Vec3(0.54, 0.89, 0.63),
                           Vec3(0.316228, 0.316228, 0.316228), 0.1};
Material Material::obsidian = {Vec3(0.05375, 0.05, 0.06625), Vec3(0.18275, 0.17, 0.22525),
                               Vec3(0.332741, 0.328634, 0.346435), 0.3};
Material Material::pearl = {Vec3(0.25, 0.20725, 0.20725), Vec3(1, 0.829, 0.829),
                            Vec3(0.296648, 0.296648, 0.296648), 0.088};
Material Material::ruby = {Vec3(0.1745, 0.01175, 0.01175), Vec3(0.61424, 0.04136, 0.04136),
                           Vec3(0.727811, 0.626959, 0.626959), 0.6};
Material Material::turquoise = {Vec3(0.1, 0.18725, 0.1745), Vec3(0.396, 0.74151, 0.69102),
                                Vec3(0.297254, 0.30829, 0.306678), 0.1};
Material Material::brass = {Vec3(0.329412, 0.223529, 0.027451), Vec3(0.780392, 0.568627, 0.113725),
                            Vec3(0.992157, 0.941176, 0.807843), 0.21794872};
Material Material::bronze = {Vec3(0.2125, 0.1275, 0.054), Vec3(0.714, 0.4284, 0.18144),
                             Vec3(0.393548, 0.271906, 0.166721), 0.2};
Material Material::chrome = {Vec3(0.25, 0.25, 0.25), Vec3(0.4, 0.4, 0.4),
                             Vec3(0.774597, 0.774597, 0.774597), 0.6};
Material Material::copper = {Vec3(0.19125, 0.0735, 0.0225), Vec3(0.7038, 0.27048, 0.0828),
                             Vec3(0.256777, 0.137622, 0.086014), 0.1};
Material Material::gold = {Vec3(0.24725, 0.1995, 0.0745), Vec3(0.75164, 0.60648, 0.22648),
                           Vec3(0.628281, 0.555802, 0.366065), 0.4};
Material Material::silver = {Vec3(0.19225, 0.19225, 0.19225), Vec3(0.50754, 0.50754, 0.50754),
                             Vec3(0.508273, 0.508273, 0.508273), 0.4};
Material Material::black_plastic = {Vec3(0.0, 0.0, 0.0), Vec3(0.01, 0.01, 0.01), Vec3(0.50, 0.50, 0.50),
                                    .25};
Material Material::cyan_plastic = {Vec3(0.0, 0.1, 0.06), Vec3(0.0, 0.50980392, 0.50980392),
                                   Vec3(0.50196078, 0.50196078, 0.50196078), .25};
Material Material::green_plastic = {Vec3(0.0, 0.0, 0.0), Vec3(0.1, 0.35, 0.1), Vec3(0.45, 0.55, 0.45),
                                    .25};
Material Material::red_plastic = {Vec3(0.0, 0.0, 0.0), Vec3(0.5, 0.0, 0.0), Vec3(0.7, 0.6, 0.6), .25};
Material Material::white_plastic = {Vec3(0.0, 0.0, 0.0), Vec3(0.55, 0.55, 0.55), Vec3(0.70, 0.70, 0.70),
                                    .25};
Material Material::yellow_plastic = {Vec3(0.0, 0.0, 0.0), Vec3(0.5, 0.5, 0.0), Vec3(0.60, 0.60, 0.50),
                                     .25};
Material Material::black_rubber = {Vec3(0.02, 0.02, 0.02), Vec3(0.01, 0.01, 0.01), Vec3(0.4, 0.4, 0.4),
                                   .078125};
Material Material::cyan_rubber = {Vec3(0.0, 0.05, 0.05), Vec3(0.4, 0.5, 0.5), Vec3(0.04, 0.7, 0.7),
                                  .078125};
Material Material::green_rubber = {Vec3(0.0, 0.05, 0.0), Vec3(0.4, 0.5, 0.4), Vec3(0.04, 0.7, 0.04),
                                   .078125};
Material Material::red_rubber = {Vec3(0.05, 0.0, 0.0), Vec3(0.5, 0.4, 0.4), Vec3(0.7, 0.04, 0.04),
                                 .078125};
Material Material::white_rubber = {Vec3(0.05, 0.05, 0.05), Vec3(0.5, 0.5, 0.5), Vec3(0.7, 0.7, 0.7),
                                   .078125};
Material Material::yellow_rubber = {Vec3(0.05, 0.05, 0.0), Vec3(0.5, 0.5, 0.4), Vec3(0.7, 0.7, 0.04),
                                    .078125};

Material Material::mix(Material m1, Material m2, float mix) {
    return {
            m1.ambient * mix + m2.ambient * (1 - mix),
            m1.diffuse * mix + m2.diffuse * (1 - mix),
            m1.specular * mix + m2.specular * (1 - mix),
            m1.shininess * mix + m2.shininess * (1 - mix)
    };
}

#endif //CG_ENGINE_MATERIAL_H
