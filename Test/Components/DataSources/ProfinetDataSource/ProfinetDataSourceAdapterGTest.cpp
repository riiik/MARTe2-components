/**
 * @file ProfinetDataSourceAdapterGTest.cpp
 * @brief Source file for class ProfinetDataSourceGTest
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
 * the class ProfinetDataSourceGTest (public, protected, and private). Be aware that some
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
#include "ProfinetDataSourceAdapterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(ProfinetDataSourceAdapterGTest, TestConstructor) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ProfinetDataSourceAdapterGTest, TestSetBaseData) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestSetBaseData());
}

TEST(ProfinetDataSourceAdapterGTest, TestSetIdentificationAndMaintainanceData) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestSetIdentificationAndMaintainanceData());
}

TEST(ProfinetDataSourceAdapterGTest, TestSetLLDPData) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestSetLLDPData());
}

TEST(ProfinetDataSourceAdapterGTest, TestSlotAndSubslotManagement) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestSlotAndSubslotManagement());
}

TEST(ProfinetDataSourceAdapterGTest, TestInitialize) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestInitialize());
}

TEST(ProfinetDataSourceAdapterGTest, TestInitializeFail) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestInitializeFail());
}

TEST(ProfinetDataSourceAdapterGTest, TestGetDefaultLLDPRTClass2Status) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestGetDefaultLLDPRTClass2Status());
}

TEST(ProfinetDataSourceAdapterGTest, TestGetDefaultLLDPRTClass3Status) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestGetDefaultLLDPRTClass3Status());
}

TEST(ProfinetDataSourceAdapterGTest, TestGetDefaultLLDPAutonegotiationCapability) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestGetDefaultLLDPAutonegotiationCapability());
}

TEST(ProfinetDataSourceAdapterGTest, TestGetDefaultLLDPAutonegotiationSpeed) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestGetDefaultLLDPAutonegotiationSpeed());
}

TEST(ProfinetDataSourceAdapterGTest, TestGetDefaultLLDPMAUType) {
    ProfinetDataSourceAdapterTest test;
    ASSERT_TRUE(test.TestGetDefaultLLDPMAUType());
}