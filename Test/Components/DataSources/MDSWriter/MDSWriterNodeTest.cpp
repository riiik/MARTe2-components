/**
 * @file MDSWriterNodeTest.cpp
 * @brief Source file for class MDSWriterNodeTest
 * @date 16/02/2017
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class MDSWriterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "MDSWriterNode.h"
#include "MDSWriterNodeTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#if 0
/**
 * @brief GAM which generates a given signal trigger, time and signal pattern which is then sinked to the MDSWriter
 */
class MDSWriterGAMTriggerTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()MDSWriterGAMTriggerTestHelper() {
        counter = 0;
        period = 2;
        numberOfExecutes = 0;
        signalToGenerate = NULL;
        triggerToGenerate = NULL;
        elements = NULL;
    }

    virtual ~MDSWriterGAMTriggerTestHelper() {
        using namespace MARTe;
        if (signalToGenerate != NULL) {
            delete [] signalToGenerate;
        }
        if (triggerToGenerate != NULL) {
            delete [] triggerToGenerate;
        }
        if (elements != NULL) {
            delete [] elements;
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        using namespace MARTe;
        bool ok = GAM::Initialise(data);
        AnyType signalAT = data.GetType("Signal");
        numberOfExecutes = signalAT.GetNumberOfElements(0);
        signalToGenerate = new uint32[numberOfExecutes];
        triggerToGenerate = new uint8[numberOfExecutes];
        Vector<uint32> signalV(signalToGenerate, numberOfExecutes);
        data.Read("Signal", signalV);
        Vector<uint8> triggerV(triggerToGenerate, numberOfExecutes);
        data.Read("Trigger", triggerV);
        data.Read("Period", period);
        return ok;
    }

    virtual bool Setup() {
        using namespace MARTe;
        elements = new uint32[GetNumberOfOutputSignals()];
        uint32 n;
        for (n=0; n<GetNumberOfOutputSignals(); n++) {
            uint32 el;
            GetSignalNumberOfElements(OutputSignals, n, el);
            elements[n] = el;
        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        if (counter < numberOfExecutes) {
            uint32 n;
            uint8 *trigger = reinterpret_cast<uint8 *>(GetOutputSignalMemory(0));
            *trigger = triggerToGenerate[counter];
            uint32 *timeSignal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(1));
            *timeSignal = (counter * (period * 1e6) * elements[2]);
            for (n=2; n<GetNumberOfOutputSignals(); n++) {
                TypeDescriptor signalType = GetSignalType(OutputSignals, n);
                if (signalType == UnsignedInteger16Bit) {
                    uint16 *signalOut = reinterpret_cast<uint16 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<uint16>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == UnsignedInteger32Bit) {
                    uint32 *signalOut = reinterpret_cast<uint32 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<uint32>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == UnsignedInteger64Bit) {
                    uint64 *signalOut = reinterpret_cast<uint64 *>(GetOutputSignalMemory(n));
                    uint64 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<uint64>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == SignedInteger16Bit) {
                    int16 *signalOut = reinterpret_cast<int16 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<int16>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == SignedInteger32Bit) {
                    int32 *signalOut = reinterpret_cast<int32 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<int32>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == SignedInteger64Bit) {
                    int64 *signalOut = reinterpret_cast<int64 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<int64>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == Float32Bit) {
                    float32 *signalOut = reinterpret_cast<float32 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<float32>(signalToGenerate[counter]);
                    }
                }
                else if (signalType == Float64Bit) {
                    float64 *signalOut = reinterpret_cast<float64 *>(GetOutputSignalMemory(n));
                    uint32 e;
                    for (e=0; e<elements[n]; e++) {
                        signalOut[e] = static_cast<float64>(signalToGenerate[counter]);
                    }
                }
            }

        }
        counter++;
        return true;
    }

    MARTe::uint8 *triggerToGenerate;
    MARTe::uint32 *signalToGenerate;
    MARTe::uint32 *elements;
    MARTe::uint32 counter;
    MARTe::uint32 numberOfExecutes;
    MARTe::float32 period;
};
CLASS_REGISTER(MDSWriterGAMTriggerTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the MDSWriter and the GAMs
 */
class MDSWriterSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

    MDSWriterSchedulerTestHelper () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ExecuteSingleCycle(scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].executables,
                scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler() {

        bool ret = GAMSchedulerI::ConfigureScheduler();
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {
    }

private:

    MARTe::ScheduledState * const * scheduledStates;
};

CLASS_REGISTER(MDSWriterSchedulerTestHelper, "1.0")

static bool TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    if (destroy) {
        god->Purge();
    }
    return ok;
}

