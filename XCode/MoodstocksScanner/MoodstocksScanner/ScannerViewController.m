//
//  ScannerViewController.m
//  MoodstocksScanner
//
//  Created by Santanu.K.
//  Copyright (c) Santanu Karar. All rights reserved.
//

#import "ScannerViewController.h"
#import "MBProgressHUD.h"

#import <Moodstocks/Moodstocks.h>

static int kMSResultTypes = MSResultTypeImage  |
                            MSResultTypeQRCode |
                            MSResultTypeEAN13;


@interface ScannerViewController () <MSManualScannerSessionDelegate, UIActionSheetDelegate, UIAlertViewDelegate> {
    MSManualScannerSession *_scannerSession;
}

@property (weak, nonatomic) IBOutlet UIView *previewVideo;

@end

@implementation ScannerViewController

UIActionSheet *aSheet;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _scannerSession = [[MSManualScannerSession alloc] initWithScanner:_scanner];
    _scannerSession.delegate = self;
    _scannerSession.resultTypes = kMSResultTypes;

    CALayer *videoPreviewLayer = [self.previewVideo layer];
    [videoPreviewLayer setFrame:[[UIScreen mainScreen] bounds]];
    [videoPreviewLayer setMasksToBounds:YES];
    
    CALayer *captureLayer = [_scannerSession captureLayer];
    [captureLayer setFrame:[[UIScreen mainScreen] bounds]];
    
    [videoPreviewLayer insertSublayer:captureLayer
                                below:[[videoPreviewLayer sublayers] objectAtIndex:0]];

    [_scannerSession startRunning];
    [self showOpeningAlert];
}

-(void)showOpeningAlert
{
    UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
    NSString *title = @"To execute a scan tap anywhere in screen.";
    
    NSString *other1 = @"Ok";
    NSString *other2 = @"Exit Scan";
    UIActionSheet *aSheet = [[UIActionSheet alloc] initWithTitle:title
                                                        delegate:self
                                               cancelButtonTitle:nil
                                          destructiveButtonTitle:nil
                                               otherButtonTitles:other1, other2, nil];
    
    if ([window.subviews containsObject:self.view])
    {
        [aSheet showInView:self.view];
    }
    else
    {
        [aSheet showInView:window];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - User Action

- (IBAction)previewTapped:(id)sender
{
    [_scannerSession snap];
}

#pragma mark - Orientation

- (void)viewWillLayoutSubviews
{
    [self updateInterfaceOrientation:self.interfaceOrientation];
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)orientation duration:(NSTimeInterval)duration
{
    [super willAnimateRotationToInterfaceOrientation:orientation duration:duration];
    [self updateInterfaceOrientation:orientation];
}

- (void)updateInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    [_scannerSession setInterfaceOrientation:interfaceOrientation];
    
    AVCaptureVideoPreviewLayer *captureLayer = (AVCaptureVideoPreviewLayer *) [_scannerSession captureLayer];
    
    captureLayer.frame = self.view.bounds;
    
    // AVCapture orientation is the same as UIInterfaceOrientation
    switch (interfaceOrientation) {
        case UIInterfaceOrientationPortrait:
            [[captureLayer connection] setVideoOrientation:AVCaptureVideoOrientationPortrait];
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            [[captureLayer connection] setVideoOrientation:AVCaptureVideoOrientationPortraitUpsideDown];
            break;
        case UIInterfaceOrientationLandscapeLeft:
            [[captureLayer connection] setVideoOrientation:AVCaptureVideoOrientationLandscapeLeft];
            break;
        case UIInterfaceOrientationLandscapeRight:
            [[captureLayer connection] setVideoOrientation:AVCaptureVideoOrientationLandscapeRight];
            break;
        default:
            break;
    }
}

- (void)sessionWillStartServerRequest:(id)scannerSession
{
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.labelText = @"Searching...";
}

- (void)session:(id)scannerSession didFindResult:(MSResult *)result optionalQuery:(UIImage *)query
{
    [MBProgressHUD hideHUDForView:self.view animated:YES];
    
    NSString *title = nil;
    
    if (result)
    {
        NSString *type = [result type] == MSResultTypeImage ? @"Image" : @"Barcode";
        title = [NSString stringWithFormat:@"%@:\n%@", type, [result string]];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"matchFound" object:title];
        aSheet = [[UIActionSheet alloc] initWithTitle:@"Match Found! You're returning to the Application."
                                                            delegate:self
                                                   cancelButtonTitle:nil
                                              destructiveButtonTitle:nil
                                                   otherButtonTitles:nil];
        [self performSelector:@selector(onTick:) withObject:nil afterDelay:2.0];
    }
    else
    {
        title = @"No Result Found!";
        NSString *other1 = @"Continue";
        NSString *other2 = @"Exit Scan";
        aSheet = [[UIActionSheet alloc] initWithTitle:title
                                                            delegate:self
                                                   cancelButtonTitle:nil
                                              destructiveButtonTitle:nil
                                                   otherButtonTitles:other1, other2, nil];
    }
    
    [aSheet showInView:self.view];
}

-(void)onTick:(NSTimer *)timer
{
    [aSheet dismissWithClickedButtonIndex:0 animated:YES];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"exitCam" object:nil];
}

- (void)session:(id)scannerSession didFailWithError:(NSError *)error
{
    [MBProgressHUD hideHUDForView:self.view animated:YES];
    
    [[[UIAlertView alloc] initWithTitle:@"An error occurred:"
                                message:[error ms_message]
                               delegate:self
                      cancelButtonTitle:@"OK"
                      otherButtonTitles: nil] show];
}

#pragma mark - UIActionSheetDelegate

- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    [_scannerSession resumeProcessing];
}

#pragma mark - UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [_scannerSession resumeProcessing];
}

-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        [[NSNotificationCenter defaultCenter] postNotificationName:@"exitCam" object:nil];
    }
}

@end
