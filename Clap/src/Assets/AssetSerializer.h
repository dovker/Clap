#pragma once

namespace Clap
{
    class AssetSerializer
    {
        virtual void Serialize(const Ref<Asset>& asset, const AssetMetadata& metadata) = 0;
        virtual bool Load(const Ref<Asset>& asset, const AssetMetadata& metadata) = 0;
    };
}