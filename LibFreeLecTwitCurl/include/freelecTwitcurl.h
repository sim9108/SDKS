#ifndef _TWITCURL_H_
#define _TWITCURL_H_



#include <freelecconfig.h>

#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include "oauthlib.h"
#include "curl/curl.h"
		
#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

#include <cstdio>

/* Few common types used by twitCurl */
namespace twitCurlTypes
{
    typedef enum _eTwitCurlApiFormatType
    {
        eTwitCurlApiFormatJson = 0,
        eTwitCurlApiFormatXml,
        eTwitCurlApiFormatMax
    } eTwitCurlApiFormatType;

    typedef enum _eTwitCurlProtocolType
    {
        eTwitCurlProtocolHttps = 0,
        eTwitCurlProtocolHttp,
        eTwitCurlProtocolMax
    } eTwitCurlProtocolType;
};

/* twitCurl class */
class FREELECTWITCURL_API freelecTwitCurl
{
public:
    freelecTwitCurl();
    ~freelecTwitCurl();

    /* Twitter OAuth authorization methods */
    oAuth& getOAuth();
    bool oAuthRequestToken( std::string& authorizeUrl /* out */ );
    bool oAuthAccessToken();
    bool oAuthHandlePIN( const std::string& authorizeUrl /* in */ );

    /* Twitter login APIs, set once and forget */
    std::string& getTwitterUsername();
    std::string& getTwitterPassword();
    void setTwitterUsername( std::string& userName /* in */ );
    void setTwitterPassword( std::string& passWord /* in */ );
    
    // 7장 때문에 추가
    void setTwitterUsername(const char * userName /* in */);
    void setTwitterPassword(const char * passWord /* in */);

    /* Twitter search APIs */
    bool search( std::string& searchQuery /* in */, std::string resultCount = "" /* in */ );

    /* Twitter status APIs */
    bool statusUpdate( std::string& newStatus /* in */, std::string inReplyToStatusId = "" /* in */ );
    bool statusShowById( std::string& statusId /* in */ );
    bool statusDestroyById( std::string& statusId /* in */ );
    bool retweetById( std::string& statusId /* in */ );

    /* Twitter timeline APIs */
    bool timelineHomeGet( std::string sinceId = ""  /* in */ );
    bool timelinePublicGet();
    bool timelineFriendsGet();
    bool timelineUserGet( bool trimUser /* in */, bool includeRetweets /* in */,
                          unsigned int tweetCount /* in */,
                          std::string userInfo = "" /* in */,
                          bool isUserId = false /* in */ );
    bool featuredUsersGet();
    bool mentionsGet( std::string sinceId = "" /* in */ );

    /* Twitter user APIs */
    bool userLookup( std::vector<std::string> &userInfo /* in */,  bool isUserId = false /* in */ );

    // 7장 때문에 추가
    bool userLookup(std::string userInfo /* in */, bool isUserId = false /* in */);
    bool userGet( std::string& userInfo /* in */, bool isUserId = false /* in */ );
    bool friendsGet( std::string userInfo = "" /* in */, bool isUserId = false /* in */ );
    bool followersGet( std::string userInfo = "" /* in */, bool isUserId = false /* in */ );

    /* Twitter direct message APIs */
    bool directMessageGet( std::string sinceId = "" /* in */ );
    bool directMessageSend( std::string& userInfo /* in */, std::string& dMsg /* in */, bool isUserId = false /* in */ );
    bool directMessageGetSent();
    bool directMessageDestroyById( std::string& dMsgId /* in */ );

    /* Twitter friendships APIs */
    bool friendshipCreate( std::string& userInfo /* in */, bool isUserId = false /* in */ );
    bool friendshipDestroy( std::string& userInfo /* in */, bool isUserId = false /* in */ );
    bool friendshipShow( std::string& userInfo /* in */, bool isUserId = false /* in */ );

    /* Twitter social graphs APIs */
    bool friendsIdsGet( std::string& nextCursor /* in */,
                        std::string& userInfo /* in */, bool isUserId = false /* in */ );
    bool followersIdsGet( std::string& nextCursor /* in */,
                          std::string& userInfo /* in */, bool isUserId = false /* in */ );

