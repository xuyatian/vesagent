#include "XLibcurlTransport.h"
#ifndef SPDLOG_ACTIVE_LEVEL 
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif
#include "spdlog/spdlog.h"

using namespace std;
using namespace vagt;
using namespace vagt::transport;
using namespace spdlog;

char XLibcurlTransport::curlErrorBuf_[CURL_ERROR_SIZE] = { 0 };

struct XexpremoryStuct
{
    char * memory;
    size_t size;
};

XLibcurlTransport::XLibcurlTransport(const XTransportOption& option):
    option_(option),
    curl_(nullptr),
    hdr_(nullptr)
{
}

XErrorCode XLibcurlTransport::start()
{
    auto rc = initLibcurl();

    allowPost();

    return rc;
}

XErrorCode XLibcurlTransport::stop()
{
    cancelPost();
    return cleanupLibcurl();
}

XErrorCode XLibcurlTransport::post(const std::string& event)
{
    if (shouldCancelPost())
    {
        return XErrorCanceled;
    }

    if (event.empty())
    {
        SPDLOG_WARN("Trying post empty event.");
        return XErrorClientError;
    }
    return curlPost(event);
}

XErrorCode XLibcurlTransport::initLibcurl()
{
    auto rc = curl_global_init(CURL_GLOBAL_SSL);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl:({}).", rc);
        return XErrorNok;
    }

    curl_ = curl_easy_init();
    if (!curl_)
    {
        SPDLOG_ERROR("Fail to get libcurl handle.");
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_ERRORBUFFER, curlErrorBuf_);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to set libcurl error message buffer:({}).", rc);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_URL, option_.url_.c_str());
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to set libcurl url {}:({}) {}.", option_.url_, rc, curlErrorBuf_);
        return XErrorNok;
    }

    if (!option_.sourceIp_.empty())
    {
        rc = curl_easy_setopt(curl_, CURLOPT_INTERFACE, option_.sourceIp_.c_str());
        if (rc != CURLE_OK)
        {
            SPDLOG_ERROR("Fail to bind interface {} to libcurl:({}) {}.", option_.sourceIp_, rc, curlErrorBuf_);
            return XErrorNok;
        }
    }

    if (option_.secure_)
    {
        if (!option_.certFilePath_.empty())
        {
            rc = curl_easy_setopt(curl_, CURLOPT_SSLCERT, option_.certFilePath_.c_str());
            if (rc != CURLE_OK)
            {
                SPDLOG_ERROR("Fail to init libcurl with client cert:({}) {}.", rc, curlErrorBuf_);
                return XErrorNok;
            }
        }

        if (!option_.keyFilePath_.empty())
        {
            rc = curl_easy_setopt(curl_, CURLOPT_SSLKEY, option_.keyFilePath_.c_str());
            if (rc != CURLE_OK)
            {
                SPDLOG_ERROR("Fail to init libcurl with client key:({}) {}.", rc, curlErrorBuf_);
                return XErrorNok;
            }
        }

        if (!option_.caInfo_.empty())
        {
            rc = curl_easy_setopt(curl_, CURLOPT_CAINFO, option_.caInfo_.c_str());
            if (rc != CURLE_OK)
            {
                SPDLOG_ERROR("Fail to init libcurl with CA cert:({}) {}.", rc, curlErrorBuf_);
                return XErrorNok;
            }
        }

        if (!option_.caFilePath_.empty())
        {
            rc = curl_easy_setopt(curl_, CURLOPT_CAPATH, option_.caFilePath_.c_str());
            if (rc != CURLE_OK)
            {
                SPDLOG_ERROR("Fail to init libcurl with CA cert path:({}) {}.", rc, curlErrorBuf_);
                return XErrorNok;
            }
        }

        rc = curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, option_.verifyPeer_);
        if (rc != CURLE_OK)
        {
            SPDLOG_ERROR("Fail to init libcurl with SSL server verification:({}){}.", rc, curlErrorBuf_);
            return XErrorNok;
        }

        rc = curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, option_.verifyHost_);
        if (rc != CURLE_OK)
        {
            SPDLOG_ERROR("Fail to init libcurl with client host verification:({}) {}.", rc, curlErrorBuf_);
            return XErrorNok;
        }
    }

    rc = curl_easy_setopt(curl_, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with user agent:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_POST, 1L);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with regular post:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_READFUNCTION, readCallback);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with read callback:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    hdr_ = curl_slist_append(hdr_, "Content-type: application/json");
    hdr_ = curl_slist_append(hdr_, "Expect:");

    rc = curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, hdr_);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl to use custom headers:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_TIMEOUT, option_.timeOut_.count());
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with timeout:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with Baisc Authentication:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_USERNAME, option_.userName_.c_str());
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with user name:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_PASSWORD, option_.userPasswd_.c_str());
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with user password:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    return XErrorOk;
}

XErrorCode XLibcurlTransport::cleanupLibcurl()
{
    if (curl_)
    {
        curl_easy_cleanup(curl_);
        curl_ = nullptr;
    }

    if (hdr_)
    {
        curl_slist_free_all(hdr_);
        hdr_ = nullptr;
    }

    curl_global_cleanup();

    return XErrorOk;
}

XErrorCode XLibcurlTransport::curlPost(const string & body)
{
    XexpremoryStuct tXexprem;

    tXexprem.memory = (char*)body.c_str();
    tXexprem.size = body.size();

    auto rc = curl_easy_setopt(curl_, CURLOPT_READDATA, &tXexprem);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to init libcurl with read data:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    rc = curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, tXexprem.size);
    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Fail to set length of post data for libcurl:({}) {}.", rc, curlErrorBuf_);
        return XErrorNok;
    }

    SPDLOG_DEBUG("Libcurl posting event:{}.", body);
    rc = curl_easy_perform(curl_);

    if (rc != CURLE_OK)
    {
        SPDLOG_ERROR("Libcurl fails to post event:({}) {}.", rc, curlErrorBuf_);
        if (rc == CURLE_OPERATION_TIMEDOUT)
        {
            return XErrorTimeout;
        }
        return XErrorNok;
    }

    int httpResponseCode = 0;
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &httpResponseCode);
    SPDLOG_DEBUG("Post event:{} ({}).", body, httpResponseCode);

    if ((httpResponseCode / 100) == 2)
    {
        return XErrorOk;
    }

    if (httpResponseCode == 401)
    {
        return XErrorUnauthorized;
    }

    if ((httpResponseCode / 100) == 4)
    {
        return XErrorClientError;
    }

    if ((httpResponseCode / 100) == 5)
    {
        return XErrorServerError;
    }

    return XErrorNok;
}

size_t XLibcurlTransport::readCallback(void * ptr, size_t size, size_t nmemb, void * userp)
{
    size_t rtn = 0;
    size_t bytesToWrite = 0;
    XexpremoryStuct* tXexprem = (XexpremoryStuct*)userp;

    bytesToWrite = std::min(size*nmemb, tXexprem->size);

    if (bytesToWrite > 0)
    {
        strncpy((char *)ptr, tXexprem->memory, bytesToWrite);
        tXexprem->memory += bytesToWrite;
        tXexprem->size -= bytesToWrite;
        rtn = bytesToWrite;
    }
    return rtn;
}

size_t XLibcurlTransport::writeCallback(void * ptr, size_t size, size_t nmemb, void * userp)
{
    return size_t();
}

