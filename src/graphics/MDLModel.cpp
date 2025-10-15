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
    
    // ВКЛЮЧАЕМ blending для прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Bind texture
    if (!m_textureIDs.empty() && m_currentSkin >= 0 && m_currentSkin < static_cast<int>(m_textureIDs.size())) {
        glBindTexture(GL_TEXTURE_2D, m_textureIDs[m_currentSkin]);
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_BLEND);
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
    unsigned char palette[256][3];
    LoadPalette(palette); // Используем палитру
    
    // Convert 8-bit indexed to RGBA
    std::vector<unsigned char> rgbData(m_header.skinWidth * m_header.skinHeight * 4);
    
    for (int i = 0; i < m_header.skinWidth * m_header.skinHeight; ++i) {
        int paletteIndex = skin.data[i];
        
        // Прозрачные пиксели (индекс 255)
        if (paletteIndex == 255) {
            rgbData[i * 4 + 0] = 0;
            rgbData[i * 4 + 1] = 0;
            rgbData[i * 4 + 2] = 0;
            rgbData[i * 4 + 3] = 0; // Полная прозрачность
        } else {
            rgbData[i * 4 + 0] = palette[paletteIndex][0];
            rgbData[i * 4 + 1] = palette[paletteIndex][1];
            rgbData[i * 4 + 2] = palette[paletteIndex][2];
            rgbData[i * 4 + 3] = 255; // Полная непрозрачность
        }
    }
    
    // Create OpenGL texture
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_header.skinWidth, m_header.skinHeight, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbData.data());
    
    return textureID;
}

