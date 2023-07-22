package toms_toolbox;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;


public class MainActivity extends android.app.Activity
{
    private static final String TAG = "MainActivity";

    private static MainActivity s_instance;

    private MainView m_view;
    private ConnectionManager m_connectionManager;
    private AdManager m_adManager;

    public static MainActivity getInstance()
    {
        return s_instance;
    }

    @Override
    protected void onCreate( final Bundle savedInstanceState )
    {
        s_instance = this;

        super.onCreate( savedInstanceState );

        final Context context = getApplicationContext();

        m_connectionManager = new ConnectionManager( context );
        m_adManager = new AdManager( this );

        ApplicationLib.init();

        ApplicationLib.init_asset_mng( context.getAssets(), context.getFilesDir().toString() );

        m_view = new MainView( this );
        setContentView( m_view );
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

    @Override
    protected void onPause()
    {
        m_view.onPause();
        super.onPause();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        m_view.onResume();

        hideUi();
    }

    @Override
    public void onWindowFocusChanged( final boolean hasFocus )
    {
        if( hasFocus )
        {
            hideUi();
        }
    }

    public ConnectionManager getConnectionManager()
    {
        return m_connectionManager;
    }

    public AdManager getAdManager()
    {
        return m_adManager;
    }

    private void hideUi()
    {
        final View decorView = getWindow().getDecorView();

        decorView.setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_IMMERSIVE
            // Set the content to appear under the system bars so that the
            // content doesn't resize when the system bars hide and show.
            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
            View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            // Hide the nav bar and status bar
            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN );
    }
}
