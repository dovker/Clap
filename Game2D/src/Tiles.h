#pragma once
namespace Clap
{
    struct Tile
    {
        uint16_t ID = 0;
        //TODO: PACK THESE WITH BIT INFO

        Tile(uint16_t id = 0)
            : ID(id)
        {}
    };

    class TileMap
    {
    public:
        TileMap(uint32_t width = 1, uint32_t height = 1, uint16_t id = 0)
            : m_Width(width), m_Height(height)
        {
            m_Map = new Tile[width * height];
            for(uint32_t i = 0; i < width * height; i++)
            {
                m_Map[i] = Tile(id);
            }
        }

        ~TileMap()
        {
            delete[] m_Map;
        }

        bool InBounds(uint32_t x, uint32_t y)
        {
            return (x >= 0 && x < m_Width) && (y >= 0 && y < m_Height);
        }

        Tile& At(uint32_t x, uint32_t y)
        {
            if(InBounds(x, y))
                return m_Map[y * m_Width + x];
            return Tile(0);
        }

        uint32_t GetWidth()
        {
            return m_Width;
        }

        uint32_t GetHeigth()
        {
            return m_Height;
        }

        static Ref<TileMap> Create(uint32_t width = 1, uint32_t height = 1, uint16_t id = 0)
        {
            return CreateRef<TileMap>(width, height, id);
        }

    private:
        uint32_t m_Width = 0, m_Height = 0;
        Tile* m_Map;
    };
}