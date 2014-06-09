//
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSImage;
@class MSResult;
@class MSSync;
@class MSApiSearch;

/** Enumeration for search options.
 */
typedef NS_ENUM(int, MSSearchOption) {
    /** Default search mode. */
    MSSearchDefault           = 0,
    /** Disables "partial matching".
     Use this flag to avoid returning false positive results due to partial
     matching, for example if several of the indexed images share the exact same
     logo.
     */
    MSSearchNoPartial         = 1 << 0,
    /** Enables "small image" recognition. Use this flag to boost
     * the scale invariance so that smaller or farther images can be recognized.
     * Slightly slower that default mode.
     */
    MSSearchSmallTarget       = 1 << 1
};

/** The name of the notification dispatched when the synchronization state changes.
 */
extern NSString * const MSScannerSyncChangeNotification;

/** The `MSScanner` class implements an image and barcode scanner. This means you use a
 * `MSScanner` object to perform image recognition (a.k.a image matching) over a collection
 * of reference images, or decode 1D or 2D barcodes.
 *
 * Before going further you must create a developer account on https://moodstocks.com so as
 * to obtain a valid API key / secret pair that is mandatory to use the scanner. If you plan
 * to perform image matching you must of course also index a collection of reference images
 * (please follow the Getting Started from the https://moodstocks.com/docs section).
 *
 * In all cases the first thing you need to do to interact with the scanner - and thus the rest
 * of this framework, is to create, initialize and open a scanner instance with openWithPath:key:secret:error:.
 * We recommend you to do so in your application delegate, and then pass the scanner around to the
 * controllers or other parts of the app that need to use it.
 *
 * Another thing that you will systematically want to do is keeping the local scanner database
 * up-to-date with your API key: you can do this easily with syncInBackground. This has for effect
 * to locally synchronize the signatures of the images you flagged *offline* on your API key. This
 * makes it possible to perform instant, on-device image matching over this set of images. If not
 * all your collection of reference images can be made offline, you can still perform a server-side
 * search (a.k.a API search).
 *
 * In most cases, we greatly recommend you to use the scanner object with a scanner session - either
 * MSAutoScannerSession or MSManualScannerSession, since the sessions take care of all the *plumbing*
 * for you (initialize the device camera, consume frames, report results, etc).
 *
 * Please note that this class is KVO compliant regarding the isSyncing, syncProgress and syncError
 * properties, which is convenient when you need to inform the end user that a synchronization is
 * pending, or failed. Changes on these properties are also reported via `MSScannerSyncChangeNotification`
 * notifications.
 */
@interface MSScanner : NSObject

///---------------------------------------------------------------------------------------
/// @name Properties
///---------------------------------------------------------------------------------------

/** Internal scanner handle.
 */
@property (nonatomic, readonly) void *handle;

/** Value indicating whether a synchronization is currently pending.
 *
 * This value is observable via Key-Value Observing. Alternatively you can also be notified
 * when this value has possibly changed by observing `MSScannerSyncChangeNotification`
 * notifications dispatched by the default notification center.
 */
@property (nonatomic, readonly) BOOL isSyncing;

/** Value indicating the current sync progress in percent, if a synchronization is
 * pending.
 *
 * If there is no pending synchronization, this value refers to the last
 * synchronization performed.
 *
 * The sync progress is reported as an integer value between 0 and 100. It is set to
 * -1 by default e.g when a synchronization begins. You should ignore it when it is negative.
 *
 * This value is observable via Key-Value Observing. Alternatively you can also be notified
 * when this value has possibly changed by observing `MSScannerSyncChangeNotification`
 * notifications dispatched by the default notification center.
 */
@property (nonatomic, readonly) NSInteger syncProgress;

