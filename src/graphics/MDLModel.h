#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "../math/Matrix4.h"

namespace Revolt {

struct MDLHeader {
    int32_t ident;           // Magic number: "IDPO"
    int32_t version;         // Must be 6
    float scale[3];          // Scale factor
    float translate[3];      // Translation vector
    float boundingRadius;
    float eyePosition[3];    // Eyes' position
    int32_t numSkins;        // Number of textures
    int32_t skinWidth;       // Texture width
    int32_t skinHeight;      // Texture height
    int32_t numVerts;        // Number of vertices
    int32_t numTris;         // Number of triangles
    int32_t numFrames;       // Number of frames
    int32_t syncType;        // 0 = synchron, 1 = random
    int32_t flags;           // State flag
    float size;
};

struct MDLTexCoord {
    int32_t onSeam;
    int32_t s, t;
};

struct MDLTriangle {
    int32_t facesFront;      // 0 = backface, 1 = frontface
    int32_t vertices[3];     // Vertex indices
};

struct MDLVertex {
    uint8_t v[3];           // Compressed coordinates
    uint8_t normalIndex;    // Normal vector index
};

struct MDLSimpleFrame {
    MDLVertex bboxMin;      // Bounding box min
    MDLVertex bboxMax;      // Bounding box max
    char name[16];
    std::vector<MDLVertex> vertices;
};

struct MDLFrame {
    int32_t type;           // 0 = simple, !0 = group
    MDLSimpleFrame frame;
};

struct MDLSkin {
    int32_t group;          // 0 = single, 1 = group
    std::vector<uint8_t> data; // Texture data (8-bit palette indices)
};

class MDLModel {
public:
    MDLModel();
    ~MDLModel();
    
    bool LoadFromFile(const std::string& filename);
    void Render(int frame = 0);
    void RenderInterpolated(int frame1, int frame2, float interp);
    
    int GetFrameCount() const { return static_cast<int>(m_frames.size()); }
    
    const MDLHeader& GetHeader() const { return m_header; }
    
private:
    bool ReadHeader(FILE* fp);
    bool ReadSkins(FILE* fp);
    bool ReadTexCoords(FILE* fp);
    bool ReadTriangles(FILE* fp);
    bool ReadFrames(FILE* fp);
    
    void ConvertVertex(const MDLVertex& vertex, float result[3]) const;
    
    MDLHeader m_header;
    std::vector<MDLSkin> m_skins;
    std::vector<MDLTexCoord> m_texCoords;
    std::vector<MDLTriangle> m_triangles;
    std::vector<MDLFrame> m_frames;
    
    // OpenGL texture IDs
    std::vector<unsigned int> m_textureIDs;
    int m_currentSkin;
    
    // Normal vectors table (from anorms.h)
    static constexpr int NUM_NORMALS = 162;
    float m_normals[NUM_NORMALS][3];
    
    void InitializeNormals();
    unsigned int CreateTextureFromSkin(const MDLSkin& skin);
};

} // namespace Revolt