#include "MDLModel.h"
#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>

namespace Revolt {

MDLModel::MDLModel() : m_currentSkin(0) {
    InitializeNormals();
}

MDLModel::~MDLModel() {
    // Clean up OpenGL textures
    for (unsigned int texID : m_textureIDs) {
        glDeleteTextures(1, &texID);
    }
}

bool MDLModel::LoadFromFile(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        std::cerr << "Failed to open MDL file: " << filename << std::endl;
        return false;
    }
    
    // Read all sections
    bool success = ReadHeader(fp) && 
                  ReadSkins(fp) && 
                  ReadTexCoords(fp) && 
                  ReadTriangles(fp) && 
                  ReadFrames(fp);
    
    fclose(fp);
    
    if (success) {
        std::cout << "Loaded MDL model: " << filename << std::endl;
        std::cout << "  Vertices: " << m_header.numVerts << std::endl;
        std::cout << "  Triangles: " << m_header.numTris << std::endl;
        std::cout << "  Frames: " << m_header.numFrames << std::endl;
        std::cout << "  Skins: " << m_header.numSkins << std::endl;
    }
    
    return success;
}

bool MDLModel::ReadHeader(FILE* fp) {
    if (fread(&m_header, sizeof(MDLHeader), 1, fp) != 1) {
        return false;
    }
    
    // Check magic number and version
    if (m_header.ident != 1330660425) { // "IDPO"
        std::cerr << "Invalid MDL file (bad magic number)" << std::endl;
        return false;
    }
    
    if (m_header.version != 6) {
        std::cerr << "Unsupported MDL version: " << m_header.version << std::endl;
        return false;
    }
    
    return true;
}

bool MDLModel::ReadSkins(FILE* fp) {
    m_skins.resize(m_header.numSkins);
    m_textureIDs.resize(m_header.numSkins);
    
    for (int i = 0; i < m_header.numSkins; ++i) {
        MDLSkin& skin = m_skins[i];
        
        // Read skin group
        if (fread(&skin.group, sizeof(int32_t), 1, fp) != 1) {
            return false;
        }
        
        // For now, we only handle single skins (not groups)
        if (skin.group != 0) {
            std::cerr << "Group skins not supported yet" << std::endl;
            return false;
        }
        
        // Read skin data
        int skinSize = m_header.skinWidth * m_header.skinHeight;
        skin.data.resize(skinSize);
        
        if (fread(skin.data.data(), sizeof(uint8_t), skinSize, fp) != skinSize) {
            return false;
        }
        
        // Create OpenGL texture
        m_textureIDs[i] = CreateTextureFromSkin(skin);
    }
    
    return true;
}

bool MDLModel::ReadTexCoords(FILE* fp) {
    m_texCoords.resize(m_header.numVerts);
    return fread(m_texCoords.data(), sizeof(MDLTexCoord), m_header.numVerts, fp) == m_header.numVerts;
}

bool MDLModel::ReadTriangles(FILE* fp) {
    m_triangles.resize(m_header.numTris);
    return fread(m_triangles.data(), sizeof(MDLTriangle), m_header.numTris, fp) == m_header.numTris;
}

bool MDLModel::ReadFrames(FILE* fp) {
    m_frames.resize(m_header.numFrames);
    
    for (int i = 0; i < m_header.numFrames; ++i) {
        MDLFrame& frame = m_frames[i];
        
        // Read frame type
        if (fread(&frame.type, sizeof(int32_t), 1, fp) != 1) {
            return false;
        }
        
        // For now, we only handle simple frames
        if (frame.type != 0) {
            std::cerr << "Group frames not supported yet" << std::endl;
            return false;
        }
        
        // Read bounding box
        if (fread(&frame.frame.bboxMin, sizeof(MDLVertex), 1, fp) != 1 ||
            fread(&frame.frame.bboxMax, sizeof(MDLVertex), 1, fp) != 1) {
            return false;
        }
        
        // Read frame name
        if (fread(frame.frame.name, sizeof(char), 16, fp) != 16) {
            return false;
        }
        
        // Read vertices
        frame.frame.vertices.resize(m_header.numVerts);
        if (fread(frame.frame.vertices.data(), sizeof(MDLVertex), m_header.numVerts, fp) != m_header.numVerts) {
            return false;
        }
    }
    
    return true;
}

void MDLModel::ConvertVertex(const MDLVertex& vertex, float result[3]) const {
    for (int i = 0; i < 3; ++i) {
        result[i] = (m_header.scale[i] * vertex.v[i]) + m_header.translate[i];
    }
}

