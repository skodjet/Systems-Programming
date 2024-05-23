/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#define MAGIC_NUMBER 0x7261746D

void createArchive(char *inputDirectoryName, char *archiveName);
void extractDirectory(char *tarFileName);
void directoryRecurse(DIR *inputDirectory, char *directoryName, 
                      char *parentName, FILE *tarFile);
void fileRecurse(DIR *inputDirectory, char *inputDirectoryName, 
                 char *fileName, FILE *tarFile);
void hardLinkRecurse(char *hardLinkName, FILE *tarFile);
void printArchive(char *archiveName);
void set_inode(ino_t i, const char *f);
const char * get_inode(ino_t i);



extern char const ** Map;


int main(int argc, char *argv[]) {
    int cFlag, xFlag, tFlag, fFlag = 0;

    char *directoryName;
    char *argumentForF;
    

    int getoptValue = getopt(argc, argv, ":cxtf:");
    int modes = 0;

    // Read input form the command line
    while(getoptValue != -1) {

        switch(getoptValue) {
            case 'c':

                cFlag = 1;
                modes++;
                break;

            case 'x':

                xFlag = 1;
                modes++;
                break;

            case 't':

                tFlag = 1;
                modes++;
                break;

            case 'f':

                fFlag = 1;
                // Copy the argument given for f into argumentForF.
                int length = 0;

                length = strlen(optarg);

                argumentForF = (char *) malloc(length * sizeof(char));

                strncpy(argumentForF, optarg, length);
                break;

            // Error: No argument specified for -f
            case ':':
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);

            //Error: Unrecognized option
            case '?':
                fprintf(stderr, "Error: Unrecognized option -%c\n", optopt);
                exit(-1);

        }

        getoptValue = getopt(argc, argv, ":cxtf:");
    }

    // Multiple modes error
    if(modes > 1) {
        fprintf(stderr, "Error: Multiple modes specified.\n");
        exit(-1);
    }

    // No modes error
    if(modes == 0) {
        fprintf(stderr, "Error: No mode specified.\n");
        exit(-1);
    }

    // No tar file error
    if(fFlag == 0) {
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);
    }

    
    // copying a directory
    if(cFlag == 1) {

        // No directory target specified error
        if(argv[optind] == NULL) {
            fprintf(stderr, "Error: No directory target specified.\n");
            exit(-1);
        }

        int directoryNameLength = strlen(argv[optind]);
        directoryName = (char *) malloc((directoryNameLength + 1) * sizeof(char)); 

        strncpy(directoryName, argv[optind], directoryNameLength);
        createArchive(directoryName, argumentForF);
        free(directoryName);
    }

    // Extracting a directory
    if(xFlag == 1) {
        extractDirectory(argumentForF);
    }


    // Printing a directory
    if(tFlag == 1) {
        printArchive(argumentForF); 
    }

    free(argumentForF);
    if(Map != NULL) {
        free(Map);
    }

    return 0;
}



