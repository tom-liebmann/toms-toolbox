package toms_toolbox;

import android.os.Bundle;
import android.util.Log;
import android.view.View;


public class MainActivity extends android.app.Activity
{
    private static final String TAG = "MainActivity";

    private MainView m_view;

    @Override
    protected void onCreate( final Bundle savedInstanceState )
    {
        super.onCreate( savedInstanceState );

        m_view = new MainView( this );
        setContentView( m_view );

        ApplicationLib.test_callback();
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
