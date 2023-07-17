package toms_toolbox;

import android.app.Activity;
import androidx.annotation.NonNull;
import android.util.Log;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.OnUserEarnedRewardListener;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;
import com.google.android.gms.ads.initialization.InitializationStatus;


class AdManager
{
    private static final String TAG = "AdManager";

    private Activity m_activity;

    private boolean m_initialized = false;

    public AdManager( Activity activity )
    {
        m_activity = activity;
    }

    public void initialize()
    {
        if( m_initialized )
        {
            return;
        }

        MobileAds.initialize( m_activity, new OnInitializationCompleteListener() {
            @Override
            public void onInitializationComplete( InitializationStatus initializationStatus )
            {
                ApplicationLib.on_ad_init_complete();
            }
        } );
    }

    public void runRewardedAd()
    {
        final AdRequest adRequest = new AdRequest.Builder().build();

        RewardedAd.load( m_activity,
                         "ca-app-pub-3940256099942544/5224354917",
                         adRequest,
                         new RewardedAdLoadCallback() {
                             @Override
                             public void onAdFailedToLoad( @NonNull LoadAdError loadAdError )
                             {
                                 ApplicationLib.on_ad_load_failed();
                             }

                             @Override
                             public void onAdLoaded( @NonNull RewardedAd ad )
                             {
                                 showAd( ad );
                             }
                         } );
    }

    private void showAd( RewardedAd ad )
    {
        ad.setFullScreenContentCallback( new FullScreenContentCallback() {
            @Override
            public void onAdClicked()
            {
                // Do nothing
            }

            @Override
            public void onAdDismissedFullScreenContent()
            {
                // Called when ad is dismissed.
                // Set the ad reference to null so you don't show the ad a second time.
                Log.d( TAG, "Ad dismissed fullscreen content." );
            }

            @Override
            public void onAdFailedToShowFullScreenContent( AdError adError )
            {
                // Called when ad fails to show.
                Log.e( TAG, "Ad failed to show fullscreen content." );
            }

            @Override
            public void onAdImpression()
            {
                // Called when an impression is recorded for an ad.
                Log.d( TAG, "Ad recorded an impression." );
            }

            @Override
            public void onAdShowedFullScreenContent()
            {
                // Called when ad is shown.
                Log.d( TAG, "Ad showed fullscreen content." );
            }
        } );

        ad.show( m_activity, new OnUserEarnedRewardListener() {
            @Override
            public void onUserEarnedReward( @NonNull RewardItem rewardItem )
            {
                ApplicationLib.on_ad_reward_earned( rewardItem.getAmount(), rewardItem.getType() );
            }
        } );
    }
}
