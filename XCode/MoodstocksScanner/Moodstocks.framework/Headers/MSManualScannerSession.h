//
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import <AVFoundation/AVFoundation.h> // for AVCaptureVideoPreviewLayer
#import <UIKit/UIKit.h> // for UIInterfaceOrientation

@class MSScanner;
@class MSResult;

@protocol MSManualScannerSessionDelegate;

/** The `MSManualScannerSession` class allows you to easily setup a scanning session
 * with tap-to-scan capability. This means the end-user typically triggers a scan via
 * a *manual* tap interaction and lets the session:
 *
 * - capture the next camera frame,
 * - perform a server-side search if no image result can be found on-device,
 * - report whether a match is found or not.
 *
 * This is simply achieved by calling snap when a tap occurs.
 *
 * Since this class conveniently handles server-side search it is the perfect candidate
 * when you have a large collection of reference images that exceeds the on-device database
 * capacity.
 *
 * Think of it as a convenient and configurable wrapper around a MSScanner object
 * that hides the messy details of setting up an AVFoundation video capture and
 * scanning incoming frames.
 *
 * To make it easy to display the live video capture on-screen, the scanner session
 * includes a captureLayer that you can directly insert into your view.
 *
 * If you plan to support multiple orientations on the UI side you are responsible to:
 *
 * - tell the session which orientation currently takes effect via the interfaceOrientation
 * property,
 * - update the captureLayer bounds to reflect the current UI,
 * - set the right video orientation via the `AVCaptureVideoPreviewLayer` `connection`
 * property.
 */
@interface MSManualScannerSession : NSObject

/** The scanner session delegate that will be notified of scan results.
 */
@property (nonatomic, readwrite, weak) id<MSManualScannerSessionDelegate> delegate;

/** A setting that specifies which image matching and barcode decoding operations
 * have to be performed on each camera frame.
 *
 * It must be provided as a bitwise-OR combination of the MSResultType flags, e.g:
 * set `MSResultTypeImage | MSResultTypeQRCode` to enable image matching and QR code
 * decoding.
 *
 * `MSResultTypeImage` is set by default.
 */
@property (nonatomic, assign) int resultTypes;

/** A boolean value indicating whether the query image should be returned after each successful
 * snap.
 *
 * Obtaining the query image is optional. If requested via this option, it is returned by
 * means of the [MSManualScannerSessionDelegate session:didFindResult:optionalQuery:] method.
 *
 * `NO` is set by default.
 */
@property (nonatomic, assign) BOOL wantsQuery;

/** A value that gives the scanner session knowledge of which orientation currently takes
 * effect on the UI side.
 *
 * `UIInterfaceOrientationPortrait` is set by default.
 */
@property (nonatomic, assign) UIInterfaceOrientation interfaceOrientation;

/** The layer used to display the video capture on a given UIView.
 */
@property (nonatomic, readonly) AVCaptureVideoPreviewLayer *captureLayer;

///---------------------------------------------------------------------------------------
/// @name Initialization Methods
///---------------------------------------------------------------------------------------

/** Initializes a scanner session.
 *
 * The caller is responsible to provide a scanner successfully opened. Also the scanner session does not
 * perform any synchronization: it must be handled by the caller.
 *
 * @param scanner the scanner used by the session as the underlying processor for each incoming camera frame.
 * @return the scanner session instance.
 * @see [MSScanner openWithPath:key:secret:error:]
 * @see [MSScanner syncInBackground]
 */
- (id)initWithScanner:(MSScanner *)scanner;

///---------------------------------------------------------------------------------------
/// @name Video Capture Methods
///---------------------------------------------------------------------------------------

/** Starts the scanner session.
 *
 * This is a pre-requisite so that the scanner session can start receiving and processing camera frames.
 */
- (void)startRunning;

/** Stops the scanner session after use.
 *
 * You must always stop the session once done to release underlying camera resources.
 */
