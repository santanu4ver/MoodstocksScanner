//
//  ScannerViewController.h
//  MSSantanu
//
//  Created by James Heng on 16/04/2014.
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSScanner;

@interface ScannerViewController : UIViewController

-(void)showOpeningAlert;

@property (weak, nonatomic) MSScanner *scanner;
@property (weak, nonatomic) NSString *APIKEY;
@property (weak, nonatomic) NSString *APISECRET;

@end
