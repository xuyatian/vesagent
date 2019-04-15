#pragma once

#include <memory>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include "XQueue.h"

namespace vagt 
{
    namespace transport
    {
        enum XErrorCode: unsigned short
        {
            XErrorOk,
            XErrorNok,
            XErrorTimeout,
            XErrorUnauthorized,
            XErrorCanceled,
            XErrorClientError,
            XErrorServerError,
            XErrorNetworkError,
        };

        class XTransportOption
        {
        public:
            std::string host_;
            int port_;
            std::chrono::milliseconds timeOut_;
            std::string url_;
            std::string sourceIp_;
            bool secure_;
            std::string caInfo_;
            std::string caFilePath_;
            std::string certFilePath_;
            std::string keyFilePath_;
            long verifyPeer_;
            long verifyHost_;
            std::string userName_;
            std::string userPasswd_;

            XTransportOption():secure_(false),verifyPeer_(1),verifyHost_(2)
            {}
        };

        class XTransport
        {
        public:
            XTransport();

            virtual XErrorCode start() = 0;

            virtual XErrorCode stop() = 0;

            virtual XErrorCode post(const std::string& event) = 0;

            virtual void cancelPost();

            virtual void allowPost();

            virtual bool shouldCancelPost();
 
            static std::shared_ptr<XTransport> LibCurlTransport(const XTransportOption& option);

            static std::shared_ptr<XTransport> RetryTransport(std::shared_ptr<XTransport> transport,
                std::chrono::milliseconds retryInterval, 
                int retryTimes);

            static std::shared_ptr<XTransport> SwitchableTransport(std::vector<std::shared_ptr<XTransport>>& transports);

            static std::shared_ptr<XTransport> SynchronizedTransport(std::shared_ptr<XTransport> transport);

            static std::shared_ptr<XTransport> BufferedTransport(std::shared_ptr<XTransport> transport, std::shared_ptr<vagt::queue::XQueue> queue);

            static std::shared_ptr<XTransport> RpcClientTransport(const XTransportOption& option);

            static std::shared_ptr<XTransport> RpcServerTransport(std::shared_ptr<XTransport> transport, const XTransportOption& option);

        protected:
           std::atomic<bool> cancel_;

        private:
            XTransport(const XTransport&) = delete;
            XTransport& operator=(const XTransport&) = delete;
        };
    }
}
