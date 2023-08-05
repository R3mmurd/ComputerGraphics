#pragma once

#include <ShadowMap.hpp>

class OmnidirectionalShadowMap : public ShadowMap
{
public:
    static constexpr size_t NUM_FACES{6};

    OmnidirectionalShadowMap() noexcept;

    ~OmnidirectionalShadowMap();

    bool init(GLuint w, GLuint h) noexcept override;
    
    void read(GLenum texture_unit) noexcept override;
};