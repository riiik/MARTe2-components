/**
 * @file XMLParser.cpp
 * @brief Source file for class XMLParser
 * @date 09/dic/2015
 * @author pc
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
 * the class XMLParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "XMLParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = { 0

, 3, 11, 12, 24, 3, 12, 13, 20, 11, 13, 1, 25, 2, 3, 14, 1, 4, 2, 3, 26, 15, 13, 1, 25, 2, 3, 5, 27, 2, 6, 14, 1, 4, 2, 3, 26, 10, 13, 1, 28, 2, 3, 18, 1, 4, 2,
        3, 2, 14, 15, 2, 14, 16, 2, 14, 17, 3, 15, 29, 19, 5, 16, 7, 15, 21, 8, 5, 17, 7, 16, 22, 8, 3, 18, 13, 23, 2, 19, 2, 2, 19, 9, 3, 20, 13, 20, 1, 20, 3,
        21, 15, 21, 2, 21, 30, 3, 22, 16, 22, 2, 22, 31, 3, 23, 13, 23, 2, 23, 32, 0 };

static uint32 Production_row[] = { 0

, 1, 5, 9, 21, 37, 48, 51, 54, 57, 61, 67, 73, 77, 80, 83, 87, 89, 93, 96, 100, 103, 107, 0 };

static uint32 ParseArray[] = {

0, 0, 1, 6, 2, 17, 9, 13, 24, 15, 6, 18, 17, 9, 14, 19, 20, 23, 16, 10, 11, 12, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static uint32 Parse_row[] = { 0

, 1, 3, 16, 1, 4, 12, 13, 20, 5, 8, 3, 8, 21, 0 };

static uint32 Conflict[] = {

0, 0, 0, 26, 7, 21, 27, 22, 0, 8, 0, 7, 5, 3, 0, 0, 4, 0, 3, 0, 3 };

static uint32 Conflict_row[] = { 0

, 1, 2, 3, 3, 11, 0 };

/*
 #define START_SYMBOL 11
 #define END_OF_SLK_INPUT_ 10
 #define START_STATE 0
 #define START_CONFLICT 23
 #define END_CONFLICT 28
 #define START_ACTION 24
 #define END_ACTION 33
 #define TOTAL_CONFLICTS 5
 #define PARSE_STACK_SIZE 512
 */
static const uint32 Constants[] = { 11u, 10u, 0u, 23u, 28u, 24u, 33u, 5u };

static const char8 * Nonterminal_name[] = { "0"

, "expression", "cdbFile", "expressions", "variables", "scalar", "vector", "matrix", "block", "token", "expressions_*", "scalar_*", "vector_*",
        "expressions_2_*" };

static const char8 * Terminal_name[] = { "0"

, "<", "STRING", ">", "/", "(", ")", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };

static const char8 * Action_name[] = { "0"

, "__End", "__GetNodeName", "__AddLeaf", "__GetTypeCast", "__CreateNode", "__AddScalar", "__EndVector", "__EndMatrix", "__BlockEnd" };

/*
 #define START_SYMBOL 11
 #define START_ACTION 24
 #define END_ACTION 33
 #define GET_NONTERMINAL_NAME(symbol) (Nonterminal_name [symbol - 10])
 #define GET_TERMINAL_NAME(symbol) (Terminal_name [symbol])
 #define GET_ACTION_NAME(symbol) (Action_name [symbol-(START_ACTION-1)])
 #define GET_PRODUCTION_NAME(number) (Production_name [number])

 */
static const char8 *GetActionName(const uint32 symbol) {
    return Action_name[symbol - (Constants[ParserConstant::START_ACTION] - 1u)];
}

static const char8 *GetNonTerminalName(const uint32 symbol) {
    return Nonterminal_name[symbol - (Constants[ParserConstant::START_SYMBOL] - 1u)];
}

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

XMLParser::XMLParser(StreamI &stream,
                     ConfigurationDatabase &databaseIn,
                     BufferedStreamI * const err) :
        ParserI(stream, databaseIn, err, XMLGrammar) {
    Action[0] = 0;
    Action[1] = &XMLParser::End;
    Action[2] = &XMLParser::GetNodeName;
    Action[3] = &XMLParser::AddLeaf;
    Action[4] = &XMLParser::GetTypeCast;
    Action[5] = &XMLParser::CreateNode;
    Action[6] = &XMLParser::AddScalar;
    Action[7] = &XMLParser::EndVector;
    Action[8] = &XMLParser::EndMatrix;
    Action[9] = &XMLParser::BlockEnd;

}

XMLParser::~XMLParser() {

}

void XMLParser::Execute(uint32 number) {
    (this->*Action[number])();
}

const char8 *XMLParser::GetSymbolName(const uint32 symbol)const  {
    const char8 *symbolName = static_cast<const char8 *>(NULL);
    if((symbol >= Constants[ParserConstant::START_ACTION]) && (symbol < Constants[ParserConstant::END_ACTION])) {
        symbolName=GetActionName(symbol);
    }
    else if(symbol >= Constants[ParserConstant::START_SYMBOL]) {
        symbolName=GetNonTerminalName(symbol);
    }
    else if(symbol > 0u) {
        symbolName=GetTerminalName(symbol);
    }
    else {
        symbolName="not a symbol";
    }
    return symbolName;
}

uint32 &XMLParser::GetProduction(const uint32 index)const  {
    return Production[index];
}

uint32 XMLParser::GetProductionRow(const uint32 index)const  {
    return Production_row[index];
}

uint32 XMLParser::GetParse(const uint32 index)const  {
    return ParseArray[index];
}

uint32 XMLParser::GetParseRow(const uint32 index)const  {
    return Parse_row[index];
}

uint32 XMLParser::GetConflict(const uint32 index)const  {
    return Conflict[index];
}

uint32 XMLParser::GetConflictRow(const uint32 index)const  {
    return Conflict_row[index];
}

uint32 XMLParser::GetConstant(const uint32 index)const  {
    return Constants[index];
}

uint32 XMLParser::GetConditionalProduction(const uint32 symbol)const  {
    return 0u;
}

uint32 XMLParser::GetPredictedEntry(const uint32 productionNumber,
                                    const uint32 tokenId,
                                    const uint32 level,
                                    const uint32 x)const  {
    return 0u;
}

}
