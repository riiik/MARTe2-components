/**
 * @file NI6259ADCGTest.cpp
 * @brief Source file for class NI6259ADCGTest
 * @date 14/02/2017
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
 * the class NI6259ADCGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MDSWriterNodeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MDSWriterNodeGTest,TestConstructor) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MDSWriterNodeGTest,TestInitialise_NoDecimatedNodeName) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_NoDecimatedNodeName());
}

TEST(MDSWriterNodeGTest,TestInitialise_DecimatedNodeName) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_DecimatedNodeName());
}

TEST(MDSWriterNodeGTest,TestInitialise_SamplePhase) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_SamplePhase());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_BadMakeSegmentAfterNWrites) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadMakeSegmentAfterNWrites());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_BadMinMaxResampleFactor) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadMinMaxResampleFactor());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_BadNumberOfElements) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadNumberOfElements());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_BadPeriod) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadPeriod());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_BadType) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoType());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_NoMakeSegmentAfterNWrites) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoMakeSegmentAfterNWrites());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_NoMinMaxResampleFactor) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoMinMaxResampleFactor());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_NoNodeName) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoNodeName());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_NoNumberOfElements) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoNumberOfElements());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_NoPeriod) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoPeriod());
}

TEST(MDSWriterNodeGTest,TestInitialise_False_NoType) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoType());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_UInt16) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_UInt16());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_Int16) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_Int16());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_UInt32) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_UInt32());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_Int32) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_Int32());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_UInt64) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_UInt64());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_Int64) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_Int64());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_Float32) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_Float32());
}

TEST(MDSWriterNodeGTest,TestInitialise_Type_Float64) {
    MDSWriterNodeTest test;
    ASSERT_TRUE(test.TestInitialise_Type_Float64());
}
