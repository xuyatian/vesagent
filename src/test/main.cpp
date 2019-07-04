#include "EventSamples.h"
#include "TransportSamples.h"
#include "XLog.h"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    // encode
    vector<shared_ptr<XSerialable>> events = {
        buildFault(),
        buildHeartbeat(),
        buildMeasurement(),
        buildNotification(),
        buildOther(),
        buildPnfRegistration(),
        buildStateChange(),
        buildSyslog(),
        buildThresholdCrossingAlert(),
        buildXVoiceQuality(),
        buildBatch()
    };

    for (auto event : events)
    {
        event->toString();
    }

    //transport
    testLibcurlTransport();
    testRetryTransport();
    testSwitchableTransport();
    testMemBufferedTransport();
    testDiskBufferedTransport();
    testRpcClientTransport();
    testRpcServerTransport();

    return 0;
}
