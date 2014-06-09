# AIR Native Extension for Moodstocks SDK

Native Extension enabling picture recognition technoloy by Moodstocks (https://moodstocks.com/).

### Getting Started

After you have set up your app on the Moodstocks web portal and get your API key and secret, you are ready to begin integrating MoodstocksScanner ANE into your mobile AIR project.

First, import the MoodstocksScanner native extension into your AIR app (get it inside Build folder).  We recommend creating a directory in your project for native extensions, and copy `MoodstocksScanner.ane` to that directory.  Then, if you are using *Flash Builder*, you can just add that directory as a native extension directory in your project settings.

Second, make sure you add the `<extensionID>` declaration to your AIR application descriptor's root `<application>` element like in the following example:

```xml
<extensions>
	<extensionID>com.webspiders.MoodstocksScanner</extensionID>
</extensions>
```
##### Details for iOS

This extenstion tested with iOS 6+ so far.
 
### Usage

##### MoodstocksScanner Setup

First, import the MoodstocksScanner classes into your code.

```actionscript
import com.chartboost.plugin.air.Chartboost;
import com.chartboost.plugin.air.ChartboostEvent;
```

We recommend making a variable in your class to store a reference to the global Chartboost instance.

```actionscript
private var chartboost:Chartboost;

// later...
chartboost = Chartboost.getInstance();
```

To initialize Chartboost, call the `init()` method with your application's ID and signature from the Chartboost web portal.  You'll probably need to call it conditionally for different platforms, so we've provided some helper functions for you to use.

```actionscript
if (Chartboost.isAndroid()) {
	chartboost.init("ANDROID_APP_ID", "ANDROID_APP_SIGNATURE");
} else if (Chartboost.isIOS()) {
	chartboost.init("IOS_APP_ID", "IOS_APP_SIGNATURE");
}
```

##### Calling Chartboost methods

In `/actionscript/src/com/chartboost/plugin/air/Chartboost.as` you will find the AIR-to-native methods used to interact with the Chartboost plugin:

```java
/** Initializes the Chartboost plugin and, on iOS, records the beginning of a user session */
public function init(appID:String, appSignature:String):void

/** Caches an interstitial. Location is optional. */
public function cacheInterstitial(location:String = "default"):void

/** Shows an interstitial. Location is optional. */
public function showInterstitial(location:String = "default"):void

/** Checks to see if an interstitial is cached. Location is optional. */
public function hasCachedInterstitial(location:String = "default"):Boolean

/** Caches the more apps screen. */
public function cacheMoreApps():void

/** Shows the more apps screen. */
public function showMoreApps():void

/** Checks to see if the more apps screen is cached. */
public function hasCachedMoreApps():Boolean

/** Force impressions to show in a particular orientation.
  * Pass one of the following: "Portrait", "LandscapeLeft", "PortraitUpsideDown", "LandscapeRight".
  * Pass an empty string (default) to stop overriding the default orientation. */
public function forceOrientation(orientation:String = ""):void
```

##### Listening to Chartboost Events

Chartboost fires off many different events to inform you of the status of impressions.  In order to react these events, you must explicitly listen for them.  The best place to do this is the initialization code for your active screen:

```actionscript
// in some initializing code
chartboost.addEventListener(ChartboostEvent.INTERSTITIAL_CACHED, onAdCached);

// in the same class
private function onAdCached( event:ChartboostEvent ):void {
	trace( "Chartboost: on Interstitial cached: ", event.location );
}
```

In `/actionscript/src/com/chartboost/plugin/air/ChartboostEvent.as` you will find all of the events that are available to listen to:

```actionscript	
/** Fired when an interstitial fails to load. */
public static const INTERSTITIAL_FAILED:String;

/** Fired when an interstitial is finished via any method.
  * This will always be paired with either a close or click event. */
public static const INTERSTITIAL_CLICKED:String;

/** Fired when an interstitial is closed
  * (i.e. by tapping the X or hitting the Android back button). */
public static const INTERSTITIAL_CLOSED:String;

/** Fired when an interstitial is clicked. */
public static const INTERSTITIAL_DISMISSED:String;		

/** Fired when an interstitial is cached. */
public static const INTERSTITIAL_CACHED:String;

/** Fired when an interstitial is shown. */
public static const INTERSTITIAL_SHOWED:String;

/** Fired when the more apps screen fails to load. */
public static const MOREAPPS_FAILED:String;

/** Fired when the more apps screen is finished via any method.
  * This will always be paired with either a close or click event. */
public static const MOREAPPS_CLICKED:String;

/** Fired when the more apps screen is closed
  * (i.e. by tapping the X or hitting the Android back button). */
public static const MOREAPPS_CLOSED:String;

/** Fired when a listing on the more apps screen is clicked. */
public static const MOREAPPS_DISMISSED:String;

/** Fired when the more apps screen is cached. */
public static const MOREAPPS_CACHED:String;

/** Fired when the more app screen is shown. */
public static const MOREAPPS_SHOWED:String;
```

