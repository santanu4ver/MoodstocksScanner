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
import com.webspiders.extension.MoodstocksScanner;
```

We recommend making a variable in your class to store a reference to the global Chartboost instance.

```actionscript
private var scanner:MoodstocksScanner;

// later...
scanner = MoodstocksScanner.instance;
```

To start Moodstocks scanner, call the `runScanner()` method with your API Key and Secret from the Moodstocks web portal. This shall open you a native camera window having requisite functionalities.

```actionscript
scanner.runScanner("API_KEY", "API_SECRET");
```

##### Listening to Moodstocks Event

To get a matched value for an image scanning you'll need to attach Event.CHANGE (`flash.events.Event`) listener to the Moodstocks instance and therefore call 'getValue()' method of MoodstocksScanner API:

```actionscript
// listener to Moodstocks matched event
scanner.addEventListener(Event.CHANGE, onMoodstocksMatch);
scanner.runScanner("API_KEY", "API_SECRET");
```

In your scanner listener function

```actionscript
private function onMoodstocksMatch(event:Event):void
{
	trace("Matched image name: "+ scanner.getValue());
}
```

##### Destroy Moodstocks Instance Manually

Call the 'dispose()' method to the MoodstocksScanner API

```actionscript
scanner.dispose();
```