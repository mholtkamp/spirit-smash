#include "Platform.h"
#include "Constants.h"
#include "Assets.h"

OrientedBoxCollider* Platform::s_pPlatformCollider = 0;

Platform::Platform()
{
    m_nSize    = 0;
    m_arMatter = 0;
}

Platform::~Platform()
{
    int i = 0;

    if (m_arMatter != 0)
    {
        delete [] m_arMatter;
        m_arMatter = 0;
    }
}

void Platform::Generate(int nSize,
                        float fX,
                        float fY,
                        float fZ)
{
    m_nSize = nSize;
    m_arMatter = new Matter[m_nSize];

    // Check to see if the common oriented box collider exists 
    if (s_pPlatformCollider == 0)
    {
        s_pPlatformCollider = new OrientedBoxCollider();
        s_pPlatformCollider->SetHalfExtents(PLATFORM_HALF_EXTENT,
                                            PLATFORM_HALF_EXTENT, 
                                            PLATFORM_HALF_EXTENT);
        s_pPlatformCollider->SetRelativePosition(0.0f, 0.0f, 0.0f);
        s_pPlatformCollider->SetRelativePosition(0.0f, 0.0f, 0.0f);
    }

    // Generate each section in the platform. 
    // Each section is 2 units wide.
    for (int i = 0; i < m_nSize; i++)
    {
        m_arMatter[i].SetPosition(fX + PLATFORM_SECTION_WIDTH*i,
                                  fY,
                                  fZ);
        m_arMatter[i].SetMaterial(g_pDefaultMaterial);
        m_arMatter[i].AddCollider(s_pPlatformCollider);
        
        // Set the appropriate mesh and texture
        if (i == 0)
        {
            m_arMatter[i].SetMesh(g_pPlatformLeftMesh);
            m_arMatter[i].SetTexture(g_pPlatformEndTex);
        }
        else if (i == m_nSize - 1)
        {
            m_arMatter[i].SetMesh(g_pPlatformRightMesh);
            m_arMatter[i].SetTexture(g_pPlatformEndTex);
        }
        else
        {
            m_arMatter[i].SetMesh(g_pPlatformMidMesh);

            // Alternate textures for mid platforms
            m_arMatter[i].SetTexture( (i % 2) ? g_pPlatformMid1 : g_pPlatformMid2);
        }
    }
}