/** Value indicationg the *last* synchronization error, if any. It is set to `nil`
 * if there is no error, or if no synchronization was ever performed.
 *
 * This value is observable via Key-Value Observing. Alternatively you can also be notified
 * when this value has possibly changed by observing `MSScannerSyncChangeNotification`
 * notifications dispatched by the default notification center.
 */
@property (nonatomic, readonly) NSError *syncError;

///---------------------------------------------------------------------------------------
/// @name Initialization and disposal
///---------------------------------------------------------------------------------------

/** Initializes a newly allocated scanner object.
 *
 * @return An initialized scanner handle.
 * @see openWithPath:key:secret:error:
 *
 * @warning **Important:** once initialized you must open the scanner by means of the
 * openWithPath:key:secret:error: method.
 */
- (id)init;

/** Opens the scanner.
 *
 * You must first create a developer account on https://moodstocks.com/ to obtain a valid
 * API key / secret pair.
 *
 * The scanner will store its internal database file at the given path. Since the database
 * file can be re-created and re-synchronized, a typical location to store the scanner
 * database is the `/Library/Caches` directory. You can use the convenient cachesPathFor:
 * method to obtain such a database path.
 *
 * @param path the path of the internal scanner database file.
 * @param key a valid Moodstocks API key
 * @param secret a valid Moodstocks API secret
 * @param error the pointer to the error object, if any.
 * @return `YES` if it succeeded, `NO` otherwise.
 * @see cachesPathFor:
 *
 * @warning **Important:** successfully opening the scanner is mandatory before you start
 * interacting with any other methods.
 */
- (BOOL)openWithPath:(NSString *)path
                 key:(NSString *)key
              secret:(NSString *)secret
               error:(NSError **)error;

/** Closes the scanner to stop interacting with.
 *
 * We encourage you to close the scanner when the application will terminate.
 *
 * @param error the pointer to the error object, if any.
 * @return `YES` if it succeeded, `NO` otherwise.
 */
- (BOOL)close:(NSError **)error;

/** Gets the absolute path that allows to store `fileName` under `/Library/Caches`.
 *
 * @param fileName the requested file name.
 * @return The corresponding path that you may want to use as an input for scannerWithPath:
 */
+ (NSString *)cachesPathFor:(NSString *)fileName;

///---------------------------------------------------------------------------------------
/// @name Import Method
///---------------------------------------------------------------------------------------

/** Pre-populate the scanner via bulk data import from a compatible bundle.
 *
 * You can think of it as a post-open operation that you can use to *bootstrap* the scanner
 * with a prebuilt database of image signatures bundled with your application. This improves
 * the first-launch user experience.
 *
 * This does not require any Internet connection.
 *
 * @param bundle the bundle directory that contains the data to be imported.
 * @param error the pointer to the error object, if any.
 * @return `YES` if it succeeded, `NO` otherwise.
 */
- (BOOL)importBundle:(NSBundle *)bundle
               error:(NSError **)error;

///---------------------------------------------------------------------------------------
/// @name Synchronization Methods
///---------------------------------------------------------------------------------------

/** Synchronizes the local image signatures database to keep it up-to-date with the
 * current server-side state.
 *
 * This method runs in the background so you can safely call it from the main thread.
 *
 * If you want to display a progress bar or inform the end user of the sync progress or
 * error, you can observe isSyncing, syncProgress and syncError properties, or alternatively
 * add an observer for `MSScannerSyncChangeNotification` notifications.
 * @see syncInBackgroundWithBlock:progressBlock:
 */
- (void)syncInBackground;

/** Synchronizes the local image signatures database to keep it up-to-date with the
 * current server-side state.
 *
 * This method runs in the background so you can safely call it from the main thread.
 *
 * If you want to display a progress bar or inform the end user of the sync progress or
 * error, you can observe isSyncing, syncProgress and syncError properties, or alternatively
 * add an observer for `MSScannerSyncChangeNotification` notifications.
 *
 * @param completedBlock the block that is called when the synchronization completed. If an
 * error occurred the `error` object provides the appropriate information, otherwise it is
 * `nil` which indicates the synchronization completed successfully.
 * @param progressBlock the block that is called to report sync progress in percent.
 */
