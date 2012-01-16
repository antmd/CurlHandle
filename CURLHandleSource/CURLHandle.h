//
//  CURLHandle.h
//
//  Created by Dan Wood <dwood@karelia.com> on Fri Jun 22 2001.
//  This is in the public domain, but please report any improvements back to the author.
//
//	The current version of CURLHandle is 1.9.
//

#import <Foundation/Foundation.h>
#import <curl/curl.h>

	/*" The cache has been removed "*/
extern	NSString *CURLHandleCacheDeleteNotification;

	/*" The cache has been created "*/
extern	NSString *CURLHandleCacheCreateNotification;

	/*" The cache has been changed "*/
extern	NSString *CURLHandleCacheChangeNotification;

	/*" A handle has been created; the object is the handle itself. "*/

extern	NSString *CURLHandleCreatedNotification;

	/*" Cache of URL contents, keyed by URL "*/
extern 	NSMutableDictionary *sCurlCache;

	/*" Set of URLs that CURLHandle will process "*/
extern	NSMutableSet		*sAcceptedURLs;

	/*" YES if CURLHandle will accept %all HTTP "*/
extern	BOOL				sAcceptAllHTTP;

/*" YES if CURLHandle will allow use of a proxy server "*/
extern	BOOL				sAllowsProxy;

/*" Proxy User ID:Password combo for all uses of CURL. "*/
extern NSString				*sProxyUserIDAndPassword;

/*"	Callbacks from reading a chunk of data.  Since we pass "self" in as the "data pointer",
	we can use that to get back into Objective C and do the work with the class.
"*/

extern size_t curlBodyFunction(void *ptr, size_t size, size_t nmemb, void *inSelf);
extern size_t curlHeaderFunction(void *ptr, size_t size, size_t nmemb, void *inSelf);

@interface CURLHandle : NSURLHandle
{
	NSThread		*mMainThread;			/*" Reference to main thread so thread can determine if it's a background thread or not "*/
	CURL			*mCURL;					/*" Pointer to the actual CURL object that does all the hard work "*/
	char 			mErrorBuffer[CURL_ERROR_SIZE];	/*" Buffer to hold string generated by CURL; this is then converted to an NSString. "*/
	int				mResult;				/*" Result after performing a CURL operation; it is displayed as an error code in case there was no error string generated. "*/

	NSURLRequest    *_request;

	NSMutableData	*mHeaderBuffer;			/*" The buffer that is filled with data from the header as the download progresses; it's appended to one line at a time. "*/

	NSHTTPURLResponse   *_response;			/*" The header buffer, converted into a response, upon demand. "*/

	NSMutableDictionary	*mStringOptions;	/*" Dictionary of keys(ints) & values (NSStrings) for performing curl_easy_setopt.  We store the options in a dictionary and then invoke #curl_easy_setopt on each option right before the #curl_easy_perform so that we can retain their memory until it is needed."*/

	NSDictionary	*mProxies;	/*" Dictionary of proxy information; it's released when the handle is deallocated since it's needed for the transfer."*/

	id					mProgressIndicator;		/*" A progress indicator, to animate during foreground loads.  This will help give some indication of loading progress, though of course you're better off loading in the background. "*/

	// Backgrounding support
	NSPort			*mPort;						/*" A port for communicating between the background thread and the foreground thread. "*/

	BOOL			mAbortBackground;		/*" A flag that is set by the foreground thread and read by the background thread; it's an indicator that the user has cancelled. "*/

	FILE *mPutFile;  /*" The FILE stream if putFile: is used.  It's only saved so it can be closed after perform "*/

}

/*" CURLHandle-specific interfaces. "*/

+ (void) curlGoodbye;
+ (void) curlHelloSignature:(NSString *) inSignature acceptAll:(BOOL)inAcceptAllHTTP;
+ (void)curlAcceptURL:(NSURL *)url;
+ (void)curlFlushEntireCache;
- (CURL *) curl;
- (void) setString:(NSString *)inString forKey:(CURLoption) inCurlOption;
- (void) setStringOrNumberObject:(id)inString forKey:(CURLoption) inCurlOption;
- (NSURL *)url;
+ (void) setProxyUserIDAndPassword:(NSString *)inString;
+ (void) setAllowsProxy:(BOOL) inBool;
- (void) setPutFile:(NSString *)path;
- (void) setPutFileOffset:(int)offset;
- (void) setPutFile:(NSString *)path resumeUploadFromOffset:(off_t)offset_;
+ (NSString *) curlVersion;

/*" NSURLRequest-based API "*/
- (void)setRequest:(NSURLRequest *)request;
- (NSHTTPURLResponse *)response;

/*" NSURLHandle overrides "*/

+ (BOOL)canInitWithURL:(NSURL *)anURL;
+ (NSURLHandle *)cachedHandleForURL:(NSURL *)anURL;
- (NSData *)loadInForeground;
- (NSString *)curlError;
- (id) initWithURL:(NSURL *)anURL cached:(BOOL)willCache;
- (id)propertyForKey:(NSString *)propertyKey;
- (id)propertyForKeyIfAvailable:(NSString *)propertyKey;
- (void) dealloc;
- (void)beginLoadInBackground;
- (void)cancelLoadInBackground;
- (void)endLoadInBackground;

/*" Support Methods "*/

- (size_t) curlWritePtr:(void *)inPtr size:(size_t)inSize number:(size_t)inNumber message:(int)inMessageID;
- (void) curlThreadBackgroundLoad:(id)inParam;
- (void) prepareAndPerformCurl;
- (void)handlePortMessage:(NSPortMessage *)portMessage;
//- (NSString *)headerString;

@end

@interface NSDictionary ( CurlHTTPExtensions )

- (NSString *) formatForHTTP;
- (NSString *) formatForHTTPUsingEncoding:(NSStringEncoding)inEncoding;
- (NSString *) formatForHTTPUsingEncoding:(NSStringEncoding)inEncoding ordering:(NSArray *)inOrdering;

@end


