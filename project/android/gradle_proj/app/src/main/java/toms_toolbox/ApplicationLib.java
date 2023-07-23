package toms_toolbox;

import android.content.res.AssetManager;
import android.util.Log;
import toms_toolbox.MainActivity;

public class ApplicationLib
{
    public static final String TAG = "ApplicationLib";

    static
    {
        System.loadLibrary( "project_library" );
    }

    public static native void init();

    // ================ OpenGL and rendering

    public static native void init_gl();

    public static native void on_resize( int width, int height );

    public static native void step();

    // ================ Asset management

    public static native void init_asset_mng( AssetManager assetMngr, String appDir );

    // ================ Input handling

    public static native void on_pointer_down( int pointerId, float x, float y );

    public static native void on_pointer_up( int pointerId, float x, float y );

    public static native void on_pointer_move( int pointerId, float x, float y );

    // ================ Google ID

    public static native void on_silent_sign_in_success( String authCode );

    public static native void on_silent_sign_in_failure();

    public static native void on_explicit_sign_in_success( String authCode );

    public static native void on_explicit_sign_in_failure();

    // ================ Networking

    public static native void on_network_available();

    public static native void on_network_lost();

    // ================ AdMob

    public static native void on_ad_init_complete();

    public static native void on_ad_load_failed();

    public static native void on_ad_reward_earned( int amount, String type );
}
