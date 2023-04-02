#pragma once
#include <string>

#include <GL/glew.h>
#include <Magick++.h>

class Texture
{
public:
    // Позиция текстуры (её типа: 1D, 2D, 3D) и имя файла
    Texture(GLenum TextureTarget, const std::string& FileName);

    // Загрузка файла
    bool Load();

    // Привязка к текстурному модулю
    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName; // Имя файла
    GLenum m_textureTarget; // Грубо говоря мерность текстуры
    GLuint m_textureObj; // Данные о самом изображении текстуры: тексели
    Magick::Image* m_pImage; // Указатель на изображение (указатель на точку начала данных в памяти)
    // большой бинарный объект - это полезный механизм для хранения
    // зашифрованного изображения в память так,
    // что оно может быть использовано сторонними программами
    Magick::Blob m_blob;
};
