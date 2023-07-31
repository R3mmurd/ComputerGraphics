#include <ShadowMap.hpp>

ShadowMap::~ShadowMap()
{
    if (FBO_id)
    {
        glDeleteFramebuffers(1, &FBO_id);
        FBO_id = 0;
    }

    if (shadow_map_id)
    {
        glDeleteTextures(1, &shadow_map_id);
        shadow_map_id = 0;
    }
}

bool ShadowMap::init(GLuint w, GLuint h) noexcept
{
    width = w;
    height = h;

    glGenFramebuffers(1, &FBO_id);

    glGenTextures(1, &shadow_map_id);
    glBindTexture(GL_TEXTURE_2D, shadow_map_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_id, 0);

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

void ShadowMap::write() const noexcept
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_id);
}

void ShadowMap::read(GLenum texture_unit) noexcept
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, shadow_map_id);
}