//
//  MoodstocksScanner.m
//  MoodstocksScanner
//
//  Created by Santanu.K on 08/04/14.
//  Copyright (c) 2014 Web Spiders India. All rights reserved.
//

#import "MoodstocksScanner.h"
#import "FlashRuntimeExtensions.h"
#import "RootViewController.h"

#import <Moodstocks/Moodstocks.h>

#import "ScannerViewController.h"

@implementation UIViewExtension
@synthesize camView;

id refToSelf;
MSScanner *_scanner;
ScannerViewController *scannerUIViewController;
FREContext *context;
NSString *cachesPath;
BOOL isFirstTime;


//Removes the camView from the main View root View Controller
-(void)hideCam
{
    if(scannerUIViewController.view.superview != nil)
    {
        NSLog(@"Removing a Cam View");
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        [scannerUIViewController dismissViewControllerAnimated:TRUE completion:^
        {
            [_scanner cancelApiSearches];
            [_scanner cancelSync];
            [_scanner close:nil];
        }];
    }
}

-(void)showCam:(NSString *)apikey apisecret:(NSString *)apisecret
{
    NSLog(@"Adding a Cam View");
    
    if (scannerUIViewController == nil)
    {
        // for first time run
        cachesPath = [MSScanner cachesPathFor:@"scanner.db"];
        _scanner = [[MSScanner alloc] init];
        NSError *error = nil;
        
        if (![_scanner openWithPath:cachesPath
                                key:apikey
                             secret:apisecret
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
        
        scannerUIViewController = [[ScannerViewController alloc] initWithNibName:@"ScannerViewController" bundle:newBundle];
        scannerUIViewController.scanner = _scanner;
        NSAssert(scannerUIViewController, @"scanner view not found", nil);
    }
    else
    {
        // for every second time run
        [_scanner openWithPath:cachesPath
                           key:apikey
                        secret:apisecret
                         error:nil];
        [_scanner syncProgress];
        [scannerUIViewController showOpeningAlert];
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(exitHandler:) name:@"exitCam" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(matchFound:) name:@"matchFound" object:nil]; 
    
    [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:scannerUIViewController animated:YES completion:nil];
    isFirstTime = TRUE;


    //[[[[[UIApplication sharedApplication] windows] objectAtIndex:0] rootViewController].view addSubview:[scannerVC view]];
    
    // this code adds view without blocking any UI element at flex side
    //[[[[UIApplication sharedApplication] keyWindow] rootViewController].view addSubview:[scannerVC view]];   
}

-(void)exitHandler:(NSNotification *)notification
{
    [self hideCam];
}

-(void)matchFound:(NSNotification *)notification
{
    NSString *eventValue = [notification object];
    NSString *eventName = @"scanCompleted";
    const uint8_t* valueEvent = (const uint8_t*) [eventValue UTF8String]; // event.data
    const uint8_t* nameEvent = (const uint8_t*) [eventName UTF8String]; // event.type
    FREDispatchStatusEventAsync(context, nameEvent, valueEvent);
}


//
//  Public Methods
//  Exposed to ActionScript
//

FREObject runScanner(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
    NSLog(@"Run Scanner being called.");
    
    uint32_t urlLength;
    const uint8_t *apikey;
    const uint8_t *apisecret;
    FREGetObjectAsUTF8(argv[0], &urlLength, &apikey);
    FREGetObjectAsUTF8(argv[1], &urlLength, &apisecret);
    NSString *nsapikey = [NSString stringWithUTF8String:(char*)apikey];
    NSString *nsapisecret = [NSString stringWithUTF8String:(char*)apisecret];
    
    context = ctx;
    refToSelf = [[UIViewExtension alloc] init];
    [refToSelf showCam:nsapikey apisecret:nsapisecret];
    return NULL;
}

FREObject releaseScanner(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
    [refToSelf hideCam];
    return NULL;
}


//
//  Required API
//  To Build AIR/ObjectiveC Bridge
//

void MoodstocksExtContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctionsToTest, const FRENamedFunction** functionsToSet)
{
    NSLog(@"ExtConInit Called");
    *numFunctionsToTest = 2;
    FRENamedFunction* func = (FRENamedFunction*) malloc(sizeof(FRENamedFunction) * *numFunctionsToTest);
    
    func[0].name = (const uint8_t*) "runScanner";
    func[0].functionData = NULL;
    func[0].function = &runScanner;
    
    func[1].name = (const uint8_t*) "releaseScanner";
    func[1].functionData = NULL;
    func[1].function = &releaseScanner;

    *functionsToSet = func;
}

void MoodstocksExtensionInitializer(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, FREContextFinalizer* ctxFinalizerToSet)
{
    NSLog(@"ExtInit Called");
    *extDataToSet = NULL;
    *ctxInitializerToSet = &MoodstocksExtContextInitializer;
}

@end