void MDLModel::Render(int frame) {
    if (frame < 0 || frame >= static_cast<int>(m_frames.size())) {
        frame = 0;
    }
    
    // Проверяем, что у нас есть данные для рендеринга
    if (m_frames.empty() || m_triangles.empty() || m_texCoords.empty()) {
        return;
    }
    
    const MDLFrame& mdlFrame = m_frames[frame];
    
    // Bind texture
    if (!m_textureIDs.empty() && m_currentSkin >= 0 && m_currentSkin < static_cast<int>(m_textureIDs.size())) {
        glBindTexture(GL_TEXTURE_2D, m_textureIDs[m_currentSkin]);
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    
    // Render triangles
    glBegin(GL_TRIANGLES);
    
    for (const MDLTriangle& tri : m_triangles) {
        for (int j = 0; j < 3; ++j) {
            int vertexIndex = tri.vertices[j];
            if (vertexIndex < 0 || vertexIndex >= static_cast<int>(mdlFrame.frame.vertices.size()) ||
                vertexIndex >= static_cast<int>(m_texCoords.size())) {
                continue;
            }
            
            const MDLVertex& vertex = mdlFrame.frame.vertices[vertexIndex];
            const MDLTexCoord& texCoord = m_texCoords[vertexIndex];
            
            // Calculate texture coordinates
            float s = static_cast<float>(texCoord.s);
            float t = static_cast<float>(texCoord.t);
            
            // Adjust for backface if needed
            if (!tri.facesFront && texCoord.onSeam) {
                s += m_header.skinWidth * 0.5f;
            }
            
            // Normalize texture coordinates
            s = (s + 0.5f) / m_header.skinWidth;
            t = (t + 0.5f) / m_header.skinHeight;
            
            glTexCoord2f(s, t);
            
            // Set normal
            int normalIndex = vertex.normalIndex;
            if (normalIndex >= 0 && normalIndex < NUM_NORMALS) {
                glNormal3fv(m_normals[normalIndex]);
            }
            
            // Calculate and set vertex position
            float pos[3];
            ConvertVertex(vertex, pos);
            glVertex3fv(pos);
        }
    }
    
    glEnd();
    
    // Отключаем текстурирование
    glDisable(GL_TEXTURE_2D);
}

unsigned int MDLModel::CreateTextureFromSkin(const MDLSkin& skin) {
    // Базовая палитра grayscale для тестирования
    unsigned char palette[256][3];
    for (int i = 0; i < 256; ++i) {
        palette[i][0] = static_cast<unsigned char>(i);  // ИСПРАВЛЕНО
        palette[i][1] = static_cast<unsigned char>(i);  // ИСПРАВЛЕНО
        palette[i][2] = static_cast<unsigned char>(i);  // ИСПРАВЛЕНО
    }
    
    // Convert 8-bit indexed to RGB
    std::vector<unsigned char> rgbData(m_header.skinWidth * m_header.skinHeight * 3);
    for (int i = 0; i < m_header.skinWidth * m_header.skinHeight; ++i) {
        int paletteIndex = skin.data[i];
        rgbData[i * 3 + 0] = palette[paletteIndex][0];
        rgbData[i * 3 + 1] = palette[paletteIndex][1];
        rgbData[i * 3 + 2] = palette[paletteIndex][2];
    }
    
    // Create OpenGL texture
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_header.skinWidth, m_header.skinHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, rgbData.data());
    
    return textureID;
}

void MDLModel::InitializeNormals() {
    // Полная таблица нормалей (первые 20 из 162 нормалей для примера)
    float tempNormals[][3] = {
        {-0.525731f, 0.000000f, 0.850651f},
        {-0.442863f, 0.238856f, 0.864188f},
        {-0.295242f, 0.000000f, 0.955423f},
        {-0.309017f, 0.500000f, 0.809017f},
        {-0.162460f, 0.262866f, 0.951056f},
        {0.000000f, 0.000000f, 1.000000f},
        {0.000000f, 0.850651f, 0.525731f},
        {-0.147621f, 0.716567f, 0.681718f},
        {0.147621f, 0.716567f, 0.681718f},
        {0.000000f, 0.525731f, 0.850651f},
        {0.309017f, 0.500000f, 0.809017f},
        {0.525731f, 0.000000f, 0.850651f},
        {0.295242f, 0.000000f, 0.955423f},
        {0.442863f, 0.238856f, 0.864188f},
        {0.162460f, 0.262866f, 0.951056f},
        {-0.681718f, 0.147621f, 0.716567f},
        {-0.809017f, 0.309017f, 0.500000f},
        {-0.587785f, 0.425325f, 0.688191f},
        {-0.850651f, 0.525731f, 0.000000f},
        {-0.864188f, 0.442863f, 0.238856f}
    };
    
    const int numProvidedNormals = sizeof(tempNormals) / sizeof(tempNormals[0]);
    
    // Копируем нормали в массив класса
    for (int i = 0; i < NUM_NORMALS && i < numProvidedNormals; ++i) {
        m_normals[i][0] = tempNormals[i][0];
        m_normals[i][1] = tempNormals[i][1];
        m_normals[i][2] = tempNormals[i][2];
    }
    
    // Для остальных нормалей устанавливаем значения по умолчанию
    for (int i = numProvidedNormals; i < NUM_NORMALS; ++i) {
        m_normals[i][0] = 0.0f;
        m_normals[i][1] = 0.0f;
        m_normals[i][2] = 1.0f;
    }
}

} // namespace Revolt