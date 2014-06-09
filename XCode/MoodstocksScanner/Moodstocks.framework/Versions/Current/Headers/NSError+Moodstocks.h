//
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import <Foundation/Foundation.h>

/** Enumeration for main Moodstocks SDK error codes.
 */
typedef NS_ENUM(NSInteger, MSErrorCode) {
    /** @abstract 0:  success. */
    MSSuccess = 0,
    /** @abstract 1:  unspecified error. */
    MSError,
    /** @abstract 2:  invalid use of the library. */
    MSErrorMisuse,
    /** @abstract 3:  access permission denied. */
    MSErrorNoPerm,
    /** @abstract 4:  file not found. */
    MSErrorNoFile,
    /** @abstract 5:  database file locked.  */
    MSErrorBusy,
    /** @abstract 6:  database file corrupted. */
    MSErrorCorrupt,
    /** @abstract 7:  empty database. */
    MSErrorEmpty,
    /** @abstract 8:  authorization denied. */
    MSErrorAuth,
    /** @abstract 9:  no internet connection. */
    MSErrorNoConn,
    /** @abstract 10: operation timeout. */
    MSErrorTimeout,
    /** @abstract 11: threading error. */
    MSErrorThread,
    /** @abstract 12: credentials mismatch. */
    MSErrorCredMismatch,
    /** @abstract 13: internet connection too slow. */
    MSErrorSlowConn,
    /** @abstract 14: record not found. */
    MSErrorNoRec,
    /** @abstract 15: operation aborted. */
    MSErrorAbort,
    /** @abstract 16: resource temporarily unavailable. */
    MSErrorUnavail,
    /** @abstract 17: image size or format not supported. */
    MSErrorImg,
    /** @abstract 18: wrong API key or no offline image. */
    MSErrorApiKey,
    /** @abstract 19: a network problem occurred. */
    MSErrorNetworkFail,
    /** @abstract 20: scanner is not open. */
    MSErrorNotOpen,
    /** @abstract 21: could not read bundle. */
    MSErrorBundle,
    /** @abstract 22: wrong API secret. */
    MSErrorApiSecret,
};

extern NSString *const MoodstocksErrorDomain;

/** Category that represents a Moodstocks error.
 */
@interface NSError (Moodstocks)

    /** Create an error object configured with the Moodstocks error domain.
     *
     * @param code the Moodstocks error code (e.g `MSErrorMisuse`)
     * @return The created error object.
     */
    + (NSError *)ms_errorWithCode:(NSInteger)code;

    /** Return the message string related to the error code.
     *
     * @return The user friendly message string.
     */
    - (NSString *)ms_message;

@end