template<typename typeToCheck> static bool CheckSegmentData(MARTe::int32 numberOfSegments, MDSplus::TreeNode *node, MARTe::uint32 *signalToVerify,
        MARTe::uint32 *timeToVerify) {
    using namespace MARTe;
    int32 i = 0u;
    int32 s;
    bool ok = true;
    typeToCheck typeDiscoverTmp;
    AnyType typeDiscover(typeDiscoverTmp);
    for (s = 0u; (s < numberOfSegments) && (ok); s++) {
        int32 numberOfElements;
        MDSplus::Array *segment = node->getSegment(s);
        typeToCheck *data;
        MDSplus::Data *segTimeD = node->getSegmentDim(s);
        uint32 *segTime;
        segTime = segTimeD->getIntUnsignedArray(&numberOfElements);

        if (typeDiscover.GetTypeDescriptor() == UnsignedInteger16Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getShortUnsignedArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == UnsignedInteger32Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getIntUnsignedArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == UnsignedInteger64Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getLongUnsignedArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == SignedInteger16Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getShortArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == SignedInteger32Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getIntArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == SignedInteger64Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getLongArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == Float32Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getFloatArray(&numberOfElements));
        }
        else if (typeDiscover.GetTypeDescriptor() == Float64Bit) {
            data = reinterpret_cast<typeToCheck *>(segment->getDoubleArray(&numberOfElements));
        }
        int32 e;
        for (e = 0; (e < numberOfElements) && (ok); e++) {
            ok = (static_cast<typeToCheck>(signalToVerify[i]) == static_cast<typeToCheck>(data[e]));
            ok &= (timeToVerify[i] == segTime[e]);
            i++;
        }
        deleteData(segment);
        deleteData(segTimeD);
    }
    return ok;
}

