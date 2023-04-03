#pragma once
// Методы освящения
#include "technique.h"
#include "math_3d.h"

// Определение направления света
struct DirectionLight
{
    Vector3f Color;
    float AmbientIntensity;
};

// Методы освящения
class LightingTechnique : public Technique
{
public:
    LightingTechnique();
    virtual bool Init(); // Создание шейдеров вызывает методы добавления шейдеров и линковки с проверкой

    void SetWVP(const Matrix4f* WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionLight& Light);

private:
    GLuint m_WVPLocation; // Система координат камеры
    GLuint m_samplerLocation; // Семплер для текстуры
    GLuint m_dirLightColorLocation; // Цвет освещения
    GLuint m_dirLightAmbientIntensityLocation; // Интенсивность
};