#include <iostream>
#include "texture.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName = FileName;
    m_pImage = nullptr;
}

// �������� �����������
bool Texture::Load()
{
    try {
        // �������� ������ � ������� ��������� �� ������ ������, ���������� �����������
        // ������� �������: ��������� �������� � ������ � �������� ��������� �� � ������
        m_pImage = new Magick::Image(m_fileName);
        // ���������� ����������� � ������ Magick::Blob, ��������� ������ RGBA
        m_pImage->write(&m_blob, "RGBA");
    }
    // ������������ ������
    catch (Magick::Error& Error) {
        std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
        return false;
    }

    // ���������� ��������� ����� �������� ������� � �������� �� � ��������� �� ������ GLuint
    glGenTextures(1, &m_textureObj);
    // ��������� ������ ��������, ����������� �� ���� ������� ��������� � ����������,
    // ���� �� ������ �����, ����� ��������� ������� �������� � ����� ������ ��� 2D-��������
    // GL_TEXTURE_2D
    glBindTexture(m_textureTarget, m_textureObj);
    // �������� ������ ��������, ������� ������ ������ � ����������� �� ������� ��������
    // 1 - ������� ��������, 2 - ������� �����������, 3 - ���������� ������ (����� ����� �����������)
    // 4 � 5 - ������ � ������ �������� � ��������, 6 - �����
    // ��������� ��� ��������� - ��������� �������� �������� ������: ������, ��� � ����� ������
    // ������ ������ ��������������� �������� � BLOB, ��� - ��� ������ ������������ ������� ������
    glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    // ������� glTexParameterf ��������� ������� ��������� �������� ������� �������.
    // ��� ������� - ����� ��������� �������� ��������
    // ��������� ��������� �� �������, ���� ����������� ����� ������� ������� ��� ��������� ���
    // ������, ����������� ������ �������� ������������ ��� ����� �������
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

// ������� ��������� ������ �������� ��� �������� ���� enum (GL_TEXTURE0, GL_TEXTURE1 � �.�.).
// ��� ����� �� ������ �������� ����� glActiveTexture() � ����� ����������� ������ ������� � ������
void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}