static bool TestIntegratedExecution(const MARTe::char8 * const config, MARTe::uint32 *signalToGenerate, MARTe::uint32 toGenerateNumberOfElements,
        MARTe::uint8 *triggerToGenerate, MARTe::uint32 *signalToVerify, MARTe::uint32 *timeToVerify,
        MARTe::uint32 toVerifyNumberOfElements, MARTe::uint32 numberOfBuffers, MARTe::uint32 numberOfPreTriggers,
        MARTe::uint32 numberOfPostTriggers, MARTe::float32 period, const MARTe::char8 * const treeName, MARTe::uint32 pulseNumber,
        MARTe::int32 numberOfSegments, bool needsFlush, MARTe::uint32 sleepMSec = 100) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    cdb.Delete("Signal");
    Vector<uint32> signalV(signalToGenerate, toGenerateNumberOfElements);
    cdb.Write("Signal", signalV);
    cdb.Delete("Period");
    cdb.Write("Period", period);
    if (triggerToGenerate != NULL) {
        cdb.Delete("Trigger");
        Vector<uint8> triggerV(triggerToGenerate, toGenerateNumberOfElements);
        cdb.Write("Trigger", triggerV);
    }

    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("NumberOfBuffers");
    cdb.Write("NumberOfBuffers", numberOfBuffers);
    cdb.Delete("NumberOfPreTriggers");
    cdb.Write("NumberOfPreTriggers", numberOfPreTriggers);
    cdb.Delete("NumberOfPostTriggers");
    cdb.Write("NumberOfPostTriggers", numberOfPostTriggers);
    cdb.Delete("PulseNumber");
    cdb.Write("PulseNumber", pulseNumber);
    cdb.Delete("StoreOnTrigger");
    uint32 storeOnTrigger = (triggerToGenerate != NULL) ? 1 : 0;
    cdb.Write("StoreOnTrigger", storeOnTrigger);
    cdb.MoveRelative("Signals");
    uint32 nSignals = cdb.GetNumberOfChildren();
    uint32 n;
    for (n = 0u; n < nSignals; n++) {
        cdb.MoveRelative(cdb.GetChildName(n));
        cdb.Delete("Period");
        cdb.Write("Period", period);
        cdb.MoveToAncestor(1u);
    }

    cdb.MoveToRoot();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    ReferenceT<MDSWriterSchedulerTestHelper> scheduler;
    ReferenceT<MDSWriterGAMTriggerTestHelper> gam;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    //Open the tree and check if the data was correctly stored.
    //Create a pulse. It assumes that the tree template is already created!!
    MDSplus::Tree *tree = NULL;
    if (ok) {
        try {
            tree = new MDSplus::Tree(treeName, pulseNumber);
        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Failed opening tree %s with the pulseNUmber = %d. Trying to create pulse", treeName,
                    pulseNumber)
            delete tree;
            tree = NULL;
        }
    }
    if (ok) {
        ok = (tree != NULL);
    }
    MDSplus::TreeNode *sigUInt16;
    MDSplus::TreeNode *sigInt16;
    MDSplus::TreeNode *sigUInt32;
    MDSplus::TreeNode *sigInt32;
    MDSplus::TreeNode *sigUInt64;
    MDSplus::TreeNode *sigInt64;
    MDSplus::TreeNode *sigFloat32;
    MDSplus::TreeNode *sigFloat64;

    MDSplus::TreeNode *sigUInt16D;
    MDSplus::TreeNode *sigUInt16F;
    MDSplus::TreeNode *sigInt16D;
    MDSplus::TreeNode *sigInt16F;
    MDSplus::TreeNode *sigUInt32D;
    MDSplus::TreeNode *sigUInt32F;
    MDSplus::TreeNode *sigInt32D;
    MDSplus::TreeNode *sigInt32F;
    MDSplus::TreeNode *sigUInt64D;
    MDSplus::TreeNode *sigUInt64F;
    MDSplus::TreeNode *sigInt64D;
    MDSplus::TreeNode *sigInt64F;
    MDSplus::TreeNode *sigFloat32D;
    MDSplus::TreeNode *sigFloat32F;
    MDSplus::TreeNode *sigFloat64D;
    MDSplus::TreeNode *sigFloat64F;
    if (ok) {
        try {
            sigUInt16 = tree->getNode("SIGUINT16");
            sigInt16 = tree->getNode("SIGINT16");
            sigUInt32 = tree->getNode("SIGUINT32");
            sigInt32 = tree->getNode("SIGINT32");
            sigUInt64 = tree->getNode("SIGUINT64");
            sigInt64 = tree->getNode("SIGINT64");
            sigFloat32 = tree->getNode("SIGFLT32");
            sigFloat64 = tree->getNode("SIGFLT64");
            sigUInt16->deleteData();
            sigInt16->deleteData();
            sigUInt32->deleteData();
            sigInt32->deleteData();
            sigUInt64->deleteData();
            sigInt64->deleteData();
            sigFloat32->deleteData();
            sigFloat64->deleteData();

            sigUInt16D = tree->getNode("SIGUINT16D");
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigInt16D = tree->getNode("SIGINT16D");
            sigInt16F = tree->getNode("SIGINT16F");
            sigUInt32D = tree->getNode("SIGUINT32D");
            sigUInt32F = tree->getNode("SIGUINT32F");
            sigInt32D = tree->getNode("SIGINT32D");
            sigInt32F = tree->getNode("SIGINT32F");
            sigUInt64D = tree->getNode("SIGUINT64D");
            sigUInt64F = tree->getNode("SIGUINT64F");
            sigInt64D = tree->getNode("SIGINT64D");
            sigInt64F = tree->getNode("SIGINT64F");
            sigFloat32D = tree->getNode("SIGFLT32D");
            sigFloat32F = tree->getNode("SIGFLT32F");
            sigFloat64D = tree->getNode("SIGFLT64D");
            sigFloat64F = tree->getNode("SIGFLT64F");
            sigUInt16D->deleteData();
            sigUInt16F->deleteData();
            sigInt16D->deleteData();
            sigInt16F->deleteData();
            sigUInt32D->deleteData();
            sigUInt32F->deleteData();
            sigInt32D->deleteData();
            sigInt32F->deleteData();
            sigUInt64D->deleteData();
            sigUInt64F->deleteData();
            sigInt64D->deleteData();
            sigInt64F->deleteData();
            sigFloat32D->deleteData();
            sigFloat32F->deleteData();
            sigFloat64D->deleteData();
            sigFloat64F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    uint32 i;
    for (i = 0; (i < gam->numberOfExecutes) && (ok); i++) {
        scheduler->ExecuteThreadCycle(0);
        Sleep::MSec(sleepMSec);
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        const uint64 maxTimeoutSeconds = 2;
        uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
        while ((sigFloat64F->getNumSegments() != numberOfSegments) && (ok)) {
            Sleep::MSec(10);
            ok = (HighResolutionTimer::Counter() < maxTimeout);
        }
    }
    if (ok) {
        //If the number of elements is not a multiple of the segment size then we have to flush so that an extra segment
        //with the missing data is added.
        if (needsFlush) {
            ReferenceT<Message> messageFlush = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageFlush");
            if (ok) {
                ok = messageFlush.IsValid();
            }
            if (ok) {
                MessageI::SendMessage(messageFlush, NULL);
            }
            if (ok) {
                const uint64 maxTimeoutSeconds = 2;
                uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
                while ((sigFloat64F->getNumSegments() != (numberOfSegments + 1)) && (ok)) {
                    Sleep::MSec(10);
                    ok = (HighResolutionTimer::Counter() < maxTimeout);
                }
            }
        }
    }
    if (ok) {
        ok &= CheckSegmentData<uint16>(numberOfSegments, sigUInt16, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<uint32>(numberOfSegments, sigUInt32, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<uint64>(numberOfSegments, sigUInt64, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<int16>(numberOfSegments, sigInt16, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<int32>(numberOfSegments, sigInt32, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<int64>(numberOfSegments, sigInt64, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<float32>(numberOfSegments, sigFloat32, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<float64>(numberOfSegments, sigFloat64, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<uint16>(numberOfSegments, sigUInt16F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<uint32>(numberOfSegments, sigUInt32F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<uint64>(numberOfSegments, sigUInt64F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<int16>(numberOfSegments, sigInt16F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<int32>(numberOfSegments, sigInt32F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<int64>(numberOfSegments, sigInt64F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<float32>(numberOfSegments, sigFloat32F, signalToVerify, timeToVerify);
        ok &= CheckSegmentData<float64>(numberOfSegments, sigFloat64F, signalToVerify, timeToVerify);
        //Not going to verify that MDSplus is correctly decimating the data
        ok &= (sigUInt16D->getNumSegments() > 0);
        ok &= (sigUInt32D->getNumSegments() > 0);
        ok &= (sigUInt64D->getNumSegments() > 0);
        ok &= (sigInt16D->getNumSegments() > 0);
        ok &= (sigInt32D->getNumSegments() > 0);
        ok &= (sigInt64D->getNumSegments() > 0);
        ok &= (sigFloat32D->getNumSegments() > 0);
        ok &= (sigFloat64D->getNumSegments() > 0);
    }

    if (tree != NULL) {
        delete tree;
    }
    godb->Purge();
    return ok;
}

//Standard configuration with no trigger source
static const MARTe::char8 * const config1 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = MDSWriterGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16F = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32F = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64F = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt16F = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32F = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                }"
"                SignalInt64F = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                }"
"                SignalFloat32F = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                }"
"                SignalFloat64F = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = ReferenceContainer"
"        DefaultDataSource = DDB1"
"        +Timings = {"
"            Class = TimingDataSource"
"        }"
"        +Drv1 = {"
"            Class = MDSWriter"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            TreeName = \"mds_m2test\""
"            PulseNumber = 1"
"            StoreOnTrigger = 0"
"            EventName = \"updatejScope\""
"            TimeRefresh = 5"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt16F = {"
"                    NodeName = \"SIGUINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt32F = {"
"                    NodeName = \"SIGUINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt64F = {"
"                    NodeName = \"SIGUINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt16F = {"
"                    NodeName = \"SIGINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt32F = {"
"                    NodeName = \"SIGINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt64F = {"
"                    NodeName = \"SIGINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat32F = {"
"                    NodeName = \"SIGFLT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat64F = {"
"                    NodeName = \"SIGFLT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt16 = {"
"                    NodeName = \"SIGUINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt32 = {"
"                    NodeName = \"SIGUINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt64 = {"
"                    NodeName = \"SIGUINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt16 = {"
"                    NodeName = \"SIGINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt32 = {"
"                    NodeName = \"SIGINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt64 = {"
"                    NodeName = \"SIGINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat32 = {"
"                    NodeName = \"SIGFLT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat64 = {"
"                    NodeName = \"SIGFLT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = {GAM1}"
"                }"
"            }"
"        }"
"    }"
"    +Scheduler = {"
"        Class = MDSWriterSchedulerTestHelper"
"        TimingDataSource = Timings"
"    }"
"}"
"+TestMessages = {"
"    Class = ReferenceContainer"
"    +MessageFlush = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = FlushSegments"
"    }"
"}";

//Standard configuration with trigger source
static const MARTe::char8 * const config2 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = MDSWriterGAMTriggerTestHelper"
"            Signal =   {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            Trigger =  {0 1 0 1 0 1 0 1 1 1 1 1 1 1}"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16F = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32F = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64F = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt16F = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32F = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                }"
"                SignalInt64F = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                }"
"                SignalFloat32F = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                }"
"                SignalFloat64F = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = ReferenceContainer"
"        DefaultDataSource = DDB1"
"        +Timings = {"
"            Class = TimingDataSource"
"        }"
"        +Drv1 = {"
"            Class = MDSWriter"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            TreeName = \"mds_m2test\""
"            PulseNumber = 1"
"            StoreOnTrigger = 1"
"            EventName = \"updatejScope\""
"            TimeRefresh = 5"
"            NumberOfPreTriggers = 2"
"            NumberOfPostTriggers = 1"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                    TimeSignal = 1"
"                }"
"                SignalUInt16F = {"
"                    NodeName = \"SIGUINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt32F = {"
"                    NodeName = \"SIGUINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt64F = {"
"                    NodeName = \"SIGUINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt16F = {"
"                    NodeName = \"SIGINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt32F = {"
"                    NodeName = \"SIGINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt64F = {"
"                    NodeName = \"SIGINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat32F = {"
"                    NodeName = \"SIGFLT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat64F = {"
"                    NodeName = \"SIGFLT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt16 = {"
"                    NodeName = \"SIGUINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt32 = {"
"                    NodeName = \"SIGUINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt64 = {"
"                    NodeName = \"SIGUINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt16 = {"
"                    NodeName = \"SIGINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt32 = {"
"                    NodeName = \"SIGINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt64 = {"
"                    NodeName = \"SIGINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat32 = {"
"                    NodeName = \"SIGFLT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat64 = {"
"                    NodeName = \"SIGFLT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = {GAM1}"
"                }"
"            }"
"        }"
"    }"
"    +Scheduler = {"
"        Class = MDSWriterSchedulerTestHelper"
"        TimingDataSource = Timings"
"    }"
"}"
"+TestMessages = {"
"    Class = ReferenceContainer"
"    +MessageFlush = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = FlushSegments"
"    }"
"}";

//Standard configuration with number of elements > 0
static const MARTe::char8 * const config3 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = MDSWriterGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16F = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt32F = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt64F = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt16F = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt32F = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt64F = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat32F = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat64F = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = ReferenceContainer"
"        DefaultDataSource = DDB1"
"        +Timings = {"
"            Class = TimingDataSource"
"        }"
"        +Drv1 = {"
"            Class = MDSWriter"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            TreeName = \"mds_m2test\""
"            PulseNumber = 1"
"            StoreOnTrigger = 0"
"            EventName = \"updatejScope\""
"            TimeRefresh = 5"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt16F = {"
"                    NodeName = \"SIGUINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt32F = {"
"                    NodeName = \"SIGUINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt64F = {"
"                    NodeName = \"SIGUINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt16F = {"
"                    NodeName = \"SIGINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt32F = {"
"                    NodeName = \"SIGINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt64F = {"
"                    NodeName = \"SIGINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat32F = {"
"                    NodeName = \"SIGFLT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat64F = {"
"                    NodeName = \"SIGFLT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt16 = {"
"                    NodeName = \"SIGUINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt32 = {"
"                    NodeName = \"SIGUINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt64 = {"
"                    NodeName = \"SIGUINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt16 = {"
"                    NodeName = \"SIGINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt32 = {"
"                    NodeName = \"SIGINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt64 = {"
"                    NodeName = \"SIGINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat32 = {"
"                    NodeName = \"SIGFLT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat64 = {"
"                    NodeName = \"SIGFLT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = {GAM1}"
"                }"
"            }"
"        }"
"    }"
"    +Scheduler = {"
"        Class = MDSWriterSchedulerTestHelper"
"        TimingDataSource = Timings"
"    }"
"}"
"+TestMessages = {"
"    Class = ReferenceContainer"
"    +MessageFlush = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = FlushSegments"
"    }"
"}";

//Standard configuration with trigger source and elements > 0
static const MARTe::char8 * const config5 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = MDSWriterGAMTriggerTestHelper"
"            Signal =   {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            Trigger =  {0 1 0 1 0 1 0 1 1 1 1 1 1 1}"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16F = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt32F = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt64F = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt16F = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt32F = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt64F = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat32F = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat64F = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = Drv1"
"                    NumberOfElements = 4"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = ReferenceContainer"
"        DefaultDataSource = DDB1"
"        +Timings = {"
"            Class = TimingDataSource"
"        }"
"        +Drv1 = {"
"            Class = MDSWriter"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            TreeName = \"mds_m2test\""
"            PulseNumber = 1"
"            StoreOnTrigger = 1"
"            EventName = \"updatejScope\""
"            TimeRefresh = 5"
"            NumberOfPreTriggers = 2"
"            NumberOfPostTriggers = 1"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                    TimeSignal = 1"
"                }"
"                SignalUInt16F = {"
"                    NodeName = \"SIGUINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt32F = {"
"                    NodeName = \"SIGUINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt64F = {"
"                    NodeName = \"SIGUINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt16F = {"
"                    NodeName = \"SIGINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt32F = {"
"                    NodeName = \"SIGINT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalInt64F = {"
"                    NodeName = \"SIGINT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGINT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat32F = {"
"                    NodeName = \"SIGFLT32F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT32D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalFloat64F = {"
"                    NodeName = \"SIGFLT64F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGFLT64D\""
"                    MinMaxResampleFactor = 4"
"                }"
"                SignalUInt16 = {"
"                    NodeName = \"SIGUINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt32 = {"
"                    NodeName = \"SIGUINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalUInt64 = {"
"                    NodeName = \"SIGUINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt16 = {"
"                    NodeName = \"SIGINT16\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt32 = {"
"                    NodeName = \"SIGINT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalInt64 = {"
"                    NodeName = \"SIGINT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat32 = {"
"                    NodeName = \"SIGFLT32\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"                SignalFloat64 = {"
"                    NodeName = \"SIGFLT64\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = {GAM1}"
"                }"
"            }"
"        }"
"    }"
"    +Scheduler = {"
"        Class = MDSWriterSchedulerTestHelper"
"        TimingDataSource = Timings"
"    }"
"}"
"+TestMessages = {"
"    Class = ReferenceContainer"
"    +MessageFlush = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = FlushSegments"
"    }"
"}";

//Configuration to test the OpenTree message interface
static const MARTe::char8 * const config6 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = MDSWriterGAMTriggerTestHelper"
"            Signal =   {8 1 2 3 4 5 6 7 }"
"            Trigger =  {0 1 0 1 0 1 0 1 }"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16F = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = ReferenceContainer"
"        DefaultDataSource = DDB1"
"        +Timings = {"
"            Class = TimingDataSource"
"        }"
"        +Drv1 = {"
"            Class = MDSWriter"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            TreeName = \"mds_m2test\""
"            StoreOnTrigger = 0"
"            EventName = \"updatejScope\""
"            TimeRefresh = 5"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt16F = {"
"                    NodeName = \"SIGUINT16F\""
"                    Period = 2"
"                    MakeSegmentAfterNWrites = 4"
"                    DecimatedNodeName = \"SIGUINT16D\""
"                    MinMaxResampleFactor = 4"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = {GAM1}"
"                }"
"            }"
"        }"
"    }"
"    +Scheduler = {"
"        Class = MDSWriterSchedulerTestHelper"
"        TimingDataSource = Timings"
"    }"
"}"
"+TestMessages = {"
"    Class = ReferenceContainer"
"    +MessageFlush = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = FlushSegments"
//"        Mode = ExpectsReply"
"    }"
"    +MessageOpenTree = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
//"        Mode = ExpectsReply"
"        Function = OpenTree"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = -1"
"        }"
"    }"
"}";

#endif
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool MDSWriterNodeTest::TestConstructor() {
    using namespace MARTe;
    MDSWriterNode test;
    bool ok = !test.IsDecimatedMinMax();
    ok &= (test.GetDecimatedNodeName() == "");
    ok &= (test.GetExecutePeriodMicroSecond() == 0);
    ok &= (test.GetMakeSegmentAfterNWrites() == 0);
    ok &= (test.GetMinMaxResampleFactor() == 0);
    ok &= (test.GetNodeName() == "");
    ok &= (test.GetNodeType() == 0);
    ok &= (test.GetNumberOfElements() == 0);
    ok &= (test.GetPeriod() == 0.F);
    ok &= (test.GetPhaseShift() == 0);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == 0);
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_NoDecimatedNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= !test.IsDecimatedMinMax();
    ok &= (test.GetDecimatedNodeName() == "");
    ok &= (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
    ok &= (test.GetMakeSegmentAfterNWrites() == 4);
    ok &= (test.GetMinMaxResampleFactor() == 0);
    ok &= (test.GetNodeName() == "AAA");
    ok &= (test.GetNodeType() == DTYPE_WU);
    ok &= (test.GetNumberOfElements() == 100);
    ok &= (test.GetPeriod() == 5e-7);
    ok &= (test.GetPhaseShift() == 0);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == sizeof(uint16));
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_DecimatedNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("MinMaxResampleFactor", 5);

    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.IsDecimatedMinMax());
    ok &= (test.GetDecimatedNodeName() == "BBB");
    ok &= (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
    ok &= (test.GetMakeSegmentAfterNWrites() == 4);
    ok &= (test.GetMinMaxResampleFactor() == 5);
    ok &= (test.GetNodeName() == "AAA");
    ok &= (test.GetNodeType() == DTYPE_WU);
    ok &= (test.GetNumberOfElements() == 100);
    ok &= (test.GetPeriod() == 5e-7);
    ok &= (test.GetPhaseShift() == 0);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == sizeof(uint16));
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_SamplePhase() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("SamplePhase", 3);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= !test.IsDecimatedMinMax();
    ok &= (test.GetDecimatedNodeName() == "");
    ok &= (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
    ok &= (test.GetMakeSegmentAfterNWrites() == 4);
    ok &= (test.GetMinMaxResampleFactor() == 0);
    ok &= (test.GetNodeName() == "AAA");
    ok &= (test.GetNodeType() == DTYPE_WU);
    ok &= (test.GetNumberOfElements() == 100);
    ok &= (test.GetPeriod() == 5e-7);
    ok &= (test.GetPhaseShift() == 3);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == sizeof(uint16));
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_False_NoNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint22");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 0.0F);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoNumberOfElements() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadNumberOfElements() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 0);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}


bool MDSWriterNodeTest::TestInitialise_False_NoMakeSegmentAfterNWrites() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 4);
    cdb.Write("Period", 5e-7);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadMakeSegmentAfterNWrites() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 10);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 0);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoMinMaxResampleFactor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadMinMaxResampleFactor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("MinMaxResampleFactor", 0);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_Type_UInt16() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_WU);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Int16() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "int16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_W);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_UInt32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_LU);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Int32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "int32");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_L);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_UInt64() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint64");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_QU);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Int64() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "int64");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_Q);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Float32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "float32");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_FLOAT);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Float64() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "float64");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_DOUBLE);
    return ok;
}

#if 0
bool MDSWriterTest::TestIntegratedInApplication_NoTrigger() {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    uint32 timeToVerify[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    const char8 * const treeName = "mds_m2test";
    const uint32 numberOfBuffers = 16;
    const uint32 pulseNumber = 1;
    const uint32 writeAfterNSegments = 4;
    const uint32 numberOfSegments = numberOfElements / writeAfterNSegments;
    const float32 period = 2;
    return TestIntegratedExecution(config1, signalToGenerate, numberOfElements, NULL, signalToGenerate, timeToVerify, numberOfElements, numberOfBuffers, 0, 0,
            period, treeName, pulseNumber, numberOfSegments, false);
}

bool MDSWriterTest::TestIntegratedInApplication_NoTrigger_Flush() {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    uint32 timeToVerify[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    const char8 * const treeName = "mds_m2test";
    const uint32 numberOfBuffers = 16;
    const uint32 pulseNumber = 2;
    const uint32 writeAfterNSegments = 4;
    const uint32 numberOfSegments = numberOfElements / writeAfterNSegments;
    const float32 period = 2;
    return TestIntegratedExecution(config1, signalToGenerate, numberOfElements, NULL, signalToGenerate, timeToVerify, numberOfElements, numberOfBuffers, 0, 0,
            period, treeName, pulseNumber, numberOfSegments, true);
}

bool MDSWriterTest::TestIntegratedInApplication_Trigger() {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    uint32 signalToVerify[] = {1, 2, 3, 4, 6, 7, 8, 9};
    uint32 timeToVerify[] = {0, 2, 4, 6, 10, 12, 14, 16};
    uint8 triggerToGenerate[] = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    uint32 numberOfElementsToVerify = sizeof(signalToVerify) / sizeof(uint32);
    const char8 * const treeName = "mds_m2test";
    const uint32 numberOfBuffers = 16;
    const uint32 numberOfPreTriggers = 2;
    const uint32 numberOfPostTriggers = 1;
    const uint32 pulseNumber = 3;
    const uint32 numberOfSegments = 2;
    const float32 period = 2;
    return TestIntegratedExecution(config2, signalToGenerate, numberOfElements, triggerToGenerate, signalToVerify, timeToVerify, numberOfElementsToVerify,
            numberOfBuffers, numberOfPreTriggers, numberOfPostTriggers, period, treeName, pulseNumber, numberOfSegments, false);
}

bool MDSWriterTest::TestIntegratedInApplication_NoTrigger_Elements() {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint32 signalToVerify[] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8};
    uint32 timeToVerify[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    uint32 numberOfElementsToVerify = sizeof(signalToVerify) / sizeof(uint32);
    const char8 * const treeName = "mds_m2test";
    const uint32 numberOfBuffers = 16;
    const uint32 writeAfterNSegments = 4;
    const uint32 numberOfSegments = numberOfElements / writeAfterNSegments;
    const float32 period = 2;
    const uint32 pulseNumber = 4;
    return TestIntegratedExecution(config3, signalToGenerate, numberOfElements, NULL, signalToVerify, timeToVerify, numberOfElementsToVerify, numberOfBuffers,
            0, 0, period, treeName, pulseNumber, numberOfSegments, false);
}

bool MDSWriterTest::TestIntegratedInApplication_Trigger_Elements() {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    uint32 signalToVerify[] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9};
    uint32 timeToVerify[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70};
    uint8 triggerToGenerate[] = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    uint32 numberOfElementsToVerify = sizeof(signalToVerify) / sizeof(uint32);
    const char8 * const treeName = "mds_m2test";
    const uint32 numberOfBuffers = 16;
    const uint32 numberOfPreTriggers = 2;
    const uint32 numberOfPostTriggers = 1;
    const uint32 pulseNumber = 5;
    const uint32 numberOfSegments = 2;
    const float32 period = 2;
    return TestIntegratedExecution(config5, signalToGenerate, numberOfElements, triggerToGenerate, signalToVerify, timeToVerify, numberOfElementsToVerify,
            numberOfBuffers, numberOfPreTriggers, numberOfPostTriggers, period, treeName, pulseNumber, numberOfSegments, false);
}

bool MDSWriterTest::TestOpenTree() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config6, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    //Get the current pulse number
    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    StreamString treeName = "mds_m2test";
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
        delete tree;
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    ReferenceT<MDSWriter> msdWriter;
    if (ok) {
        msdWriter = godb->Find("Test.Data.Drv1");
        ok = msdWriter.IsValid();
    }
    ReferenceT<Message> messageFlush = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageOpenTree");
    if (ok) {
        ok = messageFlush.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageFlush, NULL);
    }

    int32 currentPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), currentPulseNumber);
        currentPulseNumber = tree->getCurrent(treeName.Buffer());
        delete tree;
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }
    if (ok) {
        ok = ((currentPulseNumber - lastPulseNumber) == 1);
    }

    //Check that data can be successfully stored in the new pulse number
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    ReferenceT<MDSWriterSchedulerTestHelper> scheduler;
    ReferenceT<MDSWriterGAMTriggerTestHelper> gam;
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    //Open the tree and check if the data was correctly stored.
    //Create a pulse. It assumes that the tree template is already created!!
    if (ok) {
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), currentPulseNumber);
        }
        catch (MDSplus::MdsException &exc) {
            delete tree;
            tree = NULL;
            ok = false;
        }
    }
    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();
        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    uint32 i;
    for (i = 0; (i < gam->numberOfExecutes) && (ok); i++) {
        scheduler->ExecuteThreadCycle(0);
        Sleep::MSec(10);
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    const int32 numberOfSegments = 2;
    if (ok) {
        const uint64 maxTimeoutSeconds = 2;
        uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
        while ((sigUInt16F->getNumSegments() != numberOfSegments) && (ok)) {
            Sleep::MSec(10);
            ok = (HighResolutionTimer::Counter() < maxTimeout);
        }
    }
    if (ok) {
        uint32 signalToVerify[] = {8, 1, 2, 3, 4, 5, 6, 7};
        uint32 timeToVerify[] = {0, 2, 4, 6, 8, 10, 12, 14};
        ok &= CheckSegmentData<uint16>(numberOfSegments, sigUInt16F, signalToVerify, timeToVerify);
    }

    if (tree != NULL) {
        delete tree;
    }
    godb->Purge();
    return ok;
}
#endif
