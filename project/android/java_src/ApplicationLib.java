package toms_toolbox;

import android.content.res.AssetManager;
import toms_toolbox.MainActivity;
import android.util.Log;

public class ApplicationLib
{
    public static final String TAG = "ApplicationLib";

    static
    {
        System.loadLibrary( "project_library" );
        Log.d( TAG, "Library loaded" );
    }

    public static native void init_gl();

    public static native void on_resize( int width, int height );

    public static native void step();

    public static native void init_asset_mng( AssetManager assetMngr );

    public static native void on_pointer_down( int pointerId, float x, float y );

    public static native void on_pointer_up( int pointerId, float x, float y );

    public static native void on_pointer_move( int pointerId, float x, float y );

    public static native void on_sign_in_success( String authCode );

    public static native void on_sign_in_failure();
}
