#include "EventSamples.h"

shared_ptr<XSerialable> buildFault()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto fault = XFaultFields::create(header,
        "condition",
        "specProblem",
        XEnumFaultFieldsEventSourceTypeVirtualMachine,
        XEnumFaultFieldsEventSeverityNORMAL,
        XEnumFaultFieldsVfStatusActive);

    auto header2 = fault->getCommonHeader();
    header2->setSourceId("XXSourceID");

    fault->setAlarmInterfaceA("Interface");
    fault->setEventSeverity(XEnumFaultFieldsEventSeverityMAJOR);
    fault->setAlarmCondition("XXCondition");
    fault->setVfStatus(XEnumFaultFieldsVfStatusRequestingTermination);
    fault->setEventCategory("Category");
    fault->setEventSourceType(XEnumFaultFieldsEventSourceTypeVirtualMachine);
    fault->setSpecificProblem("XXProblem");
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    fault->setAlarmAdditionalInformation(hm);

    return fault;
}

shared_ptr<XSerialable> buildHeartbeat()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XHeartbeatFields::create(header, 100);
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    event->setAdditionalFields(hm);

    return event;
}

shared_ptr<XSerialable> buildMeasurement()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XMeasurementFields::create(header, 100.0);

    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };

    {
        XArrayOfXCodecsInUse arr{ { "xxx", 100 },{ "yyy", 200 } };
        event->setCodecUsageArray(arr);
    }

    event->setFeatureUsageArray(hm);
    event->setConfiguredEntities(100);

    {
        XArrayOfNamedHashMap arr{ { "name1", hm },{ "name2", hm } };
        event->setAdditionalMeasurements(arr);
    }

    {
        XArrayOfXCpuUsage arr;

        XCpuUsage u1("cpu1", 100);
        u1.setCpuUsageSystem(90);

        XCpuUsage u2("cpu2", 10);
        u2.setCpuUsageSystem(99);

        arr.push_back(u1);
        arr.push_back(u2);
        event->setCpuUsageArray(arr);
    }

    event->setRequestRate(99);

    {
        XArrayOfXFilesystemUsage arr{ { "/root", 1, 2, 3, 4, 5, 6 },{ "/tmp", 1, 2, 3, 4, 5, 6 } };
        event->setFilesystemUsageArray(arr);
    }

    {
        XArrayOfXLoad arr;

        XLoad l1;
        l1.setLongTerm(100);
        l1.setMidTerm(50);

        XLoad l2;
        l2.setLongTerm(100);
        l2.setShortTerm(10);

        arr.push_back(l1);
        arr.push_back(l2);
        event->setLoadArray(arr);
    }

    {
        XArrayOfXMachineCheckException arr{ { "Machine1" } };

        XMachineCheckException m("Machine2");
        m.setCorrectedMemoryErrors(10000);
        arr.push_back(m);

        event->setMachineCheckExceptionArray(arr);
    }

    event->setMeanRequestLatency(2000);
    event->setConcurrentSessions(300);
    event->setAdditionalFields(hm);
    {
        XArrayOfXProcessStats arr{ { "process1" } };

        XProcessStats p2("p2");
        p2.setForkRate(50);

        arr.push_back(p2);

        event->setProcessStatsArray(arr);
    }

    {
        XArrayOfXNicPerformance arr{ { "Nic1", XEnumNicPerformanceValuesAreSuspectTrue } };

        XNicPerformance n2("Nic2", XEnumNicPerformanceValuesAreSuspectFalse);
        n2.setAdministrativeState(XEnumNicPerformanceAdministrativeStateInService);
        n2.setOperationalState(XEnumNicPerformanceOperationalStateOutOfService);
        n2.setTransmittedDiscardedPacketsAccumulated(60);
        arr.push_back(n2);
        event->setNicPerformanceArray(arr);
    }

    {
        XArrayOfXDiskUsage arr{ { "d1" } };

        XDiskUsage d2("d2");
        d2.setDiskFlushTime(70);
        arr.push_back(d2);
        event->setDiskUsageArray(arr);
    }

    {
        XIpmi ipmi;

        ipmi.setSystemAirflow(100);

        XArrayOfXIpmiFan fans{ { "fan1" } };
        XIpmiFan fan("fan2");
        fan.setFanSpeed(100);
        fans.push_back(fan);
        ipmi.setIpmiFanArray(fans);

        XArrayOfXIpmiProcessor processors{ { "processor1" } };
        XIpmiProcessor processor2("processor2");
        processor2.setProcessorThermalControlPercent(90);
        processor2.setProcessorDtsThermalMargin(80);

        XArrayOfXProcessorDimmAggregateThermalMargin margins{ { "m1",1 },{ "m2",2 } };
        processor2.setProcessorDimmAggregateThermalMarginArray(margins);
        processors.push_back(processor2);
        ipmi.setIpmiProcessorArray(processors);

        ipmi.setExitAirTemperature(36.5);


        XArrayOfXIpmiBaseboardVoltageRegulator vols{ { "vol1" } };
        XIpmiBaseboardVoltageRegulator vol2("vol2");
        vol2.setVoltageRegulatorTemperature(70);
        vols.push_back(vol2);
        ipmi.setIpmiBaseboardVoltageRegulatorArray(vols);

        XArrayOfXIpmiGlobalAggregateTemperatureMargin ts{ { "t1",11 },{ "t2", 22 } };
        ipmi.setIpmiGlobalAggregateTemperatureMarginArray(ts);

        XArrayOfXIpmiNic nics{ { "nic1" } };
        XIpmiNic nic2{ "nic2" };
        nic2.setNicTemperature(333);
        nics.push_back(nic2);
        ipmi.setIpmiNicArray(nics);

        XArrayOfXIpmiBaseboardTemperature bts{ { "t1" } };
        XIpmiBaseboardTemperature bt2("b2");
        bt2.setBaseboardTemperature(55);
        bts.push_back(bt2);
        ipmi.setIpmiBaseboardTemperatureArray(bts);

        XArrayOfXIpmiPowerSupply pss{ { "p1" } };
        XIpmiPowerSupply ps2("p2");
        ps2.setPowerSupplyCurrentOutputPercent(1);
        ps2.setPowerSupplyInputPower(2);
        ps2.setPowerSupplyTemperature(3);
        pss.push_back(ps2);
        ipmi.setIpmiPowerSupplyArray(pss);

        ipmi.setIoModuleTemperature(66);

        event->setIpmi(ipmi);
    }

    {
        XArrayOfXLatencyBucketMeasure arr{ 1 };
        XLatencyBucketMeasure lat2(2);
        lat2.setCountsInTheBucket(100);
        lat2.setHighEndOfLatencyBucket(1000);
        lat2.setLowEndOfLatencyBucket(1);
        arr.push_back(lat2);

        event->setLatencyDistribution(arr);
    }

    {
        XArrayOfXHugePages arr{ { "h1" } };
        XHugePages h2("h2");
        h2.setBytesUsed(100);
        arr.push_back(h2);
        event->setHugePagesArray(arr);
    }

    event->setNumberOfMediaPortsInUse(111);
    event->setNfcScalingMetric(222);

    {
        XArrayOfXMemoryUsage arr{ { "mem1", 100, 200 } };
        XMemoryUsage mem2("mem2", 2, 3);
        mem2.setMemoryBuffered(11);
        arr.push_back(mem2);
        event->setMemoryUsageArray(arr);
    }

    {
        XArrayOfJsonObject objs;

        XArrayOfXJsonObjectInstance ins;
        XArrayOfXJsonObjectInstance ins2;

        XJsonObject obj2("obj2", ins2);

        XJsonObjectInstance ins1;
        ins1.setJsonObject(obj2);
        ins1.setObjectInstance(R"({"aa":"bb"})");
        ins1.setObjectInstanceEpochMicrosec(111);
        XArrayOfXKey keys{ { "k1" } };
        XKey k2("k2");
        k2.setKeyName("keyname");
        k2.setKeyOrder(11);
        k2.setKeyValue("value");
        keys.push_back(k2);
        ins1.setObjectKeys(keys);

        ins.push_back(ins1);

        XJsonObject obj("obj1", ins);
        objs.push_back(obj);

        event->setAdditionalObjects(objs);
    }

    return event;
}

