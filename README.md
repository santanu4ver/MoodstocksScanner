AIR Native Extension for Moodstocks SDK
==============================================

Native Extension enabling picture recognition technoloy by Moodstocks (https://moodstocks.com/).

Requirement
---------------------------------------------
Get a developer key from: https://moodstocks.com/try-moodstocks

Build Script
---------------------------------------------
To build an .ane in OSX, open runTerminal.rtf in Build directory; copy and run on Terminal. Usage shown with .p12 file (optional).

ActionScript Usage
--------------------------------------------
Found MoodstocksScanner.ane in Build directory. Add it to your AIR mobile project. Copy the following codes to make it run:

private var scanner:MoodstocksScanner = MoodstocksScanner.instance();
private function runScanner():void
{
	scanner.addEventListener(Event.COMPLETE, onScannerComplete);
	scanner.runScanner(<moodstocksAPIKey>, <moodstocksAPISecret>);
}

private function onScannerComplete(event:Event):void
{
	trace("Image Match Name: "+ scanner.searchResult);
}