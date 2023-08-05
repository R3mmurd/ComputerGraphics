#include <OmnidirectionalShadowMap.hpp>

OmnidirectionalShadowMap::OmnidirectionalShadowMap() noexcept
    : ShadowMap{}
{

}

OmnidirectionalShadowMap::~OmnidirectionalShadowMap()
{

}

bool OmnidirectionalShadowMap::init(GLuint w, GLuint h) noexcept
{
    width = w;
    height = h;

    glGenFramebuffers(1, &FBO_id);

    glGenTextures(1, &shadow_map_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map_id);

    for (size_t i = 0; i < NUM_FACES; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO_id);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_map_id, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_INIT_CERR();
        log(LOG_ERR) << "Framebuffer error: " << status << "\n";
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void OmnidirectionalShadowMap::read(GLenum texture_unit) noexcept
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map_id);
}