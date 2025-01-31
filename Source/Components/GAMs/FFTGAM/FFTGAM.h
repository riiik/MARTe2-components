/**
 * @file FFTGAM.h
 * @brief Header file for class FFTGAM
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

 * @details This header file contains the declaration of the class FFTGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FFT_GAM_H_
#define FFT_GAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "MessageI.h"
#include "CircularStaticList.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which provides constant output signals. 
 * @details This GAM provides constant output signals, the value of which is defined through configuration, and
 * may be asynchronously altered using MARTe messages sent e.g. from a StateMachine.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 *
 * <pre>
 * +Constants = {
 *     Class = ConstantGAM
 *     OutputSignals = {
 *         Signal1 = {
 *             DataSource = "DDB"
 *             Type = uint32
 *             Default = 0
 *         }
 *         Signal2 = {
 *             DataSource = "DDB"
 *             Type = int32
 *             Default = 100
 *         }
 *         Signal3 = {
 *             DataSource = "DDB"
 *             Type = int8
 *             NumberOfDimensions = 1
 *             NumberOfElements = 8
 *             Default = {1 2 3 4 5 6 7 8}
 *         }
 *     }
 * }
 * </pre>
 *
 * The GAM registers a messageable 'SetOutput' method which allows to update signals through messages:
 *
 * <pre>
 * +Message = {
 *     Class = Message
 *     Destination = "Functions.FFTs"
 *     Function = "SetOutput"
 *     +Parameters = {
 *         Class = ConfigurationDatabase
 *         SignalName = "Signal1" // The name of the signal to modify, or
 *         SignalIndex = 0 // Alternatively, the index of the signal.
 *         SignalValue = 10
 *     }
 * }
 * </pre>
 */
class FFTGAM: public GAM, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    FFTGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~FFTGAM();

    // the buffer upon which the FFT transformation will be performed
    //
    // MARTe::CircularStaticList< Type >::CircularStaticList( 	( 	const uint32  	bufferSize	) 	)
    // MARTe::CircularStaticList<uint32>::CircularStaticList() buffer;
    // CircularStaticList buffer;
    void * buffer;


    /**
     * @brief Initialises the output signal memory with default values provided through configuration.
     * @return true if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() && GetNumberOfInputSignals() == 0 &&
     *   for each signal i: The default value provided corresponds to the expected type and dimensionality.
     */
    virtual bool Setup();

    /**
     * @brief Execute method. NOOP.
     * @return true.
     */
    virtual bool Execute();

    template <typename Type> bool ExecuteT();

    /**
     * @brief SetOutput method.
     * @details The method is registered as a messageable function. It assumes the ReferenceContainer
     * includes a reference to a StructuredDataI instance which contains a valid 'SignalName' attribute, 
     * or alternatively, a valid 'SignalIndex' from which the output signal can be identified. 
     * The 'SignalValue' attribute must match the expected type and dimensionality of the output signal.
     * @return ErrorManagement::NoError if the pre-conditions are met, ErrorManagement::ParametersError
     * otherwise.
     * @pre
     *   'SignalIndex' < GetNumberOfOutputSignals() &&
     *   The 'SignalValue' provided corresponds to the expected type and dimensionality.
     */
    ErrorManagement::ErrorType SetOutput(ReferenceContainer& message);

private:
    /**
     * @brief The common signal type attribute.
     */
    TypeDescriptor signalType;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FFT_GAM_H_ */

