/**
 * @file Directory.h
 * @brief Header file for class Directory
 * @date 04/11/2015
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

 * @details This header file contains the declaration of the class Directory
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
#include "GeneralDefinitions.h"
#include "TimeStamp.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,DirectoryCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class which encapsulates attributes and information about a file or a directory.
 */
class Directory: public LinkedListable {


public:
    /**
     * @brief Default constructor.
     * @param[in] path is the path of the file/directory
     * @post
     *   GetName() = path
     */
    Directory(const char8 * const path = NULL);

    /**
     * @brief Default destructor.
     * @post
     *   GetName() = NULL
     */
    ~Directory();

    /**
     * @brief Sets this file/directory absolute path.
     * @param[in] path is the absolute path of the file/directory.
     * @return true if the file/directory path is correctly set.
     * @post
     *   GetName() = path
     */
    bool SetByName(const char8 * const path);

    /**
     * @brief Gets the file/directory name.
     * @return the file/directory absolute path name.
     */
    const char8 *GetName() const;

    /**
     * @brief Check if this instance represents is a directory.
     * @return true if this instance represents is a directory.
     */
    bool IsDirectory() const;

    /**
     * @brief Check if this instance represents a file.
     * @return true if this instance represents a file.
     */
    bool IsFile() const;

    /**
     * @brief Gets the file/directory size.
     * @return the file/directory size.
     */
    uint64 GetSize() const;

    /**
     * @brief Gets the last write time.
     * @return the last write time.
     */
    TimeStamp GetLastWriteTime() const;

    /**
     * @brief Gets the last access time.
     * @return the last access time.
     */
    TimeStamp GetLastAccessTime() const;


    /**
     * @brief Creates a new file/directory.
     * @param[in] path the absolute path of the new file/directory to be created.
     * @param[in] isFile specifies if a file or a folder should be created.
     * @return true if the file/directory is created correctly. Note that if a file is being
     * created and it already exists, this will not affect the creation of the file.
     * @pre
     *    IsDirectory() = true && Exists() = false
     * @post
     *    GetName() = path
     */
    static bool Create(const char8 * const path,
                       const bool isFile = false);

    /**
     * @brief Deletes an existent file/directory.
     * @param[in] path the path of the file/directory to be deleted.
     * @return true if the file/directory is deleted correctly, false otherwise.
     */
    static bool Delete(const char8 * const path);

    /**
     * @brief Check if a file/directory exists.
     * @param[in] path the path of the file/directory to check.
     * @return true if the file/directory exists, false otherwise.
     */
    static bool Exists(const char8 * const path);

private:
    /**
     * The file or directory name
     */
    char8 *fname;

    /**
     * File/directory attributes.
     */
    DirectoryCore directoryHandle;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORY_H_ */

