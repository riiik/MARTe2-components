/**
 * @file TypeConversionTest.h
 * @brief Header file for class TypeConversionTest
 * @date 12/11/2015
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

 * @details This header file contains the declaration of the class TypeConversionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECONVERSIONTEST_H_
#define TYPECONVERSIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TypeConversion.h"
#include "String.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

template<typename T1, typename T2>
struct TypeToTypeTableTest {
    T1 typeToConvert;
    T2 result;
    bool go;
    bool expected;
};

template<typename T1, typename T2, uint32 nCols>
struct TypeToTypeVectorTableTest {
    T1 typeToConvert[nCols];
    T2 result[nCols];
    bool go;
};

class TypeConversionTest {
public:
    template<typename T1, typename T2>
    bool TestTypeConvert(const TypeToTypeTableTest<T1, T2>* table);

    template<typename T1, typename T2, uint32 nCols>
    bool TestTypeConvertVector(const TypeToTypeVectorTableTest<T1, T2, nCols>* table);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T1, typename T2>
bool TypeConversionTest::TestTypeConvert(const TypeToTypeTableTest<T1, T2>* table) {

    uint32 i = 0;
    while (table[i].go) {
        T2 element;
        T2 result = table[i].result;
        T1 toConvert = table[i].typeToConvert;

        bool ret = TypeConvert(element, toConvert);

        if (element != result) {
            //  printf("\n%s %s %d\n", element.Buffer(), result.Buffer(), i);

            return false;
        }
        if (ret != table[i].expected) {
            printf("\n%d\n",i);
            return false;
        }

        i++;
    }
    return true;

}

template<typename T1, typename T2, uint32 nCols>
bool TypeConversionTest::TestTypeConvertVector(const TypeToTypeVectorTableTest<T1, T2, nCols>* table) {

    uint32 i = 0;
    while (table[i].go) {
        T2 element[nCols];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;

        TypeConvert(element, table[i].typeToConvert);

        for (uint32 j = 0; j < nCols; j++) {

            if (element[j] != table[i].result[j]) {
                printf("\n%d %d %d\n", element[j], table[i].result[j], i);

                return false;
            }
        }

        i++;
    }
    return true;

}

#endif /* TYPECONVERSIONTEST_H_ */

