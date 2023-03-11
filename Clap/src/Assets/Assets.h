#pragma once

namespace Clap
{
    class AssetType
    {
        None     = 0,
        TEXTURE  = 1,
        SHADER   = 2,
        AUDIO    = 3,
        FONT     = 4,
        SCRIPT   = 5,
    };

    class Asset
    {
    public:
        static AssetType GetStaticType() { return AssetType::None; }
        virtual AssetType GetAssetType() = 0;

        bool Loaded = false;
        uint64_t ID;
        std::string Name;
    };

    struct AssetMetadata
    {
        std::string Path;
        std::string Metadata; //JSON format
        uint32_t Version;
        bool LoadOnStart;
        bool Packed;
    };

    class Assets
    {
        /*
        Get<T>
        Load<T>
        Reload<T>
        Save<T>
        Init("Index") .clapassets file

        Private

        GenerateID

        */
    private:
        std::unordered_map<uint64_t, std::pair<Ref<Asset>, AssetMetadata>> m_Assets;
        std::unordered_map<std::string, uint64_t> m_Handles;
        // std::unordered_map<AssetType, Scope<AssetSerializer>> m_Serializers;
    };
}