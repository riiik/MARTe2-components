/**
 * @file HighResolutionTimer.h
 * @brief Header file for class HighResolutionTimer
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class HighResolutionTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIME_H_
#define HIGHRESOLUTIONTIME_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This class implements useful functions for high resolution timing using the cpu clock.
 *
 * @details These functions are used a lot in each functions that need Timeouts implementation like for examples semaphores.\n
 * Most of the implementation is delegated to HighResolutionTimerA.h which use very low level code (assembly)
 * for the Counter functions, while the Frequency and Period functions are delegated to HighResolutionTimerCalibratorOs.h
 * reading on a system file that returns the current cpu frequency.
 */
class HighResolutionTimer {

public:

    /**
     * @brief An high resolution time counter. Only valid on pentiums CPUs and above.
     * @details Reads the cpu ticks on an 64 bits integer.
     */
    static inline int64 Counter();

    /**
     * @brief An high resolution time counter.
     * @details Reads the cpu ticks on an 32 bits integer.
     */
    static inline uint32 Counter32();

    /**
     * @brief The length of a clock period in seconds.
     * @return the current period of the cpu.
     */
    static inline float64 Period();

    /**
     * @brief To interpret the value returned by HRTCounter.
     * @return the current frequency of the cpu.
     */
    static inline int64 Frequency();

    /**
     * @brief Converts HRT ticks to time.
     * @param[in] tStop is the final ticks number.
     * @param[in] tStart is the initial ticks number.
     * @return the time elapsed in TStop-TStart ticks in seconds
     */
    static inline float64 TicksToTime(int64 tStop,
                                     int64 tStart = 0);

    /**
     * @brief Get the current time stamp [microseconds, seconds, minutes, hour, day, month, year].
     * @see TimeValues.
     * @param[out] date is a TimeValues structure which must be filled by this method.
     */
    static inline bool GetTimeStamp(TimeValues &date);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,HighResolutionTimerA.h)
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,HighResolutionTimerOS.h)

#endif /* HIGHRESOLUTIONTIME_H_ */