- (void)syncInBackgroundWithBlock:(void (^)(MSSync *operation, NSError *error))completedBlock
                    progressBlock:(void (^)(NSInteger percentDone))progressBlock;

/** Cancels any pending synchronization.
 */
- (void)cancelSync;

///---------------------------------------------------------------------------------------
/// @name Information Methods
///---------------------------------------------------------------------------------------

/** Gets the total number of image signatures found within the local database.
 *
 * @param error the pointer to the error object, if any.
 * @return the number of image signatures found within the local database, or -1 if an error occurred.
 */
- (NSInteger)count:(NSError **)error;

/** Gets an array made of all images identifiers found into the local database.
 *
 * @param error the pointer to the error object, if any.
 * @return the array of `NSString` identifiers for each image found within the local database.
 */
- (NSArray *)info:(NSError **)error;

///---------------------------------------------------------------------------------------
/// @name On-device Image Matching Methods
///---------------------------------------------------------------------------------------

/** Performs on-device image matching over the local database.
 *
 * @param query the query image.
 * @param options the search options specified as a bitwise-OR combination of the requested
 * MSSearchOption options.
 * @param extras the result extras specified as a bitwise-OR combination of the requested
 * MSResultExtra flags.
 * @param error the pointer to the error object, if any.
 * @return the result found, or `nil` if no match found or an error occurred.
 */
- (MSResult *)searchWithQuery:(MSImage *)query
                      options:(int)options
                       extras:(int)extras
                        error:(NSError **)error;

///---------------------------------------------------------------------------------------
/// @name On-device Barcode Decoding Methods
///---------------------------------------------------------------------------------------

/** Performs on-device barcode decoding.
 *
 * @param query the query image.
 * @param formats the barcode formats to be decoded specified as a bitwise-OR combination
 * of the requested MSResultType types.
 * @param extras the result extras specified as a bitwise-OR combination of the requested
 * MSResultExtra flags.
 * @param error the pointer to the error object, if any.
 * @return the result found, or `nil` if no barcode found with the specified formats or an
 * error occurred.
 */
- (MSResult *)decodeWithQuery:(MSImage *)query
                      formats:(int)formats
                       extras:(int)extras
                        error:(NSError **)error;

///---------------------------------------------------------------------------------------
/// @name Server-side Image Matching Methods
///---------------------------------------------------------------------------------------

/** Performs a remote image search on Moodstocks API (a.k.a server-side image matching).
 *
 * This method runs in the background so you can safely call it from the main thread.
 *
 * @param query the query image.
 * @param completedBlock the block that is called when the request completed. If an error
 * occurred the `error` object provides the appropriate information, otherwise it is `nil`
 * which indicates the request completed successfully. In such a case the search result can
 * be obtained via `operation.result`. It may be `nil` in case of no match found.
 *
 * @warning **Note:** this method requires an Internet connection.
 */
- (void)apiSearchInBackgroundWithQuery:(MSImage *)query
                                 block:(void (^)(MSApiSearch *operation, NSError *error))completedBlock;

/** Cancels any pending API search(es) that run in the background.
 *
 * @see apiSearchInBackgroundWithQuery:block:
 */
- (void)cancelApiSearches;

///---------------------------------------------------------------------------------------
/// @name Proxy Settings
///---------------------------------------------------------------------------------------

/** Auto-detect the HTTP proxy settings that should be used to configure the scanner.
 *
 * This is a convenience method that ease the discovery of the proxy settings. It reads the
 * system proxy settings and handles the following cases:
 *
 * - (1) there is no HTTP proxy configured,
 *
 * - (2) there is an HTTP proxy manually configured,
 *
 * - (3) there is an HTTP proxy automatically configured with a PAC file URL.
 *
 * In case (1) a `nil` dictionary is returned. In case (2) and (3) a non `nil` dictionary
 * is returned with entries for the proxy's host and port (under `kCFProxyHostNameKey` and
 * `kCFProxyPortNumberKey` respectively).
 *
 * @param error the pointer to the error object, if any.
 * @return the proxy settings, or `nil` if there is none or an error occurred.
 */
