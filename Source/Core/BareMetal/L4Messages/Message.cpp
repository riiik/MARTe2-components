/**
 * @file Message.cpp
 * @brief Source file for class Message
 * @date Apr 15, 2016
 * @author fsartori
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
 * the class Message (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Message.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool Message::Initialise(StructuredDataI &data){
    bool ret = true;

    // TODO handle errors
    ret = data.Read("Destination",destination);

    // TODO handle errors
    ret &= data.Read("Function",function);

    uint32 msecWait;
    if (data.Read("MaxWait",msecWait)){
        maxWait = msecWait;
    } else {
        maxWait = TTInfiniteWait;
        // TODO warning about maxWait set to infinite
    }

    StreamString messageFlags;
    if (data.Read("Mode",messageFlags)){
        flags = MessageFlags(messageFlags);
    } else {
        // TODO warning about flags set to default
    }

    ret &= ReferenceContainer::Initialise(data);

    return ret;
}


	
}
