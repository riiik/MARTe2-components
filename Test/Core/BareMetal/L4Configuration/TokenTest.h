/**
 * @file TokenTest.h
 * @brief Header file for class TokenTest
 * @date 02/12/2015
 * @author Giuseppe ferrò
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

 * @details This header file contains the declaration of the class TokenTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TOKENTEST_H_
#define TOKENTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Token.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class TokenTest {
public:
    bool TestDefaultConstructor();

    bool TestFullConstructor(uint32 id,
                             const char8 *description,
                             const char8 *data,
                             uint32 lineNumber);

    bool TestConstructorFromTokenInfo(uint32 id,
                                      const char8 * description,
                                      const char8 * const data,
                                      const uint32 lineNumber);

    bool TestGetId(uint32 id);

    bool TestAssignOperator(uint32 id,
                            const char8 * description,
                            const char8 * const data,
                            const uint32 lineNumber);

    bool TestGetDescription(const char8 * description);

    bool TestGetData(const char8 * data);

    bool TestGetLineNumber(uint32 lineNumber);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TOKENTEST_H_ */

