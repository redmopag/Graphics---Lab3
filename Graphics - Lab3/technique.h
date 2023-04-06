#pragma once
// Класс, ответсвенный за линковку и компиляцию шейдеров
#include <GL/glew.h>
#include <list>

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class Technique
{
public:
    // Создаёт объект без программ шейдеров
    Technique();
    ~Technique(); // Деструктор
    virtual bool Init(); // Создание шейдера, false, если нет кода для шейдера
    void Enable(); // Назначаем программу шейдеров для конвейера

protected:
    // Используем этот метод для добавления шейдеров в программу.
    // Когда заканчиваем - вызываем finalize()
    bool AddShader(GLenum ShaderType, const char* pShaderText);
    // После добавления всех шейдеров в программу вызываем эту функцию
    // для линковки и проверки программу на ошибки
    bool Finalize();
    // Запрашиваем позицию uniform-переменной
    GLint GetUniformLocation(const char* pUniformName);

private:
    GLuint m_shaderProg; // Программа шейдера
    typedef std::list<GLuint> ShaderObjList; // Обычный typedef
    ShaderObjList m_shaderObjList; // Список программ шейдера
};