shared_ptr<XSerialable> buildNotification()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XNotificationFields::create(header, "notif", "removed");
    event->setNewState("newState");
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    event->setAdditionalFields(hm);

    XArrayOfNamedHashMap arr{ { "name1", hm },{ "name2", hm } };
    event->setArrayOfNamedHashMap(arr);

    return event;
}

shared_ptr<XSerialable> buildOther()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XOtherFields::create(header);
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    XArrayOfNamedHashMap arr{ { "name1", hm },{ "name2", hm } };

    event->setHashMap(hm);
    event->setArrayOfNamedHashMap(arr);

    {
        XArrayOfJsonObject objs;

        XArrayOfXJsonObjectInstance ins;
        XArrayOfXJsonObjectInstance ins2;

        XJsonObject obj2("obj2", ins2);

        XJsonObjectInstance ins1;
        ins1.setJsonObject(obj2);
        ins1.setObjectInstance(R"({"aa":"bb"})");
        ins1.setObjectInstanceEpochMicrosec(111);
        XArrayOfXKey keys{ { "k1" } };
        XKey k2("k2");
        k2.setKeyName("keyname");
        k2.setKeyOrder(11);
        k2.setKeyValue("value");
        keys.push_back(k2);
        ins1.setObjectKeys(keys);

        ins.push_back(ins1);

        XJsonObject obj("obj1", ins);
        objs.push_back(obj);

        event->setJsonObjects(objs);
    }
    return event;
}

