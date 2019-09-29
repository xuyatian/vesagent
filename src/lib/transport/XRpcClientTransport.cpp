#include "XRpcClientTransport.h"
#ifndef SPDLOG_ACTIVE_LEVEL 
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif
#include "spdlog/spdlog.h"
#include "thrift/TOutput.h"

using namespace std;
using namespace vagt;
using namespace vagt::transport;
using namespace vagt::transport::rpc;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

void DummyOutput(const char* msg)
{
    SPDLOG_INFO("{}", msg);
}

vagt::transport::XRpcClientTransport::XRpcClientTransport(const XTransportOption & option):
    option_(option)
{
}

vagt::transport::XRpcClientTransport::~XRpcClientTransport()
{
}

XErrorCode vagt::transport::XRpcClientTransport::start()
{
    auto rc = startRPC();

    allowPost();

    return rc;
}

XErrorCode vagt::transport::XRpcClientTransport::stop()
{
    cancelPost();
    return stopRPC();
}

XErrorCode vagt::transport::XRpcClientTransport::post(const std::string & event)
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

    if (!rpcClient_)
    {
        SPDLOG_ERROR("RPC client is not started.");
        return XErrorNok;
    }

    try
    {
        return (XErrorCode)(rpcClient_->post(event));
    }
    catch (TTransportException& ex)
    {
        SPDLOG_ERROR("Fail to post event:({}).", ex.what());
        return XErrorNetworkError;
    }
    catch (TException& ex)
    {
        SPDLOG_ERROR("Fail to post event:({}).", ex.what());
        return XErrorNetworkError;
    }
    catch (...)
    {
        SPDLOG_ERROR("Fail to post event.");
        return XErrorNetworkError;
    }
}

XErrorCode vagt::transport::XRpcClientTransport::control(int controlFlag1,
    int controlFlag2,
    int controlFlag3,
    const std::string& controlString1,
    const std::string& controlString2,
    const std::string& controlString3)
{
    if (!rpcClient_)
    {
        SPDLOG_ERROR("RPC client is not started.");
        return XErrorNok;
    }

    try
    {
        XRpcControlField controlField;
        controlField.control1 = controlFlag1;
        controlField.control2 = controlFlag2;
        controlField.control3 = controlFlag3;
        if (!controlString1.empty()) controlField.__set_control4(controlString1);
        if (!controlString2.empty()) controlField.__set_control5(controlString2);
        if (!controlString3.empty()) controlField.__set_control6(controlString3);

        return (XErrorCode)(rpcClient_->control(controlField));
    }
    catch (TTransportException& ex)
    {
        SPDLOG_ERROR("Fail to post control command:({}).", ex.what());
        return XErrorNetworkError;
    }
    catch (TException& ex)
    {
        SPDLOG_ERROR("Fail to post control command:({}).", ex.what());
        return XErrorNetworkError;
    }
    catch (...)
    {
        SPDLOG_ERROR("Fail to post control command.");
        return XErrorNetworkError;
    }
}

void vagt::transport::XRpcClientTransport::cancelPost()
{
    XTransport::cancelPost();
}

void vagt::transport::XRpcClientTransport::allowPost()
{
    XTransport::allowPost();
}

bool vagt::transport::XRpcClientTransport::shouldCancelPost()
{
    return XTransport::shouldCancelPost();
}

XErrorCode vagt::transport::XRpcClientTransport::startRPC()
{
    GlobalOutput.setOutputFunction(DummyOutput);

    string host("127.0.0.1");
    if (!option_.host_.empty())
    {
        host = option_.host_;
    }

    int port = 5678;
    if (option_.port_ > 1024)
    {
        port = option_.port_;
    }

    SPDLOG_INFO("Connecting to RPC server:({}:{}).", host, port);

    int timeout = 1000;
    if (option_.timeOut_.count() >0)
    {
        timeout = option_.timeOut_.count();
    }

    auto socket = std::make_shared<TSocket>(host, port);
    socket->setConnTimeout(timeout);
    socket->setRecvTimeout(timeout);
    socket->setSendTimeout(timeout);
    rpcTransport_ = std::make_shared<TBufferedTransport>(socket);
    auto protocol = std::make_shared<TCompactProtocol>(rpcTransport_);
    rpcClient_ = std::make_shared<vagt::transport::rpc::XRpcTransportClient>(protocol);

    try
    {
        rpcTransport_->open();
    }
    catch (TTransportException& ex)
    {
        SPDLOG_ERROR("Fail to start rpc client:({}).", ex.what());
        return XErrorNok;
    }
    catch (TException& ex)
    {
        SPDLOG_ERROR("Fail to start rpc client:({}).", ex.what());
        return XErrorNok;
    }
    catch (...)
    {
        SPDLOG_ERROR("Fail to start rpc client.");
        return XErrorNok;
    }

    return XErrorOk;
}

XErrorCode vagt::transport::XRpcClientTransport::stopRPC()
{
    if (!rpcTransport_)
    {
        return XErrorNok;
    }

    try
    {
        rpcTransport_->close();
        rpcTransport_.reset();
    }
    catch (TTransportException& ex)
    {
        SPDLOG_ERROR("Fail to stop rpc client:({}).", ex.what());
        return XErrorNok;
    }
    catch (TException& ex)
    {
        SPDLOG_ERROR("Fail to stop rpc client:({}).", ex.what());
        return XErrorNok;
    }
    catch (...)
    {
        SPDLOG_ERROR("Fail to stop rpc client.");
        return XErrorNok;
    }
    return XErrorOk;
}
