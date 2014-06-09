//
//  RootViewController.m
//  MSSantanu
//
//  Created by James Heng on 16/04/2014.
//  Copyright (c) 2014 Moodstocks. All rights reserved.
//

#import "RootViewController.h"

#import "ManagerObject.h"

@interface RootViewController ()

@end

@implementation RootViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    NSLog(@"showScanner Function request to ManagerObject");
    
    [ManagerObject showScanner];
}

@end