shared_ptr<XSerialable> buildPnfRegistration()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XPnfRegistrationFields::create(header);
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    event->setAdditionalFields(hm);
    event->setOamV6IpAddress("::1");
    return event;
}

shared_ptr<XSerialable> buildStateChange()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XStateChangeFields::create(header,
        XEnumStateChangeFieldsStateInService,
        XEnumStateChangeFieldsStateOutOfService,
        "interface"
        );
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    event->setAdditionalFields(hm);
    return event;
}

shared_ptr<XSerialable> buildSyslog()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    auto event = XSyslogFields::create(header,
        XEnumSyslogFieldsEventSourceTypePort,
        "tag",
        "message"
        );
    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    event->setAdditionalFields(hm);
    event->setSyslogSev(XEnumSyslogFieldsSyslogSevInfo);
    return event;
}

shared_ptr<XSerialable> buildThresholdCrossingAlert()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    XHashMap hm{ { "xxx","123" },{ "yyy","456" } };
    XArrayOfXCounter counters{ { XEnumCounterCriticalityMAJ, hm, "thres1" } };
    auto event = XThresholdCrossingAlertFields::create(header,
        XEnumThresholdCrossingAlertFieldsAlertActionSET,
        XEnumThresholdCrossingAlertFieldsAlertTypeINTERFACE_ANOMALY,
        XEnumThresholdCrossingAlertFieldsEventSeverityMINOR,
        "description",
        "collect time",
        "start time",
        counters
        );
    event->setAdditionalFields(hm);
    event->setAssociatedAlertIdList({ { "xxx" },{ "yyy" } });

    return event;
}

shared_ptr<XSerialable> buildXVoiceQuality()
{
    auto header = XCommonEventHeader::create(
        "EventName",
        "EventId",
        "SourceName",
        "ReportName",
        XEnumCommonEventHeaderPriorityNormal,
        111
        );

    XVendorNfNameFields vendor("testVendor");

    auto event = XVoiceQualityFields::create(header,
        "calleeSideCodec",
        "callerSideCodec",
        "correlator",
        "midCallRtcp",
        vendor
        );
    
    XEndOfCallVqmSummaries summaries("summeries", XEnumEndOfCallVqmSummariesEndpointDescriptionCallee);
    event->setEndOfCallVqmSummaries(summaries);

    return event;
}

shared_ptr<XSerialable> buildBatch()
{
    auto batch = XBatch::create();
    batch->addEvent(buildFault());
    batch->addEvent(buildSyslog());
    return batch;
}
