/**
 * @file EpicsInputDataSourceTest.cpp
 * @brief Source file for class EpicsInputDataSourceTest
 * @date 24/01/2017
 * @author Ivan Herrero
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
 * the class EpicsInputDataSourceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "EpicsDataSourceSupport.h"
#include "EpicsInputDataSource.h"
#include "EpicsInputDataSourceTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "Platform.h"
#include "RealTimeApplication.h"
#include "SharedDataArea.h"
#include "SigblockDoubleBufferSupport.h"
#include "StandardParser.h"
#include "StreamString.h"

template bool EpicsInputDataSourceTest::TestSynchronise<MARTe::uint32>();
template bool EpicsInputDataSourceTest::TestSynchronise<MARTe::float32>();

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::ConfigurationDatabase INV_CDB;

#define INVARIANT(obj) ( \
(obj.GetNumberOfMemoryBuffers() == 1u) and \
(std::strcmp(obj.GetBrokerName(INV_CDB, InputSignals), "MemoryMapSynchronisedInputBroker") == 0) and \
(std::strcmp(obj.GetBrokerName(INV_CDB, OutputSignals), "") == 0) \
)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EpicsInputDataSourceTest::TestConstructor() {
    using namespace MARTe;
    bool ok = false;
    EpicsInputDataSource target;

    //Check postcondition:
    ok = (target.GetSharedDataAreaName().Size() == 0);

    //Check invariant:
    ok &= INVARIANT(target);

    return ok;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_One_Signal_Per_GAM() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_MoreThan2Signals() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_No32BitsSignal1() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_No32BitsSignal2() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_NoFrequencySet() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_IntegerSignal1() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_IntegerSignal2() {
	return false;
}

template<typename SignalType>
bool EpicsInputDataSourceTest::TestSynchronise() {
    using namespace MARTe;
    bool success = false;
    MARTe::StreamString tmp_SharedDataAreaName; //TODO: Remove this when autorelease will be added to EpicsInputDataSourceTest.

    {
    	const unsigned int maxTests = 30;
    	const char targetName[] = "EpicsInputDataSourceTest_TestSynchronise";
    	EpicsInputDataSource target;
    	DataSet dataset(maxTests);
    	SharedDataArea sdaClient;
    	SharedDataArea::SigblockProducer* producer;
    	const uint32 numberOfSignals = 5;
    	void* signals[numberOfSignals];

    	//Initialize the name of the data source:
    	target.SetName(targetName);

    	//Initialize signals configuration on data source:
    	ConfigurationDatabase cdb;
    	success = BuildConfigurationDatabase(cdb, numberOfSignals);
    	success &= target.SetConfiguredDatabase(cdb);

    	//Allocate memory of data source (it setups the shared data area):
    	target.AllocateMemory();
    	tmp_SharedDataAreaName = target.GetSharedDataAreaName(); //TODO: Remove this when autorelease will be added to EpicsInputDataSourceTest.

    	//Cache an array of pointers to the signal's addresses:
    	for (uint32 i = 0; i < numberOfSignals; i++) {
    		target.GetSignalMemoryBuffer(i, 0, signals[i]);
    	}

    	//Setup producers's interface to shared data area:
    	sdaClient = SharedDataArea::BuildSharedDataAreaForEPICS(target.GetSharedDataAreaName().Buffer());
    	producer = sdaClient.GetSigblockProducerInterface();

    	//Allocate memory for dataset:
    	MallocDataSet(dataset, producer->GetSigblockMetadata()->GetTotalSize());

    	//Initialize items of dataset:
    	InitDataSet<SignalType>(dataset, numberOfSignals);

    	//Write all the sigblocks of the dataset to the shared data area, checking
    	//that they can be read by the input data source and have the same values
    	//than those from the dataset. They will be written and read taking turns
    	//(1 write, 1 read).
    	{
    		bool error = false;
    		unsigned int i = 0;

    		//Write and read sigblocks taking turns:
    		while (i < dataset.size && !error) {
    			bool writeOk;

    			//Write the sigblock on the position i of the dataset to the shared data area:
    			writeOk = producer->WriteSigblock(*(dataset.items[i]));

    			if (writeOk) {
    				bool readOk;

    				//Synchronise the input data source with the shared data area
    				//(i.e. reads a sigblock from the shared data area and make
    				//their values available as signals of the input data source):
    				readOk = target.Synchronise();

    				if (readOk) {
    					//Check the values of the signals into the data source
    					//(accessible via signals[]) against those of the data set:
    					unsigned int j = 0;
    					while (j < numberOfSignals && !error) {
    						SignalType* cursig = reinterpret_cast<SignalType*>(signals[j]);
    						SignalType* refsig = reinterpret_cast<SignalType*>(dataset.items[i] + producer->GetSigblockMetadata()->GetSignalOffsetByIndex(j));
    						error = (*cursig != *refsig);
    						j++;
    					}
    				}
    				else {
    					error = true;
    				}
    			}
    			else {
    				error = true;
    			}
    			i++;
    		}

    		//Check execution's status:
    		success &= !error;
    	}

    	//Free memory of dataset:
    	FreeDataSet(dataset);

    }

    //Release shared data area: //TODO: Remove this when autorelease will be added to EpicsInputDataSourceTest.
	Platform::DestroyShm(tmp_SharedDataAreaName.Buffer());

	return success;
}

bool EpicsInputDataSourceTest::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = false;
    const char targetName[] = "EpicsInputDataSourceTest_TestAllocateMemory";
    EpicsInputDataSource target;

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals/functions configuration on data source:
    ConfigurationDatabase cdb;
    ok = BuildConfigurationDatabase(cdb, 1, 0);
    ok &= target.SetConfiguredDatabase(cdb);

	//Check class invariant:
    ok &= INVARIANT(target);

    //Execute the target method:
    ok &= target.AllocateMemory();

    //Check postcondition:
	SharedDataArea::BuildSharedDataAreaForEPICS(target.GetSharedDataAreaName().Buffer());

	//Check class invariant:
    ok &= INVARIANT(target);

    return ok;
}

bool EpicsInputDataSourceTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    bool ok = false;
    EpicsInputDataSource target;

	//Check class invariant:
    ok = INVARIANT(target);

    //Execute the target method:
    ok &= (target.GetNumberOfMemoryBuffers() == 1u);

    //Check postcondition:
    ok &= (target.GetNumberOfMemoryBuffers() == 1u);

	//Check class invariant:
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    EpicsInputDataSource test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    EpicsInputDataSource test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(2, 0, (void *&) ptr);
}

bool EpicsInputDataSourceTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    //Check broker name for input signals:
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsInputDataSource target;
    	ok &= INVARIANT(target);
    	brokerName = target.GetBrokerName(config, InputSignals);
    	ok &= (brokerName == "MemoryMapSynchronisedInputBroker");
    	ok &= INVARIANT(target);
    }
    //Check broker name for output signals:
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsInputDataSource target;
    	ok &= INVARIANT(target);
    	brokerName = target.GetBrokerName(config, OutputSignals);
    	ok &= (brokerName == "");
    	ok &= INVARIANT(target);
    }
    return ok;
}

bool EpicsInputDataSourceTest::TestGetInputBrokers() {
    using namespace MARTe;
    bool ok = true;
    const char targetName[] = "EpicsInputDataSourceTest_TestGetInputBrokers";
    const uint32 numberOfSignals = 3;
    const uint32 numberOfFunctions = 5;
    char buffer[0]; //Size of buffer not relevant in this test.
    void* gamMemPtr = static_cast<void*>(buffer);
    EpicsInputDataSource target;

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals/functions configuration on data source:
    ConfigurationDatabase cdb;
    ok &= BuildConfigurationDatabase(cdb, numberOfSignals, numberOfFunctions);
    ok &= target.SetConfiguredDatabase(cdb);

    //Allocate memory for signals (it will be needed by brokers):
    ok &= target.AllocateMemory();

    //Check datasource's input broker retrieval for each function:
    for (uint32 i = 0; i < numberOfFunctions; i++) {
    	StreamString name;
    	ReferenceContainer inputBrokers;

    	//Set function name:
    	ok &= name.Printf("Function_%d", i);

    	//Check class invariant:
    	ok &= INVARIANT(target);

    	//Execute the target method:
    	ok &= (target.GetInputBrokers(inputBrokers, name.Buffer(), gamMemPtr) == true);

    	//check postcondition:
    	ok &= (inputBrokers.Size() == 1);

    	//Check class invariant:
    	ok &= INVARIANT(target);
    }

    return ok;
}

bool EpicsInputDataSourceTest::TestGetOutputBrokers() {
    using namespace MARTe;
    bool ok = true;
    ReferenceContainer outputBrokers;
    char8* functionName = NULL_PTR(char8*);
    void* gamMemPtr = NULL_PTR(void*);
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetOutputBrokers(outputBrokers, functionName, gamMemPtr) == false);
    ok &= (outputBrokers.Size() == 0);
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = true;
    char8* currentStateName = NULL_PTR(char8*);
    char8* nextStateName = NULL_PTR(char8*);
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.PrepareNextState(currentStateName, nextStateName) == true);
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetSharedDataAreaName() {
    using namespace MARTe;
    bool ok = false;
    const char targetName[] = "EpicsInputDataSourceTest_TestGetSharedDataAreaName";
    EpicsInputDataSource target;

    //Check initial value of SharedDataAreaName is an empty string:
    ok = (target.GetSharedDataAreaName().Size() == 0);

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals/functions configuration on data source:
    ConfigurationDatabase cdb;
    ok = BuildConfigurationDatabase(cdb, 1, 0);
    ok &= target.SetConfiguredDatabase(cdb);

    //Allocate memory for the data source:
    ok &= target.AllocateMemory();

    //Check working value of SharedDataAreaName is not an empty string:
    ok &= (target.GetSharedDataAreaName().Size() > 0);

    return ok;
}

bool EpicsInputDataSourceTest::TestInitialise_Empty() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    return test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestInitialise_Default() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Default");
    return test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestInitialise_Busy() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Busy");
    return test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestInitialise_False() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "False");
    return !test.Initialise(cdb);
}
