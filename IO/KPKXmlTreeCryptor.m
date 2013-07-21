//
//  KPKXmlDataCryptor.m
//  MacPass
//
//  Created by Michael Starke on 21.07.13.
//  Copyright (c) 2013 HicknHack Software GmbH. All rights reserved.
//

#import "KPKXmlTreeCryptor.h"
#import "KPKXmlHeaderReader.h"
#import "KPKPassword.h"
#import "KPKVersion.h"
#import "KPKErrors.h"
#import "KPKFormat.h"

#import "NSData+CommonCrypto.h"
#import "NSData+HashedData.h"
#import "NSData+Gzip.h"

#import "KPKXmlTreeReader.h"

#import <CommonCrypto/CommonCryptor.h>

@interface KPKXmlTreeCryptor () {
  KPKXmlHeaderReader *_cipherInfo;
}

@end

@implementation KPKXmlTreeCryptor

- (id)initWithData:(NSData *)data password:(KPKPassword *)password {
  self = [super init];
  if(self) {
    _data = data;
    _password = password;
  }
  return self;
}

- (KPKTree *)decryptTree:(NSError *__autoreleasing *)error {
  _cipherInfo = [[KPKXmlHeaderReader alloc] initWithData:_data error:error];
  if(_cipherInfo) {
    return nil;
  }
  
  /*
   Create the Key
   Supply the Data found in the header
   */
  NSData *keyData = [_password finalDataForVersion:KPKVersion2
                                        masterSeed:_cipherInfo.masterSeed
                                     transformSeed:_cipherInfo.transformSeed
                                            rounds:_cipherInfo.rounds];
  
  /*
   The datastream is AES encrypted. Decrypt using the supplied
   */
  NSData *aesDecrypted = [[_cipherInfo dataWithoutHeader] decryptedDataUsingAlgorithm:kCCAlgorithmAES128
                                                                                     key:keyData
                                                                    initializationVector:_cipherInfo.encryptionIV
                                                                                 options:kCCOptionPKCS7Padding
                                                                                   error:NULL];
  /*
   Compare the first Streambytes with the ones stores in the header
   */
  NSData *startBytes = [aesDecrypted subdataWithRange:NSMakeRange(0, 32)];
  if(![_cipherInfo.streamStartBytes isEqualToData:startBytes]) {
    KPKCreateError(error, KPKErrorIntegrityCheckFaild, @"ERROR_INTEGRITY_CHECK_FAILED", "");
    return nil;
  }
  /*
   The Stream is Hashed, read the data and verify it.
   If the Stream was Gzipped, uncrompress it.
   */
  NSData *unhashedData = [[aesDecrypted subdataWithRange:NSMakeRange(32, [aesDecrypted length] - 32)] unhashedData];
  if(_cipherInfo.compressionAlgorithm == KPKCompressionGzip) {
    unhashedData = [unhashedData gzipInflate];
  }
  
  if(!unhashedData) {
    KPKCreateError(error, KPKErrorIntegrityCheckFaild, @"ERROR_INTEGRITY_CHECK_FAILED", "");
    return nil;
  }
  //  tree.rounds = rounds;
  //  tree.compressionAlgorithm = compressionAlgorithm;
  KPKXmlTreeReader *reader = [[KPKXmlTreeReader alloc] initWithData:_data cipherInformation:_cipherInfo];
  return [reader tree];
}

@end