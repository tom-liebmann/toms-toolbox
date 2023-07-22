package toms_toolbox;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.util.Log;


class ConnectionManager
{
    private static final String TAG = "ConnectionManager";

    private Context m_context;

    private ConnectivityManager m_connectivityManager;

    private boolean m_active = false;

    private boolean m_isNetworkAvailable = false;

    public ConnectionManager( Context context )
    {
        m_context = context;

        m_connectivityManager =
            (ConnectivityManager)m_context.getSystemService( ConnectivityManager.class );

        Log.d( TAG, "Created" );
    }

    public void enableNetworkCheck()
    {
        if( m_active )
        {
            return;
        }

        Log.d( TAG, "Enabling network check" );

        final NetworkRequest networkRequest =
            new NetworkRequest.Builder()
                .addCapability( NetworkCapabilities.NET_CAPABILITY_INTERNET )
                .addTransportType( NetworkCapabilities.TRANSPORT_WIFI )
                .addTransportType( NetworkCapabilities.TRANSPORT_CELLULAR )
                .build();

        m_connectivityManager.registerNetworkCallback( networkRequest, m_networkCallback );

        m_active = true;
    }

    public void disableNetworkCheck()
    {
        if( !m_active )
        {
            return;
        }

        Log.d( TAG, "Disabling network check" );

        m_connectivityManager.unregisterNetworkCallback( m_networkCallback );

        m_active = false;
        m_isNetworkAvailable = false;
    }

    public boolean isNetworkCheckEnabled()
    {
        return m_active;
    }

    public boolean isNetworkAvailable()
    {
        return m_isNetworkAvailable;
    }

    private ConnectivityManager.NetworkCallback m_networkCallback =
        new ConnectivityManager.NetworkCallback() {
            @Override
            public void onAvailable( Network network )
            {
                Log.d( TAG, "onNetworkAvailable" );
                m_isNetworkAvailable = true;
                ApplicationLib.on_network_available();
            }

            @Override
            public void onLost( Network network )
            {
                Log.d( TAG, "onNetworkLost" );
                m_isNetworkAvailable = false;
                ApplicationLib.on_network_lost();
            }
        };
}
