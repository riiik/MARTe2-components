/**
 * @file DANStream.h
 * @brief Header file for class DANStream
 * @date 04/04/2017
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

 * @details This header file contains the declaration of the class DANStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAN_DANSTREAM_H_
#define DAN_DANSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "dan/dan_Source.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "TypeDescriptor.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Wraps a DAN stream (see dan_publisher_openStream).
 * @details The DANSource will create a new DANStream for each signal type/signal frequency pair.
 */
class DANStream {
public:
    /**
     * @brief Constructor which assigns the DANStream parameters.
     * @param[in] tdIn the TypeDescriptor of DANStream.
     * @param[in] baseName the name of the DANSource, which will be used to register the stream with the name baseName_TypeDescriptor::GetTypeNameFromTypeDescriptor(tdIn).
     * @param[in] danBufferMultiplierIn the number of buffers that will be used by the DAN library to store the signals in order to avoid buffer overwrites after a PutData.
     * @param[in] samplingFrequencyIn the stream sampling frequency.
     * @param[in] numberOfSamplesIn the number of samples written on every PutData call.
     */
    DANStream(TypeDescriptor tdIn, StreamString baseName, uint32 danBufferMultiplierIn, uint32 samplingFrequencyIn, uint32 numberOfSamplesIn);

    /**
     * @brief Frees the allocated memory and calls dan_publisher_unpublishSource.
     */
    ~DANStream();

    /**
     * @brief Gets the signal type associated to this stream.
     * @return the signal type associated to this stream.
     */
    TypeDescriptor GetType();

    /**
     * @brief Gets the signal sampling frequency associated to this stream.
     * @return the signal sampling frequency associated to this stream.
     */
    uint32 GetSamplingFrequency();

    /**
     * @brief Adds a new signal to this stream.
     * @param[in] signalIdx the index of the signal in the DANSource.
     */
    void AddSignal(uint32 signalIdx);

    /**
     * @brief All the signals have been added. Call dan_publisher_publishSource_withDAQBuffer with the final buffer size.
     * @details The computed buffer size will be given by numberOfSignals * typeSize * numberOfSamples * danBufferMultiplier
     */
    void Finalise();

    /**
     * @brief Streams the signals data into DAN.
     * @details The time stamp will be:
     * - if useExternalAbsoluteTimingSignal the time is read directly from the signal set with SetAbsoluteTimeSignal and is assumed to be the absolute time in nano-seconds from the Epoch.
     * - if useExternalRelativeTimingSignal the relative time will be read directly from the signal set with SetRelativeTimeSignal and added to the time set in SetAbsoluteStartTime.
     * - otherwise the number of times this function has been called (stored in the counter), multiplied by the period in nano-seconds will be added to the time set in SetAbsoluteStartTime.
     * @return true if dan_publisher_putDataBlock returns >= 0.
     */
    bool PutData();

    /**
     * @brief Opens the DANStream.
     * @return true if dan_publisher_openStream returns 0.
     */
    bool OpenStream();

    /**
     * @brief Close the DANStream.
     * @return true if dan_publisher_closeStream returns 0.
     */
    bool CloseStream();

    /**
     * @brief Returns the memory address associated to the signal with index = signalIdx.
     * @param[in] signalIdx the index of the signal whose memory is to be retrieved.
     * @param[out] signalAddress where the memory address will be assigned.
     * @return true if the signalIdx exists.
     */
    bool GetSignalMemoryBuffer(const uint32 signalIdx, void*& signalAddress);

    /**
     * @brief Resets the counter used by PutData.
     */
    void Reset();

    /**
     * @brief Sets the absolute time signal (in nano-seconds from the Epoch) to be used by the PutData.
     * @param[in] timeAbsoluteSignalIn the absolute time signal (in nano-seconds from the Epoch) to be used by the PutData.
     */
    void SetAbsoluteTimeSignal(uint64 *timeAbsoluteSignalIn);

    /**
     * @brief Sets the relative time signal (in micro-seconds) to be used by the PutData.
     * @param[in] timeAbsoluteSignalIn the relative time signal (in micro-seconds) to be used by the PutData.
     */
    void SetRelativeTimeSignal(uint32 *timeRelativeSignalIn);

    /**
     * @brief Sets the starting absolute time to be used by the PutData.
     * @param[in] absoluteStartTimeIn the starting absolute time to be used by the PutData.
     */
    void SetAbsoluteStartTime(uint64 absoluteStartTimeIn);

private:
    /**
     * The type descriptor of the stream.
     */
    TypeDescriptor td;

    /**
     * Stores the signal offsets in the DANSource.
     */
    uint32 *signalIndexOffset;

    /**
     * Holds the memory required to store the samples of all signals.
     */
    char8 *blockMemory;

    /**
     * The DAN memory is interleaved so that the blockMemory needs to be translated into the this interleaved memory.
     */
    char8 *blockInterleavedMemory;

    /**
     * Number of signals held by the DANStream.
     */
    uint32 numberOfSignals;

    /**
     * The size of the type descriptor of the stream.
     */
    uint32 typeSize;

    /**
     * The size of the block memory (numberOfSignals * typeSize * numberOfSamples)
     */
    uint32 blockSize;

    /**
     * Pointer to the dan_Source
     */
    dan_Source danSource;

    /**
     * The name of DANSource that holds this DANStream.
     */
    StreamString baseName;

    /**
     * Number of buffers hold by the DAN multiplier.
     */
    uint32 danBufferMultiplier;

    /**
     * Sampling frequency of the signals acquired by this DANStream.
     */
    uint32 samplingFrequency;

    /**
     * 1 / Sampling frequency of the signals acquired by this DANStream.
     */
    uint64 periodNanos;

    /**
     * Number of samples of the signals acquired by this DANStream.
     */
    uint32 numberOfSamples;

    /**
     * Number of times PutData has been called.
     */
    uint64 writeCounts;

    /**
     * True if an external absolute timing signal will be used to time stamp the data.
     */
    bool useExternalAbsoluteTimingSignal;

    /**
     * True if an external relative timing signal will be used to time stamp the data.
     */
    bool useExternalRelativeTimingSignal;

    /**
     * Pointer to the external absolute timing signal that will be used (if useExternalAbsoluteTimingSignal = true) to time stamp the data.
     */
    uint64 *timeAbsoluteSignal;

    /**
     * Pointer to the external relative timing signal that will be used (if useExternalRelativeTimingSignal = true) to time stamp the data.
     */
    uint32 *timeRelativeSignal;

    /**
     * The absolute value of the time of start (see PutData)0.
     */
    uint64 absoluteStartTime;

    /**
     * The name of the DANSource that holds this DANStream.
     */
    StreamString danSourceName;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_DAN_DANDATABLOCK_H_ */

