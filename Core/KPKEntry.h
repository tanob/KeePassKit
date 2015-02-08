//
//  KPKEntry.h
//  KeePassKit
//
//  Created by Michael Starke on 12.07.13.
//  Copyright (c) 2013 HicknHack Software GmbH. All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#import <Foundation/Foundation.h>

#import <AppKit/AppKit.h>

#import "KPKNode.h"
#import "KPKModificationRecording.h"

@class KPKGroup;
@class KPKBinary;
@class KPKAttribute;
@class KPKAutotype;

/* Entries declared as MetaEntries in KDB files
 * contain information that is stored in meta data in KDBX file
 */
FOUNDATION_EXTERN NSString *const KPKMetaEntryBinaryDescription;
FOUNDATION_EXTERN NSString *const KPKMetaEntryTitle;
FOUNDATION_EXTERN NSString *const KPKMetaEntryUsername;
FOUNDATION_EXTERN NSString *const KPKMetaEntryURL;

/* Commonly known meta entries */

FOUNDATION_EXTERN NSString *const KPKMetaEntryUIState;
FOUNDATION_EXTERN NSString *const KPKMetaEntryDefaultUsername;
FOUNDATION_EXTERN NSString *const KPKMetaEntrySearchHistoryItem;
FOUNDATION_EXTERN NSString *const KPKMetaEntryCustomKVP;
FOUNDATION_EXTERN NSString *const KPKMetaEntryDatabaseColor;
FOUNDATION_EXTERN NSString *const KPKMetaEntryKeePassXCustomIcon;
FOUNDATION_EXTERN NSString *const KPKMetaEntryKeePassXCustomIcon2;
FOUNDATION_EXTERN NSString *const KPKMetaEntryKeePassXGroupTreeState;

@interface KPKEntry : KPKNode <NSCopying, NSSecureCoding, NSPasteboardWriting, NSPasteboardReading>

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, copy) NSString *username;
@property (nonatomic, copy) NSString *url;
@property (nonatomic, copy) NSString *notes;

@property (nonatomic, assign) BOOL protectTitle;
@property (nonatomic, assign) BOOL protectPassword;
@property (nonatomic, assign) BOOL protectUsername;
@property (nonatomic, assign) BOOL protectUrl;
@property (nonatomic, assign) BOOL protectNotes;

@property (nonatomic, strong) NSArray *binaries;
// TODO: change Tags to an String array!
@property (nonatomic, copy) NSString *tags;
@property (nonatomic, copy) NSColor *foregroundColor;
@property (nonatomic, copy) NSColor *backgroundColor;
@property (nonatomic, copy) NSString *overrideURL;

@property (nonatomic, strong) NSArray *customAttributes;
@property (nonatomic, strong) KPKAutotype *autotype;
@property (nonatomic, strong) NSArray *history;
@property (nonatomic, readonly, assign) BOOL isHistory;

/**
 *  Test for Equality
 *  @param entry entry to test for equality
 *  @return YES if receiver is equal to entry based on all attributes. UUID and parents are NOT considered
 */
- (BOOL)isEqualToEntry:(KPKEntry *)entry;
/**
 *	Retrieves a list of all defaultAttributes
 */
- (NSArray *)defaultAttributes;
/**
 *	Removes the Entry from it's parent group
 */
- (void)remove;
/**
 *  Moves the Entry the the new group at the given index
 *
 *  @param group The new group that should contain the entry
 *  @param index The index at which to sort the entry into the existing entries of the group
 */
- (void)moveToGroup:(KPKGroup *)group atIndex:(NSUInteger)index;
/**
 *  Creates a duplicate of the Entry updating references and UUIDs
 *
 *  @param title   The new Title for the copy. If you leave this blank, a default title will be generated
 *
 *  @return The copied KPKEntry
 */
- (instancetype)copyWithTitle:(NSString *)title;

#pragma mark MetaEntries
/**
 *	Creates KDB meta entry with the given data and name
 *	@param	data	data to store in the entry
 *  @param  name  the name of the metaentry
 *	@return	a meta entry that can be serailized
 */
+ (KPKEntry *)metaEntryWithData:(NSData *)data name:(NSString *)name;
/**
 *	Additialn information is stores in MetaEntrie in KDB files.
 *  This function determines wheter the entry is a meta entry or not.
 *	@return	YES if this entry is a Meta Entry, NO if not
 */
- (BOOL)isMeta;

#pragma mark Custom Attributes
/**
 @param key String that identifies the attributes
 @returns the attribute with the given key
 */
- (KPKAttribute *)customAttributeForKey:(NSString *)key;
- (NSString *)valueForCustomAttributeWithKey:(NSString *)key;
/**
 @returns YES, if the supplied key is a key in the attributes of this entry
 */
- (BOOL)hasAttributeWithKey:(NSString *)key;
/**
 @returns a unique key for the proposed key.
 */
- (NSString *)proposedKeyForAttributeKey:(NSString *)key;
/**
 Adds an attribute to the entry
 @param attribute The attribute to be added
 @param index the position at wicht to add the attribute
 */
- (void)addCustomAttribute:(KPKAttribute *)attribute atIndex:(NSUInteger)index;
- (void)addCustomAttribute:(KPKAttribute *)attribute;
/**
 Removes the attribute for the given string
 @param attribute The attribute to be removed
 */
- (void)removeCustomAttribute:(KPKAttribute *)attribute;

#pragma mark Binaries

/**
 *  Adds a binary to the attachments of this entry
 *	@param	binary	Binary to add
 *	@param	index	Index to add the binary at
 */
- (void)addBinary:(KPKBinary *)binary atIndex:(NSUInteger)index;
/**
 *	Adds the given Binary to the binaries of this entry
 *	@param	binary	Binary to add
 */
- (void)addBinary:(KPKBinary *)binary;

/**
 *	Removes the provied Binary from the entry attachments
 *	@param	binary	Binary to be removed
 */
- (void)removeBinary:(KPKBinary *)binary;

#pragma mark History

/**
 *	Adds an Item to the Entries history
 *	@param	entry	Entry element to be added as history
 */
- (void)addHistoryEntry:(KPKEntry *)entry;
- (void)addHistoryEntry:(KPKEntry *)entry atIndex:(NSUInteger)index;
- (void)removeHistoryEntry:(KPKEntry *)entry;
/**
 *	Clears the history and removes all entries
 */
- (void)clearHistory;

#pragma mark Maintainance
/**
 *  Calcualtes the size of this entry. Included are all attributes for this entry
 *
 *  @return Size of this entry in bytes
 */
- (NSUInteger)calculateByteSize;

@end

