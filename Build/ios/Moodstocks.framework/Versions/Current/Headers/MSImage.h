//
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

/** The `MSImage` class represents an image that you can use as input for image matching or
 * barcode decoding operations.
 *
 * This class provides convenient class factory methods used to convert AVFoundation camera
 * frames, UIKit images or raw pixel arrays into a suitable `MSImage` object.
 *
 * Note that it is solely intended for the purpose of scanning. In other words it is not
 * possible to convert back an `MSImage` into another image type.
 *
 * Also note that all input data **must** fit the following dimension requirements. Otherwise,
 * the `MSImage` construction will fail with error `MSErrorImg`:
 *
 * - the largest input dimension must be higher than or equal to 480 pixels.
 *
 * - Image size must not exceed 1280x1280 pixels.
 */
@interface MSImage : NSObject

/** Internal image handle.
 */
@property (nonatomic, readonly) void *handle;

///---------------------------------------------------------------------------------------
/// @name Initialization
///---------------------------------------------------------------------------------------

/** Creates and returns an image instance from an AVFoundation camera frame.
 *
 * This method processes the input frame without any re-orientation. In other words it works
 * with the frame as delivered by the camera, keeping in mind the default orientation is
 * AVCaptureVideoOrientationLandscapeRight.
 *
 * @param imageBuffer the AVFoundation camera raw image buffer.
 * @param error the pointer to the error object, if any.
 * @return the image instance, or `nil` if an error occurred.
 *
 * @warning **Important:** this method only supports the kCVPixelFormatType_32BGRA pixel format.
 */
+ (id)imageWithBuffer:(CMSampleBufferRef)imageBuffer
                error:(NSError **)error;

/** Creates and returns an image instance from an AVFoundation camera frame
 * and video orientation.
 *
 * This method does re-orient the input frame according to the provided orientation. Typically
 * this orientation is driven by the user interface. Here it is useful to create an image that
 * exactly corresponds to what the end user sees on-screen (think of it as an EXIF orientation).
 *
 * @param imageBuffer the AVFoundation camera raw image buffer.
 * @param videoOrientation the orientation used to rotate the input buffer.
 * @param error the pointer to the error object, if any.
 * @return the image instance, or `nil` if an error occurred.
 *
 * @warning **Important:** this method only supports the kCVPixelFormatType_32BGRA pixel format.
 */
+ (id)imageWithBuffer:(CMSampleBufferRef)imageBuffer
          orientation:(AVCaptureVideoOrientation)videoOrientation
                error:(NSError **)error;

/** Creates and returns an image instance from an UIKit image.
 *
 * @param image the UIKit input image.
 * @param error the pointer to the error object, if any.
 * @return the image instance, or `nil` if an error occurred.
 */
+ (id)imageWithUIImage:(UIImage *)image
                 error:(NSError **)error;


/** Creates and returns an image instance from a byte array of *grayscaled* pixels.
 *
 * @param pixels the byte array containing the image pixels.
 * @param width the image width in pixels.
 * @param height the image height in pixels.
 * @param stride the number of bytes per row in the input byte array.
 * @param orientation the orientation used to rotate the input buffer.
 * @param error the pointer to the error object, if any.
 * @return the image instance, or `nil` if an error occurred.
 */
+ (id)imageWithGrayscalePixels:(const unsigned char *)pixels
                         width:(int)width
                        height:(int)height
                        stride:(int)stride
                   orientation:(AVCaptureVideoOrientation)orientation
                         error:(NSError **)error;

@end