- (void)stopRunning;

///---------------------------------------------------------------------------------------
/// @name Session State Methods
///---------------------------------------------------------------------------------------

/** Stops processing frames temporarily.
 *
 * The scanner session keeps receiving frames from the camera, but they are no more processed.
 *
 * This means the video capture layer keeps being refreshed, but the session can no more report
 * any scan result.
 *
 * @return `YES` if the session has been paused, `NO` otherwise.
 */
- (BOOL)pauseProcessing;

/** Starts processing frames again.
 *
 * @return `YES` if the session has been resumed, `NO` otherwise.
 */
- (BOOL)resumeProcessing;

/** Requests the scanner session to capture and scan the next incoming camera frame.
 *
 * If no result can be found on-device, and if the `MSResultTypeImage` has been configured on this
 * session, the captured frame is sent to Moodstocks API for server-side image matching.
 *
 * If this method returns `YES` then:
 *
 * - if no error occurs, the [MSManualScannerSessionDelegate session:didFindResult:optionalQuery:] method
 * is called in reply with the result or `nil` if no match found,
 * - if an error occurs, the [MSManualScannerSessionDelegate session:didFailWithError:] method is called,
 * - if the operation is cancelled no method is called.
 *
 * @return `YES` if the snap request has been successfully deferred, `NO` otherwise.
 * @see cancel
 */
- (BOOL)snap;

/** Cancel any pending server-side search operation.
 *
 * @return `YES` if an operation has been cancelled, `NO` otherwise.
 * @see snap
 */
- (BOOL)cancel;

@end

/** The protocol by which the delegate is kept notified of MSManualScannerSession scan results.
 */
@protocol MSManualScannerSessionDelegate <NSObject>

/** Informs the `MSManualScannerSessionDelegate` that a server-side request has just been started.
 *
 * This is a convenient method used to display an indeterminate progress control on the UI side.
 *
 * @param scannerSession the current `MSManualScannerSession`.
 * @see [MSManualScannerSession snap]
 */
- (void)sessionWillStartServerRequest:(id)scannerSession;

/** Informs the `MSManualScannerSessionDelegate` that a scan result has been found.
 *
 * This method is triggered in response to a successful [MSManualScannerSession snap]. The reported
 * result can be `nil` if no match or barcode found.
 *
 * This method is called in the main thread.
 *
 * @param scannerSession the current `MSManualScannerSession`.
 * @param result the scan result found. It is `nil` if no match or barcode found.
 * @param query the query image that corresponds to the still image captured at snap time, or `nil`
 * if the `wantsQuery` option has not been enabled. It is returned with the same resolution it was
 * delivered by the camera.
 * @see [MSManualScannerSession wantsQuery]
 *
 * @warning **Important:** the manual scanner session automatically pauses processing before calling
 * this method. This means the caller must call [MSManualScannerSession resumeProcessing] when frames
 * should start being processed again.
 */
- (void)session:(id)scannerSession didFindResult:(MSResult *)result optionalQuery:(UIImage *)query;

/** Informs the `MSManualScannerSessionDelegate` that an error has been encountered while scanning
 * or in response to a server-side search.
 *
 * This method is called in the main thread.
 *
 * @param scannerSession the current `MSManualScannerSession`.
 * @param error the encountered scanning error.
 *
 * @warning **Important:** the manual scanner session automatically pauses processing before calling
 * this method. This means the caller must call [MSManualScannerSession resumeProcessing] when frames
 * should start being processed again.
 */
- (void)session:(id)scannerSession didFailWithError:(NSError *)error;

@optional

/** Notifies the caller that a scan warning has been encountered at some point.
 *
 * This is intended for development and debugging purposes.
 *
 * @param scannerSession the current `MSManualScannerSession`.
 * @param warning a debugging string that describes the encountered warning.
 */
- (void)session:(id)scannerSession didEncounterWarning:(NSString *)warning;

@end
