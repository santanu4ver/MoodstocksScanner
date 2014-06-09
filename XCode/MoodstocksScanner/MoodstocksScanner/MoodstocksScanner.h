//
//  MoodstocksScanner.h
//  MoodstocksScanner
//
//  Created by Santanu.K on 08/04/14.
//  Copyright (c) 2014 Web Spiders India. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface UIViewExtension :NSObject {
    UIWindow *camView;
}

-(void)hideCam;
-(void)showCam:(NSString *)apikey apisecret:(NSString *)apisecret;

@property(retain, nonatomic) UIWindow *camView;

@end
