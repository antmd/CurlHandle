//
//  CURLHandle+MultiSupport.h
//  CURLHandle
//
//  Created by Sam Deane on 27/03/2013.
//  Copyright (c) 2013 Karelia Software. All rights reserved.

#import "CURLHandle.h"


/**
 Private API used by CURLMulti.
 Not exported in the framework, and not recommended for general use.
 */

@interface CURLHandle(MultiSupport)

/** @name Internal Methods */

/**
 The CURL handle managed by this object.

 @warning Not intended for general use.

 @return The curl handle.

 */

- (CURL*)curl;

/**
 Called by <CURLMulti> to tell the handle that it has completed.
 
 @param code The completion code.
 @param isMultiCode Is the code a result from the handle itself, or the multi managing it?
 
 @warning Not intended for general use.

 */

- (void)completeWithCode:(NSInteger)code isMulti:(BOOL)isMultiCode;

/**
 Has the handle completed?
 
 @return YES if the handle has completed.
 
 @warning Not intended for general use.

 */

- (BOOL)hasCompleted;

/**
 Called by <CURLMulti> when it stops managing the handle.
 
 @param multi The multi object that was managing this handle.
 
 @warning Not intended for general use.

 */

- (void)removedByMulti:(CURLMulti*)multi;

@end

