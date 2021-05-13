#include <ttb/core/resources/AssetManager.hpp>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


namespace
{
    static AAssetManager* android_asset_manager = nullptr;
}


namespace ttb::resources
{
    AssetManager& AssetManager::instance()
    {
        static AssetManager assetManager;
        return assetManager;
    }

    std::string AssetManager::loadContent( std::string const& filename )
    {
        if( !android_asset_manager )
        {
            throw std::runtime_error( "Asset manager not initialized" );
        }

        auto asset =
            AAssetManager_open( android_asset_manager, filename.c_str(), AASSET_MODE_BUFFER );

        if( !asset )
        {
            throw std::runtime_error( "Unable to open asset: " + filename );
        }

        std::string const src( static_cast< char const* >( AAsset_getBuffer( asset ) ),
                               AAsset_getLength( asset ) );

        AAsset_close( asset );

        return src;
    }

    std::vector< uint8_t > AssetManager::loadData( std::string const& filename )
    {
        if( !android_asset_manager )
        {
            throw std::runtime_error( "Asset manager not initialized" );
        }

        auto asset =
            AAssetManager_open( android_asset_manager, filename.c_str(), AASSET_MODE_BUFFER );

        if( !asset )
        {
            throw std::runtime_error( "Unable to open asset: " + filename );
        }

        auto ptr = static_cast< uint8_t const* >( AAsset_getBuffer( asset ) );
        std::vector< uint8_t > const data( ptr, ptr + AAsset_getLength( asset ) );

        AAsset_close( asset );

        return data;
    }

    AssetManager::AssetManager() = default;
}


extern "C"
{
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_init_1asset_1mng( JNIEnv* env,
                                                                               jobject /* obj */,
                                                                               jobject assetMgr )
    {
        auto android_java_asset_manager = env->NewGlobalRef( assetMgr );

        android_asset_manager = AAssetManager_fromJava( env, android_java_asset_manager );
    }
}
