/**
 * @file SingleThreadGAMScheduler.h
 * @brief Header file for class SingleThreadGAMScheduler
 * @date 22/03/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class SingleThreadGAMScheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SINGLETHREADGAMSCHEDULER_H_
#define SINGLETHREADGAMSCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class SingleThreadGAMScheduler: public GAMSchedulerI {

public:

    SingleThreadGAMScheduler();

    virtual void StartExecution(const uint32 activeBuffer);

    virtual void StopExecution();


private:

    ThreadIdentifier tid;

    uint32 currentActiveBuffer;

    volatile int32 spinLock;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SINGLETHREADGAMSCHEDULER_H_ */