// Extracts the directory from the tar file.
void extractDirectory(char *tarFileName) {
    // Open the tar file
    FILE *tarFile;
    tarFile = fopen(tarFileName, "r");

    // Target does not exist error
    if(tarFile == NULL) {
        fprintf(stderr, "Specified target(\"%s\") does not exist.\n", tarFileName);
        exit(-1);
    }

    // Check the magic number
    int magic = getw(tarFile);

    if(magic != MAGIC_NUMBER) {
        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n", magic, MAGIC_NUMBER);
        exit(-1);
    }

    /* Loop through the entries and create the necessary files, 
       directories, and hard links.
    */
    struct stat tarStat;
    int statVal = stat(tarFileName, &tarStat);
    long tarSize = tarStat.st_size;
    long currentOffset = 0;

    while(currentOffset < tarSize) {

        // Get the entry inode value
        int freadVal;
        long *inode = (long *) malloc(sizeof(long) * 1);
        freadVal = fread(inode, 8, 1, tarFile);
        if(freadVal != 1) {
            perror("fread");
            exit(-1);
        }

        currentOffset += 8;

        // Get the entry name length
        int *nameLength = (int *) malloc(sizeof(int) * 1);
        freadVal = fread(nameLength, 4, 1, tarFile);
        if(freadVal != 1) {
            perror("fread");
            exit(-1);
        }

        currentOffset += 4;

        // Get the entry name

        char *name = (char *) malloc(sizeof(char) * (*nameLength) + 1);
        freadVal = fread(name, 1, *nameLength, tarFile);
        name[*nameLength] = '\0';

        currentOffset += *nameLength;

        // Check if the entry is a hard link
        int isHardLink = 0;
        const char *getInodeVal = get_inode(*inode);

        if(getInodeVal != NULL) {
            isHardLink = 1;
        }

        if(isHardLink == 0) {
            long inodeVal = *inode;

            set_inode(inodeVal, name);
        }


        // If the entry is a hard link, create the hard link.
        if(isHardLink == 1) {
            long inodeVal = *inode;

            const char *originalFilePath = get_inode(inodeVal);

            int linkVal = link(originalFilePath, name);

            if(linkVal == -1) {
                perror("link");
                exit(-1);
            }

            // Input the hard link inode number
            struct stat linkStat;
            int statVal = stat(name, &linkStat);
            if(statVal == -1) {
                perror("stat");
                exit(-1);
            }

            linkStat.st_ino = *inode;

            free(inode);
            free(nameLength);
            free(name);

            continue;
        }


        // Get the entry mode
        int *mode = (int *) malloc(sizeof(int) * 1);

        freadVal = fread(mode, 4, 1, tarFile);
        if(freadVal != 1) {
            perror("fread");
            exit(-1);
        }

        currentOffset += 4;

        /* fileType:
        0 = Directory. 
        1 = Regular file. 
        2 = Executable file. 
        */
        int fileType;

        char stringMode[6];
        int sprintfVal = sprintf(stringMode, "%o", *mode);
        if(sprintfVal < 0) {
            perror("sprintf");
            exit(-1);
        }

        // Determine the entry type and create the appropriate new entry
        FILE *newFile;
        DIR *newDir;


        // Create the directory and input its information
        if(stringMode[0] == '4') {
            fileType = 0;
            int mkdirVal = mkdir(name, *mode);
            if(mkdirVal == -1) {
                perror("mkdir");
                exit(-1);
            }

            // Input the inode number of the new directory
            struct stat dirStat;
            newDir = opendir(name);
            int statVal = stat(name, &dirStat);

            dirStat.st_ino = *inode;

            // Set the offset properly for the next file
            int fseekVal = fseek(tarFile, 8, SEEK_CUR);
            if(fseekVal != 0) {
                perror("fseek");
                exit(-1);
            }
            
            currentOffset += 8;

            closedir(newDir);

        // Create the regular file and input its information
        } else {
            fileType = 1;

            newFile = fopen(name, "w+");
            if(newFile == NULL) {
                perror("fopen");
                exit(-1);
            }

            // Input the file content
            int fseekVal = fseek(tarFile, 8, SEEK_CUR);
            if(fseekVal != 0) {
                perror("fseek");
                exit(-1);
            }

            currentOffset += 8;

            // Get the file size
            long fileSize[1];
            freadVal = fread(fileSize, 8, 1, tarFile);
            if(freadVal != 1) {
                perror("fread");
                exit(-1);
            }

            currentOffset += 8;

            // Write the file content
            char contentBuffer[fileSize[0] + 1];
            freadVal = fread(contentBuffer, 1, fileSize[0], tarFile);
            if(freadVal != fileSize[0]) {
                perror("fread");
                exit(-1);
            }
            
            FILE *fileToWriteTo = fopen(name, "a");
            if(fileToWriteTo == NULL) {
                perror("fopen");
                exit(-1);
            }

            int fwriteVal = fwrite(&contentBuffer, 1, fileSize[0], fileToWriteTo);
            if(fwriteVal != fileSize[0]) {
                perror("fwrite");
                exit(-1);
            }

            int fcloseVal = fclose(fileToWriteTo);
            if(fcloseVal == EOF) {
                perror("fclose");
                exit(-1);
            }


            contentBuffer[fileSize[0]] = '\0';

            // Reset the offset.
            long negativeOffset = 16 + fileSize[0];

            fseekVal = fseek(tarFile, -1 * (negativeOffset), SEEK_CUR);

            // Input the file inode number
            struct stat fileStat;
            int statVal = stat(name, &fileStat);
            if(statVal == -1) {
                perror("stat");
                exit(-1);
            }

            fileStat.st_ino = *inode;

            // Input file mode
            int chmodVal = chmod(name, *mode);
            if(chmodVal == -1) {
                perror("chmod");
                exit(-1);
            }

            // Get the current time
            struct timeval times[2];
            struct timeval accessVals;
            int gettimeVal = gettimeofday(&accessVals, NULL);
            if(gettimeVal == -1) {
                perror("gettimeofday");
                exit(-1);
            }

            times[0] = accessVals;

            // Get the modification time from the tar file
            struct timeval modValues;
            long modTimeBuffer[1];

            freadVal = fread(modTimeBuffer, 8, 1, tarFile);

            if(freadVal != 1) {
                perror("fread");
                exit(-1);
            }

            currentOffset += 8;

            modValues.tv_sec = modTimeBuffer[0];
            modValues.tv_usec = 0;
            times[1] = modValues;

            // Set the times for the file
            int utimesVal = utimes(name, times);
            if(utimesVal == -1) {
                perror("utimes");
                exit(-1);
            }

            // Increase currentOffset by file size + file content
            currentOffset += fileSize[0] + 8;

            fseekVal = fseek(tarFile, fileSize[0] + 8, SEEK_CUR);
            if(fseekVal != 0) {
                perror("fseek");
                exit(-1);
            }

            fclose(newFile);
        }

        free(inode);
        free(nameLength);
        free(mode);
    }
}



