/**
 * @file StateMachineMessage.cpp
 * @brief Source file for class StateMachineMessage
 * @date 30/09/2016
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
 * the class StateMachineMessage (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StateMachineMessage.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

StateMachineMessage::StateMachineMessage() {
    code = 0u;
    timeout = TTInfiniteWait;
}

StateMachineMessage::~StateMachineMessage() {
}

uint32 StateMachineMessage::GetCode() {
    return code;
}

CCString StateMachineMessage::GetContent() {
    return content.Buffer();
}

TimeoutType StateMachineMessage::GetTimeout() {
    return timeout;
}

bool StateMachineMessage::Initialise(StructuredDataI& data) {
    ErrorManagement::ErrorType err = (Message::Initialise(data));
    if (err.ErrorsCleared()) {
        err = data.Read("Code", code);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Code not set");
        }
    }
    if (err.ErrorsCleared()) {
        if (!data.Read("Content", content)) {
            REPORT_ERROR(ErrorManagement::Warning, "Content not set");
        }
        content.Seek(0u);
    }
    uint32 msecTimeout;
    if (err.ErrorsCleared()) {
        if (err.ErrorsCleared()) {
            err.parametersError = !data.Read("Timeout", msecTimeout);
            if (!!err.ErrorsCleared()) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Timeout was not specified");
            }
        }
    }
    if (err.ErrorsCleared()) {
        if (msecTimeout == 0u) {
            timeout = TTInfiniteWait.GetTimeoutMSec();
        }
        else {
            timeout = msecTimeout;
        }
    }

    return err.ErrorsCleared();
}

CLASS_REGISTER(StateMachineMessage, "1.0")
}
