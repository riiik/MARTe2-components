/**
 * @file JsonParserTest.h
 * @brief Header file for class JsonParserTest
 * @date 10/12/2015
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

 * @details This header file contains the declaration of the class JsonParserTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef JSONPARSERTEST_H_
#define JSONPARSERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "JsonParser.h"
#include "ConfigurationDatabase.h"
#include "ParserTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class JsonParserTest {

public:

    bool TestConstructor();

    bool TestParseScalar();

    bool TestParseVector();

    bool TestParseMatrix();

    bool TestNestedBlocks();

    bool TestParseErrors(const char8 *configStringIn);

    bool TestStandardCast();

    bool TestExistentFile();

    template<typename T>
    bool TestTypeCast_Scalar(const TypeCastTableTest<T> *table);

    template<typename T, uint32 nElements>
    bool TestTypeCast_Vector(const TypeCastTableTestVector<T, nElements> *table);

    template<typename T, uint32 nRows, uint32 nCols>
    bool TestTypeCast_Matrix(const TypeCastTableTestMatrix<T, nRows, nCols> *table);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



template<typename T>
bool JsonParserTest::TestTypeCast_Scalar(const TypeCastTableTest<T> *table) {
    ConfigurationDatabase database;
    StreamString errors;

    uint32 i = 0;
    while (table[i].configString != NULL) {
        StreamString configString = table[i].configString;

        configString.Seek(0);
        JsonParser myParser(configString, database, &errors);
        bool ret = myParser.Parse();

        if (table[i].expectedRet != ret) {
            printf("failed ret %d\n", i);
            return false;
        }
        if (table[i].expectedRet) {

            T output;
            if (!database.Read(table[i].varName, output)) {
                printf("failed read %d\n", i);
                return false;
            }
            if (output != table[i].expectedResult) {
                printf("\nfailed result %d\n", i);
                return false;
            }
        }
        else {
            printf("\nerrors=%s, %d\n", errors.Buffer(), i);
        }

        i++;
    }
    return true;
}

template<typename T, uint32 nElements>
bool JsonParserTest::TestTypeCast_Vector(const TypeCastTableTestVector<T, nElements> *table) {
    ConfigurationDatabase database;
    StreamString errors;

    uint32 i = 0;
    while (table[i].configString != NULL) {
        StreamString configString = table[i].configString;

        configString.Seek(0);
        JsonParser myParser(configString, database, &errors);
        bool ret = myParser.Parse();

        if (table[i].expectedRet != ret) {
            printf("failed ret %d\n", i);
            return false;
        }
        if (table[i].expectedRet) {

            T output[nElements];
            if (!database.Read(table[i].varName, output)) {
                printf("failed read %d\n", i);
                return false;
            }
            for (uint32 h = 0; h < nElements; h++) {
                if (table[i].expectedResult[h] != output[h]) {
                    printf("\nfailed result %d %d\n", i, h);
                    return false;
                }
            }

        }
        else {
            printf("\nerrors=%s, %d\n", errors.Buffer(), i);
        }

        i++;
    }
    return true;
}

template<typename T, uint32 nRows, uint32 nCols>
bool JsonParserTest::TestTypeCast_Matrix(const TypeCastTableTestMatrix<T, nRows, nCols> *table) {
    ConfigurationDatabase database;
    StreamString errors;

    uint32 i = 0;
    while (table[i].configString != NULL) {
        StreamString configString = table[i].configString;

        configString.Seek(0);
        JsonParser myParser(configString, database, &errors);
        bool ret = myParser.Parse();

        if (table[i].expectedRet != ret) {
            printf("failed ret %d\n", i);
            return false;
        }
        if (table[i].expectedRet) {

            T output[nRows][nCols];
            if (!database.Read(table[i].varName, output)) {
                printf("failed read %d\n", i);
                return false;
            }
            for (uint32 h = 0; h < nRows; h++) {
                for (uint32 k = 0; k < nCols; k++) {
                    if (table[i].expectedResult[h][k] != output[h][k]) {
                        printf("\nfailed result %d %d %d\n", i, h, k);
                        return false;
                    }
                }
            }

        }
        else {
            printf("\nerrors=%s, %d\n", errors.Buffer(), i);
        }

        i++;
    }
    return true;
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* JSONPARSERTEST_H_ */

