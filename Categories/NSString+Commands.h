//
//  NSString+Commands.h
//  MacPass
//
//  Created by Michael Starke on 10/11/13.
//  Copyright (c) 2013 HicknHack Software GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>

@class KPKTree;

@interface NSString (Commands)

- (BOOL)isReference;
- (NSString *)resolveReferenceWithTree:(KPKTree *)tree;

@end
