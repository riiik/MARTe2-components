/**
 * @file MDSStructuredDataI.cpp
 * @brief Source file for class MDSStructuredDataI
 * @date 04/09/2018
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
 * the class MDSStructuredDataI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "MDSStructuredDataI.h"
#include "Reference.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MDSStructuredDataI::MDSStructuredDataI() {
    currentNode = NULL_PTR(MDSplus::TreeNode *);
    rootNode = NULL_PTR(MDSplus::TreeNode *);
    tree = NULL_PTR(MDSplus::Tree *);
}

MDSStructuredDataI::~MDSStructuredDataI() {
}

bool MDSStructuredDataI::Read(const char8* const name, const AnyType& value) {
    return false;
}

bool MDSStructuredDataI::Write(const char8 * const name, const AnyType &value) {
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    try {
        node = currentNode->getNode(name);
    }
    catch (const MDSplus::MdsException &exc) {
    }
    if (node == NULL_PTR(MDSplus::TreeNode *)) {
        node = currentNode->addNode(name, "ANY");
    }
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        MDSplus::Data *data;
        TypeDescriptor mdsType = value.GetTypeDescriptor();
        if (mdsType == UnsignedInteger8Bit) {
            data = new MDSplus::Uint8(*reinterpret_cast<uint8 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger8Bit) {
            data = new MDSplus::Int8(*reinterpret_cast<int8 *>(value.GetDataPointer()));
        }
        else if (mdsType == UnsignedInteger16Bit) {
            data = new MDSplus::Uint16(*reinterpret_cast<uint16 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger16Bit) {
            data = new MDSplus::Int16(*reinterpret_cast<int16 *>(value.GetDataPointer()));
        }
        else if (mdsType == UnsignedInteger32Bit) {
            data = new MDSplus::Uint32(*reinterpret_cast<uint32 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger32Bit) {
            data = new MDSplus::Int32(*reinterpret_cast<int32 *>(value.GetDataPointer()));
        }
        else if (mdsType == UnsignedInteger64Bit) {
            data = new MDSplus::Uint64(*reinterpret_cast<uint64 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger64Bit) {
            data = new MDSplus::Int64(*reinterpret_cast<int64 *>(value.GetDataPointer()));
        }
        else if (mdsType == Float32Bit) {
            data = new MDSplus::Float32(*reinterpret_cast<float32 *>(value.GetDataPointer()));
        }
        else if (mdsType == Float64Bit) {
            data = new MDSplus::Float64(*reinterpret_cast<float64 *>(value.GetDataPointer()));
        }
        else if (mdsType == CharString) {
            data = new MDSplus::String(reinterpret_cast<char8 *>(value.GetDataPointer()));
        }
        else if (mdsType == ConstCharString) {
            data = new MDSplus::String(reinterpret_cast<const char8 *>(value.GetDataPointer()));
        }
        if (ok) {
            node->putData(data);
            MDSplus::deleteData(data);
        }
    }

    return ok;
}

AnyType MDSStructuredDataI::GetType(const char8* const name) {
    AnyType at;

    MDSplus::TreeNode *node = currentNode->getNode(name);
    if (node != NULL_PTR(MDSplus::TreeNode *)) {
        StreamString mdsType = node->getDType();
        TypeDescriptor marteType;
        if (mdsType == "DTYPE_BU") {
            marteType = UnsignedInteger8Bit;
        }
        else if (mdsType == "DTYPE_B") {
            marteType = SignedInteger8Bit;
        }
        else if (mdsType == "DTYPE_WU") {
            marteType = UnsignedInteger16Bit;
        }
        else if (mdsType == "DTYPE_W") {
            marteType = SignedInteger16Bit;
        }
        else if (mdsType == "DTYPE_LU") {
            marteType = UnsignedInteger32Bit;
        }
        else if (mdsType == "DTYPE_L") {
            marteType = SignedInteger32Bit;
        }
        else if (mdsType == "DTYPE_QU") {
            marteType = UnsignedInteger64Bit;
        }
        else if (mdsType == "DTYPE_Q") {
            marteType = SignedInteger64Bit;
        }
        else if (mdsType == "DTYPE_FS") {
            marteType = Float32Bit;
        }
        else if (mdsType == "DTYPE_FT") {
            marteType = Float64Bit;
        }
        else {

        }
        at = AnyType(marteType, 0u, NULL_PTR(void *));

    }
    return at;
}

bool MDSStructuredDataI::Copy(StructuredDataI& destination) {
    return false;
}

bool MDSStructuredDataI::AddToCurrentNode(Reference node) {
    return false;
}

bool MDSStructuredDataI::MoveToRoot() {
    currentNode = rootNode;
    return true;
}

bool MDSStructuredDataI::MoveToAncestor(uint32 generations) {
    MDSplus::TreeNode *node = currentNode;
    uint32 i = 0u;
    while ((i < generations) && (node != NULL_PTR(MDSplus::TreeNode *))) {
        node = node->getParent();
        i++;
    }
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveAbsolute(const char8* const path) {
    MDSplus::TreeNode *node = tree->getNode(path);
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveRelative(const char8* const path) {
    MDSplus::TreeNode *node = currentNode->getNode(path);
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveToChild(const uint32 childIdx) {
    MDSplus::TreeNode *node = currentNode->getChild();
    uint32 i = 0u;
    while ((i < childIdx) && (node != NULL_PTR(MDSplus::TreeNode *))) {
        node = node->getBrother();
        i++;
    }
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::CreateAbsolute(const char8* const path) {
    MDSplus::TreeNode *node = rootNode->addNode(path, "STRUCTURE");
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::CreateRelative(const char8* const path) {
    MDSplus::TreeNode *node = currentNode->addNode(path, "STRUCTURE");
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::Delete(const char8* const name) {
    return false;
}

const char8* MDSStructuredDataI::GetName() {
    return currentNode->getNodeName();
}

const char8* MDSStructuredDataI::GetChildName(const uint32 index) {
    const char8* ret = NULL_PTR(const char8*);
    MDSplus::TreeNode *node = currentNode->getChild();
    uint32 i = 0u;
    while ((i < index) && (node != NULL_PTR(MDSplus::TreeNode *))) {
        node = node->getBrother();
        i++;
    }
    if (node != NULL_PTR(MDSplus::TreeNode *)) {
        ret = node->getNodeName();
    }
    return ret;
}

uint32 MDSStructuredDataI::GetNumberOfChildren() {
    return currentNode->getNumChildren();
}

void MDSStructuredDataI::SetTree(MDSplus::Tree *treeIn) {
    tree = treeIn;
    rootNode = tree->getDefault();
    currentNode = rootNode;
}



CLASS_REGISTER(MDSStructuredDataI, "1.0")
}