void MDLModel::InitializeNormals() {
    // ПОЛНАЯ таблица нормалей из 162 значений
    float tempNormals[][3] = {
        {-0.525731f, 0.000000f, 0.850651f}, {-0.442863f, 0.238856f, 0.864188f},
        {-0.295242f, 0.000000f, 0.955423f}, {-0.309017f, 0.500000f, 0.809017f},
        {-0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f},
        {0.000000f, 0.850651f, 0.525731f}, {-0.147621f, 0.716567f, 0.681718f},
        {0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f},
        {0.309017f, 0.500000f, 0.809017f}, {0.525731f, 0.000000f, 0.850651f},
        {0.295242f, 0.000000f, 0.955423f}, {0.442863f, 0.238856f, 0.864188f},
        {0.162460f, 0.262866f, 0.951056f}, {-0.681718f, 0.147621f, 0.716567f},
        {-0.809017f, 0.309017f, 0.500000f}, {-0.587785f, 0.425325f, 0.688191f},
        {-0.850651f, 0.525731f, 0.000000f}, {-0.864188f, 0.442863f, 0.238856f},
        {-0.716567f, 0.681718f, 0.147621f}, {-0.688191f, 0.587785f, 0.425325f},
        {-0.500000f, 0.809017f, 0.309017f}, {-0.238856f, 0.864188f, 0.442863f},
        {-0.425325f, 0.688191f, 0.587785f}, {-0.716567f, 0.681718f, 0.147621f},
        {-0.500000f, 0.809017f, 0.309017f}, {-0.525731f, 0.850651f, 0.000000f},
        {0.000000f, 0.850651f, -0.525731f}, {-0.238856f, 0.864188f, -0.442863f},
        {0.000000f, 0.955423f, -0.295242f}, {-0.262866f, 0.951056f, -0.162460f},
        {0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f},
        {-0.262866f, 0.951056f, 0.162460f}, {-0.238856f, 0.864188f, 0.442863f},
        {-0.262866f, 0.951056f, 0.162460f}, {-0.500000f, 0.809017f, 0.309017f},
        {-0.238856f, 0.864188f, -0.442863f}, {-0.262866f, 0.951056f, -0.162460f},
        {-0.500000f, 0.809017f, -0.309017f}, {-0.850651f, 0.525731f, 0.000000f},
        {-0.716567f, 0.681718f, 0.147621f}, {-0.716567f, 0.681718f, -0.147621f},
        {-0.525731f, 0.850651f, 0.000000f}, {-0.425325f, 0.688191f, 0.587785f},
        {-0.864188f, 0.442863f, 0.238856f}, {-0.688191f, 0.587785f, 0.425325f},
        {-0.809017f, 0.309017f, 0.500000f}, {-0.681718f, 0.147621f, 0.716567f},
        {-0.587785f, 0.425325f, 0.688191f}, {-0.955423f, 0.295242f, 0.000000f},
        {0.000000f, 0.000000f, 1.000000f}, {-0.951056f, 0.162460f, 0.262866f},
        {-1.000000f, 0.000000f, 0.000000f}, {-0.850651f, 0.000000f, 0.525731f},
        {-0.955423f, -0.295242f, 0.000000f}, {-0.951056f, -0.162460f, 0.262866f},
        {-0.864188f, 0.442863f, -0.238856f}, {-0.951056f, 0.162460f, -0.262866f},
        {-0.809017f, 0.309017f, -0.500000f}, {-0.864188f, -0.442863f, -0.238856f},
        {-0.951056f, -0.162460f, -0.262866f}, {-0.809017f, -0.309017f, -0.500000f},
        {-0.681718f, 0.147621f, -0.716567f}, {-0.681718f, -0.147621f, -0.716567f},
        {-0.850651f, 0.000000f, -0.525731f}, {-0.688191f, 0.587785f, -0.425325f},
        {-0.587785f, 0.425325f, -0.688191f}, {-0.425325f, 0.688191f, -0.587785f},
        {-0.425325f, -0.688191f, -0.587785f}, {-0.587785f, -0.425325f, -0.688191f},
        {-0.688191f, -0.587785f, -0.425325f}, {-0.955423f, 0.295242f, 0.000000f},
        {-0.951056f, 0.162460f, 0.262866f}, {-1.000000f, 0.000000f, 0.000000f},
        {-0.850651f, 0.000000f, 0.525731f}, {-0.955423f, -0.295242f, 0.000000f},
        {-0.951056f, -0.162460f, 0.262866f}, {-0.864188f, 0.442863f, -0.238856f},
        {-0.951056f, 0.162460f, -0.262866f}, {-0.809017f, 0.309017f, -0.500000f},
        {-0.864188f, -0.442863f, -0.238856f}, {-0.951056f, -0.162460f, -0.262866f},
        {-0.809017f, -0.309017f, -0.500000f}, {-0.681718f, 0.147621f, -0.716567f},
        {-0.681718f, -0.147621f, -0.716567f}, {-0.850651f, 0.000000f, -0.525731f},
        {-0.688191f, 0.587785f, -0.425325f}, {-0.587785f, 0.425325f, -0.688191f},
        {-0.425325f, 0.688191f, -0.587785f}, {-0.425325f, -0.688191f, -0.587785f},
        {-0.587785f, -0.425325f, -0.688191f}, {-0.688191f, -0.587785f, -0.425325f},
        {0.525731f, 0.000000f, 0.850651f}, {0.442863f, 0.238856f, 0.864188f},
        {0.295242f, 0.000000f, 0.955423f}, {0.309017f, 0.500000f, 0.809017f},
        {0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f},
        {0.000000f, 0.850651f, 0.525731f}, {0.147621f, 0.716567f, 0.681718f},
        {-0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f},
        {-0.309017f, 0.500000f, 0.809017f}, {-0.525731f, 0.000000f, 0.850651f},
        {-0.295242f, 0.000000f, 0.955423f}, {-0.442863f, 0.238856f, 0.864188f},
        {-0.162460f, 0.262866f, 0.951056f}, {0.681718f, 0.147621f, 0.716567f},
        {0.809017f, 0.309017f, 0.500000f}, {0.587785f, 0.425325f, 0.688191f},
        {0.850651f, 0.525731f, 0.000000f}, {0.864188f, 0.442863f, 0.238856f},
        {0.716567f, 0.681718f, 0.147621f}, {0.688191f, 0.587785f, 0.425325f},
        {0.500000f, 0.809017f, 0.309017f}, {0.238856f, 0.864188f, 0.442863f},
        {0.425325f, 0.688191f, 0.587785f}, {0.716567f, 0.681718f, 0.147621f},
        {0.500000f, 0.809017f, 0.309017f}, {0.525731f, 0.850651f, 0.000000f},
        {0.000000f, 0.850651f, -0.525731f}, {0.238856f, 0.864188f, -0.442863f},
        {0.000000f, 0.955423f, -0.295242f}, {0.262866f, 0.951056f, -0.162460f},
        {0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f},
        {0.262866f, 0.951056f, 0.162460f}, {0.238856f, 0.864188f, 0.442863f},
        {0.262866f, 0.951056f, 0.162460f}, {0.500000f, 0.809017f, 0.309017f},
        {0.238856f, 0.864188f, -0.442863f}, {0.262866f, 0.951056f, -0.162460f},
        {0.500000f, 0.809017f, -0.309017f}, {0.850651f, 0.525731f, 0.000000f},
        {0.716567f, 0.681718f, 0.147621f}, {0.716567f, 0.681718f, -0.147621f},
        {0.525731f, 0.850651f, 0.000000f}, {0.425325f, 0.688191f, 0.587785f},
        {0.864188f, 0.442863f, 0.238856f}, {0.688191f, 0.587785f, 0.425325f},
        {0.809017f, 0.309017f, 0.500000f}, {0.681718f, 0.147621f, 0.716567f},
        {0.587785f, 0.425325f, 0.688191f}, {0.955423f, 0.295242f, 0.000000f},
        {0.951056f, 0.162460f, 0.262866f}, {1.000000f, 0.000000f, 0.000000f},
        {0.850651f, 0.000000f, 0.525731f}, {0.955423f, -0.295242f, 0.000000f},
        {0.951056f, -0.162460f, 0.262866f}, {0.864188f, 0.442863f, -0.238856f},
        {0.951056f, 0.162460f, -0.262866f}, {0.809017f, 0.309017f, -0.500000f},
        {0.864188f, -0.442863f, -0.238856f}, {0.951056f, -0.162460f, -0.262866f},
        {0.809017f, -0.309017f, -0.500000f}, {0.681718f, 0.147621f, -0.716567f},
        {0.681718f, -0.147621f, -0.716567f}, {0.850651f, 0.000000f, -0.525731f},
        {0.688191f, 0.587785f, -0.425325f}, {0.587785f, 0.425325f, -0.688191f},
        {0.425325f, 0.688191f, -0.587785f}, {0.425325f, -0.688191f, -0.587785f},
        {0.587785f, -0.425325f, -0.688191f}, {0.688191f, -0.587785f, -0.425325f}
    };
    
    // Копируем все нормали
    for (int i = 0; i < NUM_NORMALS; ++i) {
        m_normals[i][0] = tempNormals[i][0];
        m_normals[i][1] = tempNormals[i][1];
        m_normals[i][2] = tempNormals[i][2];
    }
}

