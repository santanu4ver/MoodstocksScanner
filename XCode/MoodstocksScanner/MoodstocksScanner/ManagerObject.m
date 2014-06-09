//
//  ManagerObject.m
//  MSSantanu
//
//  Created by James Heng on 16/04/2014.
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import "ManagerObject.h"

#import "ScannerViewController.h"

#import <Moodstocks/Moodstocks.h>

#define MS_API_KEY    @"k17k9daoly0lsgo7f0kf"
#define MS_API_SECRET @"xcepqJ51BQLwXI1v"

@implementation ManagerObject


+ (void)showScanner
{
    NSString *cachesPath = [MSScanner cachesPathFor:@"scanner.db"];

    MSScanner *_scanner = [[MSScanner alloc] init];

    NSError *error = nil;
    
    if (![_scanner openWithPath:cachesPath
                            key:MS_API_KEY
                         secret:MS_API_SECRET
                          error:&error]) {
        
        MSDLog(@" [MOODSTOCKS SDK] SCANNER OPEN ERROR: %@", [error ms_message]);
        return;
    }
    
    MSDLog(@"[MOODSTOCKS] OPEN SCANNER SUCCEED");
    
    // don't forget to perform sync
    void (^completionBlock)(MSSync *, NSError *) = ^(MSSync *op, NSError *error) {
        if (error)
            NSLog(@"Sync failed with error: %@", [error ms_message]);
        else
            NSLog(@"Sync succeeded (%li images(s))", (long)[_scanner count:nil]);
    };
    
    void (^progressionBlock)(NSInteger) = ^(NSInteger percent) {
        NSLog(@"Sync progressing: %li%%", (long)percent);
    };
    
    // Launch the synchronization
    [_scanner syncInBackgroundWithBlock:completionBlock progressBlock:progressionBlock];
    
    
    NSBundle * mainBundle = [NSBundle mainBundle];
    NSString * pathToMyBundle = [mainBundle pathForResource:@"MoodstocksScannerBundle" ofType:@"bundle"];
    NSAssert(pathToMyBundle, @"bundle not found", nil);
    NSBundle * newBundle = [NSBundle bundleWithPath:pathToMyBundle];

    // don't forget to perform sync //
    //ScannerViewController *scanVC = [[ScannerViewController alloc] initWithNibName:@"ScannerViewController" bundle:nil];
    
    ScannerViewController *scannerUIViewController = [[ScannerViewController alloc] initWithNibName:@"ScannerViewController" bundle:newBundle];
    scannerUIViewController.scanner = _scanner;
    NSAssert(scannerUIViewController, @"scanner view not found", nil);

    
    UIViewController *rootVC = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
    [rootVC presentViewController:scannerUIViewController animated:YES completion:nil];
}

@end