+ (NSDictionary *)detectProxySettings:(NSError **)error;

/** Auto-detect the HTTP proxy settings that should be used to configure the scanner.
 *
 * This does exactly the same than `detectProxySettings:` except that it returns in addition a
 * copy of the system proxy settings (if `systemSettings` is not `nil`) which is useful:
 *
 * - to check if the proxy is authenticated: check the `@"HTTPProxyAuthenticated"` key on
 * `systemSettings`. If it exists, the value is a boolean (`NSNumber`) indicating whether or
 * not the proxy is authentified,
 *
 * - to get the username if the proxy is authenticated: check the `@"HTTPProxyUsername"` key on
 * `systemSettings`. If it exists, the value is a string (`NSString`) containing the username.
 *
 * @param error the pointer to the error object, if any.
 * @param systemSettings the pointer to the system settings (optional)
 * @return the proxy settings, or `nil` if there is none or an error occurred.
 *
 * @warning **Note:** in case the proxy is authenticated, there is **NO** way to automatically
 * retrieve the password. In such a case, you should implement your own UI to ask the end user
 * to type in the proxy password for the corresponding username (e.g with a `UIAlertViewStyleSecureTextInput`).
 */
+ (NSDictionary *)detectProxySettings:(NSDictionary **)systemSettings error:(NSError **)error;

/** Set HTTP proxy settings for all subsequent transfers.
 *
 * see `detectProxySettings:`
 *
 * @param host the host name or dotted IP address of the proxy.
 * @param port the port number of the proxy.
 * @param error the pointer to the error object, if any.
 * @return `YES` if it succeeded, `NO` otherwise.
 *
 * @warning **IMPORTANT**: you must call this function *before* opening the scanner. Any
 * attempt to change the proxy settings on an opened scanner will result in a misuse error.
 * If you need to change the settings later on, close the scanner, call this function, then
 * re-open the scanner.
 *
 * If you need to zero out the proxy settings (i.e no more use a HTTP proxy),
 * close the scanner and call this function with a `nil` host.
 *
 * @warning **WARNING**: only HTTP Basic Authentication is supported.
 */
- (BOOL)setProxySettings:(NSString *)host
                    port:(NSNumber *)port
                   error:(NSError **)error;

/** Set HTTP proxy settings for all subsequent transfers.
 *
 * see `detectProxySettings:`
 *
 * @param host the host name or dotted IP address of the proxy.
 * @param port the port number of the proxy.
 * @param username the user name to use for the connection to the HTTP proxy (optional).
 * @param password the password to use for the connection to the HTTP proxy (optional).
 * @param error the pointer to the error object, if any.
 * @return `YES` if it succeeded, `NO` otherwise.
 *
 * @warning **IMPORTANT**: you must call this function *before* opening the scanner. Any
 * attempt to change the proxy settings on an opened scanner will result in a misuse error.
 * If you need to change the settings later on, close the scanner, call this function, then
 * re-open the scanner.
 *
 * If you need to zero out the proxy settings (i.e no more use a HTTP proxy),
 * close the scanner and call this function with a `nil` host.
 *
 * Note that the authentication parameters are optional: if your proxy is not
 * authenticated pass `nil` for `username` and `password`.
 *
 * @warning **WARNING**: only HTTP Basic Authentication is supported.
 */
- (BOOL)setProxySettings:(NSString *)host
                    port:(NSNumber *)port
                username:(NSString *)username
                password:(NSString *)password
                   error:(NSError **)error;

@end
