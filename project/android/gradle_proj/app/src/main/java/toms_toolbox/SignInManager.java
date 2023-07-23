package toms_toolbox;


class SignInManager
{
    private Context m_context;
    private GoogleSignInClient m_signInClient;
    private ActivityResultLauncer< Intent > m_activityResultLauncher;

    private boolean m_signedIn = false;

    public SignInManager( Context context )
    {
        m_context = context;

        // Request authCode so we can send the code to the server.
        final String serverClientId = getString( R.string.server_client_id );

        final GoogleSignInOptions options =
            new GoogleSignInOptions.Builder( GoogleSignInOptions.DEFAULT_SIGN_IN )
                .requestProfile()
                .requestServerAuthCode( serverClientId )
                .build();

        m_signInClient = GoogleSignIn.getClient( this, options );

        m_activityResultLauncher = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(), result -> {
                if( result.getResultCode() == Activity.RESULT_OK )
                {
                    final Intent intent = result.getData();
                    final Task< GoogleSignInAccount > task =
                        GoogleSignIn.getSignedInAccountFromIntent( intent );

                    try
                    {
                        m_signedIn = true;
                        ApplicationLib.on_explicit_sign_in_success(
                            task.getResult( ApiException.class ) );
                    }
                    catch( ApiException apiException )
                    {
                        ApplicationLib.on_sign_in_failure();
                    }
                }
            } );
    }

    public boolean isSignedIn()
    {
        return m_signedIn;
    }

    public void performSilentSignIn()
    {
        m_signInClient.silentSignIn().addOnCompleteListener( this, task -> {
            if( task.isSuccessful() )
            {
                final GoogleSignInAccount account = task.getResult();
                final String authCode = account.getServerAuthCode();

                if( authCode != null )
                {
                    m_signedIn = true;
                    ApplicationLib.on_silent_sign_in_success( authCode );
                }
                else
                {
                    ApplicationLib.on_silent_sign_in_failure();
                }
            }
            else
            {
                ApplicationLib.on_silent_sign_in_failure();
            }
        } );
    }

    public void performExplicitSignIn()
    {
        m_activityResultLauncher.launch( m_signInClient.getSignInIntent() );
    }
}