// Creates an archive of inputDirectory as a tar file
void createArchive(char *inputDirectoryName, char *archiveName) {
    DIR *inputDirectory;
    inputDirectory = opendir(inputDirectoryName);

    // Check if input directory is actually a file and not a directory
    FILE *errFile;
    errFile = fopen(inputDirectoryName, "r");

    // Specified target does not exist error
    if(inputDirectory == NULL && errFile == NULL) {
        fprintf(stderr, "Error: Specified target(\"%s\") does not exist.\n", 
        inputDirectoryName);
        exit(-1);
    
    // Specified target is not a directory error
    } else if(inputDirectory == NULL) {
        fprintf(stderr, "Error: Specified target(\"%s\") is not a directory.\n", 
        inputDirectoryName);
    }

    int fcloseVal = fclose(errFile);

    if(fcloseVal == EOF) {
        perror("fclose");
        exit(-1);
    }

    // Create the tar file
    FILE *tarFile;
    tarFile = fopen(archiveName, "w+");
    if(tarFile == NULL) {
        perror("fopen");
        exit(-1);
    }

    // Input the magic number at the start of the tar file
    int magicNumber = MAGIC_NUMBER;
    int fwriteVal;
    fwriteVal = fwrite(&magicNumber, 4, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the inode of the root directory into the tar file.
    struct stat fileStat;
    int statVal;
    statVal = stat(inputDirectoryName, &fileStat);

    if(statVal == -1) {
        perror("stat");
        exit(-1);
    }

    fwriteVal = fwrite(&fileStat.st_ino, 8, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }
      
    // Write the root directory name length
    int fileNameLength = strlen(inputDirectoryName);
    fwriteVal = fwrite(&fileNameLength, 4, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the root directory file name
    char *fileName = (char *) malloc(fileNameLength * sizeof(char));
    fwriteVal = fwrite(inputDirectoryName, 1, fileNameLength, tarFile);
    if(fwriteVal != fileNameLength) {
        perror("fwrite");
        exit(-1);
    }

    // Store the inode value into the map
    set_inode(fileStat.st_ino, fileName);

    free(fileName);

    // Write the root directory file mode
    fwriteVal = fwrite(&fileStat.st_mode, 4, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the root directory file modification time
    fwriteVal = fwrite(&fileStat.st_mtime, 8, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Recurse within the root directory
    struct dirent *item;
    errno = 0;
    readdir(inputDirectory);
    readdir(inputDirectory);
    item = readdir(inputDirectory);
    if(item == NULL && errno == 0) {
        perror("readdir");
        exit(-1);
    }

    if(item != NULL) {
        char originalDirectory[strlen(inputDirectoryName) + 1];
            strncpy(originalDirectory, inputDirectoryName, strlen(inputDirectoryName));
            originalDirectory[strlen(inputDirectoryName)] = '\0';


        char *slash = "/";
        char *directoryWithSlash = strncat(inputDirectoryName, slash, 1);
        char *itemName = 
        strncat(directoryWithSlash, item->d_name, strlen(item->d_name));

        statVal = lstat(itemName, &fileStat);
        if(statVal == -1) {
            perror("lstat");
            exit(-1);
        }
        
        if(S_ISDIR(fileStat.st_mode)) {
            directoryRecurse(inputDirectory, itemName, originalDirectory, tarFile);

        } else if(S_ISREG(fileStat.st_mode)) {
            fileRecurse(inputDirectory, originalDirectory, itemName, tarFile);

        } else if(S_ISLNK(fileStat.st_mode)) {
            char rootDir[strlen(originalDirectory)];
            strncpy(rootDir, originalDirectory, strlen(originalDirectory));
            rootDir[strlen(originalDirectory)] = '\0';

            // Loop through the directory until a non-soft link is found
            while(S_ISLNK(fileStat.st_mode)) {

                item = readdir(inputDirectory);
                directoryWithSlash = strncat(originalDirectory, slash, 1);

                itemName = 
                strncat(directoryWithSlash, item->d_name, strlen(item->d_name));
                statVal = lstat(itemName, &fileStat);

                if(statVal == -1) {
                    perror("lstat");
                    exit(-1);
                }
            }

            // After a non-soft link is found, call the appropriate recursive method.
            if(S_ISDIR(fileStat.st_mode)) {
                directoryRecurse(inputDirectory, rootDir, rootDir, tarFile);
            } else if(S_ISREG(fileStat.st_mode)) {
                fileRecurse(inputDirectory, rootDir, itemName, tarFile);
            }
        }
    }
}



/* Recursively writes the information within the given directory
   into the tar file
*/
void directoryRecurse(DIR *inputDirectory, char *directoryName, char *parentName, FILE *tarFile) {
    if(inputDirectory == NULL) {
        perror("opendir");
        exit(-1);
    }

    struct stat dirInfo;
    int statVal = lstat(directoryName, &dirInfo);
    if(statVal == -1) {
        perror("lstat");
        exit(-1);
    }

    // Input the directory inode value
    int fwriteVal = fwrite(&dirInfo.st_ino, 8, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory file name length
    int fileNameLength = strlen(directoryName);

    fwriteVal = fwrite(&fileNameLength, 4, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory file name
    fwriteVal = fwrite(directoryName, 1, fileNameLength, tarFile);
    if(fwriteVal != fileNameLength) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory mode
    fwriteVal = fwrite(&dirInfo.st_mode, 4, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Input the directory modification time
    fwriteVal = fwrite(&dirInfo.st_mtime, 8, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Recurse on the next entry
    struct dirent *ent;
    errno = 0;
    ent = readdir(inputDirectory);
    if(ent == NULL && errno != 0) {
        perror("readdir");
        exit(-1);
    }

    if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)) {
        errno = 0;
        ent = readdir(inputDirectory);
        if(ent == NULL && errno != 0) {
            perror("readdir");
            exit(-1);
        }
    }

    if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)) {
        errno = 0;
        ent = readdir(inputDirectory);
        if(ent == NULL && errno != 0) {
            perror("readdir");
            exit(-1);
        }
    }


    while(ent != NULL) {
        // Keep track of the parent directory name
        char originalDirectory[strlen(directoryName) + 1];
        strncpy(originalDirectory, directoryName, strlen(directoryName));
        originalDirectory[strlen(directoryName)] = '\0';

        char *slash = "/";
        char *newEntryName = strncat(directoryName, slash, 1);
        newEntryName = strncat(directoryName, ent->d_name, strlen(ent->d_name));

        int statVal = lstat(newEntryName, &dirInfo);

        struct stat fileInfo;
        char fileOriginalDir[strlen(parentName) + 1];
        strncpy(fileOriginalDir, parentName, strlen(parentName));
        fileOriginalDir[strlen(parentName)] = '\0';
        char *newFileEntry = strncat(parentName, slash, 1);
        newFileEntry = strncat(parentName, ent->d_name, strlen(ent->d_name));
        int fileStatVal = lstat(newFileEntry, &fileInfo);

        if(statVal == -1 && fileStatVal == -1) {
            perror("lstat");
            exit(-1);
        }

        if(S_ISREG(fileInfo.st_mode) || S_ISREG(dirInfo.st_mode)) {
            if(S_ISREG(fileInfo.st_mode)) {
                fileRecurse(inputDirectory, fileOriginalDir, newFileEntry, tarFile);
            } else if(S_ISREG(dirInfo.st_mode)) {
                fileRecurse(inputDirectory, directoryName, newEntryName, tarFile);
            }

        } else if(S_ISDIR(dirInfo.st_mode) || S_ISDIR(fileInfo.st_mode)) {
            DIR *newInputDirectory;
            if(S_ISDIR(dirInfo.st_mode)) {
                newInputDirectory = opendir(newEntryName);

                if(newInputDirectory == NULL) {
                    perror("opendir");
                    exit(-1);
                }

                directoryRecurse(newInputDirectory, newEntryName, originalDirectory, tarFile);

            } else if(S_ISDIR(fileInfo.st_mode)){
                newInputDirectory = opendir(newFileEntry);
                if(newInputDirectory == NULL) {
                    perror("opendir");
                    exit(-1);
                }

                directoryRecurse(newInputDirectory, newFileEntry, originalDirectory, tarFile);
            }

            int closedirVal = closedir(newInputDirectory);

            if(closedirVal == -1) {
                perror("closedir");
                exit(-1);
            }

            directoryName = originalDirectory;
            directoryName[strlen(originalDirectory)] = '\0';
            parentName[strlen(fileOriginalDir)] = '\0';
        }

        errno = 0;
        ent = readdir(inputDirectory);
    }
}



// Writes the information for the given file or hard link into the tar file.
void fileRecurse(DIR *inputDirectory, char *inputDirectoryName, char *fileName, FILE *tarFile) {
    FILE *currentFile;
    currentFile = fopen(fileName, "r");

    if(currentFile == NULL) {
        perror("fopen");
        exit(-1);
    }

    struct stat fileInfo;
    int statVal = lstat(fileName, &fileInfo);
    if(statVal == -1) {
        perror("stat");
        exit(-1);
    }

    
    // Write the file inode number
    int fwriteVal;
    fwriteVal = fwrite(&fileInfo.st_ino, 8, 1, tarFile);

    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    // Write the file name length
    int nameLength = strlen(fileName);
    fwriteVal = fwrite(&nameLength, 4, 1, tarFile);
    if(fwriteVal != 1) {
        perror("fwrite");
        exit(-1);
    }

    fwriteVal = fwrite(fileName, 1, nameLength, tarFile);

    if(fwriteVal != nameLength) {
        perror("fwrite");
        exit(-1);
    }

    /* Put the inode and name into the map
       and check if the file is a hard link
    */
    int isHardLink = 0;

    if(get_inode(fileInfo.st_ino) != NULL) {
        isHardLink = 1;
    }

    if(isHardLink == 0) {
        set_inode(fileInfo.st_ino, fileName);

        // Write the file mode
        fwriteVal = fwrite(&fileInfo.st_mode, 4, 1, tarFile);

        if(fwriteVal != 1) {
            perror("fwrite");
            exit(-1);
        }

        // Write the file modification time
        fwriteVal = fwrite(&fileInfo.st_mtime, 8, 1, tarFile);
        if(fwriteVal != 1) {
            perror("fwrite");
            exit(-1);
        }

        // Write the file size
        fwriteVal = fwrite(&fileInfo.st_size, 8, 1, tarFile);

        if(fwriteVal != 1) {
            perror("fwrite");
            exit(-1);
        }

        // Write the file content
        char fileContent[fileInfo.st_size];
        int freadVal = fread(fileContent, 1, fileInfo.st_size, currentFile);

        if(freadVal != fileInfo.st_size) {
            perror("fread");
            exit(-1);
        }

        fwriteVal = fwrite(fileContent, 1, fileInfo.st_size, tarFile);
        if(fwriteVal != fileInfo.st_size) {
            perror("fwrite");
            exit(-1);
        }
    }

    int fcloseVal = fclose(currentFile);
    if(fcloseVal == EOF) {
        perror("fclose");
        exit(-1);
    }


    // read the next entry and continue
    struct dirent *item;
    errno = 0;
    item = readdir(inputDirectory);

    while(item != NULL) {
        // Keep track of the parent directory name
        char originalDirectory[strlen(inputDirectoryName) + 1];
        strncpy(originalDirectory, inputDirectoryName, strlen(inputDirectoryName));
        originalDirectory[strlen(inputDirectoryName)] = '\0';

        char *slash = "/";
        strncat(inputDirectoryName, slash, 1);
        char *itemName = 
        strncat(inputDirectoryName, item->d_name, strlen(item->d_name));

        statVal = lstat(itemName, &fileInfo);
        if(statVal == -1) {
            perror("lstat");
            exit(-1);
        }

        if(S_ISDIR(fileInfo.st_mode)) {
            DIR *newInputDirectory = opendir(itemName);
            if(newInputDirectory == NULL) {
                perror("opendir");
                exit(-1);
            }

            directoryRecurse(newInputDirectory, itemName, originalDirectory, tarFile);

            inputDirectoryName = originalDirectory;
            inputDirectoryName[strlen(originalDirectory)] = '\0';

            int closedirVal = closedir(newInputDirectory);
            if(closedirVal == -1) {
                perror("closedir");
                exit(-1);
            }


        } else if(S_ISREG(fileInfo.st_mode)) {
            fileRecurse(inputDirectory, originalDirectory, itemName, tarFile);
        }

        errno = 0;
        item = readdir(inputDirectory);
    }
}



// Prints the contents of the tar file archiveName
void printArchive(char *archiveName) {
    FILE *archive;
    archive = fopen(archiveName, "r");
    if(archive == NULL) {
        fprintf(stderr, "Specified target(\"%s\") does not exist.\n", archiveName);
        exit(-1);
    }

    // Check the magic number
    int archiveMagic = getw(archive);  
    if(archiveMagic != MAGIC_NUMBER) {
        fprintf(stderr, "Error: Bad magic number (%d), should be %d.\n", archiveMagic, MAGIC_NUMBER);
        exit(-1);
    }

    // Print the contents of the archive
    int endOfFile = fgetc(archive);
    int fseekVal = fseek(archive, -1, SEEK_CUR);
    if(fseekVal != 0) {
        perror("fseek");
        exit(-1);
    }
    while(endOfFile != EOF) {
        int offset = 0;

        /* fileType:
        0 = Directory. 
        1 = Regular file. 
        2 = Executable file. 
        */
        int fileType;

        long *inode = (long *) malloc(1 * sizeof(long));
        int *fileNameLength = (int *) malloc(1 * sizeof(int));
        char *fileName;
        int *fileMode = (int *) malloc(1 * sizeof(int));

        // A string version of fileMode to properly format the print.
        char fileModeString[6];

        long *modTime = (long *) malloc(1 * sizeof(long));
        long *fileSize = (long *) malloc(1 * sizeof(long));
        int freadVal;
        int inodeCounter = 0;
        int isHardLink = 0;
        
        while(offset <= 6) {

            switch(offset) {
                // Get the inode number
                case 0:
                    freadVal = fread(inode, sizeof(long), 1, archive);

                    if(freadVal != 1) {
                        perror("fread");
                        exit(-1);
                    }

                    offset++;
                    break;

                // Get the file name length
                case 1:
                    freadVal = fread(fileNameLength, sizeof(int), 1, archive);

                    if(freadVal != 1) {
                        perror("fread");
                    }

                    fileName = (char *) malloc((*fileNameLength) * sizeof(char) + 1);

                    offset++;
                    break;

                // Get the file name and check if the entry is a hard link
                case 2:
                    freadVal = fread(fileName, sizeof(char), *fileNameLength, archive);
                    fileName[*fileNameLength] = '\0';

                    if(freadVal != *fileNameLength) {
                        perror("fread");
                    }

                    // Check if the inode value is already stored
                    const char *getInodeValue = get_inode(*inode);

                    if(getInodeValue != NULL) {
                        isHardLink = 1;
                    } 

                    // Store the inode value (if it isn't already stored).
                    if(isHardLink == 0) {
                        set_inode(*inode, fileName);
                    }

                    offset++;
                    break;

                // Get the file mode if the entry is not a hard link
                case 3:

                    if(isHardLink == 0) {

                        freadVal = fread(fileMode, sizeof(int), 1, archive);

                        if(freadVal != 1) {
                            perror("fread");
                            exit(-1);
                        }

                        // Determine if the entry is a file or a directory
                        int sprintfValue = sprintf(fileModeString, "%o", *fileMode);

                        if(sprintfValue < 0) {
                            perror("sprintf");
                            exit(-1);
                        }

                        if(fileModeString[0] == '4') {
                            fileType = 0;

                        // Check if any execute bits are on.
                        } else {
                            int firstNum = atoi(&fileModeString[2]);
                            int secondNum = atoi(&fileModeString[3]);
                            int thirdNum = atoi(&fileModeString[4]);

                            if(firstNum % 2 == 1 || secondNum % 2 == 1 || thirdNum % 2 == 1) {
                                fileType = 2;
                            } else {
                                fileType = 1;
                            }
                        }
                    }

                    offset++;
                    break;
                
                // Get the file modification time if the file is not a hard link
                case 4:
                    if(isHardLink == 0) {
                        freadVal = fread(modTime, sizeof(long), 1, archive);

                        if(freadVal != 1) {
                            perror("fread");
                            exit(-1);
                        } 
                    }

                    offset++;
                    break;

                // Get the file size if the entry is not a directory or a hard link.
                case 5:
                    if(isHardLink == 0 && fileType != 0) {
                        freadVal = fread(fileSize, sizeof(long), 1, archive);

                        if(freadVal != 1) {
                            perror("fread");
                            exit(-1);
                        }
                    }

                    offset++;
                    break;

                // Set the offset to the next file if the entry is a regular or executable file.
                case 6:

                    if(isHardLink == 0 && (fileType == 1 || fileType == 2)) {
                        int fseekVal = fseek(archive, *fileSize, SEEK_CUR);
                        if(fseekVal != 0) {
                            perror("fseek");
                        }
                    }

                    offset++;
                    break;


                default:
                    offset++;
                    break; 
            }
        }

        // Print the output for the current file.
        
        // Format the mode correctly by removing the first 2 or 3 digits.
        const char *fileModeFormatted;
        if(isHardLink == 0) {
            if(fileType == 0) {
                fileModeFormatted = &fileModeString[2];
            } else if(fileType == 1 || fileType == 2) {
                fileModeFormatted = &fileModeString[3];
            }
        }

        int fileModeOutput = atoi(fileModeFormatted);

        if(isHardLink == 1) {
            printf("%s/ -- inode: %lu\n", fileName, *inode);
        } else {
            if(fileType == 0) {
                printf("%s/ -- inode: %lu, mode %d, mtime: %lu\n",
                    fileName, *inode, fileModeOutput, *modTime);
            } else if(fileType == 1) {
                printf("%s -- inode: %lu, mode: %d, mtime: %lu, size: %lu\n", 
                        fileName, *inode, fileModeOutput, *modTime, *fileSize);
            } else if(fileType == 2) {
                printf("%s* -- inode: %lu, mode: %d, mtime: %lu, size: %lu\n",
                    fileName, *inode, fileModeOutput, *modTime, *fileSize);
            }
        }

        isHardLink == 0;
        offset = 0;

        free(inode);
        free(fileNameLength);
        free(fileName);
        free(fileMode);
        free(modTime);
        free(fileSize);

        endOfFile = fgetc(archive);
        fseek(archive, -1, SEEK_CUR);
    }

    fclose(archive);
}