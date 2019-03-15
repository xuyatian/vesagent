#include "EventSamples.h"
#include "TransportSamples.h"
#include "XLog.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void(*testFunctions[])() = {
    testLibcurlTransport,
    testRetryTransport,
    testSwitchableTransport,
    testMemBufferedTransport,
    testDiskBufferedTransport,
    testRpcClientTransport,
    testRpcServerTransport
};

int main(int argc, char** argv)
{
    vagt::log::init("xsample.log");
    vagt::log::setLevel(vagt::log::XLogDebug);

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << "0~6" << endl;
        cout << "0: testLibcurlTransport" << endl;
        cout << "1: testRetryTransport" << endl;
        cout << "2: testSwitchableTransport" << endl;
        cout << "3: testMemBufferedTransport" << endl;
        cout << "4: testDiskBufferedTransport" << endl;
        cout << "5: testRpcClientTransport" << endl;
        cout << "6: testRpcServerTransport" << endl;
        return 1;
    }

    int idx = atoi(argv[1]);

    testFunctions[idx]();

    return 0;
}
