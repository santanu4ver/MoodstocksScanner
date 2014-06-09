package com.webspiders.extension
{
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	import flash.utils.setTimeout;
	
	public class MoodstocksScanner extends EventDispatcher
	{
		protected var extContext			: ExtensionContext;
		
		private static var INSTANCE			: MoodstocksScanner;
		
		public function MoodstocksScanner(enforcer:SingletonEnforcer)
		{
			super();
			
			extContext = ExtensionContext.createExtensionContext( "com.webspiders.MoodstocksScanner", "" );
			if ( !extContext ) throw new Error( "MoodstocksScanner extension is not supported on this platform." );
			else extContext.addEventListener( StatusEvent.STATUS, onStatus );
		}
		
		public static function get instance() : MoodstocksScanner
		{
			if ( !INSTANCE ) INSTANCE = new MoodstocksScanner( new SingletonEnforcer() );
			return INSTANCE;
		}
		
		public function runScanner( apiKey:String, apiSecret:String ) : void
		{
			extContext.call( "runScanner", apiKey, apiSecret );
		}
		
		public function dispose() : void
		{
			extContext.call( "releaseScanner" );
			setTimeout( extContext.dispose, 500 );
		}
		
		private function onStatus( event:StatusEvent ) : void
		{
			trace("RETURN FROM IOS " + event.level);
		}
	}
}

class SingletonEnforcer {
	
}