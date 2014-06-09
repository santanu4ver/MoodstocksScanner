//
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import <AVFoundation/AVFoundation.h> // for AVCaptureVideoPreviewLayer
#import <UIKit/UIKit.h> // for UIInterfaceOrientation

@class MSScanner;
@class MSResult;

@protocol MSAutoScannerSessionDelegate;

/** The `MSAutoScannerSession` class allows you to easily setup a scanning session
 * that automatically processes camera frames in real-time and report successful
 * on-device scan results.
 *
 * If your collection of reference images does not exceed the on-device database capacity,
 * then this session provides the best user experience possible since it never introduces
 * network latencies. Otherwise you may want to use a MSManualScannerSession.
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
@interface MSAutoScannerSession : NSObject

/** The scanner session delegate that will be notified of scan results.
 */
@property (nonatomic, readwrite, weak) id<MSAutoScannerSessionDelegate> delegate;

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

/** A setting that specifies which extra information should be computed - if possible, and
 * attached to the returned results.
 *
 * It must be provided as a bitwise-OR combination of the MSResultExtra flags, e.g:
 * set `MSResultExtraHomography | MSResultExtraDimensions` to request the homography
 * and reference image dimensions.
 *
 * `MSResultExtraNone` is set by default.
 */
@property (nonatomic, assign) int resultExtras;

/** A setting that specifies which advanced search options should be used when scanning
 * images.
 *
 * It must be provided as a bitwise-OR combination of the MSSearchOption flags.
 *
 * `MSSearchDefault` is set by default.
 */
@property (nonatomic, assign) int searchOptions;

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
 * Example: this is useful if you want to display an help section on-top of the scanner while
 * making sure you will not be notified with unexpected scan results.
 *
 * @return `YES` if the session has been paused, `NO` otherwise.
 */
- (BOOL)pauseProcessing;

/** Starts processing frames again after a call to `pauseProcessing`.
 *
 * @return `YES` if the session has been resumed, `NO` otherwise.
 */
- (BOOL)resumeProcessing;

@end

/** The protocol by which the delegate is kept notified of MSAutoScannerSession scan results.
 */
@protocol MSAutoScannerSessionDelegate <NSObject>

/** Informs the `MSAutoScannerSessionDelegate` that a scan result has been found.
 *
 * This method is called in the main thread.
 *
 * @param scannerSession the current `MSAutoScannerSession`.
 * @param result the scan result found. Guaranteed to be non-`nil`.
 *
 * @warning **Important:** the auto scanner session automatically pauses processing before calling
 * this method. This means the caller must call [MSAutoScannerSession resumeProcessing] when frames
 * should start being processed again.
 */
- (void)session:(id)scannerSession didFindResult:(MSResult *)result;

@optional

/** Informs the `MSAutoScannerSessionDelegate` that a scan result has been found and deliver the
 * corresponding video frame in addition.
 *
 * This method behaves similarly to [MSAutoScannerSessionDelegate session:didFindResult:] except
 * that it returns in addition the video frame that matched successfully.
 *
 * If it is implemented by the delegate then it takes precedence, i.e it is called instead of
 * [MSAutoScannerSessionDelegate session:didFindResult:] and is thus mutually exclusive.
 *
 * @param scannerSession the current `MSAutoScannerSession`.
 * @param result the scan result found. Guaranteed to be non-`nil`.
 * @param videoFrame the video frame that triggered this scan result. It is returned with the same
 * resolution it was delivered by the camera.
 * @see session:didFindResult:
 * @see [MSResult warpImage:block:]
 *
 * @warning **Note**: this method is intended for experts. The video frame returned is not suited
 * for end-user displaying but rather for custom post-processing.
 */
- (void)session:(id)scannerSession didFindResult:(MSResult *)result forVideoFrame:(UIImage *)videoFrame;

/** Notifies the caller that a scan warning has been encountered at some point.
 *
 * This is intended for development and debugging purposes.
 *
 * @param scannerSession the current `MSManualScannerSession`.
 * @param warning a debugging string that describes the encountered warning.
 */
- (void)session:(id)scannerSession didEncounterWarning:(NSString *)warning;

@end
