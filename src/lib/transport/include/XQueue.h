#pragma once

#include <string>
#include <memory>

namespace vagt
{
    namespace queue
    {
        enum XErrorCode : unsigned short
        {
            XErrorOk,
            XErrorNok,
            XErrorFull,
            XErrorEmpty,
        };

        class XQueue
        {
        public:
            virtual bool empty() = 0;
            virtual XErrorCode push(const std::string& val) = 0;
            virtual void pop() = 0;
            virtual std::string front() = 0;

            static std::shared_ptr<XQueue> create(int capacity);
            static std::shared_ptr<XQueue> create(const std::string& path);
        };
    }
}