/**
 * @file CallRegisteredMethodLauncherTest.cpp
 * @brief Source file for class CallRegisteredMethodLauncherTest
 * @date 27/07/2016
 * @author Ivan Herrero
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
 * the class CallRegisteredMethodLauncherTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CallRegisteredMethodLauncherTest.h"
#include "CallRegisteredMethodLauncher.h"
#include "ClassWithCallableMethods.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

/**
 * @brief This class is a fresh version of ClassWithCallableMethods, providing
 * the same methods but without automatic registering.
 */
class ClassWithCallableMethods2: public ClassWithCallableMethods {};

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CallRegisteredMethodLauncherTest::CallRegisteredMethodLauncherTest() {
}

CallRegisteredMethodLauncherTest::~CallRegisteredMethodLauncherTest() {
}

bool CallRegisteredMethodLauncherTest::TestDefaultConstructor() {
    bool result;
    /*
     * The state of the object after construction is not queryable, so it is
     * not possible to make a unit test for it. Therefore, the unit test
     * defaults to true.
     */
    result = true;
    return result;
}

bool CallRegisteredMethodLauncherTest::TestTest() {
    using namespace MARTe;
    bool result = true;
    ClassRegistryItem* const cri = ClassRegistryItemT<ClassWithCallableMethods2>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod };
    const char* names = "&ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod";

    /*
     * Warning about the registered methods with a cast like:
     *
     *   (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod
     *
     * If the method does not exist in the derived class, the cast must be referred to the base class, while the method must be to the derived one.
     */

    ClassMethodsRegistryItem cmri(cri, cmim, names);
    {
        bool status;
        ClassWithCallableMethods2 context;
        CallRegisteredMethodLauncher target(&context, "OverloadedMethod");
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod()");
    }

    return result;
}

bool CallRegisteredMethodLauncherTest::TestGetResults() {
    bool result;
    /*
     * The method GetResults() work in tandem with Test(), in fact the
     * execution of GetResults() is linked to the last execution of Test(),
     * so it is not worth it to create a different unit test. Therefore,
     * we call TestTest() again.
     */
    result = TestTest();
    return result;
}
