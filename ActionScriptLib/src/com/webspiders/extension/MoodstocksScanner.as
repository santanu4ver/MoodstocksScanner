package com.webspiders.extension
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	import flash.utils.setTimeout;
	
	/**
	 * MoodstocksScanner
	 * 
	 * @author santanu.k
	 * @date 04.21.2014
	 * @version 1.0
	 */
	public class MoodstocksScanner extends EventDispatcher
	{
		//--------------------------------------------------------------------------
		//
		//  PRIVATE STATIC
		//
		//--------------------------------------------------------------------------
		
		private static var INSTANCE			: MoodstocksScanner;
		
		//--------------------------------------------------------------------------
		//
		//  PROTECTED VARIABLES
		//
		//--------------------------------------------------------------------------
		
		protected var extContext			: ExtensionContext;
		protected var matchValue			: String;
		
		/**
		 * CONSTRUCTOR
		 */
		public function MoodstocksScanner(enforcer:SingletonEnforcer)
		{
			super();
			
			// singleton enforcer.. 
			extContext = ExtensionContext.createExtensionContext( "com.webspiders.MoodstocksScanner", "" );
			if ( !extContext ) throw new Error( "MoodstocksScanner extension is not supported on this platform." );
			else extContext.addEventListener( StatusEvent.STATUS, onStatus );
		}
		
		/**
		 * RETURN INSTANCE
		 */
		public static function get instance() : MoodstocksScanner
		{
			if ( !INSTANCE ) INSTANCE = new MoodstocksScanner( new SingletonEnforcer() );
			return INSTANCE;
		}
		
		//--------------------------------------------------------------------------
		//
		//  PUBLIC API
		//
		//--------------------------------------------------------------------------
		
		/**
		 * Opens native Moodstocks camera UI
		 * 
		 * @required
		 * APIKey String
		 * APISecret String
		 */
		public function runScanner( apiKey:String, apiSecret:String ) : void
		{
			extContext.call( "runScanner", apiKey, apiSecret );
		}
		
		/**
		 * Dispose Moodstocks instance
		 */
		public function dispose() : void
		{
			extContext.call( "releaseScanner" );
			setTimeout( extContext.dispose, 500 );
			matchValue = null;
		}
		
		/**
		 * Returns an image match value
		 * 
		 * @return
		 * Image Name String
		 */
		public function getValue() : String
		{
			return matchValue;
		}
		
		//--------------------------------------------------------------------------
		//
		//  LISTENERS API
		//
		//--------------------------------------------------------------------------
		
		/**
		 * On image matched event
		 */
		private function onStatus( event:StatusEvent ) : void
		{
			matchValue = event.level;
			dispatchEvent( new Event(Event.CHANGE) );
		}
	}
}

class SingletonEnforcer
{
}