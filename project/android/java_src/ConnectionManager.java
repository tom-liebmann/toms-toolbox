package toms_toolbox;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkRequest;
import android.net.NetworkCapabilities;
import android.net.Network;


class ConnectionManager
{
    private Context m_context;

    private ConnectivityManager m_connectivityManager;

    private boolean m_active = false;

    public ConnectionManager( Context context )
    {
        m_context = context;

        m_connectivityManager = (ConnectivityManager) m_context.getSystemService( ConnectivityManager.class );
    }

    public void enableNetworkCheck()
    {
        if( m_active )
        {
            return;
        }

        final NetworkRequest networkRequest = new NetworkRequest.Builder()
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

        m_connectivityManager.unregisterNetworkCallback( m_networkCallback );

        m_active = false;
    }

    private ConnectivityManager.NetworkCallback m_networkCallback = new ConnectivityManager.NetworkCallback()
    {
        @Override
        public void onAvailable( Network network )
        {
            on_network_available();
        }

        @Override
        public void onLost( Network network )
        {
            on_network_lost();
        }
    };

    private static native void on_network_available();

    private static native void on_network_lost();
}
