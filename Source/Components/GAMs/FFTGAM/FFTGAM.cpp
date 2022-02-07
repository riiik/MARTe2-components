/**
 * @file FFTGAM.cpp
 * @brief Source file for class FFTGAM
 * @date 22/03/2018
 * @author Bertrand Bauvir
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
 * the class FFTGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"
#include "FFTGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

FFTGAM::FFTGAM() :
        GAM(), MessageI() {

    signalType = InvalidType;
}

FFTGAM::~FFTGAM() {
    // currently redundant code
    if (signalType == Float32Bit) {

    }

}

bool FFTGAM::Setup() {

    // check there is at least one input signal and one output signal
    bool ret = ((GetNumberOfInputSignals() != 0u) && (GetNumberOfOutputSignals() != 0u));

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "(GetNumberOfInputSignals() == 0u) || (GetNumberOfOutputSignals() == 0u)");
    }

    // check that the input and output signal types match
    if (ret) {
        ret = (GetSignalType(InputSignals, 0u) == GetSignalType(OutputSignals, 0u));
    }

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalType(InputSignals, 0u) != GetSignalType(OutputSignals, 0u)");
    }

    // get input signal type
    if (ret) {
      signalType = GetSignalType(InputSignals, 0u);
    }

    // check that input signal no. of dimensions = 0
    uint32 signalNumberOfDimensions = 0u;

    if (ret) {
        ret = GetSignalNumberOfDimensions(InputSignals, 0u, signalNumberOfDimensions);
    }

    if (ret) {
        ret = (signalNumberOfDimensions == 0u);
    }  

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfDimensions(InputSignals, 0u) != 0u");
    }

    // check that input signal no. of elements = 1
    uint32 signalNumberOfElements = 0u;

    if (ret) {
        ret = GetSignalNumberOfElements(InputSignals, 0u, signalNumberOfElements);
    }

    if (ret) {
        ret = (signalNumberOfElements == 1u);
    }  

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements(InputSignals, 0u) != 1u");
    }

    // loop through output signals
    uint32 signalIndex;

    for (signalIndex = 0u; (signalIndex < GetNumberOfOutputSignals()) && (ret); signalIndex++) {

        // check the output signal has a valid type
        ret = (signalType == GetSignalType(OutputSignals, signalIndex));

		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalType(OutputSignals, %u) != signalType", signalIndex);
		}

		if (ret) {
			ret = GetSignalNumberOfDimensions(OutputSignals, signalIndex, signalNumberOfDimensions);
		}

		if (ret) {
				ret = (signalNumberOfDimensions == 0u);
		}  

		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfDimensions(OutputSignals, %u) != 0u", signalIndex);
		}

		if (ret) {
				ret = GetSignalNumberOfElements(OutputSignals, signalIndex, signalNumberOfElements);
		}

		if (ret) {
			ret = (signalNumberOfElements == 1u);
		}  

		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements(OutputSignals, %u) != 1u", signalIndex);
		}

    }

    // instantiate the CircularStaticList
    // TODO - parameterise the buffer size (1024, maybe insist on powers of 2 for now)
    buffer = new CircularStaticList<uint32>(8);


    // Install message filter
    ReferenceT<RegisteredMethodsMessageFilter> registeredMethodsMessageFilter("RegisteredMethodsMessageFilter");

    if (ret) {
        ret = registeredMethodsMessageFilter.IsValid();
    }

    if (ret) {
        registeredMethodsMessageFilter->SetDestination(this);
        ret = InstallMessageFilter(registeredMethodsMessageFilter);
    }

    return ret;
}

bool FFTGAM::Execute() {

    bool ret = this->FFTGAM::ExecuteT<uint32>();
    // this->FFTGAM::ExecuteT<uint32>();

    return ret;
}

template <class Type> bool FFTGAM::ExecuteT() {

    CircularStaticList<Type> * ref = NULL_PTR(CircularStaticList<Type> *);
    Type input = (Type) 0;
    Type output = (Type) 0;

    bool ret = MemoryOperationsHelper::Copy(&input, GetInputSignalMemory(0u), sizeof(Type));

    if (ret) {
        ret = (buffer != NULL_PTR(void *));
    }

    if (ret) {
        ref = static_cast<CircularStaticList<Type> *>(buffer);
        ret = ref->PushData(input);
    }

    if (ret) {
        // ref->GetLast(output);
        ref->Peek(1u, output);
        ret = MemoryOperationsHelper::Copy(GetOutputSignalMemory(0u), &output, sizeof(Type));
    }

    return ret;

}

ErrorManagement::ErrorType FFTGAM::SetOutput(ReferenceContainer& message) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    // Assume one ReferenceT<StructuredDataI> contained in the message

    bool ok = (message.Size() == 1u);
    ReferenceT<StructuredDataI> data = message.Get(0u);

    if (ok) {
        ok = data.IsValid();
    }

    if (!ok) {
        ret = ErrorManagement::ParametersError;
        REPORT_ERROR(ret, "Message does not contain a ReferenceT<StructuredDataI>");
    }

    StreamString signalName;
    uint32 signalIndex = 0u;

    if (ok) {
        if (data->Read("SignalName", signalName)) {
            ok = GetSignalIndex(OutputSignals, signalIndex, signalName.Buffer());
        }
        else {
            ok = data->Read("SignalIndex", signalIndex);
        }
    }

    if (ok) {
        ok = (signalIndex < GetNumberOfOutputSignals());
    }

    if (!ok) {
        ret = ErrorManagement::ParametersError;
        REPORT_ERROR(ret, "No valid signal name or index provided");
    }

    TypeDescriptor signalType = InvalidType;

    if (ok) {
        signalType = GetSignalType(OutputSignals, signalIndex);
        ok = (signalType != InvalidType);
    }

    if (ok) {

        // Signal index and type are tested and valid ... go ahead with AnyType instantiation

        // Use the default value type to query the signal properties (dimensions, ...)
        /*lint -e{534}  [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2]. Justification: SignalIndex is tested valid prio to this part of the code.*/
        MoveToSignalIndex(OutputSignals, signalIndex);
        AnyType signalDefType = configuredDatabase.GetType("Default");
        AnyType signalNewValue(signalType, 0u, GetOutputSignalMemory(signalIndex));

        uint8 signalNumberOfDimensions = signalDefType.GetNumberOfDimensions();
        signalNewValue.SetNumberOfDimensions(signalNumberOfDimensions);

        uint32 dimensionIndex;

        for (dimensionIndex = 0u; dimensionIndex < signalNumberOfDimensions; dimensionIndex++) {
            uint32 dimensionNumberOfElements = signalDefType.GetNumberOfElements(static_cast<uint32>(dimensionIndex));
            signalNewValue.SetNumberOfElements(static_cast<uint32>(dimensionIndex), dimensionNumberOfElements);
        }

        if (data->Read("SignalValue", signalNewValue)) {
            REPORT_ERROR(ErrorManagement::Information, "Signal '%!' new value '%!'", signalName.Buffer(), signalNewValue);
        }
        else {
            ret = ErrorManagement::ParametersError;
            REPORT_ERROR(ret, "Failed to read and apply new signal value");
        }

    }

    return ret;
}

CLASS_REGISTER(FFTGAM, "1.0")

/*lint -e{1023} Justification: Macro provided by the Core.*/
CLASS_METHOD_REGISTER(FFTGAM, SetOutput)

} /* namespace MARTe */

