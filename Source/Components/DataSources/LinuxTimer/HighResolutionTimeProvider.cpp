/**
 * @file HighResolutionTimeProvider.cpp
 * @brief Source file for class HighResolutionTimeProvider
 * @date 31/10/2019
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimeProvider (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "HighResolutionTimeProvider.h"
#include "HighResolutionTimer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    HighResolutionTimeProvider::HighResolutionTimeProvider() : TimeProvider() {
        yieldSleepPercentage = 0u;        
        SleepProvidingFunction = &HighResolutionTimeProvider::NullDelegate;
    }

    HighResolutionTimeProvider::~HighResolutionTimeProvider() {
    }

    bool HighResolutionTimeProvider::Initialise(StructuredDataI &data) {
        bool returnValue = Object::Initialise(data);
        
        StreamString tempSleepNature;
        uint8 tempSleepPercentage = 0u;

        if(data.Read("SleepNature", tempSleepNature)) {
            if(tempSleepNature == "Busy") {
                if(data.Read("SleepPercentage", tempSleepPercentage)) {
                    if(tempSleepPercentage <= 100u) {
                        yieldSleepPercentage = tempSleepPercentage;
                        SleepProvidingFunction = &HighResolutionTimeProvider::SemiBusy;
                        REPORT_ERROR(ErrorManagement::Information, "Sleep percentage was specified (%d %)", yieldSleepPercentage);
                        REPORT_ERROR(ErrorManagement::Information, "SemiBusy delegate selected");
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Sleep percentage cannot be > 100");
                        returnValue = false;
                    }
                }
                else {
                    SleepProvidingFunction = &HighResolutionTimeProvider::BusySleep;
                    REPORT_ERROR(ErrorManagement::Information, "Sleep percentage parameter not specified, 0 assumed");
                    REPORT_ERROR(ErrorManagement::Information, "BusySleep delegate selected");
                }                
            }
            else if(tempSleepNature == "Default") {
                SleepProvidingFunction = &HighResolutionTimeProvider::NoMore;
                REPORT_ERROR(ErrorManagement::Information, "Default sleep nature selected (Sleep::NoMore mode)");
                REPORT_ERROR(ErrorManagement::Information, "NoMore delegate selected");
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Specified sleep nature [%s] is not valid", tempSleepNature.Buffer());
                returnValue = false;
            }
        }
        else {
            SleepProvidingFunction = &HighResolutionTimeProvider::NoMore;
            REPORT_ERROR(ErrorManagement::Information, "Sleep nature was not specified, falling back to default (Sleep::NoMore mode)");
        }
    
        return returnValue;    
    }

    uint64 HighResolutionTimeProvider::Counter() {
        return HighResolutionTimer::Counter();
    }

    float64 HighResolutionTimeProvider::Period() {
        return HighResolutionTimer::Period();
    }

    uint64 HighResolutionTimeProvider::Frequency() {
        return HighResolutionTimer::Frequency();
    }

    bool HighResolutionTimeProvider::BusySleep(const uint64 start, const uint64 delta) {
        while ((HighResolutionTimer::Counter() - start) < delta) {
            ;
        }
        //As long the HRT is based on the internals, there should be no way of failing
        return true;
    }

    bool HighResolutionTimeProvider::SemiBusy(const uint64 start, const uint64 delta) {
        float64 totalSleepTime = static_cast<float32> (static_cast<float64> (delta) * Period());
        uint8 busyPercentage = (100u - yieldSleepPercentage);
        float64 busyTime = totalSleepTime * (static_cast<float32> (busyPercentage) / 100.F);
        Sleep::SemiBusy(totalSleepTime, busyTime);
        
        //As long the HRT is based on the internals, there should be no way of failing
        return true;
    }

    bool HighResolutionTimeProvider::NoMore(const uint64 start, const uint64 delta) {
        Sleep::NoMore(delta * Period());

        //As long the HRT is based on the internals, there should be no way of failing
        return true;
    }

    bool HighResolutionTimeProvider::Sleep(const uint64 start, const uint64 delta) {
        return (this->*SleepProvidingFunction)(start, delta);  
    }

    bool HighResolutionTimeProvider::NullDelegate(const uint64 start, const uint64 delta) {
        REPORT_ERROR(ErrorManagement::FatalError, "Call to the null delegate with %d start and %d delta.", start, delta);
        REPORT_ERROR(ErrorManagement::FatalError, "Reached uninitialized portion of the code");
        return false;
    }

    CLASS_REGISTER(HighResolutionTimeProvider, "1.0")
}
