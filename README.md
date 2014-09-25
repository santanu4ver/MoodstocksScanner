# AIR Native Extension for Moodstocks SDK

Native Extension enabling picture recognition technology by Moodstocks (https://moodstocks.com/).

### Getting Started

After you have set up your app on the Moodstocks web portal and get your API key and secret, download Moodstocks Uploader for your desktop, upload some image(s) through it which you wants to be scan and recognize by mobile device; you are ready to begin integrating MoodstocksScanner ANE into your mobile AIR project.

First, import the MoodstocksScanner native extension into your AIR app (get it inside Build folder).  We recommend creating a directory in your project for native extensions, and copy `MoodstocksScanner.ane` to that directory.  Then, if you are using *Flash Builder*, you can just add that directory as a native extension directory in your project settings.

Second, make sure you add the `<extensionID>` declaration to your AIR application descriptor's root `<application>` element like in the following example:

```xml
<extensions>
	<extensionID>com.webspiders.MoodstocksScanner</extensionID>
</extensions>
```
##### Details for iOS

This extension tested with >iOS6 and <iOS8 so far.
 
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


### License

This project made available under the MIT License.

Copyright (C) 2014 Santanu Karar

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.