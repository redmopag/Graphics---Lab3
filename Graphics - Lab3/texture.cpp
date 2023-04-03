#include <iostream>
#include "texture.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName = FileName;
    m_pImage = nullptr;
}

// Загрузка изображения
bool Texture::Load()
{
    try {
        // выделяем память и передаём указатель на начало данных, содержащих изображение
        // Другими словами: загрузака текстуры в память и передача указателя на её начало
        m_pImage = new Magick::Image(m_fileName);
        // записываем изображение в объект Magick::Blob, используя формат RGBA
        m_pImage->write(&m_blob, "RGBA");
    }
    // Обрабатываем ошибку
    catch (Magick::Error& Error) {
        std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
        return false;
    }

    // генерирует указанное число объектов текстур и помещает их в указатель на массив GLuint
    glGenTextures(1, &m_textureObj);
    // Указываем объект текстуры, относящийся ко всем вызовам связанным с текстурами,
    // пока не укажем новый, также указываем позицию текстуры в нашем случае это 2D-текстура
    // GL_TEXTURE_2D
    glBindTexture(m_textureTarget, m_textureObj);
    // Загрузка данных текстуры, функция бывает разной в зависимости от позиции текстуры
    // 1 - позиция текстуры, 2 - уровень детализации, 3 - внутренний формат (выбор цвета отображения)
    // 4 и 5 - ширина и высота текстуры в текселях, 6 - рамка
    // Последние три параметра - указывают источник входящих данных: формат, тип и адрес памяти
    // Формат должен соответствовать значению в BLOB, тип - вид данных относительно каждого канала
    glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    // функция glTexParameterf управляет многими аспектами операции выборки текстур.
    // Эти аспекты - часть состояния сэмплера текстуры
    // Подправка текстелей на пиксель, если иозбражение будет слишком большим или маленьким под
    // фигуру, использован фильтр линейной интерполяции для обоих случаев
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

// функция принимает модуль текстуры как параметр типа enum (GL_TEXTURE0, GL_TEXTURE1 и т.д.).
// Тем самым он станет активным через glActiveTexture() и затем привязываем объект текстур к модулю
void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}