/**
 * @file MessageI.h
 * @brief Header file for class MessageI
 * @date Apr 5, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class MessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEI_H_
#define MESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Message.h"
#include "TimeoutType.h"
#include "ReferenceT.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The interface which has to be injected in an Object giving it the skills for sending and receiving
 * Messages.
 * @details To implement a new MARTe::Object able to send and receive messages, it is necessary a declare a class which inherits from both
 * Object and MessageI.
 */
class DLL_API MessageI {
public:

    /**
     * @brief Constructor.
     */
    MessageI();

    /**
     * @brief Destructor.
     */
    virtual ~MessageI();

    /**
     * @brief Sends a Message to the destination (specified in Message).
     * @details Depending on the flags to be read from \a message, this function can behave in synchronous or asynchronous mode,
     * wait or not for a reply, ecc.
     * If Message::ExpectsReply() == true then a reply message is requested and expected to be sent asynchronously to this object.
     * if Message::ExpectsImmediateReply() == true then reply message is requested and expected to be returned at the end of this call.
     * @param[in,out] message is the message to be sent. It will be modified to contain the destination reply if ExpectsImmediateReply is true.
     * @param[in] sender is the Object sending the message.

     * @return
     *   ErrorManagement::noError() if the destination object is found and the called function returns true.
     *   ErrorManagement::fatalError if the function to be called returns false.
     *   ErrorManagement::unsupportedFeature if the message was refused
     *   ErrorManagement::timeout if a wait for reply times out
     *   ErrorManagement::communicationError if the reply was not produced when requested
     *   ErrorManagement::parametersError if the message is invalid or if sender is NULL and reply was expected
     */
    static ErrorManagement::ErrorType SendMessage(ReferenceT<Message> &message,const Object * const sender = NULL_PTR(Object *));

    /**
     * @brief Before calling SendMessage, the message flag ExpectsImmediateReply() is set and the message timeout is set as maxWait.
     * @param[in,out] message is the message to be sent. It will be modified to contain the reply.
     * @param[in] sender is the Object sending the message.
     * @param[in] maxWait is the maximum time allowed waiting for the message reply.
     */
    static ErrorManagement::ErrorType SendMessageAndWaitReply(ReferenceT<Message> &message,
                                                              const Object * const sender = NULL_PTR(Object *),
                                                              const TimeoutType &maxWait = TTInfiniteWait);

    /**
     * @brief Marks the message to be sent requiring a late reply before sending it.
     * @param[in,out] message is the message to be sent. It can be modified if the destination re-sends it to the sender as a reply.
     * @param[in] sender is the Object sending the message.
     */
    static ErrorManagement::ErrorType SendMessageAndExpectReplyLater(ReferenceT<Message> &message,
                                                                     const Object * const sender = NULL_PTR(Object*));

protected:

    /**
     * @brief Default message handling mechanism.
     * @details Handles the reception of a message and by default simply calls SortMessage(). Can be overridden to implement message Queues etc...
     * @param[in,out] message is the received to be received.
     * @return
     *   ErrorManagement::noError if the function specified in \a message is called correctly and returns true.
     *   ErrorManagement::unsupportedFeature if something goes wrong trying to call the registered function.
     *
     */
    virtual ErrorManagement::ErrorType ReceiveMessage(ReferenceT<Message> &message);

    /**
     * TODO
     * Default message sorting mechanism
     * By default checks if there are usable registered methods
     * Otherwise calls HandleMessage.
     * in the case of delayed reply, the reply will be sent from here if a reply message is returned from the user code.
     * otherwise the user has to Send the reply independently
     * */
    virtual ErrorManagement::ErrorType SortMessage(ReferenceT<Message> &message);

    /**
     * TODO
     * Default message handling mechanism
     * By default refuses messages and returns false
     * */
    virtual ErrorManagement::ErrorType HandleMessage(ReferenceT<Message> &message);

private:
    /**
     * TODO
     *
     * */
    static ReferenceT<MessageI> FindDestination(CCString destination);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEI_H_ */