void MDLModel::LoadPalette(unsigned char palette[256][3]) {
    unsigned char Palette[256][3] = {
        { 0, 0, 0}, { 15, 15, 15}, { 31, 31, 31}, { 47, 47, 47},
        { 63, 63, 63}, { 75, 75, 75}, { 91, 91, 91}, {107, 107, 107},
        {123, 123, 123}, {139, 139, 139}, {155, 155, 155}, {171, 171, 171},
        {187, 187, 187}, {203, 203, 203}, {219, 219, 219}, {235, 235, 235},
        { 15, 11, 7}, { 23, 15, 11}, { 31, 23, 11}, { 39, 27, 15},
        { 47, 35, 19}, { 55, 43, 23}, { 63, 47, 23}, { 75, 55, 27},
        { 83, 59, 27}, { 91, 67, 31}, { 99, 75, 31}, {107, 83, 31},
        {115, 87, 31}, {123, 95, 35}, {131, 103, 35}, {143, 111, 35},
        { 11, 11, 15}, { 19, 19, 27}, { 27, 27, 39}, { 39, 39, 51},
        { 47, 47, 63}, { 55, 55, 75}, { 63, 63, 87}, { 71, 71, 103},
        { 79, 79, 115}, { 91, 91, 127}, { 99, 99, 139}, {107, 107, 151},
        {115, 115, 163}, {123, 123, 175}, {131, 131, 187}, {139, 139, 203},
        { 0, 0, 0}, { 7, 7, 0}, { 11, 11, 0}, { 19, 19, 0},
        { 27, 27, 0}, { 35, 35, 0}, { 43, 43, 7}, { 47, 47, 7},
        { 55, 55, 7}, { 63, 63, 7}, { 71, 71, 7}, { 75, 75, 11},
        { 83, 83, 11}, { 91, 91, 11}, { 99, 99, 11}, {107, 107, 15},
        { 7, 0, 0}, { 15, 0, 0}, { 23, 0, 0}, { 31, 0, 0},
        { 39, 0, 0}, { 47, 0, 0}, { 55, 0, 0}, { 63, 0, 0},
        { 71, 0, 0}, { 79, 0, 0}, { 87, 0, 0}, { 95, 0, 0},
        {103, 0, 0}, {111, 0, 0}, {119, 0, 0}, {127, 0, 0},
        { 19, 19, 0}, { 27, 27, 0}, { 35, 35, 0}, { 47, 43, 0},
        { 55, 47, 0}, { 67, 55, 0}, { 75, 59, 7}, { 87, 67, 7},
        { 95, 71, 7}, {107, 75, 11}, {119, 83, 15}, {131, 87, 19},
        {139, 91, 19}, {151, 95, 27}, {163, 99, 31}, {175, 103, 35},
        { 35, 19, 7}, { 47, 23, 11}, { 59, 31, 15}, { 75, 35, 19},
        { 87, 43, 23}, { 99, 47, 31}, {115, 55, 35}, {127, 59, 43},
        {143, 67, 51}, {159, 79, 51}, {175, 99, 47}, {191, 119, 47},
        {207, 143, 43}, {223, 171, 39}, {239, 203, 31}, {255, 243, 27},
        { 11, 7, 0}, { 27, 19, 0}, { 43, 35, 15}, { 55, 43, 19},
        { 71, 51, 27}, { 83, 55, 35}, { 99, 63, 43}, {111, 71, 51},
        {127, 83, 63}, {139, 95, 71}, {155, 107, 83}, {167, 123, 95},
        {183, 135, 107}, {195, 147, 123}, {211, 163, 139}, {227, 179, 151},
        {171, 139, 163}, {159, 127, 151}, {147, 115, 135}, {139, 103, 123},
        {127, 91, 111}, {119, 83, 99}, {107, 75, 87}, { 95, 63, 75},
        { 87, 55, 67}, { 75, 47, 55}, { 67, 39, 47}, { 55, 31, 35},
        { 43, 23, 27}, { 35, 19, 19}, { 23, 11, 11}, { 15, 7, 7},
        {187, 115, 159}, {175, 107, 143}, {163, 95, 131}, {151, 87, 119},
        {139, 79, 107}, {127, 75, 95}, {115, 67, 83}, {107, 59, 75},
        { 95, 51, 63}, { 83, 43, 55}, { 71, 35, 43}, { 59, 31, 35},
        { 47, 23, 27}, { 35, 19, 19}, { 23, 11, 11}, { 15, 7, 7},
        {219, 195, 187}, {203, 179, 167}, {191, 163, 155}, {175, 151, 139},
        {163, 135, 123}, {151, 123, 111}, {135, 111, 95}, {123, 99, 83},
        {107, 87, 71}, { 95, 75, 59}, { 83, 63, 51}, { 67, 51, 39},
        { 55, 43, 31}, { 39, 31, 23}, { 27, 19, 15}, { 15, 11, 7},
        {111, 131, 123}, {103, 123, 111}, { 95, 115, 103}, { 87, 107, 95},
        { 79, 99, 87}, { 71, 91, 79}, { 63, 83, 71}, { 55, 75, 63},
        { 47, 67, 55}, { 43, 59, 47}, { 35, 51, 39}, { 31, 43, 31},
        { 23, 35, 23}, { 15, 27, 19}, { 11, 19, 11}, { 7, 11, 7},
        {255, 243, 27}, {239, 223, 23}, {219, 203, 19}, {203, 183, 15},
        {187, 167, 15}, {171, 151, 11}, {155, 131, 7}, {139, 115, 7},
        {123, 99, 7}, {107, 83, 0}, { 91, 71, 0}, { 75, 55, 0},
        { 59, 43, 0}, { 43, 31, 0}, { 27, 15, 0}, { 11, 7, 0},
        { 0, 0, 255}, { 11, 11, 239}, { 19, 19, 223}, { 27, 27, 207},
        { 35, 35, 191}, { 43, 43, 175}, { 47, 47, 159}, { 47, 47, 143},
        { 47, 47, 127}, { 47, 47, 111}, { 47, 47, 95}, { 43, 43, 79},
        { 35, 35, 63}, { 27, 27, 47}, { 19, 19, 31}, { 11, 11, 15},
        { 43, 0, 0}, { 59, 0, 0}, { 75, 7, 0}, { 95, 7, 0},
        {111, 15, 0}, {127, 23, 7}, {147, 31, 7}, {163, 39, 11},
        {183, 51, 15}, {195, 75, 27}, {207, 99, 43}, {219, 127, 59},
        {227, 151, 79}, {231, 171, 95}, {239, 191, 119}, {247, 211, 139},
        {167, 123, 59}, {183, 155, 55}, {199, 195, 55}, {231, 227, 87},
        {127, 191, 255}, {171, 231, 255}, {215, 255, 255}, {103, 0, 0},
        {139, 0, 0}, {179, 0, 0}, {215, 0, 0}, {255, 0, 0},
        {255, 243, 147}, {255, 247, 199}, {255, 255, 255}, {159, 91, 83}
    };
    
    memcpy(palette, Palette, sizeof(Palette));
}


} // namespace Revolt