    // 7장 때문에 추가
    bool friendsIdsGet(const char *nextCursor/* in */, const char* userInfo /* in */, bool isUserId = false /* in */);

    /* Twitter account APIs */
    bool accountRateLimitGet();
    bool accountVerifyCredGet();

    /* Twitter favorites APIs */
    bool favoriteGet();
    bool favoriteCreate( std::string& statusId /* in */ );
    bool favoriteDestroy( std::string& statusId /* in */ );

    /* Twitter block APIs */
    bool blockCreate( std::string& userInfo /* in */ );
    bool blockDestroy( std::string& userInfo /* in */ );
    bool blockListGet( std::string& nextCursor /* in */,
                        bool includeEntities /* in */, bool skipStatus /* in */ );
    bool blockIdsGet( std::string& nextCursor /* in */, bool stringifyIds /* in */ );

    /* Twitter search APIs */
    bool savedSearchGet();
    bool savedSearchCreate( std::string& query /* in */ );
    bool savedSearchShow( std::string& searchId /* in */ );
    bool savedSearchDestroy( std::string& searchId /* in */ );

    /* Twitter trends APIs (JSON) */
    bool trendsGet();
    bool trendsDailyGet();
    bool trendsWeeklyGet();
    bool trendsCurrentGet();
    bool trendsAvailableGet();

    /* cURL APIs */
    bool isCurlInit();
    void getLastWebResponse( std::string& outWebResp /* out */ );
    void getLastCurlError( std::string& outErrResp /* out */);

    /* Internal cURL related methods */
    int saveLastWebResponse( char*& data, size_t size );

    /* cURL proxy APIs */
    std::string& getProxyServerIp();
    std::string& getProxyServerPort();
    std::string& getProxyUserName();
    std::string& getProxyPassword();
    void setProxyServerIp( std::string& proxyServerIp /* in */ );
    void setProxyServerPort( std::string& proxyServerPort /* in */ );
    void setProxyUserName( std::string& proxyUserName /* in */ );
    void setProxyPassword( std::string& proxyPassword /* in */ );
    
    /* Clones this object */
    freelecTwitCurl* clone();

    /* Perform auth*/
    bool performAuth();

    /* Parsing */
    bool getLastWebResponseParse();

    rapidjson::Document getParseDocument(){
        return m_document;
    }

    std::string friendsIdsParse(std::string objectname);
    std::string userLookupParse(std::string objectname);

    // 7장 때문에 추가
private:
    /* rapidjson object */
    rapidjson::Document m_document;

    /* cURL data */
    CURL* m_curlHandle;
    char* m_errorBuffer;
    std::string m_callbackData;  // Web response저장 되는곳

    /* cURL flags */
    bool m_curlProxyParamsSet;  
    bool m_curlLoginParamsSet;
    bool m_curlCallbackParamsSet;

    /* cURL proxy data */
    std::string m_proxyServerIp;
    std::string m_proxyServerPort;
    std::string m_proxyUserName;
    std::string m_proxyPassword;

    /* Twitter data */
    std::string m_twitterUsername;
    std::string m_twitterPassword;

    /* Twitter API type */
    twitCurlTypes::eTwitCurlApiFormatType m_eApiFormatType;
    twitCurlTypes::eTwitCurlProtocolType m_eProtocolType;

    /* OAuth data */
    oAuth m_oAuth;

    /* Private methods */
    void clearCurlCallbackBuffers();
    void prepareCurlProxy();
    void prepareCurlCallback();
    void prepareCurlUserPass();
    void prepareStandardParams();
    bool performGet( const std::string& getUrl );
    bool performGetInternal( const std::string& getUrl,
                             const std::string& oAuthHttpHeader );
    bool performDelete( const std::string& deleteUrl );
    bool performPost( const std::string& postUrl, std::string dataStr = "" );

    /* Internal cURL related methods */
    static int curlCallback(char* data, size_t size, size_t nmemb, freelecTwitCurl* pTwitCurlObj);
};







/* Private functions */
FREELECTWITCURL_API void utilMakeCurlParams( std::string& outStr, std::string& inParam1, std::string& inParam2 );
FREELECTWITCURL_API void utilMakeUrlForUser( std::string& outUrl, const std::string& baseUrl, std::string& userInfo, bool isUserId );

#endif // _TWITCURL_H_
